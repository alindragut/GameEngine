#pragma once

class RenderPass
{
public:
	virtual ~RenderPass() {};
	
protected:
	unsigned int width;
	unsigned int height;
};