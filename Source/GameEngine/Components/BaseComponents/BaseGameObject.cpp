#include "BaseGameObject.h"
#include "Component.h"

void BaseGameObject::AddComponent(BaseComponent* component) {
	component->SetGameObject(this);
	this->components[component->GetName()] = component;
}