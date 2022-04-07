#include "precomp.h"
#include "CheckerTexture.h"
#include "SolidColor.h"

namespace SLL
{
	CheckerTexture::CheckerTexture(Texture* even, Texture* odd) : pEven(even), pOdd(odd) {}
	CheckerTexture::CheckerTexture(Color c1, Color c2) : pEven(new SolidColor(c1)), pOdd(new SolidColor(c2)) {}

	CheckerTexture::~CheckerTexture()
	{
		delete pOdd;
		delete pEven;
		pOdd = nullptr;
		pOdd = nullptr;
	}

	Color CheckerTexture::Value(float u, float v, const Point3& point) const
	{
		float sum = sinf(10.0f * point.x) * sinf(10.0f * point.y) * sinf(10.0f * point.z);

		if (sum < 0)
		{
			return pOdd->Value(u, v, point);
		}
		else
		{
			return pEven->Value(u, v, point);
		}
	}
}