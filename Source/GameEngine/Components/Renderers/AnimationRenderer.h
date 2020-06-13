#pragma once
#include <GameEngine/Components/BaseComponents/GraphicsComponent.h>
#include <GameEngine/Utils/AnimationInfo.h>

using namespace std;

class GameEngine;

class AnimationRenderer : public GraphicsComponent
{
public:
	GAME_COMPONENT(AnimationRenderer)

	AnimationRenderer(std::string defaultModelName, std::string defaultAnimName);
	~AnimationRenderer();

	void SetAnimation(std::string animationName);
	void SetModel(std::string modelName);
	void SetShader(std::string shaderName);
	void SetLightPos(glm::vec3 lightPos);
	void SetDepth(bool depth);

	virtual void update(float deltaTimeSeconds) override;

	virtual void render() override;

private:
	float time;
	bool depth;
	glm::vec3 lightPos;
	AnimationInfo* animInfo;
	std::string shaderName;
};