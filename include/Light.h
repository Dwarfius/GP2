#ifndef _LIGHT_H
#define _LIGHT_H

#include "Common.h"
#include "BaseComponent.h"

class Light : public BaseComponent
{
private:
	vec3 color;

public:
	Light(vec3 color);
	~Light();
};

#endif