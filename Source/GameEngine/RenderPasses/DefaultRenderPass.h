#pragma once

#include <Core/Engine.h>

class DefaultRenderPass
{
public:
	DefaultRenderPass(unsigned int width, unsigned int height);

	~DefaultRenderPass();

	void BindForWriting();

private:
	unsigned int width;
	unsigned int height;
};