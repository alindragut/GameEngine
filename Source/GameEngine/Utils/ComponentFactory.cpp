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
			PointShadowRenderer* psr = new PointShadowRenderer();
			psr->SetShader("DungeonPack");
			go->AddComponent(psr);
			go->AddComponent(new RigidBodyComponent());
			go->InitComponents();
			return go;
		}
		case 4:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new PlayerComponent());
			go->AddComponent(new AnimationRenderer("model", "player_idle"));
			go->AddComponent(new PointLightComponent());
			go->AddComponent(new CombatComponent());
			go->AddComponent(new SkillTreeComponent());
			go->AddComponent(new RigidBodyComponent());
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
			psr->SetShader("Skybox");
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
			go->AddComponent(new SkeletonComponent());
			go->AddComponent(new AnimationRenderer("skeleton", "skeleton_idle"));
			go->AddComponent(new RigidBodyComponent());
			go->AddComponent(new CombatComponent());
			go->InitComponents();
			return go;
		}
		case 10:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			TextureRenderer* tr = new TextureRenderer();
			tr->SetShader("Skybox");
			tr->LoadCubemap();
			go->AddComponent(tr);
			go->InitComponents();
			return go;
		}
		case 11:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			PointShadowRenderer* psr = new PointShadowRenderer();
			psr->SetShader("DungeonPack");
			go->AddComponent(psr);
			go->AddComponent(new RigidBodyComponent());
			go->InitComponents();
			return go;
		}
		case 12:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			RigidBodyComponent* rbc = new RigidBodyComponent();
			rbc->SetWalkable(true);
			go->AddComponent(rbc);
			go->InitComponents();
			return go;
		}
		case 13:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new SkeletonComponent());
			go->AddComponent(new AnimationRenderer("skeleton", "skeleton_idle"));
			go->AddComponent(new RigidBodyComponent());
			go->AddComponent(new CombatComponent());
			go->InitComponents();
			return go;
		}
		case 14:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new BruiserComponent());
			go->AddComponent(new AnimationRenderer("bruiser", "bruiser_idle"));
			go->AddComponent(new RigidBodyComponent());
			go->AddComponent(new CombatComponent());
			go->InitComponents();
			return go;
		}
		case 15:
		{
			BaseGameObject* go(new AnimObject(globalObjectId++));
			go->AddComponent(new MageComponent());
			go->AddComponent(new AnimationRenderer("mage", "mage_idle"));
			go->AddComponent(new RigidBodyComponent());
			go->AddComponent(new CombatComponent());
			go->InitComponents();
			return go;
		}
		case 16:
		{
			BaseGameObject* go(new GameObject(globalObjectId++));
			go->AddComponent(new PointShadowRenderer());
			go->AddComponent(new RigidBodyComponent());
			go->AddComponent(new SpellComponent());
			go->InitComponents();
			return go;
		}

		default:
			return nullptr;
	}
};