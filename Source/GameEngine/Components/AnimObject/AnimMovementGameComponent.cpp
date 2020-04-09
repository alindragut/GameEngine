#include "AnimMovementGameComponent.h"
#include <GameEngine/IncludeList.h>

AnimMovementGameComponent::AnimMovementGameComponent() {
	dir = glm::vec3(0);
	crtX = 0;
	crtY = 0;
	crtZ = 0;
	isRotating = false;
	animation = false;
	state = 0;
}

void AnimMovementGameComponent::Init() {
}

void AnimMovementGameComponent::update(float deltaTimeSeconds) {

	ar = (AnimationRenderer*)object->GetComponent("AnimationRenderer");

	switch (state) {
	case 0:
		if (!animation) {
			animation = true;
			ar->SetAnimation("anim_3");
		}
	case 1:
		if (!animation) {
			animation = true;
			ar->SetAnimation("anim_1");
		}
	}

	EngineManager& em = EngineManager::GetInstance();
	auto camera = em.GetGameEngine()->GetCamera();

	glm::vec3 vector = glm::vec3(crtX, crtY, crtZ) - dir;

	glm::mat4 modelMatrix = glm::mat4(1);

	glm::mat4 rotation = glm::mat4(1);
	glm::vec3 direction = glm::vec3(0);

	if (vector != glm::vec3(0) && fabs(vector.x) > 0.1f && fabs(vector.z) > 0.1f) {
		direction = glm::normalize(vector);
		dir += direction * 5.f * deltaTimeSeconds;
		if (state != 1) {
			state = 1;
			animation = false;
		}
		
	}
	else {
		if (state != 0) {
			state = 0;
			animation = false;
		}
	}

	// https://stackoverflow.com/questions/20346000/rotate-an-object-in-direction-of-motion-in-opengl

	glm::mat4 translate = glm::translate(glm::mat4(1), dir);
	float crtTime = Engine::GetElapsedTime() - timestamp;
	if (crtTime < rotDuration) {
		if (isRotating) {
			float aux = prevRotation * (1.f - crtTime/rotDuration) + crtRotation * crtTime / rotDuration;
			rotation = glm::rotate(glm::mat4(1), aux, glm::vec3(0, 1, 0));
			crtRotation = atan2(-direction.z, direction.x);
		}
	}
	else {
		isRotating = false;
		rotation = glm::rotate(glm::mat4(1), crtRotation, glm::vec3(0, 1, 0));
	}

	//rotation = glm::inverse(glm::lookAt(dir, dir + forward, glm::vec3(0, 1, 0)));
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(1, 2, 1));
	modelMatrix = translate * rotation * scale;

	object->GetTransform()->SetPos(dir);
	object->GetTransform()->SetScale(glm::vec3(10));

	camera->SetPosition(glm::vec3(dir.x, dir.y + 10, dir.z + 10));
}

void AnimMovementGameComponent::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

	if (button == GLFW_MOUSE_BUTTON_2) {
		EngineManager& em = EngineManager::GetInstance();
		auto camera = em.GetGameEngine()->GetCamera();
		auto resolution = em.GetGameEngine()->GetWindow()->GetResolution();

		glm::vec3 camPosition = camera->transform->GetWorldPosition();
		int width = resolution.x;
		int height = resolution.y;


		// https://stackoverflow.com/questions/29997209/opengl-c-mouse-ray-picking-glmunproject

		float mouseXpos = mouseX / (width * 0.5f) - 1.0f;
		float mouseYpos = mouseY / (height * 0.5f) - 1.0f;

		glm::mat4 invVP = glm::inverse(camera->GetProjectionMatrix() * camera->GetViewMatrix());
		glm::vec4 screenPos = glm::vec4(mouseXpos, -mouseYpos, 1.0f, 1.0f);
		glm::vec4 worldPos = invVP * screenPos;

		glm::vec3 dir = glm::normalize(glm::vec3(worldPos));


		// https://www.gamedev.net/forums/topic/321357-ray-plane-intersection/

		glm::vec3 normalXOZ = glm::vec3(0, 1, 0);

		float t = -(glm::dot(camPosition, normalXOZ)) / (glm::dot(dir, normalXOZ));

		crtX = camPosition.x + t * dir.x;
		crtY = camPosition.y + t * dir.y;
		crtZ = camPosition.z + t * dir.z;

		prevRotation = crtRotation;
		forward = glm::normalize(glm::vec3(crtX, crtY, crtZ) - dir);

		crtRotation = atan2(-crtZ, crtX);
		isRotating = true;
		timestamp = Engine::GetElapsedTime();
	}


}