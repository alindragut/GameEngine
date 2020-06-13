#include <GameEngine/Utils/PhysicsManager.h>
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"

PhysicsEngine::PhysicsEngine() {
	this->world = nullptr;
}

PhysicsEngine::~PhysicsEngine() {
	delete world;
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

void PhysicsEngine::AddRigidBody(BaseGameObject* obj, btRigidBody* body, int group, int mask) {
	world->addRigidBody(body, group, mask);
	rigidBodyMap[body] = obj;
}

void PhysicsEngine::RemoveRigidBody(btRigidBody* body) {
	if (body && body->getMotionState()) {
		delete body->getMotionState();
		delete body->getCollisionShape();
		world->removeRigidBody(body);
		rigidBodyMap.erase(body);
		delete body;
	}
}

BaseGameObject* PhysicsEngine::RayCastObject(glm::vec3 from, glm::vec3 to, bool *hit, int group, int mask) {
	btVector3 btFrom = glmToBullet(from);
	btVector3 btTo = glmToBullet(to);
	BaseGameObject* res = nullptr;

	btCollisionWorld::ClosestRayResultCallback closestResults(btFrom, btTo);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	closestResults.m_collisionFilterGroup = group;
	closestResults.m_collisionFilterMask = mask;

	world->rayTest(btFrom, btTo, closestResults);

	if (closestResults.hasHit())
	{
		*hit = true;
		const btRigidBody* body = btRigidBody::upcast(closestResults.m_collisionObject);
		res = rigidBodyMap[const_cast<btRigidBody*>(body)];
	}
	else {

		*hit = false;
	}

	return res;
}

glm::vec3 PhysicsEngine::RayCast(glm::vec3 from, glm::vec3 to, bool *hit, int group, int mask) {
	btVector3 btFrom = glmToBullet(from);
	btVector3 btTo = glmToBullet(to);
	glm::vec3 res = glm::vec3(0);

	btCollisionWorld::ClosestRayResultCallback closestResults(btFrom, btTo);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	closestResults.m_collisionFilterGroup = group;
	closestResults.m_collisionFilterMask = mask;

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