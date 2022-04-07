#pragma once
#include "Texture.h"

namespace SLL
{
	class SolidColor : public Texture
	{
	public:
		SolidColor() {}
		SolidColor(Color c);
		SolidColor(float red, float blue, float green);
		~SolidColor() {}

		virtual Color Value(float u, float v, const vec3& point) const override;

	private:
		Color colorValue;
	};
}
