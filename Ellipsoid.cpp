#include "precomp.h"
#include "Ellipsoid.h"
#include "AABB.h"
#include "Material.h"

namespace SLL
{
	Ellipsoid::Ellipsoid(Point3 center, vec3 ellipoidDim, float radius, Material* matPtr)
		: center(center), radius(radius), pMaterial(matPtr)
	{
		ellipsoidScale.x = 1.0f / (radius * ellipoidDim.x);
		ellipsoidScale.y = 1.0f / (radius * ellipoidDim.y);
		ellipsoidScale.z = 1.0f / (radius * ellipoidDim.z);
	};

	Ellipsoid::~Ellipsoid()
	{
		pMaterial = nullptr;
	}

	bool Ellipsoid::Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const
	{
		vec3 ocn = ray.Origin * ellipsoidScale - (center * ellipsoidScale);
		vec3 rdn = ray.Direction * ellipsoidScale;
		float a = dot(rdn, rdn);
		float b = dot(ocn, rdn);
		float c = dot(ocn, ocn);
		float discriminant = b * b - a * (c - 1.0f);

		if (discriminant < 0)
		{
			return false;
		}

		float discriminantSqrt = sqrtf(discriminant);

		// Find the nearest root that lies in the range of tMin and tMax
		float root = (-b - discriminantSqrt) / a;
		if (root < tMin || tMax < root)
		{
			root = (-b + discriminantSqrt) / a;
			if (root < tMin || tMax < root)
			{
				return false;
			}
		}

		rec.t = root;
		rec.Point = ray.At(rec.t);
		vec3 outwardNormal = ((rec.Point - center) * ellipsoidScale * ellipsoidScale).Normalized();
		rec.SetFaceNormal(ray, outwardNormal);
		GetUV(outwardNormal, rec.U, rec.V);
		rec.pMaterial = pMaterial;
		return true;
	}

	bool Ellipsoid::BoundingBox(AABB& outputBox) const
	{
		outputBox = AABB(center - (ellipsoidScale * radius),
			center + (ellipsoidScale * radius));

		return true;
	}
}