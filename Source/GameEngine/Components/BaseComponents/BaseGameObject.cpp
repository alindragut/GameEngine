#include "BaseGameObject.h"
#include "BaseComponent.h"

BaseGameObject::~BaseGameObject() {
	for (const auto &it : components) {
		delete it.second;
	}

	delete transform;

	components.clear();
}

void BaseGameObject::AddComponent(BaseComponent* component) {
	component->SetGameObject(this);
	this->components[component->GetName()] = component;
}

BaseComponent* BaseGameObject::GetComponent(std::string name) {
	auto it = components.find(name);

	if (it == components.end()) {
		return nullptr;
	}

	return it->second;
}

void BaseGameObject::InitComponents() {
	for (const auto &it : components) {
		it.second->Init();
	}
}