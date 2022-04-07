#include "precomp.h"
#include "PrimitiveList.h"
#include "AABB.h"
#include <memory>
#include <vector>
#include "Material.h"

namespace SLL
{
	bool PrimitiveList::Hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
	{
		HitRecord tempRecord;
		bool hitAnything = false;
		float closestToMax = tMax;

		for (const Primitive* object : Objects)
		{
			if (object->Hit(ray, tMin, closestToMax, tempRecord))
			{
				hitAnything = true;
				closestToMax = tempRecord.t;
				record = tempRecord;
			}
		}

		return hitAnything;
	}

	bool PrimitiveList::IsLightReachable(const Ray& ray, float tMin, float tMax) const
	{
		HitRecord tempRecord;

		for (const Primitive* object : Objects)
		{
			if (object->Hit(ray, tMin, tMax, tempRecord))
			{
				return false;
			}
		}

		return true;
	}

	bool PrimitiveList::BoundingBox(AABB& outputBox) const
	{
		if (Objects.empty())
		{
			return false;
		}

		AABB tempBox;
		bool firstBox = true;

		for (const Primitive* object : Objects)
		{
			if (!object->BoundingBox(tempBox)) return false;

			outputBox = firstBox ? tempBox : SurroundingBox(outputBox, tempBox);
			firstBox = false;
		}

		return true;
	}
}