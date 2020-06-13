#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/Renderers/AnimationRenderer.h>
#include <GameEngine/Utils/AStarAlgorithm.h>
#include <stack>

class AnimMovementGameComponent : public BaseComponent
{
public:
	GAME_COMPONENT(AnimMovementGameComponent)

	AnimMovementGameComponent();
	~AnimMovementGameComponent() {};

	void Init() override;
	void update(float deltaTimeSeconds) override;

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void SetRotationTime(float rotationTime) { this->rotationTime = rotationTime; }

private:
	float rotationTime;
	glm::vec3 RayPick(int mouseX, int mouseY);
	glm::quat GetRotationQuat(glm::quat oldRot, glm::quat newRot, float elapsedTime);

	glm::vec3 dir;
	glm::vec3 currentDest;

	glm::quat newRotQuat;
	glm::quat rotQuat;

	float speed;
	int state;
	
	std::stack<std::pair<int, int>> crtPath;

	int **matrix;
	int **auxMatrix;

	bool isRotating;
	bool animation;
	AStarAlgorithm* astar;
};