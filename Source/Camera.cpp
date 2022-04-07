#include "precomp.h"
#include "Camera.h"

namespace SLL
{
	Camera::Camera(CameraSettings& settings)
	{
		UpdateCamera(settings);
	}

	Ray Camera::GetRay(float x, float y) const
	{
		return Ray(origin, lowerLeftCorner + x * horizontal + y * vertical - origin);
	}

	void Camera::UpdateCamera(CameraSettings& settings)
	{
		// Refer to page 3 in Ray tracing Notes for full explanation with images.
		// angle of our viewport(height)
		float theta = settings.FOV * DegToRad;

		// divide the angle by two, resulting in a half, which gives us a angle for a triangle.
		// now we can calculate height from the (-z) axis to top of the viewport.
		// height ( opposite ) = tan(angle / adjacent ( -z = 2.0f ) ) 
		float h = tanf(theta / 2.0f);

		// with the height from the middle to top ( h ) we can calculate the viewportHeight
		float viewportHeight = h * 2.0f;

		// with the viewportHeight we can calculate the viewportWidth by using our aspect ratio
		float viewportWidth = settings.AspectRatio * viewportHeight;

		// Creates a unit-vector pointing away from the lookAt.
		// W is positive, following the right-handed coordinate system.
		w = (settings.LookFrom - target).Normalized();

		// creates a perpendicular vector to both vup & w, resulting in the +X of the camera view.
		u = (cross(settings.Orientation, w)).Normalized();

		// with having the +Z and +X we can create the +Y for our camera view.
		v = cross(w, u);

		origin = settings.LookFrom;
		horizontal = viewportWidth * u;
		vertical = viewportHeight * v;
		lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - w;
	}

	void Camera::MoveCamera(CameraSettings& settings, const vec3& displacement)
	{
		// Lookat setup
		vec3 lookAt = settings.LookFrom;
		lookAt.x += cosf(settings.CameraAngle * DegToRad) * settings.TargetDistance;
		lookAt.z += sinf(settings.CameraAngle * DegToRad) * settings.TargetDistance;

		settings.CameraTilt = Clamp(settings.CameraTilt, -settings.MaxTiltUp, settings.MaxTiltUp);
		lookAt.y += settings.CameraTilt;
		target = lookAt;

		float forwardForce = displacement.z;
		float sidewaysForce = displacement.x;

		vec3 camForwardDir = settings.LookFrom - target;
		camForwardDir.Normalize();

		vec3 camSidewaysDir = cross(vec3(0.0f, 1.0f, 0.0f), camForwardDir);
		camSidewaysDir.Normalize();

		settings.LookFrom += camForwardDir * forwardForce;
		settings.LookFrom += camSidewaysDir * sidewaysForce;
	}
}
