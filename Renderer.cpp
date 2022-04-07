#include "precomp.h"
#include "Renderer.h"

namespace SLL
{
	void Renderer::Setup(Tmpl8::Surface* targetScreen)
	{
		sceneSettings.SceneIndex = SceneIndex::MaterialDisplay;
		screen = targetScreen;
		screenBuffer = targetScreen->GetBuffer();

		sceneSettings.Primitives = new PrimitiveList();
		sceneLoader.LoadScene(sceneSettings, cameraSettings);
		skydomeManager.SetSkydome(sceneSettings.PreferredSkydome);
		pActiveSkydome = skydomeManager.GetSkydome();

		imageHeight = (int)(imageWidth / cameraSettings.AspectRatio);
		screenPixelCount = imageHeight * imageWidth;
		camera = Camera(cameraSettings);

		deadzoneX.x = imageWidth / 2 - deadzoneSize;
		deadzoneX.y = imageWidth / 2 + deadzoneSize;
		deadzoneY.x = imageHeight / 2 - deadzoneSize;
		deadzoneY.y = imageHeight / 2 + deadzoneSize;

		screenBuffer = targetScreen->GetBuffer();
		colorBuffer = new Color[imageWidth * imageHeight];
		speedupBuffer = new bool[imageWidth * imageHeight];

		for (int i = 0; i < imageWidth * imageHeight; i++)
		{
			colorBuffer[i] = Color(0.0f, 0.0f, 0.0f);
			speedupBuffer[i] = false;
		}

		tileCountX = imageWidth / TILE_SIZE;
		tileCountY = imageHeight / TILE_SIZE;
		totalTiles = tileCountX * tileCountY;
		tiles = new ScreenTile[totalTiles];
		threadsAvailable = std::thread::hardware_concurrency();
		threads = new std::thread[threadsAvailable];
		workIndex.store(totalTiles - 1);

		for (unsigned int y = 0; y < tileCountY; y++)
		{
			for (unsigned int x = 0; x < tileCountX; x++)
			{
				tiles[x + y * tileCountY].X = x * TILE_SIZE;
				tiles[x + y * tileCountY].Y = y * TILE_SIZE;
				tiles[x + y * tileCountY].XMax = x * TILE_SIZE + TILE_SIZE;
				tiles[x + y * tileCountY].YMax = y * TILE_SIZE + TILE_SIZE;
			}
		}

		iterationTime.reset();

		offsetX = Random01();
		offsetY = Random01();

		for (int i = 0; i < threadsAvailable; i++)
		{
			threads[i] = std::thread([this] {RayTraceTile(); });
		}
	}

	void Renderer::Shutdown()
	{
		isRaytracing = false;
		iterationLock.notify_all();
		for (int i = 0; i < threadsAvailable; i++)
		{
			threads[i].join();
		}
	}

	void Renderer::Update(float deltaTime)
	{
		targetIterationReached = currentIteration >= maxIteration;

		if (workIndex < 0 && !targetIterationReached)
		{
			bool isDoneWithIteration = true;

			for (unsigned int i = 0; i < totalTiles; i++)
			{
				if (tiles[i].State != TileState::Done)
				{
					isDoneWithIteration = false;
				}
			}

			if (isDoneWithIteration)
			{
				for (int y = 0; y < imageHeight; y++)
				{
					for (int x = 0; x < imageWidth; x++)
					{
						WriteColor(colorBuffer, screenBuffer, x, ((imageHeight - 1) - y), imageWidth, currentIteration, screenFilter, tonemapping, exposure);
					}
				}

				if (loadNextScene)
				{
					LoadNextScene();
					return;
				}
				else if (worldUpdated)
				{
					ResetWorld();
				}
				else
				{
					currentIteration++;
					lastIterationTime = iterationTime.elapsed();
					averageMS += lastIterationTime;
				}

				SetupNextIteration();
			}
		}

		//If the target iteration is reached it only needs to check if the world/scene has changed
		if (targetIterationReached)
		{
			if (loadNextScene)
			{
				LoadNextScene();
				SetupNextIteration();
				return;
			}
			else if (worldUpdated)
			{
				ResetWorld();
				SetupNextIteration();
			}
		}

		CameraMovementInput(deltaTime);
	}

