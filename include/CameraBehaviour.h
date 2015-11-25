#ifndef _CAMERABEHAVIOUR_H
#define _CAMERABEHAVIOUR_H
#include "Common.h"
#include "Camera.h"
#include "BaseComponent.h"
class CameraBehaviour: public BaseComponent
{
private:
	Camera *camera;
	bool wireframeMode = false;
public:
	float mouseSensitivity;
	float speed;
	CameraBehaviour(Camera * c, float tS);
	~CameraBehaviour();
	void Update(float deltaTime);
};
#endif