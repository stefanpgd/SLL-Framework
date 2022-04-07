#pragma once

#include "Light.h"

namespace SLL
{
	class PointLight : public Light
	{
	public:
		PointLight(vec3 pos, Color color, float intensity);
		~PointLight() {}

		virtual vec3 GetPosition() override;
	private:
		vec3 position;
	};
}