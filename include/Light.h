#ifndef _LIGHT_H
#define _LIGHT_H

#include "Common.h"
#include "BaseComponent.h"

class Light : public BaseComponent
{
private:
	vec4 color;

public:
	Light(vec4 color);
	~Light();

	vec4 GetColor() { return color; }
};

#endif