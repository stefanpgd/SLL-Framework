#pragma once

namespace SLL
{
	class AABB;
	class Material;

	struct HitRecord
	{
		Point3 Point;
		vec3 Normal;
		Material* pMaterial = nullptr;
		float t{ 0.0f };
		float U{ 0.0f };
		float V{0.0f};
		bool FrontFace = false;

		inline void SetFaceNormal(const Ray& ray, const vec3& outwardNormal)
		{
			FrontFace = dot(ray.Direction, outwardNormal) < 0;
			Normal = FrontFace ? outwardNormal : -outwardNormal;
		}
	};

	class Primitive
	{
	public:
		virtual ~Primitive();
		virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const = 0;
		virtual bool BoundingBox(AABB& outputBox) const = 0;
	};
}