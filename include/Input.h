#ifndef _INPUT_H
#define _INPUT_H

#include "Common.h"

class Input
{
private:
	static Uint16 mod;
	static char keys[400]; //400 because 127 goes for ascii keys,
	static char keysOld[400]; //then ~250 goes on for other keys
	static ivec2 mPos, mPosOld;
	static Uint32 mState, mStateOld;

public:
	static void SetMod(Uint16 pMod) { mod = pMod; }

	static void SetKey(SDL_Keycode key, bool state);
	static bool GetKey(SDL_Keycode key); //check if it's currently down or not
	static bool GetKeyDown(SDL_Keycode key); //check if it was pressed this frame
	static bool GetKeyUp(SDL_Keycode key); //check if it was released this frame

	static ivec2 GetMousePos() { return mPos; }
	static ivec2 GetMouseDelta() { return mPos - mPosOld; }
	static void MoveMouse(ivec2 delta) { mPos += delta; }

	static bool GetMouseBtn(int btn);
	static bool GetMouseBtnDown(int btn);
	static bool GetMouseBtnUp(int btn);

	static void Update();
};

#endif
