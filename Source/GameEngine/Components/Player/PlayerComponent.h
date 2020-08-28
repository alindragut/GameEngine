#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/Renderers/AnimationRenderer.h>
#include <stack>

class PlayerComponent : public BaseComponent
{
public:
	GAME_COMPONENT(PlayerComponent)

	PlayerComponent();
	~PlayerComponent() {};

	void Init() override;
	void update(float deltaTimeSeconds) override;

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void SetRotationTime(float rotationTime) { this->rotationTime = rotationTime; }
	bool isAlive() { return this->alive; }

private:
	float rotationTime;
	glm::vec3 RayPick(int mouseX, int mouseY);
	BaseGameObject* GetObjectAtCursor(int mouseX, int mouseY);
	glm::quat GetRotationQuat(glm::quat oldRot, glm::quat newRot, float elapsedTime);

	glm::vec3 dir;
	glm::vec3 currentDest;
	glm::vec3 targetDir;
	glm::vec3 targetPos;

	glm::quat newRotQuat;
	glm::quat rotQuat;

	float speed;
	float crtAnimDuration;
	float crtAnimTimer;
	int state;
	/*
	0 - idle
	1 - walk
	2 - run
	3 - die
	4 - dead
	5 - attack
	6 - equip
	7 - unequip
	*/

	std::stack<std::pair<float, float>> crtPath;

	bool isRotating;
	bool animation;
	bool centeredCamera;
	bool alive;
	bool isArmed;
	bool canMove;
};