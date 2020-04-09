#include "RigidBodyComponent.h"
#include <GameEngine/Utils/PhysicsManager.h>

void RigidBodyComponent::Init() {
	glm::vec3 position = object->GetTransform()->GetPos();
	glm::vec3 scale = object->GetTransform()->GetScale();

	btCollisionShape* shape = new btBoxShape(btVector3(btScalar(scale.x / 2.0f), btScalar(scale.y / 2.0f), btScalar(scale.z / 2.0f)));

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));

	btScalar mass(0.);

	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	body = new btRigidBody(rbInfo);

	PhysicsManager::GetInstance().GetPhysicsEngine()->AddRigidBody(body);
}