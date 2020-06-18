#pragma once

#include <GameEngine/Components/BaseComponents/GraphicsComponent.h>

class PointShadowRenderer : public GraphicsComponent
{
public:
	GAME_COMPONENT(PointShadowRenderer)

	PointShadowRenderer();
	~PointShadowRenderer();
	
	virtual void update(float deltaTimeSeconds) override;
	virtual void render() override;
	
	void RenderDepth();
	
	void RenderLight();

	void SetRenderDepth(bool renderDepth) { this->renderDepth = renderDepth; }
	void SetShader(std::string shaderName) { this->shaderName = shaderName; }
	void SetPos(glm::vec3 pos) { this->lightPos = pos; }
	void SetSkybox(bool skybox) { this->skybox = skybox; }
	void SetDirLightView(glm::mat4 view) { this->dirLightView = view; }
	void SetDirLightProj(glm::mat4 proj) { this->dirLightProj = proj; }

	bool isSkybox() { return this->skybox; }

	void SetMesh(std::string meshName);

	void OnKeyPress(int key, int mods) override;

private:
	bool renderDepth;
	std::string meshName;
	std::string shaderName;
	std::string depthShaderName;
	glm::vec3 lightPos;
	bool pressed;
	bool skybox;
	glm::mat4 dirLightView;
	glm::mat4 dirLightProj;
};