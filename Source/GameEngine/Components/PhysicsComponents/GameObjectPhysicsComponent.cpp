#include "GameObjectPhysicsComponent.h"

GameObjectPhysicsComponent::GameObjectPhysicsComponent() {
	dir = glm::vec3(0);
}

void GameObjectPhysicsComponent::update(float deltaTimeSeconds) {
	object->GetTransform()->Step(dir, speed);
	dir = glm::vec3(0);
}

void GameObjectPhysicsComponent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	dir = glm::vec3(0, 0, 1);
}