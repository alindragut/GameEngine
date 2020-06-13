#pragma once

#include "btBulletDynamicsCommon.h"
#include <include/glm.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>

#define COL_DEFAULT 1
#define COL_FLOOR 2
#define COL_ALL -1

class PhysicsEngine {
public:
	PhysicsEngine();
	~PhysicsEngine();

	void Init();

	void Step(float deltaTimeSeconds);

	void AddRigidBody(BaseGameObject* obj, btRigidBody* body, int group = COL_DEFAULT, int mask = COL_ALL);

	void RemoveRigidBody(btRigidBody* body);

	glm::vec3 RayCast(glm::vec3 from, glm::vec3 to, bool *hit, int group = COL_DEFAULT, int mask = COL_ALL);

	BaseGameObject* RayCastObject(glm::vec3 from, glm::vec3 to, bool *hit, int group = COL_DEFAULT, int mask = COL_ALL);

private:
	btDiscreteDynamicsWorld* world;

	std::unordered_map<btRigidBody*, BaseGameObject*> rigidBodyMap;

	glm::vec3 bulletToGlm(const btVector3& v) { return glm::vec3(v.getX(), v.getY(), v.getZ()); }

	btVector3 glmToBullet(const glm::vec3& v) { return btVector3(v.x, v.y, v.z); }
};