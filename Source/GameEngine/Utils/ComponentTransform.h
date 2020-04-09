#pragma once

#include <include/glm.h>

class ComponentTransform 
{
	public:
		ComponentTransform() : scale(glm::vec3(1)), pos(glm::vec3(0)), rot(glm::vec3(0)), model(glm::mat4(1)), useModel(false) {};
		~ComponentTransform() {};

		void SetScale(glm::vec3 scale) { this->scale = scale; }
		glm::vec3 GetScale() { return this->scale; }

		void SetPos(glm::vec3 pos) { this->pos = pos; }
		glm::vec3 GetPos() { return this->pos; }

		void SetRot(glm::vec3 rot) { this->rot = rot; }
		glm::vec3 GetRot() { return this->rot; }

		void SetModel(glm::mat4 model) { this->model = model; }
		glm::mat4 GetModel() { return this->model; }

		void UseModel(bool use) { this->useModel = use; }
		bool ShouldUseModel() { return this->useModel; }

		void Step(const glm::vec3& dir, float speed) { this->pos = this->pos + dir * speed; }

	private:
		glm::vec3 scale;
		glm::vec3 pos;
		glm::vec3 rot;
		glm::mat4 model;
		bool useModel;
};