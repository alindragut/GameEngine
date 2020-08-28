#include "BruiserComponent.h"
#include <GameEngine/Utils/PhysicsManager.h>
#include <GameEngine/IncludeList.h>
#include <GameEngine/Components/Projectile/ProjectileSpawner.h>
#include <GameEngine/MapGenerator/GeneratorManager.h>
#include <GameEngine/Utils/MeshManager.h>
#include <GameEngine/Components/Combat/CombatComponent.h>
#include <GameEngine/Components/Player/PlayerComponent.h>
#include <GameEngine\Components\Combat\SkillTreeComponent.h>

BruiserComponent::BruiserComponent() {
	dir = glm::vec3(0);
	animation = false;
	speed = 1;
	currentDest = glm::vec3(0);
	spawnPos = glm::vec3(0);
	pathfindCooldown = 2.f;
	timer = 0.0f;
	state = 0;
	crtAnimDuration = 0.f;
	crtAnimTimer = 0.f;
	alive = true;
	startTimeDoT = 0;
	prevTimeDoT = 0;
	doTCounter = 0;
	doTDamage = 0;
}

void BruiserComponent::Init() {
	GeneratorManager& gm = GeneratorManager::GetInstance();
	object->GetTransform()->SetPos(spawnPos);
	object->GetTransform()->SetScale(glm::vec3(1, 3, 1));
	object->GetTransform()->UseModel(true);
}

void BruiserComponent::update(float deltaTimeSeconds) {
	int hp = static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetHP();

	timer += deltaTimeSeconds;
	glm::vec3 currentPos = object->GetTransform()->GetPos();
	glm::vec3 currentTargetPos = target->GetTransform()->GetPos();
	int crtState = state;

	if (startTimeDoT != 0) {
		startTimeDoT += deltaTimeSeconds;
		if (startTimeDoT - prevTimeDoT >= 1) {
			prevTimeDoT = startTimeDoT;
			static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->DealDamage(doTDamage);
			doTCounter++;
		}
		if (doTCounter > 10) {
			doTCounter = 0;
			startTimeDoT = 0;
			prevTimeDoT = 0;
			doTDamage = 0;
		}
	}
	glm::mat4 rotationOffset = glm::mat4(1);

	if (hp <= 0 && alive) {
		state = 3;
		alive = false;
		static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("bruiser_die");
		crtAnimDuration = MeshManager::GetInstance().GetAnimation("bruiser_die")->GetDuration();
		crtAnimTimer = 0.f;
	}

	if (!alive) {
		// skip this step
	}
	else if (!static_cast<PlayerComponent*>(target->GetComponent("PlayerComponent"))->isAlive()) {
		state = 0;
		speed = 0;
	}
	else if (glm::l1Norm(currentPos - currentTargetPos) > 10.f) {
		state = 0;
		speed = 0;
	}
	else if (glm::l1Norm(currentPos - currentTargetPos) < 1.5f) {
		state = 2;
		speed = 0;
	}
	else {
		state = 1;
		speed = 1;
	}

	if (crtState != state) {
		if (state == 0) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("bruiser_idle");
		}
		else if (state == 1) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("bruiser_walk");
		}
		else if (state == 2) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("bruiser_attack_1");

			crtAnimDuration = MeshManager::GetInstance().GetAnimation("bruiser_attack_1")->GetDuration();
			crtAnimTimer = 0.f;
		}
	}
	if (state == 2) {
		if (crtAnimTimer >= crtAnimDuration) {
			bool hit = false;
			glm::vec3 targetDir = glm::normalize(targetPos - currentPos);
			BaseGameObject* obj = PhysicsManager::GetInstance().GetPhysicsEngine()->RayCastObject(currentPos, currentPos + targetDir, &hit);
			if (obj != nullptr && dynamic_cast<PlayerComponent*>(obj->GetComponent("PlayerComponent")) != nullptr) {
				static_cast<CombatComponent*>(obj->GetComponent("CombatComponent"))->DealDamage(static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetDamage());
			}
			crtAnimTimer = 0.f;
		}
		rotationOffset = glm::rotate(rotationOffset, (float)-M_PI/6.f,glm::vec3(0, 1, 0));
		crtAnimTimer += deltaTimeSeconds;
	}

	if (state == 3) {
		crtAnimTimer += deltaTimeSeconds;
		if (crtAnimTimer >= crtAnimDuration) {
			static_cast<SkillTreeComponent*>(target->GetComponent("SkillTreeComponent"))->AddPoints(2);
			object->SetShouldDelete(true);
		}
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
	glm::mat4 modelMat = glm::translate(glm::mat4(1), currentPos) * rotationOffset * glm::inverse(glm::lookAt(glm::vec3(0), -dir, glm::vec3(0, 1, 0))) * glm::scale(glm::mat4(1), glm::vec3(1.1f));
	transf->SetModel(modelMat);
}