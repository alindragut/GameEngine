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
	
	virtual void update(float deltaTimeSeconds) override;

private:
	GameEngine* engine;
	float time;
	AnimationInfo* animInfo;
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, Animation*> animations;
	std::unordered_map<std::string, Shader*> shaders;
};