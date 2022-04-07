#include "precomp.h"
#include "CircleTexture.h"
#include "SolidColor.h"

namespace SLL
{
	CircleTexture::CircleTexture(Texture* even, Texture* odd) : pEven(even), pOdd(odd) {}
	CircleTexture::CircleTexture(Color c1, Color c2) : pEven(new SolidColor(c1)), pOdd(new SolidColor(c2)) {}

	CircleTexture::~CircleTexture()
	{
		delete pOdd;
		delete pEven;
		pOdd = nullptr;
		pEven = nullptr;
	}

	Color CircleTexture::Value(float u, float v, const Point3& point) const
	{
		float sines = tanf(15.0f * point.y);

		if (sines < 0)
		{
			return pOdd->Value(u, v, point);
		}
		else
		{
			return pEven->Value(u, v, point);
		}
	}
}