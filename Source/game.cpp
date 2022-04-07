#include "precomp.h" 
#include "game.h"
#include "Renderer.h"

using namespace SLL;

namespace Tmpl8
{
	Renderer renderer;

	void Game::Init()
	{
		renderer.Setup(screen);
	}

	void Game::Shutdown()
	{
		renderer.Shutdown();
	}

	void Game::Tick(float deltaTime)
	{
		renderer.Update(deltaTime);
	}

	void Game::KeyUp(int key)
	{
		renderer.KeyUpInput(key);
	}

	void Game::MouseWheel(int y)
	{
		renderer.ScrollWheelInput(y);
	}
};