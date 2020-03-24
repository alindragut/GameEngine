#include "GameObject.h"

GameObject::GameObject(int objectId) : BaseGameObject(objectId)
{
	transform->SetPos(glm::vec3(3, 0, 0));
}

GameObject::~GameObject()
{
	for (auto &it : components) {
		delete it.second;
	}
}

void GameObject::update(float deltaTimeSeconds) {
	for (const auto &it : components) {
		it.second->update(deltaTimeSeconds);
	}
}