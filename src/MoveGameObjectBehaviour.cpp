#include "MoveGameObjectBehaviour.h"
#include "input.h"
#include "GameObject.h"

MoveGameObjectBehaviour::MoveGameObjectBehaviour()
{

}


MoveGameObjectBehaviour::~MoveGameObjectBehaviour()
{

}

void MoveGameObjectBehaviour::Update(float deltaTime)
{
	if (Input::GetKey(SDLK_r))
		GetParentGO()->AddRotation(vec3(30 * deltaTime, 0, 0));
	if (Input::GetKey(SDLK_t))
		GetParentGO()->AddRotation(vec3(-30 * deltaTime, 0, 0));
	if (Input::GetKey(SDLK_z))
		GetParentGO()->AddRotation(vec3(0, 0, 30 * deltaTime));
	if (Input::GetKey(SDLK_x))
		GetParentGO()->AddRotation(vec3(0, 0, -30 * deltaTime));
	if (Input::GetKey(SDLK_c))
		GetParentGO()->AddRotation(vec3(0, 30 * deltaTime, 0));
	if (Input::GetKey(SDLK_v))
		GetParentGO()->AddRotation(vec3(0, -30 * deltaTime, 0));
	if (Input::GetKey(SDLK_h))
		GetParentGO()->Move(vec3(0, 0, 30 * deltaTime));
	if (Input::GetKey(SDLK_n))
		GetParentGO()->Move(vec3(0, 0, -30 * deltaTime));
	if (Input::GetKey(SDLK_j))
		GetParentGO()->Move(vec3(0, 30 * deltaTime, 0));
	if (Input::GetKey(SDLK_m))
		GetParentGO()->Move(vec3(0, -30 * deltaTime, 0));
	if (Input::GetKey(SDLK_g))
		GetParentGO()->Move(vec3(30 * deltaTime, 0, 0));
	if (Input::GetKey(SDLK_k))
		GetParentGO()->Move(vec3(-30 * deltaTime, 0, 0));
}


