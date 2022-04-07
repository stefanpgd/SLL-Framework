#include "precomp.h"
#include "Color.h"
#include "PrimitiveList.h"
#include "Material.h"
#include "AllTextures.h"
#include "SDL.h"

#define DiffuseReflection false

namespace SLL
{
	const float Epsilon = 0.01f;
	const float SEpsilon = 0.001f;

	void WriteColor(Color* sourceBuffer, Tmpl8::Pixel* destinationBuffer, int x, int y, int imageWidth, int samplesPerPixel, ScreenFilter filter, bool doTonemapping, float exposure)
	{
		Color color = sourceBuffer[x + y * imageWidth];

		float scale = 1.0f / samplesPerPixel;
		color.x *= scale;
		color.y *= scale;
		color.z *= scale;

		if (doTonemapping)
		{
			Color temp = color * exposure;
			const float a = 2.51f;
			const float b = 0.03f;
			const float c = 2.43f;
			const float d = 0.59f;
			const float e = 0.14f;
			vec3 a1 = (temp * (a * temp + b));
			vec3 b1 = (temp * (c * temp + d) + e);
			temp.x = a1.x / b1.x;
			temp.y = a1.y / b1.y;
			temp.z = a1.z / b1.z;
			temp.x = Clamp(temp.x, 0.0f, 1.0f);
			temp.y = Clamp(temp.y, 0.0f, 1.0f);
			temp.z = Clamp(temp.z, 0.0f, 1.0f);
			color = temp;
		}

		switch (filter)
		{
		case ScreenFilter::Normal:
			color.x = sqrtf(color.x);
			color.y = sqrtf(color.y);
			color.z = sqrtf(color.z);
			break;

		case ScreenFilter::Dots:
			color.x = (color.x) * cosf(float(x)) * sinf(float(y)) * tanf(float(x)) * 2.5f;
			color.y = (color.y) * cosf(float(x)) * sinf(float(y)) * 2.5f;
			color.z = (color.z) * cosf(float(x)) * sinf(float(y)) * 2.5f;
			break;

		case ScreenFilter::TV:
			color.x = sqrtf(RandomInRange(0.0f, 0.45f) * color.x);
			color.y = sqrtf(RandomInRange(0.0f, 0.45f) * color.y);
			color.z = sqrtf(RandomInRange(0.0f, 0.56f) * color.z);
			break;

		case ScreenFilter::Matrix:
			color.x = powf((color.x ), 3.0f / 2.0f);
			color.y = powf((color.y ), 4.0f / 5.0f);
			color.z = powf((color.z ), 3.0f / 2.0f);
			break;

		case ScreenFilter::Lines:
			color.x *= tanf(float(x + y));
			color.y *= tanf(float(x + y));
			color.z *= tanf(float(x + y));
			break;
		}

		unsigned int r = (int)(255.0f * Clamp(color.x, 0.0f, 1.0f));
		unsigned int g = (int)(255.0f * Clamp(color.y, 0.0f, 1.0f));
		unsigned int b = (int)(255.0f * Clamp(color.z, 0.0f, 1.0f));

		Tmpl8::Pixel pixelColor = (r << 16) + (g << 8) + b;
		destinationBuffer[x + y * ScreenWidth] = pixelColor;
	}

