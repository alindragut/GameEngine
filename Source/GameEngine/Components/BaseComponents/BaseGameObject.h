#pragma once

#include <GameEngine/Utils/ComponentTransform.h>
#include <Core/Engine.h>
#include <map>

class BaseComponent;

class BaseGameObject
{
public:
	virtual ~BaseGameObject();
	virtual void update(float deltaTimeSeconds) = 0;

	int GetObjectId() { return this->objectId; }

	void SetTransform(ComponentTransform* transform) { this->transform = transform; }
	ComponentTransform* GetTransform() { return this->transform; }

	void AddComponent(BaseComponent* component);

	BaseComponent* GetComponent(std::string name);

	std::map<std::string, BaseComponent*>* GetComponents() { return &components; }

	void InitComponents();

	void SetShouldDelete(bool shouldDelete) { this->shouldDelete = shouldDelete; }
	
	bool ShouldDelete() { return this->shouldDelete; }

protected:
	bool shouldDelete;
	int objectId;

	std::map<std::string, BaseComponent*> components;
	BaseGameObject(int id) : transform(new ComponentTransform()), objectId(id), shouldDelete(false) {};
	ComponentTransform* transform;

};