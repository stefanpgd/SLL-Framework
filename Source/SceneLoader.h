#pragma once

namespace SLL
{
	struct CameraSettings;
	class PrimitiveList;
	enum class Skydome;

	enum class SceneIndex
	{
		MaterialDisplay,
		GlassPyramid,
		SphereFlake,
		BVH,
		InfinityRoom,
		SierpinskiCarpet,
		MengerSponge,
	};

	struct SceneSettings
	{
		SceneIndex SceneIndex = SceneIndex::MaterialDisplay;
		PrimitiveList* Primitives;
		char* SceneName = "- NO NAME -";
		char* SceneDescription1 = "- PLACEHOLDER -";
		char* SceneDescription2 = "- PLACEHOLDER -";
		int ObjectCount = 0;
		Skydome PreferredSkydome;
	};

	inline void operator++(SceneIndex& state, int)
	{
		const int i = int(state) + 1;

		if (i >= 7)
		{
			state = SceneIndex::MaterialDisplay;
		}
		else
		{
			state = SceneIndex(i);
		}
	}

	class SceneLoader
	{
	public:
		SceneLoader() {};

		void LoadScene(SceneSettings& sceneSettings,
			CameraSettings& cameraSettings);
	private:
		void MaterialDisplay(PrimitiveList* primitives, int& objectCount);
		void GlassPyramid(PrimitiveList* primitives, int& objectCount);
		void FrostedGlassPyramid(PrimitiveList* primitives, int& objectCount);
		void SphereFlake(PrimitiveList* primitives, int& objectCount);
		void BVH(PrimitiveList* primitives, int& objectCount);
		void InfinityRoom(PrimitiveList* primitives, int& objectCount);
		void SierpinskiCarpet(PrimitiveList* primitives, int& objectCount);
		void MengerSponge(PrimitiveList* primitives, int& objectCount);
	};
}