#ifndef _GRAPHICS_H
#define _GRAPHICS_H

class Graphics
{
public:
	Graphics();
	~Graphics();

	static void Init();
	static void SetViewport(int width, int height);
};

#endif