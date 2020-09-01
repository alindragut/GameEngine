#include "AnimObjectGameComponent.h"
#include <GameEngine/Components/Renderers/AnimationRenderer.h>
#include <GameEngine/Utils/PhysicsManager.h>

AnimObjectGameComponent::AnimObjectGameComponent() {
	which = 1;
	speed = 0;
}

void AnimObjectGameComponent::update(float deltaTimeSeconds) {
	object->GetTransform()->Step(glm::vec3(0, 0, 1), speed);
	speed = 0;

	bool hit = false;

	glm::vec3 from = glm::vec3(object->GetTransform()->GetPos().x, 1.2, object->GetTransform()->GetPos().z);
	glm::vec3 to = glm::vec3(object->GetTransform()->GetPos().x, 1.2, object->GetTransform()->GetPos().z + 1);
	PhysicsManager& pm = PhysicsManager::GetInstance();
    glm::vec3 res = pm.GetPhysicsEngine()->RayCast(from, to, &hit);
}


void AnimObjectGameComponent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	if (!which) {
		if (AnimationRenderer* ar = dynamic_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))) {
			ar->SetAnimation("anim_1");
		}
	}
	else {
		if (AnimationRenderer* ar = dynamic_cast<AnimationRenderer*>(object->GetComponent("AnimationRenderer"))) {
			ar->SetAnimation("anim_2");
		}
	}
	which ^= 1;
}

void AnimObjectGameComponent::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_1) {
		object->GetTransform()->Step(glm::vec3(-1, 0, 0), 0.25f);
	} else if (key == GLFW_KEY_2) {
		object->GetTransform()->Step(glm::vec3(1, 0, 0), 0.25f);
	} else if (key == GLFW_KEY_3) {
		object->GetTransform()->Step(glm::vec3(0, 0, 1), 0.25f);
	} else if (key == GLFW_KEY_4) {
		object->GetTransform()->Step(glm::vec3(0, 0, -1), 0.25f);
	}
}
