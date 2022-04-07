#include "precomp.h"
#include "AreaLight.h"

namespace SLL
{
	AreaLight::AreaLight(vec3 posMin, vec3 posMax, Color color, float intensity) : min(posMin), max(posMax)
	{
		lightColor = color;
		LightIntensity = intensity;
	};

	vec3 AreaLight::GetPosition()
	{
		return vec3(RandomInRange(min.x, max.x), RandomInRange(min.y, max.y), RandomInRange(min.z, max.z));
	}
}