#include "Camera.h"

Camera::Camera()
{
	yaw = -90; 
	pitch = 0;
	pos = vec3(0, 0, 10);
	up = vec3(0, 1, 0);
}

Camera::~Camera()
{
}

void Camera::Recalculate()
{
	if (pitch > 89.f) //setting top/bottom boundaries
		pitch = 89.f;
	else if (pitch < -89.f)
		pitch = -89.f;

	float pitchRad = radians(pitch);
	float yawRad = radians(yaw);

	forward.x = cos(yawRad) * cos(pitchRad);
	forward.y = sin(pitchRad);
	forward.z = sin(yawRad) * cos(pitchRad);
	forward = normalize(forward);

	right = normalize(cross(vec3(0, 1, 0), -forward));
	up = cross(-forward, right);

	projMatrix = perspective(45.f, 640.f / 480.f, 0.1f, 100.f);
	viewMatrix = lookAt(pos, pos + forward, up);

	VP = projMatrix * viewMatrix;
}