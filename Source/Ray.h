#pragma once

namespace SLL
{
	class Ray
	{
	public:
		Ray() {}
		Ray(const vec3& origin, vec3& direction);

		vec3 At(float t) const;

	public:
		vec3 Origin;
		vec3 Direction;
	};
}