#pragma once
#include "Texture.h"

namespace SLL
{
	class CheckerTexture : public Texture
	{
	public:
		CheckerTexture() {}
		CheckerTexture(Texture* even, Texture* odd);
		CheckerTexture(Color c1, Color c2);
		~CheckerTexture();

		virtual Color Value(float u, float v, const Point3& point) const override;

	private:
		Texture* pOdd;
		Texture* pEven;
	};
}
