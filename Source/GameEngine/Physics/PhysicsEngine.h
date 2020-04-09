#pragma once

#include "btBulletDynamicsCommon.h"

#include <include/glm.h>

class PhysicsEngine {
public:
	PhysicsEngine();
	~PhysicsEngine();

	void Init();

	void Step(float deltaTimeSeconds);

	void AddRigidBody(btRigidBody* body);

	glm::vec3 RayCast(glm::vec3 from, glm::vec3 to, bool *hit);

private:
	btDiscreteDynamicsWorld* world;

	glm::vec3 bulletToGlm(const btVector3& v) { return glm::vec3(v.getX(), v.getY(), v.getZ()); }

	btVector3 glmToBullet(const glm::vec3& v) { return btVector3(v.x, v.y, v.z); }
};