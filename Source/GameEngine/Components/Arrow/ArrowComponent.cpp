#include "ArrowComponent.h"
#include <GameEngine/IncludeList.h>
#include <GameEngine/Utils/PhysicsManager.h>
#include <GameEngine/Components/NPC/NPCMovementComponent.h>

ArrowComponent::ArrowComponent() {
	creatorObject = nullptr;
	spawnPos = glm::vec3(0);
	currentPos = glm::vec3(0);
	dir = glm::vec3(0);
	speed = 0;
	maxTravelDistance = 0;
}

void ArrowComponent::Init() {
	currentPos = spawnPos + dir;
	object->GetTransform()->UseModel(true);
	UpdateModel();
}

void ArrowComponent::update(float deltaTimeSeconds) {
	if (glm::l2Norm(spawnPos, currentPos) > maxTravelDistance) {
		object->SetShouldDelete(true);
	}

	bool hit = false;

	BaseGameObject* obj = PhysicsManager::GetInstance().GetPhysicsEngine()->RayCastObject(currentPos, currentPos + dir * speed, &hit);

	if (obj != nullptr) {
		if (dynamic_cast<NPCMovementComponent*>(obj->GetComponent("NPCMovementComponent")) != nullptr) {
			obj->SetShouldDelete(true);
		}
	}

	UpdateModel();
}

void ArrowComponent::UpdateModel() {
	ComponentTransform* transf = object->GetTransform();
	currentPos += dir * speed;
	glm::mat4 modelMat =  glm::translate(glm::mat4(1), currentPos) * glm::inverse(glm::lookAt(glm::vec3(0), -dir, glm::vec3(0, 1, 0)));
	transf->SetModel(modelMat);
	transf->SetPos(currentPos);
}