#pragma once

#include <GameEngine/IncludeList.h>

class DebugDraw
{
public:
	static void DrawLine(glm::vec3 from, glm::vec3 to) {
		glm::vec3 dir = to - from;
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.01, 0.01, glm::l2Norm(dir)));
		glm::mat4 pos = glm::translate(glm::mat4(1), (from + to) / 2.0f);
		//float angle = atan2(-dir.z, dir.x);
		BaseGameObject* obj = ComponentFactory::GetInstance().createObject(5);
		//glm::mat4 rot = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));
		glm::mat4 rot = glm::inverse(glm::lookAt(from, to, glm::vec3(0, 1, 0)));
		glm::mat4 model = pos * rot * scale;
		obj->GetTransform()->SetModel(model);
		obj->GetTransform()->UseModel(true);
		//obj->GetTransform()->SetScale(scale);
		//obj->GetTransform()->SetRot(glm::vec3(0, angle, 0));
		//obj->GetTransform()->SetPos(pos);
		EngineManager::GetInstance().GetGameEngine()->AddObject(obj);
	}
};