#include "RigidBodyComponent.h"
#include <GameEngine/Utils/PhysicsManager.h>

RigidBodyComponent::~RigidBodyComponent() {
	PhysicsManager::GetInstance().GetPhysicsEngine()->RemoveRigidBody(body);
}

void RigidBodyComponent::Init() {
	glm::vec3 position = object->GetTransform()->GetPos();
	glm::vec3 scale = object->GetTransform()->GetScale();
	glm::vec3 rotate = object->GetTransform()->GetRot();
	glm::quat rotation = glm::toQuat(glm::rotate(glm::mat4(1), rotate.y, glm::vec3(0, 1, 0)));

	prevPos = position;
	prevScale = scale;
	prevRotate = rotate;

	btCollisionShape* shape = new btBoxShape(btVector3(btScalar(scale.x / 2.0f), btScalar(scale.y / 2.0f), btScalar(scale.z / 2.0f)));

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));
	transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

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
	glm::vec3 rotate = object->GetTransform()->GetRot();

	if (position != prevPos || scale != prevScale || rotate != prevRotate) {
		PhysicsManager::GetInstance().GetPhysicsEngine()->RemoveRigidBody(body);
		Init();
	}
}