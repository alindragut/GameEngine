#include "ComponentFactory.h"

BaseGameObject* ComponentFactory::createObject(int id) {
	switch (id) {
		case 1:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new GameObjectGameComponent());
			go->AddComponent(new PointShadowRenderer());
			go->AddComponent(new RigidBodyComponent());
			go->InitComponents();
			return go;
		}
		case 2:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new AnimObjectGameComponent());
			go->AddComponent(new AnimationRenderer("model", "anim_1"));
			go->AddComponent(new PointLightComponent());
			go->InitComponents();
			return go;
		}
		case 3:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new PointShadowRenderer());
			go->AddComponent(new RigidBodyComponent());
			go->InitComponents();
			return go;
		}
		case 4:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new PlayerMovementGameComponent());
			go->AddComponent(new AnimationRenderer("model", "player_run"));
			go->AddComponent(new PointLightComponent());
			go->AddComponent(new CombatComponent());
			go->InitComponents();
			return go;
		}
		case 5:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new DefaultRenderer());
			go->InitComponents();
			return go;
		}
		case 6:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			
			PointShadowRenderer* psr = new PointShadowRenderer();
			psr->SetSkybox(true);
			go->AddComponent(psr);
			
			RigidBodyComponent* rbc = new RigidBodyComponent();
			rbc->SetWalkable(true);
			go->AddComponent(rbc);

			go->InitComponents();
			return go;
		}
		case 7:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new PointShadowRenderer());
			go->AddComponent(new RigidBodyComponent());
			go->AddComponent(new ArrowComponent());
			go->InitComponents();
			return go;
		}
		case 8:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new TextureRenderer());
			go->AddComponent(new RigidBodyComponent());
			go->InitComponents();
			return go;
		}
		case 9:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new NPCMovementComponent());
			go->AddComponent(new AnimationRenderer("npc", "npc_idle"));
			go->AddComponent(new RigidBodyComponent());
			go->AddComponent(new CombatComponent());
			go->InitComponents();
			return go;
		}
		default:
			return nullptr;
	}
};