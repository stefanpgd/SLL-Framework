#pragma once

#include "Color.h"
#include "Camera.h"
#include "SceneLoader.h"
#include "Camera.h"
#include "PrimitiveList.h"
#include "SpriteTexture.h"
#include "SkydomeManager.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <SDL.h>

namespace SLL
{
	class Camera;
	class SceneLoader;
	class PrimitiveList;
	struct CameraSettings;
	struct SceneSettings;
	enum class SceneIndex;

	enum class TileState
	{
		ToDo,
		Processing,
		Done
	};

	struct ScreenTile
	{
		TileState State = TileState::ToDo;
		unsigned int X = 0;
		unsigned int Y = 0;
		unsigned int XMax = 0;
		unsigned int YMax = 0;
	};

	class Renderer
	{
	public:
		Renderer() {};

		void Setup(Tmpl8::Surface * targetScreen);
		void Update(float deltaTime);
		void KeyUpInput(int key);
		void ScrollWheelInput(int y);
		void Shutdown();

	private:
		void RayTraceTile();
		void CameraMovementInput(float deltaTime);
		void LoadNextScene();
		void ResetWorld();
		void DebugWindow();
		void SetupNextIteration();

	private:
		bool isRaytracing = true;
		bool targetIterationReached = false;
		bool worldUpdated = false;
		bool loadNextScene = false; 

		// Image/Window settings
		ScreenFilter screenFilter = ScreenFilter::Normal;
		const int maxIteration = 2500;
		int maxRayDepth = 4;
		int currentIteration = 1;
		int imageWidth = ScreenWidth;
		int imageHeight = 0;
		bool tonemapping = false;
		float exposure = 0.6f;

		Tmpl8::Surface* screen;
		Tmpl8::Pixel* screenBuffer;
		bool* speedupBuffer;
		Color* colorBuffer;

		// Scene
		SceneLoader sceneLoader;
		SceneSettings sceneSettings;
		SkydomeManager skydomeManager;
		SpriteTexture* pActiveSkydome;
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		bool normalView = false;
		bool skydomeUpdated = false;

		// Camera settings
		Camera camera;
		CameraSettings cameraSettings;
		float cameraMovespeed = 10.0f;
		float cameraRotationSpeed = 10.0f;
		float deadzoneSize = 50;
		vec2 deadzoneX;
		vec2 deadzoneY;

		// Multi-threading
		int threadsAvailable;
		ScreenTile* tiles;
		const int TILE_SIZE = 16;
		unsigned int tileCountX;
		unsigned int tileCountY;
		unsigned int totalTiles;
		std::atomic<int> workIndex;
		std::condition_variable iterationLock;
		std::mutex rayLock;
		std::thread* threads;

		// Debugging
		float lastIterationTime = 0.0f;
		float averageMS = 0.0f;
		float sceneRuntime = 0.0f;
		Tmpl8::timer iterationTime;
		int screenPixelCount = 0;
	};
}