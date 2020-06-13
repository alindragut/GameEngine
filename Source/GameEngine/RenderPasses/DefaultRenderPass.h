#pragma once

#include <Core/Engine.h>
#include "RenderPass.h"

class DefaultRenderPass : public RenderPass
{
public:
	DefaultRenderPass();

	~DefaultRenderPass();

	void Init(unsigned int width, unsigned int height);

	void BindForWriting();
};