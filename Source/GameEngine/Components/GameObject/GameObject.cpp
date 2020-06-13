#include "GameObject.h"

GameObject::GameObject(int objectId) : BaseGameObject(objectId)
{
}

GameObject::~GameObject()
{
}

void GameObject::update(float deltaTimeSeconds) {
	for (const auto &it : components) {
		it.second->update(deltaTimeSeconds);
	}
}