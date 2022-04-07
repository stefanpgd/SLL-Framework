#include "precomp.h"

namespace SLL
{
	Ray::Ray(const vec3& origin, vec3& direction) :
		Origin(origin), Direction(direction.Normalized()) {}
	
	vec3 Ray::At(float t) const
	{
		// Simple lerp ( A + tB )
		return Origin + t * Direction;
	}
}