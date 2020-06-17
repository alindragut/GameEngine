#include "NPCMovementComponent.h"
#include <GameEngine/Utils/PhysicsManager.h>
#include <GameEngine/IncludeList.h>
#include <GameEngine/Components/Arrow/ArrowSpawner.h>
#include <GameEngine/MapGenerator/GeneratorManager.h>
#include <GameEngine/Utils/MeshManager.h>
#include <GameEngine/Components/Combat/CombatComponent.h>
#include <GameEngine/Components/AnimObject/PlayerMovementGameComponent.h>

NPCMovementComponent::NPCMovementComponent() {
	dir = glm::vec3(0);
	animation = false;
	speed = 1;
	currentDest = glm::vec3(0);
	spawnPos = glm::vec3(0);
	pathfindCooldown = 0.33f;
	timer = 0.0f;
	state = 0;
	crtAnimDuration = 0.f;
	crtAnimTimer = 0.f;
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
	int crtState = state;

	if (!static_cast<PlayerMovementGameComponent*>(target->GetComponent("PlayerMovementGameComponent"))->isAlive()) {
		state = 0;
		speed = 0;
	} else if (glm::l1Norm(currentPos - currentTargetPos) > 10.f) {
		state = 0;
		speed = 0;
	} else if (glm::l1Norm(currentPos - currentTargetPos) < 2.f) {
		state = 2;
		speed = 0;
	}
	else {
		state = 1;
		speed = 1;
	}

	if (crtState != state) {
		if (state == 0) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("npc_idle");
		}
		 else if (state == 1) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("npc_walk");
		}
		 else if (state == 2) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("npc_attack_1");
			crtAnimDuration = MeshManager::GetInstance().GetAnimation("npc_attack_1")->GetDuration();
			crtAnimTimer = 0.f;
		}
	}
	else if (state == 2) {
			if (crtAnimTimer >= crtAnimDuration) {
				static_cast<CombatComponent*>(target->GetComponent("CombatComponent"))->DealDamage(static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetDamage());
			}
			crtAnimTimer += deltaTimeSeconds;
	}
	

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