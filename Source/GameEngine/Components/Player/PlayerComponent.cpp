#include "PlayerComponent.h"
#include <GameEngine/Utils/PhysicsManager.h>
#include <GameEngine/IncludeList.h>
#include <GameEngine/Components/Projectile/ProjectileSpawner.h>
#include <GameEngine/MapGenerator/GeneratorManager.h>
#include <GameEngine/Components/Combat/CombatComponent.h>
#include <GameEngine/Utils/MeshManager.h>
#include <GameEngine\Components\Combat\SkillTreeComponent.h>
#include <GameEngine\ImGUI\imgui.h>
#include <GameEngine\Components\NPC\MageComponent.h>
#include <GameEngine\Components\NPC\BruiserComponent.h>

PlayerComponent::PlayerComponent() {
	dir = glm::vec3(0);
	rotQuat = glm::quat();
	newRotQuat = glm::quat();
	isRotating = false;
	animation = false;
	state = 0;
	speed = 2.5f;
	currentDest = glm::vec3(0);
	centeredCamera = true;
	alive = true;
	isArmed = false;
	crtAnimTimer = 0.f;
	targetDir = glm::vec3(0);
	canMove = true;
}

void PlayerComponent::Init() {
	GeneratorManager& gm = GeneratorManager::GetInstance();
	dir = gm.GetGenerator()->GetSpawnPoint();
	object->GetTransform()->SetPos(dir);
	object->GetTransform()->UseModel(true);
	gm.GetGenerator()->SetPlayer(object);
	EngineManager::GetInstance().GetGameEngine()->GetSceneRenderer()->SetPlayer(object);
}

