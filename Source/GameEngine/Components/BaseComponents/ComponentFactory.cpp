#include "ComponentFactory.h"

std::unique_ptr<BaseGameObject> ComponentFactory::createObject(int id) {
	switch (id) {
		case 1:
		{
			std::unique_ptr<BaseGameObject> go(new GameObject(globalObjectId++));
			go->AddComponent(new GameObjectPhysicsComponent());
			go->AddComponent(new DefaultRenderer());
			return go;
		}
		case 2:
		{
			std::unique_ptr<BaseGameObject> go(new AnimObject(globalObjectId++));
			go->AddComponent(new AnimObjectPhysicsComponent());
			go->AddComponent(new AnimationRenderer("model", "anim_1"));
			return go;
		}
		default:
			return nullptr;
	}
};