#include "precomp.h"
#include "Light.h"

namespace SLL
{
	Light::~Light() {}

	Color Light::GetColor()
	{
		return lightColor;
	}
}