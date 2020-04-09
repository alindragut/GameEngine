#include "GameObjectGameComponent.h"

GameObjectGameComponent::GameObjectGameComponent() {
	dir = glm::vec3(0);
}

void GameObjectGameComponent::Init() {
	object->GetTransform()->SetPos(glm::vec3(0, 2, 6));
	object->GetTransform()->SetScale(glm::vec3(4));
}

void GameObjectGameComponent::update(float deltaTimeSeconds) {
}

void GameObjectGameComponent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
}