#include "DefaultRenderPass.h"

DefaultRenderPass::DefaultRenderPass() {
}

DefaultRenderPass::~DefaultRenderPass() {

}

void DefaultRenderPass::Init(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
}

void DefaultRenderPass::BindForWriting() {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
}