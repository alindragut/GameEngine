#include "DefaultRenderer.h"
#include <GameEngine/Utils/ShaderCache.h>
#include <GameEngine/Utils/MeshManager.h>

DefaultRenderer::DefaultRenderer() {
	this->shaderName = "Default";
	this->meshName = "box";

	ShaderCache& sc = ShaderCache::GetInstance();
	sc.AddShader("Default", "Source/GameEngine/Shaders", "VertexShaderDefault.glsl", "FragmentShaderDefault.glsl");
	sc.AddShader("TextureMesh", "Source/GameEngine/Shaders", "VertexShaderDefault.glsl", "FragmentShaderMeshTexture.glsl");
	sc.AddShader("Debug", "Source/GameEngine/Shaders", "VertexShaderLighting.glsl", "FragmentShaderDebug.glsl");

	MeshManager& mm = MeshManager::GetInstance();
	mm.AddMesh("box", RESOURCE_PATH::MODELS + "Primitives", "box.obj");
}

DefaultRenderer::~DefaultRenderer() {
}

void DefaultRenderer::update(float deltaTimeSeconds) {

}

void DefaultRenderer::render() {
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

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMat));

	if (useShadowMapping) {
		int locLightView = shader->GetUniformLocation("Light_View");
		glUniformMatrix4fv(locLightView, 1, GL_FALSE, glm::value_ptr(lightView));
		int locLightProj = shader->GetUniformLocation("Light_Projection");
		glUniformMatrix4fv(locLightProj, 1, GL_FALSE, glm::value_ptr(lightProj));
	}
	else {
		lightView = camera->GetViewMatrix();
		lightProj = camera->GetProjectionMatrix();
	}
	
	MeshManager::GetInstance().GetMesh(meshName)->Render();
}

void DefaultRenderer::SetMesh(std::string meshName) {
	this->meshName = meshName;
}
	
void DefaultRenderer::SetShader(std::string shaderName) {
	this->shaderName = shaderName;
}