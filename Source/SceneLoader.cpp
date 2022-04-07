#include "precomp.h"
#include "SceneLoader.h"
#include "Camera.h"
#include "PrimitiveList.h"
#include "Material.h"
#include "Sphere.h"
#include "BVH.h"
#include "Ellipsoid.h"
#include "PointLight.h"
#include "AreaLight.h"
#include "AllTextures.h"
#include "SkydomeManager.h"

namespace SLL
{
	void SceneLoader::LoadScene(SceneSettings& sceneSettings,
		CameraSettings& cameraSettings)
	{
		sceneSettings.Primitives->Clear();
		cameraSettings.DefaultSettings();

		switch (sceneSettings.SceneIndex)
		{
		case SceneIndex::MaterialDisplay:
			MaterialDisplay(sceneSettings.Primitives, sceneSettings.ObjectCount);
			sceneSettings.SceneName = "Material Display";
			sceneSettings.SceneDescription1 = "A showcase of all primitives";
			sceneSettings.SceneDescription2 = "and materials in SLL";
			sceneSettings.PreferredSkydome = Skydome::Studio;
			cameraSettings.LookFrom = Point3(-2.0f, 0.0f, 6.0f);
			cameraSettings.FOV = 75.0f;
			break;

		case SceneIndex::GlassPyramid:
			GlassPyramid(sceneSettings.Primitives, sceneSettings.ObjectCount);
			sceneSettings.SceneName = "Glass Pyramid";
			sceneSettings.SceneDescription1 = "A simple pyramid that is";
			sceneSettings.SceneDescription2 = "build with dielectrics";
			sceneSettings.PreferredSkydome = Skydome::Studio;
			cameraSettings.LookFrom = Point3(6.8f, 7.5f, 10.0f);
			cameraSettings.FOV = 45.0f;
			break;

		case SceneIndex::SphereFlake:
			SphereFlake(sceneSettings.Primitives, sceneSettings.ObjectCount);
			sceneSettings.SceneName = "SphereFlake";
			sceneSettings.SceneDescription1 = "Sphereflake with 12";
			sceneSettings.SceneDescription2 = "child spheres per branch";
			sceneSettings.PreferredSkydome = Skydome::Rainbow;
			cameraSettings.LookFrom = Point3(0.0f, 0.0f, 5.0f);
			cameraSettings.FOV = 75.0f;
			break;

		case SceneIndex::BVH:
			BVH(sceneSettings.Primitives, sceneSettings.ObjectCount);
			sceneSettings.SceneName = "BVH";
			sceneSettings.SceneDescription1 = "Scene with 80k primitives";
			sceneSettings.SceneDescription2 = "showing the speed of the BVH";
			sceneSettings.PreferredSkydome = Skydome::Studio;
			cameraSettings.LookFrom = Point3(-2.0f, 2.0f, 15.0f);
			cameraSettings.FOV = 60.0f;
			break;

		case SceneIndex::InfinityRoom:
			InfinityRoom(sceneSettings.Primitives, sceneSettings.ObjectCount);
			sceneSettings.SceneName = "'Infinity' Room";
			sceneSettings.SceneDescription1 = "A box of evenly distanced";
			sceneSettings.SceneDescription2 = "spheres, faking 'infinity'";
			sceneSettings.PreferredSkydome = Skydome::Black;
			cameraSettings.LookFrom = Point3(0.0f, 0.0f, 4.5f);
			cameraSettings.FOV = 75.0f;
			break;

		case SceneIndex::SierpinskiCarpet:
			SierpinskiCarpet(sceneSettings.Primitives, sceneSettings.ObjectCount);
			sceneSettings.SceneName = "Sierpinski Carpet";
			sceneSettings.SceneDescription1 = "A recreation of the famous";
			sceneSettings.SceneDescription2 = "Sierpinski carpet fractal";
			sceneSettings.PreferredSkydome = Skydome::White;
			cameraSettings.LookFrom = Point3(0.0f, 0.0f, 300.0f);
			cameraSettings.FOV = 105.0f;
			break;

		case SceneIndex::MengerSponge:
			MengerSponge(sceneSettings.Primitives, sceneSettings.ObjectCount);
			sceneSettings.SceneName = "Menger Sponge";
			sceneSettings.SceneDescription1 = "A recreation of the";
			sceneSettings.SceneDescription2 = "Menger Sponge fractal";
			sceneSettings.PreferredSkydome = Skydome::Black;
			cameraSettings.LookFrom = Point3(0.0f, 0.0f, 10.0f);
			cameraSettings.FOV = 75.0f;
			break;
		}
	}

