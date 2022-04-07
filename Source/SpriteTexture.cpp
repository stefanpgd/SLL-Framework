#include "precomp.h"
#include "SpriteTexture.h"

#pragma once
#include "Texture.h"

namespace SLL
{
	SpriteTexture::SpriteTexture() : width(0), height(0) {}

	SpriteTexture::SpriteTexture(Sprite* img)
	{
		buffer = img->GetBuffer();
		width = img->GetWidth();
		height = img->GetHeight();
	}

	SpriteTexture::~SpriteTexture()
	{
		// Can't delete buffer since sprite may be used for other things
		buffer = nullptr;
	}

	Color SpriteTexture::Value(float u, float v, const Point3& point) const
	{
		if (buffer == nullptr)
		{
			// return Pink debug color if image is null
			return Color(0.0f, 1.0f, 1.0f);
		}

		// Clamp input texture coords to [0,1] x [1,0]
		u = Clamp(u, 0.0f, 1.0f);
		v = 1.0f - Clamp(v, 0.0f, 1.0f);

		int i = (int)(u * width);
		int j = (int)(v * height);

		// Clamp integer mapping, since actual coords should be less than 1.0f
		if (i >= width) i = width - 1;
		if (j >= height) j = height - 1;

		Pixel pixel = buffer[i + j * width];
		int red = (pixel & RedMask) >> 16;
		int green = (pixel & GreenMask) >> 8;
		int blue = (pixel & BlueMask);

		const float colorScale = 1.0f / 255.0f;
		return Color(red * colorScale, green * colorScale, blue * colorScale);
	}
}