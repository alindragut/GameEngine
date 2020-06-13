#include "RigidBodyComponent.h"
#include <GameEngine/Utils/PhysicsManager.h>

RigidBodyComponent::~RigidBodyComponent() {
	PhysicsManager::GetInstance().GetPhysicsEngine()->RemoveRigidBody(body);
}

void RigidBodyComponent::Init() {
	glm::vec3 position = object->GetTransform()->GetPos();
	glm::vec3 scale = object->GetTransform()->GetScale();

	prevPos = position;
	prevScale = scale;

	btCollisionShape* shape = new btBoxShape(btVector3(btScalar(scale.x / 2.0f), btScalar(scale.y / 2.0f), btScalar(scale.z / 2.0f)));

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));

	btScalar mass(0.);

	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	body = new btRigidBody(rbInfo);

	if (walkable) {
		PhysicsManager::GetInstance().GetPhysicsEngine()->AddRigidBody(object, body, COL_FLOOR, COL_FLOOR);
	}
	else {
		PhysicsManager::GetInstance().GetPhysicsEngine()->AddRigidBody(object, body);
	}
	
}

void RigidBodyComponent::update(float deltaTimeSeconds) {
	glm::vec3 position = object->GetTransform()->GetPos();
	glm::vec3 scale = object->GetTransform()->GetScale();

	if (position != prevPos || scale != prevScale) {
		PhysicsManager::GetInstance().GetPhysicsEngine()->RemoveRigidBody(body);
		Init();
	}
}