	void SceneLoader::MaterialDisplay(PrimitiveList* primitives, int& objectCount)
	{
		Material* floor = new Material(new CheckerTexture(Color(0.01f, 0.01f, 0.01f), Color(0.9f, 0.9f, 0.9f)));
		floor->SetDiffuse(0.98f, 0.0f, 1500.0f);
		floor->SetReflectiveness(0.02f);
		primitives->AddMaterial(floor);
		primitives->Add(new Sphere(Point3(0.0f, -2500.5f, 0.0f), 2500.0f, floor));

		PrimitiveList objects;

		// Ellipsoids
		for (int i = 0; i < 7; i++)
		{
			float angle = (i * 45) * DegToRad;
			float x = cosf(angle);
			float z = sinf(angle);
			vec3 dir = vec3(x, 0.0f, z);
			dir *= 0.5f;
			vec3 pos = vec3(-2.20f, -0.35f + i * 0.15f, 1.25f) + dir;

			Material* diffuse = new Material(vec3(1.0f, 1.0f - i * 0.15f, 1.0f - i * 0.15f));
			diffuse->SetDiffuse(0.85f, 0.13f, 25.0f);
			diffuse->SetReflectiveness(0.02f);
			primitives->AddMaterial(diffuse);

			objects.Add(new Ellipsoid(pos, vec3(1.0f - i * 0.15f, 1.0f, 1.0f - i * 0.15f), 0.15f + i * 0.15f, diffuse));
		}

		// Phong
		for (int i = 0; i < 6; i++)
		{
			float angle = (i * 60) * DegToRad;
			float x = cosf(angle);
			float z = sinf(angle);
			vec3 dir = vec3(x, 0.0f, z);
			dir *= 0.4f;

			Material* phong = new Material(vec3(0.9f, 0.0f, 0.0f));
			float diff = 1.0f - i * 0.15f;
			float specc = 1.0f - diff;
			phong->SetDiffuse(diff, specc);
			primitives->AddMaterial(phong);

			float radius = i * 0.04f;
			float y = -0.5f + radius;
			vec3 pos = vec3(-1.0f, y, 2.5f) + dir;
			objects.Add(new Sphere(pos, radius, phong));
		}

		// Blinn Phong
		for (int i = 0; i < 6; i++)
		{
			float angle = (i * 60) * DegToRad;
			float x = cosf(angle);
			float z = sinf(angle);
			vec3 dir = vec3(x, 0.0f, z);
			dir *= 0.4f;

			Material* blinn = new Material(vec3(0.1f, 0.1f, 0.8f));
			float reflect = i * 0.03f;
			float maxDiff = 1.0f - reflect;
			float diff = maxDiff - i * 0.1f;
			float specc = maxDiff - diff;
			blinn->SetDiffuse(diff, specc);
			blinn->SetReflectiveness(reflect);
			primitives->AddMaterial(blinn);

			float radius = i * 0.04f;
			float y = -0.5f + radius;
			vec3 pos = vec3(0.25f, y, 3.0f) + dir;
			objects.Add(new Sphere(pos, radius, blinn));
		}

		Material* metal = new Material(vec3(1.0f, 1.0f, 1.0f));
		metal->SetReflectiveness(1.0f);
		primitives->Add(new Sphere(vec3(0.0f, 0.5f, 0.0f), 1.0f, metal));
		primitives->AddMaterial(metal);

		// Metal
		for (int i = 0; i < 6; i++)
		{
			float angle = (i * 60) * DegToRad;
			float x = cosf(angle);
			float z = sinf(angle);
			vec3 dir = vec3(x, 0.0f, z);
			dir *= 1.25f;
			vec3 pos = vec3(0.0f, 0.0f, 0.0f) + dir;

			Material* randomMetal = new Material(vec3::Random());
			randomMetal->SetReflectiveness(1.0f);
			objects.Add(new Sphere(vec3(pos.x, -0.25f, pos.z), 0.25, randomMetal));
			objects.Add(new Sphere(vec3(pos.x, 0.25f, pos.z), 0.15, randomMetal));
			objects.Add(new Sphere(vec3(pos.x, 0.6f, pos.z), 0.1, randomMetal));
		}

		Material* glass = new Material(vec3(0.203, 0.772, 0.701));
		glass->SetTransparency(1.0f, 1.51f, 2.25);
		primitives->AddMaterial(glass);

		primitives->Add(new Sphere(vec3(2.20f, 0.85, 1.25f), 0.5f, glass));

		// Dielectrics
		for (int i = 0; i < 3; i++)
		{
			float angle = (i * 120) * DegToRad;
			float x = cosf(angle);
			float z = sinf(angle);
			vec3 dir = vec3(x, 0.0f, z);
			dir *= 0.50f;
			vec3 pos = vec3(2.20f, 0.0f, 1.25f) + dir;
			objects.Add(new Sphere(vec3(pos.x, -0.20f, pos.z), 0.3f, glass));

			dir *= 0.75f;
			pos = vec3(2.20f, 0.0f, 1.25f) + dir;
			objects.Add(new Sphere(vec3(pos.x, 0.275f, pos.z), 0.2f, glass));
		}

		// Beer's law Dielectrics
		for (int i = 0; i < 6; i++)
		{
			Material* beersLaw = new Material(vec3(1.0f, 1.0f, 1.0f));
			beersLaw->SetTransparency(1.0f, 1.5f, 16.0f - i * 3.25f);
			primitives->AddMaterial(beersLaw);
			objects.Add(new Sphere(vec3(-2.85f, -0.35f + i * 0.3f, 3.0f), 0.15f, beersLaw));

			Material* beersGlowLaw = new Material(vec3(0.05f, 0.45f, 0.8f));
			beersGlowLaw->SetTransparency(1.0f, 1.5f, -(0.25f + i * 1.35f));
			primitives->AddMaterial(beersGlowLaw);
			objects.Add(new Sphere(vec3(2.1f, -0.35f + i * 0.3f,3.0f), 0.15f, beersGlowLaw));
		}

		int LogoLetters[7][14]
		{
			{1,1,1,1,0,2,0,0,0,0,3,0,0,0},
			{1,0,0,0,0,2,0,0,0,0,3,0,0,0},
			{1,0,0,0,0,2,0,0,0,0,3,0,0,0},
			{1,1,1,1,0,2,0,0,0,0,3,0,0,0},
			{0,0,0,1,0,2,0,0,0,0,3,0,0,0},
			{0,0,0,1,0,2,0,0,0,0,3,0,0,0},
			{1,1,1,1,0,2,2,2,2,0,3,3,3,3}
		};

		// SLL Letters
		Material* red = new Material(vec3(0.952, 0.505, 0.505));
		Material* green = new Material(vec3(0.988, 0.890, 0.545));
		Material* blue = new Material(vec3(0.584, 0.882, 0.827));
		red->IsEmmisive = true;
		green->IsEmmisive = true;
		blue->IsEmmisive = true;
		primitives->AddMaterial(red);
		primitives->AddMaterial(green);
		primitives->AddMaterial(blue);

		for (int y = 0; y < 7; y++)
		{
			for (int x = 0; x < 14; x++)
			{
				vec3 pos = vec3(x * 0.2f + -1.5f, 2.8f - y * 0.2f, 2.5f);
				pos *= 0.6f;
				
				if (LogoLetters[y][x] == 1)
				{
					objects.Add(new Sphere(pos, 0.1f, red));
				}
				else if (LogoLetters[y][x] == 2)
				{
					objects.Add(new Sphere(pos, 0.1f, green));
				}
				else if (LogoLetters[y][x] == 3)
				{
					objects.Add(new Sphere(pos, 0.1f, blue));
				}
			}
		}

		primitives->Add(new BVHNode(objects));
		objectCount = (int)(objects.Objects.size() + 1);
		primitives->AddLight(new PointLight(Point3(0.0f, 3.0f, 6.0f), Color(1.0f, 1.0f, 1.0f), 50.0f));
	}

