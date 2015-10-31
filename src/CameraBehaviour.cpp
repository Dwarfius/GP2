#include "CameraBehaviour.h"
#include "Input.h"

CameraBehaviour::CameraBehaviour(Camera * c)
{
	camera = c;
}

CameraBehaviour::~CameraBehaviour()
{
}

void CameraBehaviour::Update(float deltaTime)
{
	vec3 forward = camera->GetForward();
	vec3 right = camera->GetRight();
	if (Input::GetKey(SDLK_w))
		camera->Translate(forward * deltaTime);
	if (Input::GetKey(SDLK_s))
		camera->Translate(-forward * deltaTime);
	if (Input::GetKey(SDLK_d))
		camera->Translate(right * deltaTime);
	if (Input::GetKey(SDLK_a))
		camera->Translate(-right * deltaTime);

	ivec2 deltaPos = Input::GetMouseDelta();
	camera->Rotate((float)deltaPos.x * deltaTime, (float)-deltaPos.y * deltaTime);
}