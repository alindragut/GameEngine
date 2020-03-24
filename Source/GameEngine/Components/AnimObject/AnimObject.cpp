#include "AnimObject.h"

AnimObject::AnimObject(int objectId) : BaseGameObject(objectId)
{
	transform->SetScale(glm::vec3(10.0f));
}

AnimObject::~AnimObject()
{
}

void AnimObject::update(float deltaTimeSeconds) {
	for (const auto &it : components) {
		it.second->update(deltaTimeSeconds);
	}
}