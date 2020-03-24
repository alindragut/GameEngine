#pragma once

class GameEngine;

class EngineManager
{
public:
	EngineManager(EngineManager const&) = delete;
	void operator=(EngineManager const&) = delete;

	static EngineManager& GetInstance()
	{
		static EngineManager instance;
		return instance;
	}

	void SetGameEngine(GameEngine* gameEngine);
	GameEngine* GetGameEngine();

private:
	GameEngine* gameEngine;
	EngineManager() {};

};