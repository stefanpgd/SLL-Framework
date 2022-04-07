#include "precomp.h"
#include "SkydomeManager.h"

namespace SLL
{
	void SkydomeManager::SetSkydome(Skydome skydome)
	{
		activeSkydome = skydome;
	}

	SpriteTexture* SkydomeManager::GetSkydome()
	{
		return &skydomes[int(activeSkydome)];
	}

	Skydome SkydomeManager::GetSkydomeName()
	{
		return activeSkydome;
	}

	SpriteTexture* SkydomeManager::GetNextSkydome()
	{
		activeSkydome++;
		return &skydomes[int(activeSkydome)];
	}
}