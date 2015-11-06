#ifndef _CAMERA_H
#define _CAMERA_H

#include "Common.h"

class Camera
{
private:
	const GLfloat sensX = 1, sensY = 1;
	GLfloat yaw, pitch;
	vec3 pos, up, forward, right;
	mat4 projMatrix, viewMatrix, VP;

public:
	Camera();
	~Camera();

	mat4 Get() { return VP; }

	vec3 GetForward() { return forward; }
	vec3 GetRight() { return right; }
	vec3 GetUp() { return up; }

	vec3 GetPos() { return pos; }
	void Translate(vec3 delta) { pos += delta; }
	void SetPos(vec3 newPos) { pos = newPos; }

	void LookAt(vec3 target) { forward = normalize(target - pos); }

	void Rotate(GLfloat deltaYaw, GLfloat deltaPitch) { yaw += deltaYaw * sensX; pitch += deltaPitch * sensY; }

	void Recalculate();
};
#endif