#pragma once

#include "Light.h"

namespace SLL
{
	class AreaLight : public Light
	{
	public:
		~AreaLight() {}
		AreaLight(vec3 posMin, vec3 posMax, Color color, float intensity);
		
		virtual vec3 GetPosition() override;

	private:
		vec3 min;
		vec3 max;
	};
}