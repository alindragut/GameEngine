#include "DefaultRenderPass.h"

DefaultRenderPass::DefaultRenderPass(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
}

DefaultRenderPass::~DefaultRenderPass() {

}

void DefaultRenderPass::BindForWriting() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
}