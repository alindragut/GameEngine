#pragma once

#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/RenderPasses/DefaultRenderPass.h>
#include <GameEngine/RenderPasses/ShadowMapPass.h>
#include <GameEngine/RenderPasses/CubeShadowMapPass.h>

class ShadowMapSceneRenderer
{
public:
	ShadowMapSceneRenderer(std::list<BaseGameObject*>* objects);

	~ShadowMapSceneRenderer();

	void Init(unsigned int scene_width, unsigned int scene_height,
			  unsigned int point_shadow_width, unsigned int point_shadow_height,
			  unsigned int dir_shadow_width, unsigned int dir_shadow_height);

	void renderScene();

private:
	void renderGameObjects();
	void renderPointShadow();
	void renderDirShadow();

	std::list<BaseGameObject*>* objects;
	DefaultRenderPass* defaultPass;
	ShadowMapPass* dirShadowPass;
	CubeShadowMapPass* pointShadowPass;
	float shadowAspectRatio;
	glm::vec3 lightPos;
	glm::mat4 dirLightView;
	glm::mat4 dirLightProj;
};