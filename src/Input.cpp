#include "Input.h"

Uint16 Input::mod;
char Input::keys[400];
char Input::keysOld[400];
ivec2 Input::mPos, Input::mPosOld;
Uint32 Input::mState, Input::mStateOld;

void Input::SetKey(SDL_Keycode key, bool state)
{
	if (key >= SDLK_CAPSLOCK)
	{
		keys[key - (1 << 30) + 128] = state;
		//printf("%d - %c %d\n", state, key, key - (1 << 30) + 128);
	}
	else
	{
		keys[key] = state;
		//printf("%d - %c %d\n", state, key, key);
	}
}

bool Input::GetKey(SDL_Keycode key)
{
	if (key >= SDLK_CAPSLOCK)
		return keys[key - (1 << 30) + 128];
	else
		return keys[key];
}

bool Input::GetKeyDown(SDL_Keycode key)
{
	if (key >= SDLK_CAPSLOCK)
		return keys[key - (1 << 30) + 128] && !keysOld[key - (1 << 30) + 128];
	else
		return keys[key] && !keysOld[key];
}

bool Input::GetKeyUp(SDL_Keycode key)
{
	if (key >= SDLK_CAPSLOCK)
		return !keys[key - (1 << 30) + 128] && keysOld[key - (1 << 30) + 128];
	else
		return !keys[key] && keysOld[key];
}

bool Input::GetMouseBtn(int btn)
{
	return mState & SDL_BUTTON(btn);
}

bool Input::GetMouseBtnDown(int btn)
{
	bool newState = mState & SDL_BUTTON(btn);
	bool oldState = mStateOld & SDL_BUTTON(btn);
	return newState && !oldState;
}

bool Input::GetMouseBtnUp(int btn)
{
	bool newState = mState & SDL_BUTTON(btn);
	bool oldState = mStateOld & SDL_BUTTON(btn);
	return !newState && oldState;
}

void Input::Update()
{
	memcpy(keysOld, keys, sizeof(keys));
	mStateOld = mState;
	mPosOld = mPos;
	mState = SDL_GetMouseState(NULL, NULL);
}