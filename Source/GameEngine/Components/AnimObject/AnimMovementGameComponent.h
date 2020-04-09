#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/Renderers/AnimationRenderer.h>

class AnimMovementGameComponent : public BaseComponent
{
public:
	GAME_COMPONENT(AnimMovementGameComponent)

	AnimMovementGameComponent();
	~AnimMovementGameComponent() {};

	void Init() override;
	void update(float deltaTimeSeconds) override;

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

private:
	AnimationRenderer* ar;
	glm::vec3 dir;
	float speed;
	int state;

	GLdouble crtX, crtY, crtZ;

	bool isRotating;
	bool animation;
	float prevRotation;
	float crtRotation;
	float timestamp;
	const float rotDuration = 0.5f;

	glm::vec3 forward = glm::vec3(0, 0, -1);
};