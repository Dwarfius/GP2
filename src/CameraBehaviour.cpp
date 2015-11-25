#include "CameraBehaviour.h"
#include "Input.h"
#include "GameObject.h"
float baseSpeed;
float boostSpeed;
CameraBehaviour::CameraBehaviour(Camera *c, float tS)
{
	camera = c;
	speed = tS;
	baseSpeed = speed;
	boostSpeed = baseSpeed * 4;
	mouseSensitivity = 0.2f;
}

CameraBehaviour::~CameraBehaviour()
{
}

void CameraBehaviour::Update(float deltaTime)
{
	vec3 forward = camera->GetForward();
	vec3 right = camera->GetRight();
	vec3 up = camera->GetUp();
	if (Input::GetKey(SDLK_w))
		camera->Translate(forward * deltaTime * speed);
	if (Input::GetKey(SDLK_s))
		camera->Translate(-forward * deltaTime * speed);
	if (Input::GetKey(SDLK_d))
		camera->Translate(right * deltaTime * speed);
	if (Input::GetKey(SDLK_a))
		camera->Translate(-right * deltaTime * speed);
	if (Input::GetKey(SDLK_q))
		camera->Translate(-up * deltaTime * speed);
	if (Input::GetKey(SDLK_e))
		camera->Translate(up * deltaTime * speed);

	ivec2 deltaPos = Input::GetMouseDelta();
	camera->Rotate((float)deltaPos.x * mouseSensitivity, (float)-deltaPos.y * mouseSensitivity);
	
	if (Input::GetKey(SDLK_LSHIFT))
		speed = boostSpeed;
	else speed = baseSpeed;
}