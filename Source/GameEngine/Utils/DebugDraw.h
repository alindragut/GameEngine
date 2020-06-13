#pragma once

#include <GameEngine/IncludeList.h>

class DebugDraw
{
public:
	static void DrawLine(glm::vec3 from, glm::vec3 to) {
		glm::vec3 dir = to - from;
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.01, 0.01, glm::l2Norm(dir)));
		glm::mat4 pos = glm::translate(glm::mat4(1), (from + to) / 2.0f);
		BaseGameObject* obj = ComponentFactory::GetInstance().createObject(5);
		glm::mat4 rot = glm::inverse(glm::lookAt(glm::vec3(0), glm::normalize(dir), glm::vec3(0, 1, 0)));
		glm::mat4 model = pos * rot * scale;
		obj->GetTransform()->SetModel(model);
		obj->GetTransform()->UseModel(true);
		EngineManager::GetInstance().GetGameEngine()->AddObject(obj);
	}
};