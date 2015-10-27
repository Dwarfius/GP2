#pragma once
#pragma once
#ifndef _CAMERABEHAVIOUR_H
#define _CAMERABEHAVIOUR_H
#include "Common.h"
#include "BaseComponent.h"

class CameraBehaviour: public BaseComponent
{
private:
	float speed;
public:
	CameraBehaviour();
	~CameraBehaviour();
	void Update(float deltaTime);
};
#endif