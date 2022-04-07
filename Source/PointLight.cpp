#include "precomp.h"
#include "PointLight.h"

namespace SLL
{
	PointLight::PointLight(vec3 pos, Color color, float intensity) : position(pos)
	{
		lightColor = color;
		LightIntensity = intensity;
	}

	vec3 PointLight::GetPosition()
	{
		return position;
	}
}