void PlayerComponent::update(float deltaTimeSeconds) {
	int hp = static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetHP();
	int maxHp = static_cast<CombatComponent*>(object->GetComponent("CombatComponent"))->GetMaxHP();
	int crtState = state;
	bool p_open = true;

	ImGui::SetNextWindowPos(ImVec2(10, 850));
	ImGui::SetNextWindowSize(ImVec2(270, 55));
	ImGui::Begin("HP", nullptr, ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse);
	ImGui::ProgressBar((float)hp/maxHp);
	double mouseX, mouseY;
	glfwGetCursorPos(EngineManager::GetInstance().GetGameEngine()->GetWindow()->GetGLFWWindow(), &mouseX, &mouseY);
	
	BaseGameObject* target = GetObjectAtCursor(mouseX, mouseY);
	
	if (target != nullptr && target != object &&
	(dynamic_cast<MageComponent*>(target->GetComponent("MageComponent")) != nullptr ||
	 dynamic_cast<BruiserComponent*>(target->GetComponent("BruiserComponent")) != nullptr)) {
		int enemyHp = static_cast<CombatComponent*>(target->GetComponent("CombatComponent"))->GetHP();
		int enemyMaxHp = static_cast<CombatComponent*>(target->GetComponent("CombatComponent"))->GetMaxHP();
		ImGui::SetNextWindowPos(ImVec2(560, 10));
		ImGui::SetNextWindowSize(ImVec2(278, 55));
		ImGui::Begin("Enemy HP", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse);
		ImGui::ProgressBar((float)enemyHp / enemyMaxHp);
	}
	
	// Camera follows player
	if (centeredCamera) {
		Camera* cam = EngineManager::GetInstance().GetGameEngine()->GetCamera();
		glm::vec3 newPos = object->GetTransform()->GetPos();
		//glm::vec3 offsetDir = glm::normalize(glm::vec3(-3, 5, -3));
		glm::vec3 offsetDir = glm::normalize(glm::vec3(-1, 2, -1));
		cam->SetPosition(newPos + 20.0f * offsetDir);
		cam->SetRotation(glm::inverse(glm::lookAt(newPos + 20.0f * offsetDir, newPos, glm::vec3(0, 1, 0))));
		cam->Update();
	}

	// Check if the player was killed last iteration 
	if (hp <= 0 && alive) {
		state = 3;
		alive = false;
		canMove = false;
		crtAnimDuration = MeshManager::GetInstance().GetAnimation("player_die")->GetDuration();
		crtAnimTimer = 0.f;
	}

	// Stop movement if dead
	if (!alive) {
		crtPath = std::stack<std::pair<float, float>>();
	}

	// Update state: idle/walk
	if (alive && state != 5 && state != 6 && state != 7) {
		if (crtPath.empty()) {
			state = 0;
		}
		else if(crtState != 5) {
			state = 1;
		}
	}

	// Update animation
	if (crtState != state) {

		switch (state) {
			case 0: {
				static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_idle");
			}
			break;
			case 1: {
				static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_run");
			}
			break;
			case 2: {
				static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_walk");
			}
			break;
			case 3: {
				static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_die");
			}
			break;
		}
	}

	// Add to the "death" timer
	if (state == 3) {
		crtAnimTimer += deltaTimeSeconds;
		if (crtAnimTimer >= crtAnimDuration) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_dead");
			crtAnimDuration = 0.f;
			crtAnimTimer = 0.f;
			state = 4;
		}
	}

	// Add to the "shoot" timer + shoot
	if (state == 5) {
		crtPath = std::stack<std::pair<float, float>>();
		crtAnimTimer += deltaTimeSeconds;
		if (crtAnimTimer >= crtAnimDuration) {
			if (static_cast<SkillTreeComponent*>(object->GetComponent("SkillTreeComponent"))->GetDirectionalShoot3()) {
				glm::vec3 pos1 = glm::vec3(glm::rotate(glm::mat4(1), float(M_PI / 6), glm::vec3(0, 1, 0)) * glm::vec4(targetDir, 1.0f));
				glm::vec3 pos2 = glm::vec3(glm::rotate(glm::mat4(1), -float(M_PI / 6), glm::vec3(0, 1, 0)) * glm::vec4(targetDir, 1.0f));
				ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0), pos1, 0.1f, 10.0f);
				ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0), targetDir, 0.1f, 10.0f);
				ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0), pos2, 0.1f, 10.0f);
				if (static_cast<SkillTreeComponent*>(object->GetComponent("SkillTreeComponent"))->GetExtraArrow()) {
					ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0) + pos1, pos1, 0.1f, 10.0f);
					ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0) + targetDir, targetDir, 0.1f, 10.0f);
					ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0) + pos2, pos2, 0.1f, 10.0f);
				}

			}
			else if (static_cast<SkillTreeComponent*>(object->GetComponent("SkillTreeComponent"))->GetDirectionalShoot2()) {
				glm::vec3 pos1 = glm::vec3(glm::rotate(glm::mat4(1), float(M_PI / 6), glm::vec3(0, 1, 0)) * glm::vec4(targetDir, 1.0f));
				glm::vec3 pos2 = glm::vec3(glm::rotate(glm::mat4(1), -float(M_PI / 6), glm::vec3(0, 1, 0)) * glm::vec4(targetDir, 1.0f));
				ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0), pos1, 0.1f, 10.0f);
				ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0), pos2, 0.1f, 10.0f);
			}
			else {
				ProjectileSpawner::SpawnArrow(object, object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0), targetDir, 0.1f, 10.0f);
			}
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_idle");
			crtAnimDuration = 0.f;
			crtAnimTimer = 0.f;
			state = 0;
		}
	}

	// Add to the "equip" timer
	if (state == 6) {
		crtPath = std::stack<std::pair<float, float>>();
		crtAnimTimer += deltaTimeSeconds;
		if (!isArmed && crtAnimTimer >= crtAnimDuration / 2) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetModel("model_bow");
			isArmed = !isArmed;
		}
		if (crtAnimTimer >= crtAnimDuration) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_idle");
			crtAnimDuration = 0.f;
			crtAnimTimer = 0.f;
			state = 0;
		}
	}

	// Add to the "disarm" timer
	if (state == 7) {
		crtPath = std::stack<std::pair<float, float>>();
		crtAnimTimer += deltaTimeSeconds;
		if (isArmed && crtAnimTimer >= crtAnimDuration / 3) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetModel("model");
			isArmed = !isArmed;
		}
		if (crtAnimTimer >= crtAnimDuration) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_idle");
			crtAnimDuration = 0.f;
			crtAnimTimer = 0.f;
			state = 0;
		}
	}

	// Move character
	glm::vec3 currentPos = object->GetTransform()->GetPos();
	if (!crtPath.empty()) {
		glm::vec3 oldCurrentDest = currentDest;
		currentDest = glm::vec3(crtPath.top().second, 0, crtPath.top().first);
		if (oldCurrentDest != currentDest) {
			dir = glm::normalize(currentDest - currentPos);
			newRotQuat = glm::quat(glm::inverse(glm::lookAt(glm::vec3(0), -dir, glm::vec3(0, 1, 0))));
		}
		if (glm::l2Norm(currentPos, currentDest) < 0.5f) {
			crtPath.pop();
		}
	}
	else if(state == 5){
		dir = glm::vec3(-targetDir.x, 0.f, -targetDir.z);
		newRotQuat = glm::quat(glm::inverse(glm::lookAt(glm::vec3(0), dir, glm::vec3(0, 1, 0))));
		rotQuat = newRotQuat;
		dir = glm::vec3(0);
	}
	else {
		dir = glm::vec3(0);
	}
	rotQuat = GetRotationQuat(rotQuat, newRotQuat, deltaTimeSeconds);
	ComponentTransform* transf = object->GetTransform();
	currentPos += dir * speed * deltaTimeSeconds;
	transf->SetPos(currentPos);
	glm::mat4 modelMat = glm::translate(glm::mat4(1), currentPos) * glm::mat4(rotQuat);
	transf->SetModel(modelMat);
}

void PlayerComponent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

	if (button == GLFW_MOUSE_BUTTON_2 && canMove) {
		glm::vec3 playerPos = object->GetTransform()->GetPos();
		glm::vec3 mouseWorldPos = RayPick(mouseX, mouseY);

		crtPath = GeneratorManager::GetInstance().GetGenerator()->GetPath(playerPos, mouseWorldPos);

		if (!crtPath.empty()) {
			crtPath.pop();
		}
	}
}

