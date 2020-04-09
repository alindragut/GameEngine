#include "ComponentFactory.h"

BaseGameObject* ComponentFactory::createObject(int id) {
	switch (id) {
		case 1:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new GameObjectGameComponent());
			go->AddComponent(new DefaultRenderer());
			go->AddComponent(new RigidBodyComponent());
			return go;
		}
		case 2:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new AnimObjectGameComponent());
			go->AddComponent(new AnimationRenderer("model", "anim_1"));
			return go;
		}
		case 3:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new DefaultRenderer());
			go->AddComponent(new RigidBodyComponent());
			return go;
		}
		case 4:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new AnimMovementGameComponent());
			go->AddComponent(new AnimationRenderer("model", "anim_1"));
			return go;
		}
		case 5:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new DefaultRenderer());
			return go;
		}
		default:
			return nullptr;
	}
};