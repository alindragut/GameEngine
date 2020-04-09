#include <GameEngine/Utils/PhysicsManager.h>
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"

PhysicsEngine::PhysicsEngine() {
	this->world = nullptr;
}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::Init() {
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	world->setGravity(btVector3(0, -10, 0));

	PhysicsManager::GetInstance().SetPhysicsEngine(this);
}

void PhysicsEngine::Step(float deltaTimeSeconds) {
	world->stepSimulation(deltaTimeSeconds);
}

void PhysicsEngine::AddRigidBody(btRigidBody* body) {
	world->addRigidBody(body);
}

glm::vec3 PhysicsEngine::RayCast(glm::vec3 from, glm::vec3 to, bool *hit) {
	btVector3 btFrom = glmToBullet(from);
	btVector3 btTo = glmToBullet(to);
	glm::vec3 res = glm::vec3(0);

	btCollisionWorld::ClosestRayResultCallback closestResults(btFrom, btTo);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	world->rayTest(btFrom, btTo, closestResults);

	if (closestResults.hasHit())
	{
		*hit = true;

		res = bulletToGlm(btFrom.lerp(btTo, closestResults.m_closestHitFraction));
	}
	else {
		
		*hit = false;
	}

	return res;
}