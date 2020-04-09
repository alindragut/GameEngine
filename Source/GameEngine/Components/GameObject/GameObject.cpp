#include "GameObject.h"

GameObject::GameObject(int objectId) : BaseGameObject(objectId)
{
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