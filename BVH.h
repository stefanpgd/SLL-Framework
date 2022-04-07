#pragma once

#include "precomp.h"
#include "Primitive.h"
#include "PrimitiveList.h"
#include "AABB.h"
#include <algorithm>

namespace SLL
{
	class BVHNode : public Primitive {
	public:
		BVHNode();
		~BVHNode();

		BVHNode(const PrimitiveList& list);
		BVHNode(const std::vector<Primitive*>& primitives, int primitiveIndex, int count);

		virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
		virtual bool BoundingBox(AABB& output) const override;

	public:
		Primitive* pLeftPrimitive;
		Primitive* pRightPrimitive;
		AABB bounds;
		bool singlePrim = false;
	};

	// Partitioning
	inline bool SortAxis(const Primitive* a, const Primitive* b, int axis);
	inline bool SortX(const Primitive* a, const Primitive* b);
	inline bool SortY(const Primitive* a, const Primitive* b);
	inline bool SortZ(const Primitive* a, const Primitive* b);
}
