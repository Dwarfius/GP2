#include "DirectionalLightComp.h"



DirectionalLightComp::DirectionalLightComp()
{
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "FRAMEBUFFER DOWN MATE!" << endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


DirectionalLightComp::~DirectionalLightComp()
{
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteTextures(1, &depthMap);
}

Camera * DirectionalLightComp::ConfigureDirLightCamera()
{
	Camera *lightCamera = new Camera();
	GLfloat nearPlane = -10, farPlane = 20;
	lightCamera->SetProjOrtho(-10.0f, 10.0f, 0.1f, 10.0f, nearPlane, farPlane);
	//lightCamera->SetView(direction, pos, glm::vec3(0, 1, 0));
	lightCamera->SetPos(pos);
	lightCamera->LookAt(pos + direction); 
	lightCamera->Recalculate();
	return lightCamera;
}

void DirectionalLightComp::ShadowMapRenderStart()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT); 
}

void DirectionalLightComp::ShadowMapRenderEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_W, SCREEN_H);
}
