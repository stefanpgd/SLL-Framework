#pragma once
#include "Texture.h"

namespace SLL
{
	class CircleTexture : public Texture
	{
	public:
		CircleTexture() {}
		CircleTexture(Texture* even, Texture* odd);
		CircleTexture(Color c1, Color c2);
		~CircleTexture();

		virtual Color Value(float u, float v, const Point3& point) const override;

	private:
		Texture* pOdd;
		Texture* pEven;
	};
}
