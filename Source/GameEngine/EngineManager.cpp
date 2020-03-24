#include "EngineManager.h"
#include "GameEngine.h"

void EngineManager::SetGameEngine(GameEngine* gameEngine) { 
	this->gameEngine = gameEngine; 
}
GameEngine* EngineManager::GetGameEngine() { 
	return this->gameEngine; 
}