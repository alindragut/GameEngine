#include "TextureRenderer.h"
#include <GameEngine/Utils/ShaderCache.h>
#include <GameEngine/Utils/MeshManager.h>

TextureRenderer::TextureRenderer() {
	meshName = "box";
	ShaderCache& sc = ShaderCache::GetInstance();
	sc.AddShader("Room", "Source/GameEngine/Shaders/", "VertexShaderDefault.glsl", "FragmentShaderRoom.glsl");
	SetShader("Room");
}

TextureRenderer::~TextureRenderer() {
}

void TextureRenderer::update(float deltaTimeSeconds) {

}

void TextureRenderer::render() {
	Shader* shader = ShaderCache::GetInstance().GetShader(shaderName);
	shader->Use();

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	glm::mat4 modelMat;

	if (object->GetTransform()->ShouldUseModel()) {
		modelMat = object->GetTransform()->GetModel();
	}
	else {
		glm::mat4 trans = glm::translate(glm::mat4(1), object->GetTransform()->GetPos());
		glm::mat4 rot = glm::rotate(glm::mat4(1), object->GetTransform()->GetRot().y, glm::vec3(0, 1, 0));
		glm::mat4 scale = glm::scale(glm::mat4(1), object->GetTransform()->GetScale());

		modelMat = trans * rot * scale;
	}
	/**/
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMat));

	MeshManager::GetInstance().GetMesh(meshName)->Render();
}

void TextureRenderer::SetMesh(std::string meshName) {
	this->meshName = meshName;
}

void TextureRenderer::SetShader(std::string shaderName) {
	this->shaderName = shaderName;
}

void TextureRenderer::SetTexture(Texture2D* texture) {

	this->texture = texture;
}