	void Renderer::KeyUpInput(int key)
	{
		switch (key)
		{
		case SDL_SCANCODE_N:
			loadNextScene = true;
			break;

		case SDL_SCANCODE_F:
			screenFilter++;
			break;

		case SDL_SCANCODE_I:
			maxRayDepth++;
			worldUpdated = true;
			break;

		case SDL_SCANCODE_U:
			if (maxRayDepth > 1)
			{
				maxRayDepth--;
				worldUpdated = true;
			}
			break;

		case SDL_SCANCODE_V:
			normalView = !normalView;
			worldUpdated = true;
			break;

		case SDL_SCANCODE_B:
			skydomeUpdated = true;
			worldUpdated = true;
			break;

		case SDL_SCANCODE_T:
			tonemapping = !tonemapping;
			break;

		case SDL_SCANCODE_L:
			exposure += 0.1f;
			break;

		case SDL_SCANCODE_K:
			if (exposure > 0.1f)
			{
				exposure -= 0.1f;
			}
			break;
		}
	}

	void Renderer::RayTraceTile()
	{
		while (isRaytracing)
		{
			int index = workIndex.fetch_sub(1);

			if (index < 0)
			{
				std::unique_lock<std::mutex> lock(rayLock);
				iterationLock.wait(lock);
				continue;
			}

			if (tiles[index].State != TileState::ToDo)
			{
				continue;
			}

			tiles[index].State = TileState::Processing;

			for (unsigned int y = tiles[index].Y; y < tiles[index].YMax; y++)
			{
				for (unsigned int x = tiles[index].X; x < tiles[index].XMax; x++)
				{
					int screenIndex = x + ((imageHeight - 1) - y) * imageWidth;

					float xOffset = (float(x) + offsetX) / (imageWidth - 1);
					float yOffset = (float(y) + offsetY) / (imageHeight - 1);
					
					Ray ray = camera.GetRay(xOffset, yOffset);

#if 0
					bool hitsBackground = speedupBuffer[screenIndex];
					int rayIndex = currentIteration % 10;

					if (!hitsBackground || rayIndex == 0)
					{
						colorBuffer[screenIndex] += RayColor(ray, *pActiveSkydome, sceneSettings.Primitives, maxRayDepth, maxRayDepth, normalView, hitsBackground);
					}
					else
					{
						colorBuffer[screenIndex] += GetSkydomeColor(ray.Direction, *pActiveSkydome);
					}

					speedupBuffer[screenIndex] = hitsBackground;
#else
					bool temp = false;
					colorBuffer[screenIndex] += RayColor(ray, *pActiveSkydome, sceneSettings.Primitives, maxRayDepth, maxRayDepth, normalView, temp);
#endif
				}
			}

			tiles[index].State = TileState::Done;
		}
	}

	void Renderer::CameraMovementInput(float deltaTime)
	{
		// Camera Movement
		vec3 cameraDisplacement{ 0.0f, 0.0f, 0.0f };
		float cameraTilt = 0.0f;
		float cameraAngle = 0.0f;
		const Uint8* keystates = SDL_GetKeyboardState(NULL);
		int mouseX = 0;
		int mouseY = 0;
		SDL_GetMouseState(&mouseX, &mouseY);

		int horizontal = keystates[SDL_SCANCODE_D] - keystates[SDL_SCANCODE_A];
		int vertical = keystates[SDL_SCANCODE_S] - keystates[SDL_SCANCODE_W];
		cameraDisplacement.x += horizontal * cameraMovespeed * deltaTime;
		cameraDisplacement.z += vertical * cameraMovespeed * deltaTime;

		int tilt = keystates[SDL_SCANCODE_UP] - keystates[SDL_SCANCODE_DOWN];
		cameraTilt += tilt * cameraRotationSpeed * deltaTime;

		int angle = keystates[SDL_SCANCODE_E] - keystates[SDL_SCANCODE_Q];
		cameraAngle += angle * (cameraRotationSpeed * 10.0f) * deltaTime;

		if (cameraDisplacement.SqrLength() > 0.0f ||
			cameraAngle != 0.0f || cameraTilt != 0.0f)
		{
			cameraSettings.CameraTilt += cameraTilt;
			cameraSettings.CameraAngle += cameraAngle;
			camera.MoveCamera(cameraSettings, cameraDisplacement);
			worldUpdated = true;
		}
	}

	void Renderer::ScrollWheelInput(int y)
	{
		cameraSettings.FOV -= y;
		worldUpdated = true;
	}

