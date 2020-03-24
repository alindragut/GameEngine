#include "DefaultRenderer.h"

DefaultRenderer::DefaultRenderer() {
	SetMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj", "default");
	SetShader("Source/GameEngine/Shaders/", "VertexShaderDefault.glsl", "FragmentShaderDefault.glsl", "default");
}

DefaultRenderer::~DefaultRenderer() {
	delete mesh;
	delete shader;
}

void DefaultRenderer::update(float deltaTimeSeconds) {
	shader->Use();

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	glm::mat4 trans = glm::translate(glm::mat4(1), object->GetTransform()->GetPos());
	glm::mat4 rot = glm::rotate(glm::mat4(1), object->GetTransform()->GetRot().y, glm::vec3(0, 1, 0));
	glm::mat4 scale = glm::scale(glm::mat4(1), object->GetTransform()->GetScale());

	glm::mat4 modelMat = trans * rot * scale;


	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMat));
	
	mesh->Render();
}

void DefaultRenderer::SetMesh(std::string meshPath, std::string fileName, std::string meshName) {
	mesh = new Mesh(meshName);
	mesh->LoadMesh(meshPath, fileName);
}
	
void DefaultRenderer::SetShader(std::string shaderPath, std::string VSName, std::string FSName, std::string shaderName) {
	shader = new Shader(shaderName.c_str());
	shader->AddShader(shaderPath + VSName, GL_VERTEX_SHADER);
	shader->AddShader(shaderPath + FSName, GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
}