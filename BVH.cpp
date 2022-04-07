#include "precomp.h"
#include "BVH.h"
#include "AABB.h"

namespace SLL
{
#pragma region Partitioning 
	inline bool SortAxis(const Primitive* a, const Primitive* b, int axis) {
		AABB boxA;
		AABB boxB;

		a->BoundingBox(boxA);
		b->BoundingBox(boxB);

		return boxA.GetMin().cell[axis] < boxB.GetMin().cell[axis];
	}

	inline bool SortX(const Primitive* a, const Primitive* b) {
		return SortAxis(a, b, 0);
	}

	inline bool SortY(const Primitive* a, const Primitive* b) {
		return SortAxis(a, b, 1);
	}

	inline bool SortZ(const Primitive* a, const Primitive* b) {
		return SortAxis(a, b, 2);
	}
#pragma endregion

	BVHNode::BVHNode() : pLeftPrimitive(nullptr), pRightPrimitive(nullptr) {}
	BVHNode::BVHNode(const PrimitiveList& list) : BVHNode(list.Objects, 0, (int)list.Objects.size()) {}

	BVHNode::BVHNode(const std::vector<Primitive*>& primitives, int primitiveIndex, int count)
	{
		std::vector<Primitive*> objects = primitives; // Create a modifiable array of the source scene objects

		int axis = rand() % 3;
		auto partition = (axis == 0) ? SortX : (axis == 1) ? SortY : SortZ; 

		int prims = count - primitiveIndex;

		// if count is less than 3, it means the BVH is a leaf node
		if (prims < 3)
		{
			if (prims == 1)
			{
				singlePrim = true;
				pLeftPrimitive = objects[primitiveIndex];
				pRightPrimitive = objects[primitiveIndex];
			}
			else
			{
				// the sorted primitive is always + 1 in the source pool of primitives - Jacco
				if (partition(objects[primitiveIndex], objects[primitiveIndex + 1])) {
					pLeftPrimitive = objects[primitiveIndex];
					pRightPrimitive = objects[primitiveIndex + 1];
				}
				else {
					pLeftPrimitive = objects[primitiveIndex + 1];
					pRightPrimitive = objects[primitiveIndex];
				}
			}
		}
		else 
		{
			// Std::sort is in most cases faster than std::qsort
			std::sort(objects.begin() + primitiveIndex, objects.begin() + count, partition);

			auto mid = primitiveIndex + prims / 2;
			pLeftPrimitive = new BVHNode(objects, primitiveIndex, mid);
			pRightPrimitive = new BVHNode(objects, mid, count);
		}

		AABB leftBox, rightBox;
		pLeftPrimitive->BoundingBox(leftBox);
		pRightPrimitive->BoundingBox(rightBox);
		bounds = SurroundingBox(leftBox, rightBox);
	}

	BVHNode::~BVHNode()
	{
		if (pLeftPrimitive == pRightPrimitive)
		{
			delete pLeftPrimitive;
			pLeftPrimitive = nullptr;
			pRightPrimitive = nullptr;
		}
		else
		{
			delete pLeftPrimitive;
			delete pRightPrimitive;
			pLeftPrimitive = nullptr;
			pRightPrimitive = nullptr;
		}
	}

	bool BVHNode::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
		if (!bounds.Hit(r, tMin, tMax))
		{
			return false;
		}
		
		if (singlePrim)
		{
			return pLeftPrimitive->Hit(r, tMin, tMax, rec);
		}
		
		bool hitLeft = pLeftPrimitive->Hit(r, tMin, tMax, rec);
		float tmax = hitLeft ? rec.t : tMax;
		bool hitRight = pRightPrimitive->Hit(r, tMin, tmax, rec);

		return (hitLeft || hitRight);
	}

	bool BVHNode::BoundingBox(AABB& output) const {
		output = bounds;
		return true;
	}
}