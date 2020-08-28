#include "SpellComponent.h"
#include <GameEngine/IncludeList.h>
#include <GameEngine/Utils/PhysicsManager.h>
#include <GameEngine/Components/NPC/NPCMovementComponent.h>
#include <GameEngine/Components/NPC/BruiserComponent.h>
#include <GameEngine/Components/NPC/MageComponent.h>
#include <GameEngine/Components/Player/PlayerComponent.h>
#include <GameEngine/Components/Combat/CombatComponent.h>

SpellComponent::SpellComponent() {
	creatorObject = nullptr;
	spawnPos = glm::vec3(0);
	currentPos = glm::vec3(0);
	offset = glm::vec3(0);
	dir = glm::vec3(0);
	speed = 0;
	maxTravelDistance = 0;
}

void SpellComponent::Init() {
	currentPos = spawnPos + dir + offset;
	object->GetTransform()->UseModel(true);
	UpdateModel();
}

void SpellComponent::update(float deltaTimeSeconds) {
	if (glm::l2Norm(spawnPos, currentPos) > maxTravelDistance) {
		object->SetShouldDelete(true);
	}

	bool hit = false;

	BaseGameObject* obj = PhysicsManager::GetInstance().GetPhysicsEngine()->RayCastObject(currentPos, currentPos + dir * speed, &hit);

	if (obj != nullptr) {
		if (dynamic_cast<PlayerComponent*>(obj->GetComponent("PlayerComponent")) != nullptr) {
			static_cast<CombatComponent*>(obj->GetComponent("CombatComponent"))->DealDamage(static_cast<CombatComponent*>(creatorObject->GetComponent("CombatComponent"))->GetDamage());
		}
	}

	UpdateModel();
}

void SpellComponent::UpdateModel() {
	ComponentTransform* transf = object->GetTransform();
	currentPos += dir * speed;
	glm::mat4 modelMat = glm::translate(glm::mat4(1), currentPos) * glm::inverse(glm::lookAt(glm::vec3(0), -dir, glm::vec3(0, 1, 0)));
	transf->SetModel(modelMat);
	transf->SetPos(currentPos);
}