	void SceneLoader::GlassPyramid(PrimitiveList* primitives, int& objectCount)
	{
		PrimitiveList objects;
		auto floor = new Material(Color(1.0f, 1.0f, 1.0f));
		floor->SetDiffuse(0.5f, 0.35f, 100.0f);
		floor->SetReflectiveness(0.15f);
		primitives->AddMaterial(floor);

		primitives->Add(new Sphere(Point3(0.0f, -1000.5f, 0.0f), 1000.0f, floor));

		Material* greenGlass = new Material(Color(0.6f, 1.0f, 0.86f));
		greenGlass->SetTransparency(1.0f, 1.51f, 0.15f);
		primitives->AddMaterial(greenGlass);

		objects.Add(new Sphere(Point3(0.0f, 0.0f, 0.0f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(1.0f, 0.0f, 0.0f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(-1.0f, 0.0f, 0.0f), 0.5f, greenGlass));

		objects.Add(new Sphere(Point3(0.0f, 0.0f, -1.0f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(1.0f, 0.0f, -1.0f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(-1.0f, 0.0f, -1.0f), 0.5f, greenGlass));

		objects.Add(new Sphere(Point3(0.0f, 0.0f, -2.0f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(1.0f, 0.0f, -2.0f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(-1.0f, 0.0f, -2.0f), 0.5f, greenGlass));

		objects.Add(new Sphere(Point3(0.5f, 0.75f, -0.5f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(0.5f, 0.75f, -1.5f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(-0.5f, 0.75f, -0.5f), 0.5f, greenGlass));
		objects.Add(new Sphere(Point3(-0.5f, 0.75f, -1.5f), 0.5f, greenGlass));

		objects.Add(new Sphere(Point3(0.0f, 1.5f, -1.0f), 0.5f, greenGlass));

		primitives->Add(new BVHNode(objects));
		objectCount = (int)objects.Objects.size() + 1;
		primitives->AddLight(new PointLight(Point3(-0.0f, 20.0f, -15.0f), Color(1.0f, 1.0f, 1.0f), 100.0f));
	}

	void CreateFlake(PrimitiveList* primitives, Point3 parentPosition, float radius, float radiusDecrease, Material* mat, int sphereCount, float circleArea, int depth)
	{
		vec3 directions[2]
		{
			vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)
		};

		if (depth > 0)
		{
			for (int i = 0; i < sphereCount; i++)
			{
				int index = i % 2;
				float angle = i * (360.0f / sphereCount);
				angle *= DegToRad;
				float x = cosf(angle) * circleArea;
				float z = sinf(angle) * circleArea;

				vec3 dir = vec3(x, 0.0f, z);
				vec3 position = dir + directions[index];
				position.Normalize();
				position *= (radius + radius / radiusDecrease);
				position += parentPosition;
				primitives->Add(new Sphere(position, radius / radiusDecrease, mat));
				CreateFlake(primitives, position, radius / radiusDecrease, radiusDecrease, mat, sphereCount, circleArea, depth - 1);
			}

			for (int i = 0; i < sphereCount; i++)
			{
				float angle = i * (360.0f / sphereCount);
				angle *= DegToRad;

				float x = cosf(angle);
				float z = sinf(angle);
				vec3 dir = vec3(x, 0.0f, z);
				vec3 pos = dir * (radius + radius / radiusDecrease) + parentPosition;
				primitives->Add(new Sphere(pos, radius / radiusDecrease, mat));
				CreateFlake(primitives, pos, radius / radiusDecrease, radiusDecrease, mat, sphereCount, circleArea, depth - 1);
			}
		}
	}

	void SceneLoader::SphereFlake(PrimitiveList* primitives, int& objectCount)
	{
		PrimitiveList objects;

		const int fractalDepth = 4;
		float startRadius = 1.0f;
		float radiusDecrease = 3.5f;
		int sphereCount = 6;
		float circleArea = 0.75f;

		Material* metal = new Material(Color(1.0f, 1.0f, 1.0f));
		metal->SetDiffuse(0.0f, 0.0f, 25.0f);
		metal->SetReflectiveness(1.0f);
		primitives->AddMaterial(metal);

		objects.Add(new Sphere(Point3(0.0f, 0.0f, 0.0f), startRadius, metal)); // Base sphere

		CreateFlake(&objects, vec3(0.0f, 0.0f, 0.0f), startRadius, radiusDecrease, metal, sphereCount, circleArea, fractalDepth);

		objectCount = (int)objects.Objects.size();

		primitives->Add(new BVHNode(objects));
		primitives->AddLight(new PointLight(Point3(0.0f, 0.0f, 5.0f), Color(1.0f, 1.0f, 1.0f), 20.0f));
	}

	void SceneLoader::BVH(PrimitiveList* primitives, int& objectCount)
	{
		PrimitiveList objects;

		CheckerTexture* floorTex = new CheckerTexture(Color(0.01f, 0.01f, 0.01f), Color(0.8f, 0.8f, 0.8f));
		Material* floor = new Material(floorTex);
		floor->SetDiffuse(1.0f, 0.0f);
		primitives->Add(new Sphere(Point3(0.0f, -2500.0f, 0.0f), 2500.0f, floor));
		primitives->AddMaterial(floor);

		Material* diffuse = new Material(Color::Random());
		diffuse->SetDiffuse(1.0f, 0.0f);
		primitives->AddMaterial(diffuse);

		for (int i = 0; i < 80000; i++)
		{
			int roll = RandomInRange(0, 100);
			Point3 center = Point3(RandomInRange(-25.0f, 25.0f), 0.15f, RandomInRange(-25.0f, 25.0f));
			objects.Add(new Sphere(center, 0.15f, diffuse));
		}

		primitives->Add(new BVHNode(objects));
		objectCount = (int)(objects.Objects.size() + 1);

		primitives->AddLight(new PointLight(Point3(0.0f, 15, 0.0f), Color(1.0f, 1.0f, 1.0f), 250.0f));
	}

	void SceneLoader::InfinityRoom(PrimitiveList* primitives, int& objectCount)
	{
		PrimitiveList objects;

		Material* diff = new Material(Color(0.1f, 0.25f, 1.0));
		diff->SetDiffuse(0.85f, 0.15f);
		primitives->AddMaterial(diff);

		for (float x = -15; x < 15; x++)
		{
			for (float y = -15; y < 15; y++)
			{
				for (float z = -15; z < 15; z++)
				{
					objects.Add(new Sphere(vec3(x, y, z), 0.125, diff));
				}
			}
		}

		objectCount = (int)objects.Objects.size();
		primitives->AddLight(new PointLight(vec3(0.0f, 0.5f, 0.0f), vec3(1.0f, 1.0f, 1.0f), 250.0f));
		primitives->Add(new BVHNode(objects));
	}

	void DrawCarpet(PrimitiveList* primitives, Material* mat, int x, int y, int w, int h)
	{
		if (w > 2 && h && 2)
		{
			int w1 = w / 3;
			int h1 = h / 3;

			primitives->Add(new Sphere(vec3(float(x + w1 + w1), float(y + h1 + h1), 0.0f), 1.0f, mat));

			for (int k = 0; k < 9; k++) if (k != 4)
			{
				int i = k / 3;
				int j = k % 3;

				DrawCarpet(primitives, mat, x + i * w1, y + j * h1, h1, w1);
			}
		}
	}

	void SceneLoader::SierpinskiCarpet(PrimitiveList* primitives, int& objectCount)
	{
		Material* diffuse = new Material(Color(0.5f, 0.5f, 0.5f));
		diffuse->SetDiffuse(1.0f, 0.0f);
		primitives->AddMaterial(diffuse);

		PrimitiveList objects;
		float p = powf(3, 6);
		DrawCarpet(&objects, diffuse, int(-p / 2.0f), int(-p / 2.0f), int(p), int(p));

		objectCount = (int)objects.Objects.size();
		primitives->Add(new BVHNode(objects));
		primitives->AddLight(new PointLight(vec3(0.0f, 0.0f, 100.0f), Color(1.0f, 1.0f, 1.0f), 50.0f));
	}

	struct MengerCube
	{
		vec3 position;
		float radius;

		MengerCube(vec3 p, float r)
		{
			position = p;
			radius = r;
		}

		void Generate(std::vector<MengerCube>* list, vec3 position, float radius, float depth)
		{
			if (depth > 2)
			{
				list->clear();
			}

			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					for (int k = -1; k < 2; k++)
					{
						int sum = abs(i) + abs(j) + abs(k);

						float r = radius / 3.0f;
						vec3 p
						{
							position.x + i * r,
							position.y + j * r,
							position.z + k * r
						};

						if (sum > 1)
						{
							if (depth > 0)
							{
								Generate(list, p, r, depth - 1);
							}
							else
							{
								list->emplace_back(p, r);
							}
						}
						else
						{
							// Object is an empty space
							list->emplace_back(p, -1.0f);
						}
					}
				}
			}
		}
	};

	void SceneLoader::MengerSponge(PrimitiveList* primitives, int& objectCount)
	{
		PrimitiveList objects;
		std::vector<MengerCube> cubes;
		MengerCube baseCube{ vec3(0.0f, 0.0f, 0.0f), 10.0f };

		baseCube.Generate(&cubes, baseCube.position, baseCube.radius, 3);
		Material* m = new Material(Color(1.0f, 1.0f, 1.0f));
		m->SetDiffuse(1.0f, 0.0f, 35.0f);
		primitives->AddMaterial(m);

		for (unsigned int i = 0; i < cubes.size(); i++)
		{
			if (cubes[i].radius != -1.0f)
			{
				objects.Add(new Sphere(cubes[i].position, cubes[i].radius * 0.50f, m));
			}
		}

		objectCount = (int)cubes.size();
		primitives->Add(new BVHNode(objects));
		primitives->AddLight(new PointLight(Point3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.1f, 0.1f), 100.0f));
	}
}