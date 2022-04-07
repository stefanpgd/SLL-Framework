#pragma once

#include "Primitive.h"

namespace SLL
{
	class Ellipsoid : public Primitive
	{
	public:
		Ellipsoid() {}
		Ellipsoid(Point3 center, vec3 ellipoidDim, float radius, Material* matPtr);
		~Ellipsoid();

		virtual bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const override;
		virtual bool BoundingBox(AABB& outputBox) const override;

	private:
		Point3 center;
		vec3 ellipsoidScale;
		float radius;
		Material* pMaterial;
	};
}