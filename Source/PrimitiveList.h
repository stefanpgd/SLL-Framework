#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Material.h"

namespace SLL
{
	struct HitRecord;

	class PrimitiveList : public Primitive
	{
	public:
		PrimitiveList() {}

		void Clear()
		{
			if (Objects.size() > 0)
			{
				for (unsigned int i = 0; i < Objects.size(); i++)
				{
					Primitive* obj = Objects[i];
					delete obj;
					Objects[i] = nullptr;
				}

				Objects.clear();
			}

			if (Materials.size() > 0)
			{
				for (unsigned int i = 0; i < Materials.size(); i++)
				{
					Material* material = Materials[i];
					delete material;
					Materials[i] = nullptr;
				}

				Materials.clear();
			}

			if (Lights.size() > 0)
			{
				for (unsigned int i = 0; i < Lights.size(); i++)
				{
					Light* obj = Lights[i];
					delete obj;
					Lights[i] = nullptr;
				}

				Lights.clear();
			}
		}

		void Add(Primitive* object) { Objects.push_back(object); }
		void AddLight(Light* object) { Lights.push_back(object); }
		void AddMaterial(Material* object) { Materials.push_back(object); }

		virtual bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
		virtual bool BoundingBox(AABB& outputBox) const override;
		bool IsLightReachable(const Ray& ray, float tMin, float tMax) const;

	public:
		std::vector<Primitive*> Objects;
		std::vector<Light*> Lights;
		std::vector<Material*> Materials;
	};
}