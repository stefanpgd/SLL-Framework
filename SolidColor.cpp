#include "precomp.h"
#include "SolidColor.h"

namespace SLL
{
	SolidColor::SolidColor(Color c) : colorValue(c) {}

	SolidColor::SolidColor(float red, float blue, float green) : SolidColor(Color(red, green, blue)) {}

	Color SolidColor::Value(float u, float v, const vec3& point) const
	{
		return colorValue;
	}
}