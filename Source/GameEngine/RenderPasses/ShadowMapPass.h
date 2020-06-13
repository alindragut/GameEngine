#pragma once

#include <Core/Engine.h>
#include "RenderPass.h"

class ShadowMapPass : public RenderPass
{
public:
	ShadowMapPass();

	~ShadowMapPass();

	void Init(unsigned int width, unsigned int height);

	void BindForWriting();

	void BindForReading(GLenum textureUnit);

private:
	GLuint shadow_fbo;
	GLuint shadow_texture;
};