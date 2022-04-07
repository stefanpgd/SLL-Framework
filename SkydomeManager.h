#pragma once
#include "SpriteTexture.h"

namespace SLL
{
	class Texture;
	class Tmpl8::Sprite;

	enum class Skydome
	{
		Black,
		White,
		SimpleSky,
		Rainbow,
		Studio,
		Pool,
		Mountain,
		Galaxy,
		Snow,
		Cat
	};

	static const char* skydomeNames[] =
	{
		"Black",
		"White",
		"Simple Sky",
		"Rainbow",
		"Studio",
		"Pool",
		"Mountain",
		"Galaxy",
		"Snow",
		"Cat"
	};

	inline void operator++(Skydome& state, int)
	{
		const int i = int(state) + 1;

		if (i >= 10)
		{
			state = Skydome::Black;
		}
		else
		{
			state = Skydome(i);
		}
	}

	class SkydomeManager
	{
	public:
		void SetSkydome(Skydome skydome);
		SpriteTexture* GetSkydome();
		Skydome GetSkydomeName();
		SpriteTexture* GetNextSkydome();

	private:
		Skydome activeSkydome = Skydome::Black;

		Tmpl8::Sprite black{ new Surface("assets/skybox_black.jpg"), 1 };
		Tmpl8::Sprite white{ new Surface("assets/skybox_white.jpg"), 1 };
		Tmpl8::Sprite simpleSky{ new Surface("assets/skybox_gradient.jpg"), 1 };
		Tmpl8::Sprite rainbow{ new Surface("assets/skybox_rainbow.jpg"), 1 };
		Tmpl8::Sprite studio{ new Surface("assets/skybox_studio.jpg"), 1 };
		Tmpl8::Sprite pool{ new Surface("assets/skybox_pool.jpg"), 1 };
		Tmpl8::Sprite mountain{ new Surface("assets/skybox_mountain.jpg"), 1 };
		Tmpl8::Sprite galaxy{ new Surface("assets/skybox_galaxy2.jpg"), 1 };
		Tmpl8::Sprite snow{ new Surface("assets/skybox_snow.jpg"), 1 };
		Tmpl8::Sprite cat{ new Surface("assets/skybox_cat.jpg"), 1 };

		SpriteTexture skydomes[10]
		{
			SpriteTexture(&black),
			SpriteTexture(&white),
			SpriteTexture(&simpleSky),
			SpriteTexture(&rainbow),
			SpriteTexture(&studio),
			SpriteTexture(&pool),
			SpriteTexture(&mountain),
			SpriteTexture(&galaxy),
			SpriteTexture(&snow),
			SpriteTexture(&cat)
		};
	};
}