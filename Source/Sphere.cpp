#include "precomp.h"
#include "Sphere.h"
#include "Material.h"

namespace SLL
{
	Sphere::Sphere(Point3 center, float radius, Material* matPtr)
		: center(center), radius(radius), pMaterial(matPtr),
		radius2(radius* radius), inverseRadius(1.0f / radius) {};

	Sphere::~Sphere()
	{
		pMaterial = nullptr;
	}

	// testing attention please
	bool Sphere::Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const
	{
		// Sphere intersection equation
		vec3 oc = ray.Origin - center;
		float halfB = dot(oc, ray.Direction);
		float c = oc.SqrLength() - radius2;
		float discriminant = halfB * halfB - c;

		if (discriminant < 0.0f)
		{
			return false;
		}

		float discriminantSqrt = sqrtf(discriminant);

		// Find the nearest root that lies in the range of tMin and tMax
		float root = (-halfB - discriminantSqrt);
		if (root < tMin || tMax < root)
		{
			root = (-halfB + discriminantSqrt);
			if (root < tMin || tMax < root)
			{
				return false;
			}
		}

		rec.t = root;
		rec.Point = ray.At(rec.t);
		vec3 outwardNormal = (rec.Point - center) * inverseRadius;
		rec.SetFaceNormal(ray, outwardNormal);
		GetUV(outwardNormal, rec.U, rec.V);
		rec.pMaterial = pMaterial;

		return true;
	}

	bool Sphere::BoundingBox(AABB& outputBox) const
	{
		outputBox = AABB(center - vec3(radius, radius, radius),
			center + vec3(radius, radius, radius));

		return true;
	}
}