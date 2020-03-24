#include "AnimObjectPhysicsComponent.h"
#include <GameEngine/Components/Renderers/AnimationRenderer.h>

AnimObjectPhysicsComponent::AnimObjectPhysicsComponent() {
	which = 1;
}

void AnimObjectPhysicsComponent::update(float deltaTimeSeconds) {
}


void AnimObjectPhysicsComponent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
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
