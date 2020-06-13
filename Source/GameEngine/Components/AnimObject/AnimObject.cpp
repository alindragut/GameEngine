#include "AnimObject.h"

AnimObject::AnimObject(int objectId) : BaseGameObject(objectId)
{
}

AnimObject::~AnimObject()
{
}

void AnimObject::update(float deltaTimeSeconds) {
	for (const auto &it : components) {
		it.second->update(deltaTimeSeconds);
	}
}