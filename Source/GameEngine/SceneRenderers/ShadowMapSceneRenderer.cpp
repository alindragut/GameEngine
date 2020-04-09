#include "ShadowMapSceneRenderer.h"
#include <GameEngine/Components/ComponentsIncludeList.h>

ShadowMapSceneRenderer::ShadowMapSceneRenderer(std::vector<BaseGameObject*>* objects) {
	this->objects = objects;
}

ShadowMapSceneRenderer::~ShadowMapSceneRenderer() {
	delete defaultPass;
	delete shadowPass;
}

void ShadowMapSceneRenderer::Init(unsigned int scene_width, unsigned int scene_height,
								  unsigned int shadow_width, unsigned int shadow_height) {
	defaultPass = new DefaultRenderPass(scene_width, scene_height);
	shadowPass = new ShadowMapPass(shadow_width, shadow_height);
	shadowPass->Init();
}

void ShadowMapSceneRenderer::renderScene() {
	//shadowPass->BindForWriting();
	renderGameObjects();
	//defaultPass->BindForWriting();
	//shadowPass->BindForReading(GL_TEXTURE1);
	//renderGameObjects();
}

void ShadowMapSceneRenderer::renderGameObjects() {
	int size = objects->size();
	for (int i = 0; i < size; i++) {
		if (AnimationRenderer* ar = dynamic_cast<AnimationRenderer*>(((*objects)[i])->GetComponent("AnimationRenderer"))) {
			ar->render();
		}
		if (DefaultRenderer* dr = dynamic_cast<DefaultRenderer*>(((*objects)[i])->GetComponent("DefaultRenderer"))) {
			dr->render();
		}
	}
}