	void Renderer::ResetWorld()
	{
		memset(colorBuffer, 0, imageWidth * imageHeight * sizeof(Color));
		memset(speedupBuffer, 0, imageWidth * imageHeight * sizeof(bool));
		camera.UpdateCamera(cameraSettings);

		if (skydomeUpdated)
		{
			pActiveSkydome = skydomeManager.GetNextSkydome();
			skydomeUpdated = false;
		}

		currentIteration = 1;
		averageMS = 0.1f;
		lastIterationTime = 0.1f;
	}

	void Renderer::SetupNextIteration()
	{
		worldUpdated = false;

		iterationTime.reset();
		sceneRuntime += lastIterationTime;
		DebugWindow();

		for (unsigned int i = 0; i < totalTiles; i++)
		{
			tiles[i].State = TileState::ToDo;
		}

		offsetX = Random01();
		offsetY = Random01();

		workIndex.store(totalTiles - 1);
		iterationLock.notify_all();
	}

	void Renderer::LoadNextScene()
	{
		worldUpdated = false;
		loadNextScene = false;

		memset(screenBuffer, 0, ScreenWidth * ScreenHeight * sizeof(Tmpl8::Pixel));
		memset(colorBuffer, 0, imageWidth * imageHeight * sizeof(Color));
		memset(speedupBuffer, 0, imageWidth * imageHeight * sizeof(bool));

		currentIteration = 1;
		lastIterationTime = 0.0f;
		averageMS = 0.0f;
		sceneRuntime = 0.0f;

		sceneSettings.SceneIndex++;
		sceneLoader.LoadScene(sceneSettings, cameraSettings);
		skydomeManager.SetSkydome(sceneSettings.PreferredSkydome);
		pActiveSkydome = skydomeManager.GetSkydome();
		camera = Camera(cameraSettings);
		iterationTime.reset();

		for (unsigned int i = 0; i < totalTiles; i++)
		{
			tiles[i].State = TileState::ToDo;
		}

		workIndex.store(totalTiles - 1);
		iterationLock.notify_all();
	}

