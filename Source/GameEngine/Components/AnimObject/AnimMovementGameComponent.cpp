#include "AnimMovementGameComponent.h"
#include <GameEngine/Utils/PhysicsManager.h>
#include <GameEngine/IncludeList.h>
#include <GameEngine/Components/Arrow/ArrowSpawner.h>
#include <GameEngine/MapGenerator/GeneratorManager.h>

AnimMovementGameComponent::AnimMovementGameComponent() {
	dir = glm::vec3(0);
	rotQuat = glm::quat();
	newRotQuat = glm::quat();
	isRotating = false;
	animation = false;
	state = 0;
	speed = 1;
	currentDest = glm::vec3(0);
}

void AnimMovementGameComponent::Init() {
	GeneratorManager& gm = GeneratorManager::GetInstance();
	dir = gm.GetGenerator()->GetSpawnPoint();
	matrix = gm.GetGenerator()->GetMatrix();
	auxMatrix = new int*[70];
	for (int i = 0; i < 70; i++) {
		auxMatrix[i] = new int[70];
	}
	object->GetTransform()->SetPos(dir);
	object->GetTransform()->UseModel(true);
	astar = new AStarAlgorithm(2 * 30 + 10, 2 * 30 + 10);
}

void AnimMovementGameComponent::update(float deltaTimeSeconds) {
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

void AnimMovementGameComponent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

	if (button == GLFW_MOUSE_BUTTON_2) {
		glm::vec3 playerPos = object->GetTransform()->GetPos();
		printf("player %lf %lf %lf\n", playerPos.x, playerPos.y, playerPos.z);
		glm::vec3 mouseWorldPos = RayPick(mouseX, mouseY);
		printf("target %lf %lf %lf\n", mouseWorldPos.x, mouseWorldPos.y, mouseWorldPos.z);

		for (int i = 0; i < 70; i++) {
			for (int j = 0; j < 70; j++) {
				auxMatrix[i][j] = matrix[i][j];
			}
		}

		crtPath = astar->aStarSearch(&auxMatrix, std::make_pair((int)round(object->GetTransform()->GetPos().z),
			(int)round(object->GetTransform()->GetPos().x)),
			std::make_pair((int)round(mouseWorldPos.z),
			(int)round(mouseWorldPos.x)));
		if (!crtPath.empty()) {
			crtPath.pop();
		}
	}


}

void AnimMovementGameComponent::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_1) {
		double mouseX, mouseY;
		glfwGetCursorPos(EngineManager::GetInstance().GetGameEngine()->GetWindow()->GetGLFWWindow(), &mouseX, &mouseY);
		glm::vec3 yOffset = glm::vec3(0, 0.5, 0);

		glm::vec3 mouseWorldPos = RayPick(int(round(mouseX)), int(round(mouseY))) + yOffset;

		glm::vec3 currentPos = object->GetTransform()->GetPos() + yOffset;
		printf("1\n");
		ArrowSpawner::SpawnArrow(object, currentPos, glm::normalize(mouseWorldPos - currentPos), 0.1f, 10.0f);
	}
}

glm::vec3 AnimMovementGameComponent::RayPick(int mouseX, int mouseY) {
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

glm::quat AnimMovementGameComponent::GetRotationQuat(glm::quat oldRot, glm::quat newRot, float elapsedTime) {
	float factor = fmin(elapsedTime / 0.5f, 1.0f);
	float matching = glm::dot(oldRot, newRot);
	if (abs(matching - 1.0) < 0.01) {
		return newRot;
	}
	return glm::slerp(oldRot, newRot, factor);
}