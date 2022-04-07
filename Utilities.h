#pragma once

namespace SLL
{
	class Vector3;

	inline float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	const float halfPi = 0.5f / PI;
	inline void GetUV(const Point3& point, float& u, float& v)
	{
		float theta = acosf(-point.y);
		float phi = atan2f(-point.z, point.x) + PI;

		u = phi * (PI * 2.0f);
		v = theta * PI;
	}

	inline float Random01()
	{
		return float(rand() / (RAND_MAX + 1.0));
	}

	inline float RandomInRange(float min, float max)
	{
		return min + (max - min) * Random01();
	}

	inline float Sign(float x)
	{
		return float((x > 0.0f) - (x < 0.0f));
	}

	inline int RandomInRange(int min, int max)
	{
		return int(RandomInRange((float)min, float(max + 1.0f)));
	}

	inline vec3 Reflect(const vec3& incident, const vec3& normal)
	{
		// incident - 2 * cosI * normal.
		return incident - 2 * dot(incident, normal) * normal;
	}

	inline vec3 Refract(const vec3& incident, const vec3& normal, float n, float cosI, float cosT)
	{
		return n * incident + (n * cosI - cosT) * normal;
	}

	void PrintNumber(Tmpl8::Surface * screen, int number, int x, int y, Pixel color = 0xffffff);
	void PrintNumber(Tmpl8::Surface* screen, float number, int x, int y, Pixel color = 0xffffff);
	void PrintNumber(Tmpl8::Surface* screen, size_t number, int x, int y, Pixel color = 0xffffff);
}