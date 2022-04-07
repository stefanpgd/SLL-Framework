#pragma once

namespace SLL
{
	class PrimitiveList;
	class SpriteTexture;

	enum class ScreenFilter
	{
		Normal,
		Dots,
		TV,
		Matrix,
		Lines
	};

	static const char* filterNames[] =
	{
		"None Active",
		"Dots",
		"Old-TV",
		"The Matrix",
		"Lines"
	};

	inline void operator++(ScreenFilter& state, int)
	{
		const int i = int(state) + 1;

		if (i >= 5)
		{
			state = ScreenFilter::Normal;
		}
		else
		{
			state = ScreenFilter(i);
		}
	}

	void WriteColor(Color* sourceBuffer, Tmpl8::Pixel* destinationBuffer, int x, int y, int imageWidth, int samplesPerPixel, ScreenFilter filter, bool doTonemapping, float exposure);
	Color RayColor(const Ray& ray, const SpriteTexture& background, const PrimitiveList* world, int depth, int startDepth, bool normalView, bool& speedup);
	vec3 GetSkydomeColor(vec3 direction, const SpriteTexture& skydome);
	vec3 GetSkydomeColor(vec3 direction, const Color& skydome);
}