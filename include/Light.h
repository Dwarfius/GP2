#ifndef _LIGHT_H
#define _LIGHT_H

#include "Common.h"
#include "BaseComponent.h"

class Light : public BaseComponent
{
private:
	vec4 color;
	float intensity = 10.0f;

public:
	Light(vec4 color);
	~Light();

	vec4 GetColor() { return color; }
	float GetIntensity() { return intensity; }
};

#endif