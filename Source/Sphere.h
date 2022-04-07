#pragma once

#include "precomp.h"
#include "Primitive.h"
#include "AABB.h"

namespace SLL
{
	class Sphere : public Primitive
	{
	public:
		Sphere() {}
		Sphere(Point3 center, float radius, Material* matPtr);
		~Sphere();

		virtual bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const override;
		virtual bool BoundingBox(AABB& outputBox) const override;

	private:
		Point3 center;
		float radius;
		float radius2;
		float inverseRadius;
		Material* pMaterial;
	};
}