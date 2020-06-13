#pragma once

#include "RenderPass.h"
#include <Core/Engine.h>

class CubeShadowMapPass : public RenderPass
{
public:
	CubeShadowMapPass();

	~CubeShadowMapPass();

	void Init(unsigned int shadow_width, unsigned int shadow_height);

	void BindForWriting();

	void BindForReading(GLenum textureUnit);

private:
	GLuint shadow_fbo;
	GLuint shadow_texture;
};