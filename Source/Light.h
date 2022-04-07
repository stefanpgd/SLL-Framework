#pragma once

#include "precomp.h"

namespace SLL
{
	class Light
	{
	public:
		virtual ~Light();

		virtual vec3 GetPosition() = 0;
		virtual Color GetColor();

		float LightIntensity;
	protected:
		Color lightColor { 0.0f, 0.0f, 0.0f };
	};
}