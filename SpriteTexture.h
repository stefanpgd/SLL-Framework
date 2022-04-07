#pragma once
#include "Texture.h"

namespace SLL
{
	class SpriteTexture : public Texture
	{
	public:
		SpriteTexture();
		SpriteTexture(Sprite* img);
		~SpriteTexture();

		virtual Color Value(float u, float v, const Point3& point) const override;

	private:
		Pixel* buffer;
		int width;
		int height;
	};
}
