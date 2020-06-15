#include "NPCMovementComponent.h"
#include <GameEngine/Utils/PhysicsManager.h>
#include <GameEngine/IncludeList.h>
#include <GameEngine/Components/Arrow/ArrowSpawner.h>
#include <GameEngine/MapGenerator/GeneratorManager.h>

NPCMovementComponent::NPCMovementComponent() {
	dir = glm::vec3(0);
	animation = false;
	speed = 1;
	currentDest = glm::vec3(0);
	spawnPos = glm::vec3(0);
	pathfindCooldown = 0.33f;
	timer = 0.0f;
}

void NPCMovementComponent::Init() {
	GeneratorManager& gm = GeneratorManager::GetInstance();
	object->GetTransform()->SetPos(spawnPos);
	object->GetTransform()->SetScale(glm::vec3(1, 3, 1));
	object->GetTransform()->UseModel(true);
}

void NPCMovementComponent::update(float deltaTimeSeconds) {
	timer += deltaTimeSeconds;
	glm::vec3 currentPos = object->GetTransform()->GetPos();
	glm::vec3 currentTargetPos = target->GetTransform()->GetPos();
	if (timer > pathfindCooldown && currentTargetPos != targetPos) {
		timer = 0.0f;
		targetPos = currentTargetPos;
		crtPath = GeneratorManager::GetInstance().GetGenerator()->GetPath(currentPos, targetPos);
		if (!crtPath.empty()) {
			crtPath.pop();
		}
	}
	
	if (!crtPath.empty()) {
		glm::vec3 oldCurrentDest = currentDest;
		currentDest = glm::vec3(crtPath.top().second, 0, crtPath.top().first);
		if (oldCurrentDest != currentDest) {
			dir = glm::normalize(currentDest - currentPos);
		}
		if (glm::l2Norm(currentPos, currentDest) < 0.5f) {
			crtPath.pop();
		}
	}
	else {
		dir = glm::vec3(0);
	}
	ComponentTransform* transf = object->GetTransform();
	currentPos += dir * speed * deltaTimeSeconds;
	transf->SetPos(currentPos);
	glm::mat4 modelMat = glm::translate(glm::mat4(1), currentPos) * glm::scale(glm::mat4(1), glm::vec3(3)) * glm::inverse(glm::lookAt(glm::vec3(0), -dir, glm::vec3(0, 1, 0)));
	transf->SetModel(modelMat);
}