#pragma once

#include <GameEngine/Components/BaseComponents/GraphicsComponent.h>

class DefaultRenderer : public GraphicsComponent
{
public:
	GAME_COMPONENT(DefaultRenderer)

	DefaultRenderer();
	~DefaultRenderer();
	
	virtual void update(float deltaTimeSeconds) override;
	virtual void render() override;

	void SetShadowMapping(bool value) { this->useShadowMapping = value; }

	void SetShader(std::string shaderName);
	void SetMesh(std::string meshName);

private:
	bool useShadowMapping;
	glm::mat4 lightView;
	glm::mat4 lightProj;
	std::string meshName;
	std::string shaderName;
};