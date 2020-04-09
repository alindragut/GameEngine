#pragma once

#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/RenderPasses/DefaultRenderPass.h>
#include <GameEngine/RenderPasses/ShadowMapPass.h>

class ShadowMapSceneRenderer
{
public:
	ShadowMapSceneRenderer(std::vector<BaseGameObject*>* objects);

	~ShadowMapSceneRenderer();

	void Init(unsigned int scene_width, unsigned int scene_height,
			  unsigned int shadow_width, unsigned int shadow_height);

	void renderScene();

private:
	void renderGameObjects();

	std::vector<BaseGameObject*>* objects;
	DefaultRenderPass* defaultPass;
	ShadowMapPass* shadowPass;
};