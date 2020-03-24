#include "GraphicsComponent.h"
#include <GameEngine/IncludeList.h>

GraphicsComponent::GraphicsComponent() {
	EngineManager& em = EngineManager::GetInstance();
	camera = em.GetGameEngine()->GetCamera();
}