	Color RayColor(const Ray& ray, const SpriteTexture& background, const PrimitiveList* primitives, int depth, int startDepth, bool normalView, bool& speedup)
	{
		HitRecord record;

		// If we've exceeded the ray bounce limit, no more light is gathered
		if (depth <= 0)
		{
			return Color(0.0f, 0.0f, 0.0f);
		}

		int lightIndex = 0;

		if (primitives->Lights.size() > 1)
		{
			lightIndex = rand() % primitives->Lights.size();
		}

		bool speedupFlag = false;

		// If the ray hits nothing, return the background color
		if (primitives->Hit(ray, 0.01f, infinity, record))
		{
			if (normalView)
			{
				return (record.Normal + 1.0f) * 0.5f;
			}

			speedup = false;

			// Primitive we hit was a light, meaning that we don't need to bounce any further
			// Grab the color from that light and return it.
			if (record.pMaterial->IsEmmisive)
			{
				return record.pMaterial->GetColor(record);
			}

			// We know have the closest object we hit with, we now calculate the illumination based on the materials properties
			Color incomingLight;
			float diffuseStrength = record.pMaterial->Diffuse;
			float specularStrength = record.pMaterial->Specular;
			float reflectiveStrength = record.pMaterial->Reflectiveness;
			float transparencyStrength = record.pMaterial->Transparency;
			bool fullyReflective = diffuseStrength < Epsilon;
			Color materialColor = record.pMaterial->GetColor(record);

#if DiffuseReflection
			if (diffuseStrength > Epsilon)
			{
				vec3 diffuseReflection = record.Normal + RandomUnitVector();
				if (diffuseReflection.NearZero())
				{
					diffuseReflection = record.Normal;
				}

				Ray diffuseRay = Ray(record.Point, diffuseReflection);
				incomingLight += diffuseStrength * (RayColor(diffuseRay, background, primitives, depth - 1, startDepth, false, speedupFlag) * materialColor);
			}

			// Specular Reflection
			//Idea/source: https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
			if (reflectiveStrength > SEpsilon)
			{
				Ray reflection = Ray(record.Point, Reflect(ray.Direction, record.Normal));
				incomingLight += reflectiveStrength * RayColor(reflection, background, primitives, depth - 1, startDepth, false, speedupFlag);
			}
#endif

#if !DiffuseReflection
			// Create shadow direction
			vec3 dir = primitives->Lights[lightIndex]->GetPosition() - record.Point;
			float rayLength = dir.Length();
			Ray shadowRay{ record.Point, dir }; // direction get normalized in ray constructor

			if (primitives->IsLightReachable(shadowRay, Epsilon, rayLength - Epsilon))
			{
				// Diffuse Shading
				if (diffuseStrength > Epsilon)
				{
					float diffuse = dot(record.Normal, shadowRay.Direction);
					if (diffuse > 0.0f)
					{
						diffuse *= (primitives->Lights[lightIndex]->LightIntensity * (1.0f / (rayLength * rayLength)));
						diffuse *= diffuseStrength;
						incomingLight += diffuse * primitives->Lights[lightIndex]->GetColor() * materialColor;
					}
				}

				// Specular Highlight
				if (specularStrength > Epsilon)
				{
					vec3 reflect = Reflect(shadowRay.Direction, record.Normal);	// Reflect light from the light direction towards where it is reflecting
					float angle = dot(ray.Direction, reflect);					// Angle incidence

					if (angle > 0.0f)
					{
						float specular = powf(angle, record.pMaterial->Shininess) * specularStrength;
						incomingLight += specular * primitives->Lights[lightIndex]->GetColor();
					}
				}

				// Specular Reflection
				//Idea/source: https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
				if (reflectiveStrength > SEpsilon && !fullyReflective)
				{
					Ray reflection = Ray(record.Point, Reflect(ray.Direction, record.Normal));
					incomingLight += reflectiveStrength * RayColor(reflection, background, primitives, depth - 1, startDepth, false, speedupFlag);
				}
			}

			// Specular Reflection
			if (reflectiveStrength > SEpsilon && fullyReflective)
			{
				Ray reflection = Ray(record.Point, Reflect(ray.Direction, record.Normal));
				incomingLight += reflectiveStrength * RayColor(reflection, background, primitives, depth - 1, startDepth, false, speedupFlag);
			}
#endif
			if (transparencyStrength > Epsilon)
			{
				float n1 = record.FrontFace ? 1.0f : record.pMaterial->RefractionIndex;
				float n2 = record.FrontFace ? record.pMaterial->RefractionIndex : 1.0f;

				float n = n1 / n2;
				float cosI = -dot(record.Normal, ray.Direction);
				float k = n * n * (1.0f - cosI * cosI);

				Color c = materialColor;

				if (!record.FrontFace)
				{
					float distance = (record.Point - ray.Origin).Length();
					float beerslaw = expf(-record.pMaterial->Density * distance);
					c *= beerslaw;
				}

				if (k > 1.0f) // TIR - Total Internal Reflection
				{
					Ray reflection = Ray(record.Point, Reflect(ray.Direction, record.Normal));
					incomingLight += transparencyStrength * RayColor(reflection, background, primitives, depth - 1, startDepth, false, speedupFlag);
				}
				else
				{
					float cosT = sqrtf(1.0f - k);

					// Fresnel
					float orthogonal = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
					float parallel = (n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT);
					float fresnel = (orthogonal * orthogonal + parallel * parallel) * 0.5f;

					Ray reflection = Ray(record.Point, Reflect(ray.Direction, record.Normal));
					Ray refraction = Ray(record.Point, Refract(ray.Direction, record.Normal, n, cosI, cosT));

					Color reflectionColor = fresnel * RayColor(reflection, background, primitives, depth - 1, startDepth, false, speedupFlag);
					Color refractionColor = (1.0f - fresnel) * RayColor(refraction, background, primitives, depth - 1, startDepth, false, speedupFlag);
					incomingLight += transparencyStrength * ((reflectionColor + refractionColor) * c);
				}
			}

			return incomingLight;
		}

		if (depth == startDepth)
		{
			speedup = true;
		}

		return GetSkydomeColor(ray.Direction, background); // background/skybox
	}

	vec3 GetSkydomeColor(vec3 direction, const SpriteTexture& skydome)
	{
		float theta = acosf(-direction.y);
		float phi = atan2f(-direction.z, direction.x) + Pi;

		float u = phi / (2 * Pi);
		float v = theta / Pi;

		return skydome.Value(u, v, Point3{ 0,0,0 });
	}

	vec3 GetSkydomeColor(vec3 direction, const Color& skydome)
	{
		return skydome;
	}
}
