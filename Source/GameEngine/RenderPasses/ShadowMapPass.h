#pragma once

#include <Core/Engine.h>

class ShadowMapPass
{
public:
	ShadowMapPass(unsigned int width, unsigned int height);

	~ShadowMapPass();

	void Init();

	void BindForWriting();

	void BindForReading(GLenum textureUnit);

private:
	unsigned int shadow_width;
	unsigned int shadow_height;

	GLuint shadow_fbo;
	GLuint shadow_texture;
};