	void Renderer::DebugWindow()
	{
		memset(&screen->GetBuffer()[imageWidth * imageHeight], 0, sizeof(Tmpl8::Pixel) * (ScreenWidth * ScreenHeight - imageWidth * imageHeight - 1));

#ifdef _DEBUG
		screen->Print("DEBUG BUILD ----- DEBUG BUILD ----- DEBUG BUILD ----- DEBUG BUILD ----- DEBUG BUILD", 7, 5, 0xff0000);
#endif //_DEBUG

#pragma region Debug Scene Information
		screen->Print("Scene Information:", 5, imageHeight + 5, 0xffffff);
		screen->Print("Name:", 5, imageHeight + 20, 0xffffff);
		screen->Print(sceneSettings.SceneName, 40, imageHeight + 20, 0xffffff);
		screen->Print("Scene description:", 5, imageHeight + 35, 0xffffff);
		screen->Print(sceneSettings.SceneDescription1, 5, imageHeight + 45, 0x999999);
		screen->Print(sceneSettings.SceneDescription2, 5, imageHeight + 55, 0x999999);

		screen->Print("Screen Size:", 5, imageHeight + 70, 0xffffff);
		PrintNumber(screen, imageWidth, 100, imageHeight + 70);
		screen->Print("X", 125, imageHeight + 70, 0xffffff);
		PrintNumber(screen, imageHeight, 140, imageHeight + 70);

		screen->Print("Screen Filter:", 5, imageHeight + 80, 0xffffff);
		screen->Print(filterNames[int(screenFilter)], 100, imageHeight + 80, 0xffffff);
		
		screen->Print("Tonemapping:", 5, imageHeight + 90, 0xffffff);
		
		if (tonemapping)
		{
			screen->Print("ON", 100, imageHeight + 90, 0xffffff);
		}
		else
		{
			screen->Print("OFF", 100, imageHeight + 90, 0xffffff);
		}

		screen->Print("Skydome:", 5, imageHeight + 100, 0xffffff);
		screen->Print(skydomeNames[int(skydomeManager.GetSkydomeName())], 100, imageHeight + 100, 0xffffff);

		screen->Print("Primitives:", 5, imageHeight + 115, 0xffffff);
		PrintNumber(screen, sceneSettings.ObjectCount, 100, imageHeight + 115);

		screen->Print("Camera Position:", 5, imageHeight + 125, 0xffffff);
		screen->Print("X:", 5, imageHeight + 135, 0xffffff);
		PrintNumber(screen, (int)cameraSettings.LookFrom.x, 20, imageHeight + 135);
		screen->Print("Y:", 50, imageHeight + 135, 0xffffff);
		PrintNumber(screen, (int)cameraSettings.LookFrom.y, 65, imageHeight + 135);
		screen->Print("Z:", 95, imageHeight + 135, 0xffffff);
		PrintNumber(screen, (int)cameraSettings.LookFrom.z, 110, imageHeight + 135);
#pragma endregion

#pragma region Debug Statistics
		screen->Print("Statistics/Settings:", 190, imageHeight + 5, 0xffffff);
		screen->Print("Current FPS:", 190, imageHeight + 20, 0xffffff);
		PrintNumber(screen, int(1000.0f / lastIterationTime), 310, imageHeight + 20);

		screen->Print("Average FPS:", 190, imageHeight + 30, 0xffffff);
		int averageFps = int(1000.0f / (averageMS / currentIteration));
		int fpsColor = 0x00ff00;
		if (averageFps > 10.0f && averageFps < 30.0f)
		{
			fpsColor = 0xffff00;
		}
		else if(averageFps < 10.0f)
		{
			fpsColor = 0xff0000;
		}
		PrintNumber(screen, averageFps, 310, imageHeight + 30, fpsColor);

		screen->Print("Current iteration:", 190, imageHeight + 45, 0xffffff);
		PrintNumber(screen, currentIteration, 310, imageHeight + 45);
		screen->Print("Max Iterations:", 190, imageHeight + 55, 0xffffff);
		PrintNumber(screen, maxIteration, 310, imageHeight + 55);

		screen->Print("Iteration time:", 190, imageHeight + 70, 0xffffff);
		PrintNumber(screen, int(lastIterationTime), 310, imageHeight + 70);
		screen->Print("Average time:", 190, imageHeight + 80, 0xffffff);

		int ms = int(averageMS / currentIteration);
		int msColor = 0x00ff00;
		if (ms > 33.0f && ms < 100.0f)
		{
			msColor = 0xffff00;
		}
		else if(ms > 100.0f)
		{
			msColor = 0xff0000;
		}
		PrintNumber(screen, ms, 310, imageHeight + 80, msColor);

		screen->Print("Normal view:", 190, imageHeight + 100, 0xffffff);
		if (normalView)
		{
			screen->Print("ON", 310, imageHeight + 100, 0xffffff);
		}
		else
		{
			screen->Print("OFF", 310, imageHeight + 100, 0xffffff);
		}

		screen->Print("Rays depth:", 190, imageHeight + 110, 0xffffff);
		PrintNumber(screen, maxRayDepth, 310, imageHeight + 110);
#pragma endregion

#pragma region Debug Controls Information
		screen->Print("Movement Controls:", 355, imageHeight + 5, 0xffffff);
		screen->Print("Movement: WASD", 355, imageHeight + 17, 0xffffff);
		screen->Print("Rotation: Q-E", 355, imageHeight + 27, 0xffffff);
		screen->Print("Tilt:     Arrows UP-DOWN", 355, imageHeight + 37, 0xffffff);

		screen->Print("Scene/Screen Controls:", 355, imageHeight + 58, 0xffffff);
		screen->Print("Next Scene:            N", 355, imageHeight + 70, 0xffffff);
		screen->Print("Next Filter:           F", 355, imageHeight + 80, 0xffffff);
		screen->Print("Next Skydome:          B", 355, imageHeight + 90, 0xffffff);
		screen->Print("Normal View:           V", 355, imageHeight + 100, 0xffffff);
		screen->Print("Increase Ray Depth:    I", 355, imageHeight + 110, 0xffffff);
		screen->Print("Decrease Ray Depth:    U", 355, imageHeight + 120, 0xffffff);
		screen->Print("Zoom:     SCROLL UP-DOWN", 355, imageHeight + 130, 0xffffff);
		screen->Print("Tonemapping:           T", 355, imageHeight + 140, 0xffffff);
#pragma endregion

		screen->Line(180.0f, float(imageHeight + 1), 180.0f, float(ScreenHeight), 0xffffff);
		screen->Line(345, float(imageHeight + 1), 345, float(ScreenHeight), 0xffffff);
	}
}
