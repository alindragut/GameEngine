#include "Map.h"
#include <Core/GPU/MeshInstanced.h>
#include <GameEngine/IncludeList.h>
#include <GameEngine/Utils/ComponentFactory.h>

void Map::Init() {
	GameEngine* engine = EngineManager::GetInstance().GetGameEngine();
	ComponentFactory& factory = ComponentFactory::GetInstance();

	// Room objects
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

	// Decorations
	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("column");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("column_broken");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("banner");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("banner_wall");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("barrel");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("barrel_2");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("bones");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("bones_2");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("book");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("book_2");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("book_3");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("bucket");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("candelabrum_tall");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("candle");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("carpet");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("chair");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("chest");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("cobweb");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("cobweb_2");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("crate");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("pedestal");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("pedestal_2");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("potion");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("potion_2");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("potion_3");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("potion_4");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("potion_5");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("potion_6");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("skull");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("table_big");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("table_small");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("torch_wall");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("vase");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("window");
	engine->AddObject(obj);

	obj = factory.createObject(11);
	static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"))->SetMesh("woodfire");
	engine->AddObject(obj);
}