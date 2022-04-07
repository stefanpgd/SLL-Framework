#pragma once

namespace SLL
{
	struct CameraSettings
	{
		vec3 LookFrom{ 0.0f, 0.0f, 0.0f };
		vec3 Orientation{ 0.0f, 1.0f, 0.0f };

		float CameraAngle = 90.0f; // rotation of cameraTarget ( x and z axis )
		float CameraTilt = 0.0f;   // Y axis for cameraTarget
		float MaxTiltUp = 5.0f;
		float TargetDistance = -10.0f;
		float AspectRatio = 1.0f;
		float FOV = 60.0f;

		void DefaultSettings()
		{
			LookFrom = vec3(0.0f, 0.0f, 0.0f);
			Orientation = vec3(0.0f, 1.0f, 0.0f);

			CameraAngle = 90.0f;
			CameraTilt = 0.0f;
			MaxTiltUp = 5.0f;
			TargetDistance = -10.0f;
			AspectRatio = 1.0f;
			FOV = 60.0f;
		}
	};

	class Camera
	{
	public:
		Camera() {};
		Camera(CameraSettings& settings);

		Ray GetRay(float x, float y) const;
		void UpdateCamera(CameraSettings& settings);
		void MoveCamera(CameraSettings& cameraSettings, const vec3& displacement);

	private:
		vec3 origin;
		vec3 lowerLeftCorner;
		vec3 horizontal;
		vec3 vertical;
		vec3 u, v, w;
		vec3 target;
	};
}