void PlayerComponent::OnKeyPress(int key, int mods) {

	// Shoot arrow/s
	if (key == GLFW_KEY_1 ) {
		if (!crtAnimTimer == 0.f) {
			return;
		}
		if (!isArmed) {
			printf("You require a weapon");
			return;
		}
		static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_b_shoot");
		double mouseX, mouseY;
		glfwGetCursorPos(EngineManager::GetInstance().GetGameEngine()->GetWindow()->GetGLFWWindow(), &mouseX, &mouseY);
		glm::vec3 yOffset = glm::vec3(0, 0.5, 0);

		glm::vec3 mouseWorldPos = RayPick(int(round(mouseX)), int(round(mouseY))) + yOffset;

		glm::vec3 currentPos = object->GetTransform()->GetPos() + yOffset;

		targetDir = glm::normalize(mouseWorldPos - currentPos);
		targetPos = glm::vec3(mouseWorldPos.x, 0.f, mouseWorldPos.z);

		crtAnimDuration = MeshManager::GetInstance().GetAnimation("player_b_shoot")->GetDuration();
		crtAnimTimer = 0.f;
		state = 5;
		crtPath = std::stack<std::pair<float, float>>();
	}

	if (key == GLFW_KEY_3) {
		double mouseX, mouseY;
		glfwGetCursorPos(EngineManager::GetInstance().GetGameEngine()->GetWindow()->GetGLFWWindow(), &mouseX, &mouseY);
		glm::vec3 yOffset = glm::vec3(0, 0.5, 0);

		glm::vec3 mouseWorldPos = RayPick(int(round(mouseX)), int(round(mouseY))) + yOffset;

		object->GetTransform()->SetPos(mouseWorldPos);
		while (!crtPath.empty()) {
			crtPath.pop();
		}
	}
	if (key == GLFW_KEY_SPACE) {
		centeredCamera = !centeredCamera;
	}

	if (key == GLFW_KEY_P) {
		canMove = !canMove;
		static_cast<SkillTreeComponent*>(object->GetComponent("SkillTreeComponent"))->SetSkillTreeView(!canMove);
	}

	// Equip/ Disarm weapon
	if (key == GLFW_KEY_R) {
		if (!alive) {
			return;
		}

		if (crtAnimTimer != 0.f) {
			return;
		}

		crtPath = std::stack<std::pair<float, float>>();

		if (isArmed) {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_b_disarm");
			crtAnimDuration = MeshManager::GetInstance().GetAnimation("player_b_disarm")->GetDuration();
			crtAnimTimer = 0.f;
			state = 7;
		}
		else {
			static_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))->SetAnimation("player_b_equip");
			crtAnimDuration = MeshManager::GetInstance().GetAnimation("player_b_equip")->GetDuration();
			crtAnimTimer = 0.f;
			state = 6;
		}
		
	}
}

glm::vec3 PlayerComponent::RayPick(int mouseX, int mouseY) {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	EngineManager& em = EngineManager::GetInstance();
	auto camera = em.GetGameEngine()->GetCamera();
	auto resolution = em.GetGameEngine()->GetWindow()->GetResolution();

	glm::vec3 camPosition = camera->transform->GetWorldPosition();
	int width = resolution.x;
	int height = resolution.y;

	glm::vec3 res = glm::unProject(glm::vec3(mouseX, height - mouseY - 1, 1.0f), camera->GetViewMatrix() * glm::translate(glm::mat4(1), camPosition), camera->GetProjectionMatrix(), glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

	glm::vec3 dir = glm::normalize(res - camPosition);

	bool hit;

	return PhysicsManager::GetInstance().GetPhysicsEngine()->RayCast(camPosition, res * 100.0f, &hit, COL_FLOOR, COL_FLOOR);
}

BaseGameObject* PlayerComponent::GetObjectAtCursor(int mouseX, int mouseY)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	EngineManager& em = EngineManager::GetInstance();
	auto camera = em.GetGameEngine()->GetCamera();
	auto resolution = em.GetGameEngine()->GetWindow()->GetResolution();

	glm::vec3 camPosition = camera->transform->GetWorldPosition();
	int width = resolution.x;
	int height = resolution.y;

	glm::vec3 res = glm::unProject(glm::vec3(mouseX, height - mouseY - 1, 1.0f), camera->GetViewMatrix() * glm::translate(glm::mat4(1), camPosition), camera->GetProjectionMatrix(), glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

	glm::vec3 dir = glm::normalize(res - camPosition);

	bool hit;
	return PhysicsManager::GetInstance().GetPhysicsEngine()->RayCastObject(camPosition, res * 100.0f, &hit);
}

glm::quat PlayerComponent::GetRotationQuat(glm::quat oldRot, glm::quat newRot, float elapsedTime) {
	float factor = fmin(elapsedTime * speed, 1.0f) ;
	float matching = glm::dot(oldRot, newRot);
	if (abs(matching - 1.0) < 0.001) {
		return newRot;
	}
	return glm::slerp(oldRot, newRot, factor);
}