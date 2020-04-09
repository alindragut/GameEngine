#pragma once

#include <include/glm.h>
#include <GameEngine/Physics/PhysicsEngine.h>

class PhysicsManager
{
public:
	PhysicsManager(PhysicsManager const&) = delete;
	void operator=(PhysicsManager const&) = delete;

	static PhysicsManager& GetInstance()
	{
		static PhysicsManager instance;
		return instance;
	}

	void SetPhysicsEngine(PhysicsEngine* physics) { this->physics = physics; }

	PhysicsEngine* GetPhysicsEngine() { return physics; }

private:
	PhysicsManager() {}
	PhysicsEngine* physics;
};