#include "Map.h"
#include <Core/GPU/MeshInstanced.h>
#include <GameEngine/IncludeList.h>
#include <GameEngine/Utils/ComponentFactory.h>

void Map::Init() {
	GameEngine* engine = EngineManager::GetInstance().GetGameEngine();
	ComponentFactory& factory = ComponentFactory::GetInstance();


	BaseGameObject* obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("wall01");
	engine->AddObject(obj);
	
	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("fence01");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("fence02");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("fence03");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("door01");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("floor01");
	engine->AddObject(obj);
}