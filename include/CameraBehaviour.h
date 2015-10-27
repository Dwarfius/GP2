#ifndef _CAMERABEHAVIOUR_H
#define _CAMERABEHAVIOUR_H
#include "Common.h"
#include "Camera.h"
#include "BaseComponent.h"
class CameraBehaviour: public BaseComponent
{
private:
	float speed;
	Camera *camera;
public:
	CameraBehaviour(Camera * c);
	~CameraBehaviour();
	void Update(float deltaTime);
};
#endif