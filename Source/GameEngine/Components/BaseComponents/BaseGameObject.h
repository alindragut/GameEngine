#pragma once

#include <GameEngine/Utils/Transform.h>
#include <Core/Engine.h>
#include <map>

class BaseComponent;

class BaseGameObject
{
public:
	virtual ~BaseGameObject() {}
	virtual void update(float deltaTimeSeconds) = 0;

	int GetObjectId() { return this->objectId; }

	void SetTransform(ComponentTransform* transform) { this->transform = transform; }
	ComponentTransform* GetTransform() { return this->transform; }

	void AddComponent(BaseComponent* component);

	BaseComponent* GetComponent(std::string name) { return components[name]; }

	std::map<std::string, BaseComponent*>* GetComponents() { return &components; }

protected:
	int objectId;

	std::map<std::string, BaseComponent*> components;
	BaseGameObject(int id) : transform(new ComponentTransform()), objectId(id) {};
	ComponentTransform* transform;

};