#include "PointShadowRendererInstanced.h"
#include <GameEngine/Utils/ShaderCache.h>
#include <GameEngine/Utils/MeshManager.h>

PointShadowRendererInstanced::PointShadowRendererInstanced() {
	lightPos = glm::vec3(10, 3, -10);
	pressed = false;
	renderDepth = true;
	shaderName = "PointShadowLight";
	depthShaderName = "PointShadowDepth";
	meshName = "box";
	skybox = false;
	dirLightView = glm::mat4(1);
	dirLightProj = glm::mat4(1);

	ShaderCache& sc = ShaderCache::GetInstance();
	sc.AddShader("PointShadowLight", "Source/GameEngine/Shaders", "VertexShaderLighting.glsl", "FragmentShaderLighting.glsl");
	sc.AddShader("DungeonPack", "Source/GameEngine/Shaders", "VertexShaderLighting.glsl", "FragmentShaderDungeon.glsl");
	sc.AddShader("PointShadowDepth", "Source/GameEngine/Shaders", "VertexShaderPointShadows.glsl", "FragmentShaderPointShadows.glsl", true, "GeometryShaderPointShadows.glsl");

	MeshManager& mm = MeshManager::GetInstance();
	mm.AddMesh("arrow", "Source/GameEngine/Models", "Arrow.fbx");
	mm.AddMesh("box", RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	//mm.AddMesh("wall01", "Source/GameEngine/Models", "SM_Bld_Castle_Wall_01.fbx", true, true, true);
	mm.AddMesh("wall01", "Source/GameEngine/Models", "ModularStoneWall.fbx", true, true, false, true);
	mm.AddMesh("fence01", "Source/GameEngine/Models", "Fence_Straight_Modular.fbx", true, true, false, true);
	mm.AddMesh("fence02", "Source/GameEngine/Models", "Fence_End_Modular.fbx", true, true, false, true);
	mm.AddMesh("fence03", "Source/GameEngine/Models", "Fence_90_Modular.fbx", true, true, false, true);
	mm.AddMesh("door01", "Source/GameEngine/Models", "SM_Bld_Rockwall_Archway_01.fbx", true, true, true);
	mm.AddMesh("floor01", "Source/GameEngine/Models", "ModularFloor.fbx", true, true, true, true);
}

PointShadowRendererInstanced::~PointShadowRendererInstanced() {
}

void PointShadowRendererInstanced::update(float deltaTimeSeconds) {
	if (pressed) {
		lightPos = camera->transform->GetWorldPosition();
	}
}

void PointShadowRendererInstanced::AddInstance(glm::mat4 model) {
	models.push_back(model);
}

void PointShadowRendererInstanced::render() {
	glBindVertexArray(buffers->VAO);
	for (unsigned int i = 0; i < meshEntries.size(); i++)
	{
		if (useMaterial)
		{
			auto materialIndex = meshEntries[i].materialIndex;

			if (materialIndex != INVALID_MATERIAL && materials[materialIndex]->texture)
			{
				(materials[materialIndex]->texture)->BindToTextureUnit(GL_TEXTURE0);
			}
			else {
				TextureManager::GetTexture(static_cast<unsigned int>(0))->BindToTextureUnit(GL_TEXTURE0);
			}

			if (useShader) {
				if (materialIndex != INVALID_MATERIAL)
				{
					glUniform3fv(shader->GetUniformLocation("diff"), 1, glm::value_ptr(materials[materialIndex]->diffuse));
				}
			}
		}

		glDrawElementsBaseVertex(glDrawMode, meshEntries[i].nrIndices,
			GL_UNSIGNED_SHORT, (void*)(sizeof(unsigned short) * meshEntries[i].baseIndex),
			meshEntries[i].baseVertex);
	}
	glBindVertexArray(0);
}

void PointShadowRendererInstanced::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_7) {
		pressed = !pressed;
	}
}

void PointShadowRendererInstanced::RenderDepth() {
	Shader* depthShader = ShaderCache::GetInstance().GetShader(depthShaderName);
	depthShader->Use();

	glm::mat4 proj = camera->GetProjectionMatrix();
	
	glUniformMatrix4fv(depthShader->GetUniformLocation("view[0]"), 1, GL_FALSE, glm::value_ptr(proj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0))));
	glUniformMatrix4fv(depthShader->GetUniformLocation("view[1]"), 1, GL_FALSE, glm::value_ptr(proj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0))));
	glUniformMatrix4fv(depthShader->GetUniformLocation("view[2]"), 1, GL_FALSE, glm::value_ptr(proj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0))));
	glUniformMatrix4fv(depthShader->GetUniformLocation("view[3]"), 1, GL_FALSE, glm::value_ptr(proj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0))));
	glUniformMatrix4fv(depthShader->GetUniformLocation("view[4]"), 1, GL_FALSE, glm::value_ptr(proj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0))));
	glUniformMatrix4fv(depthShader->GetUniformLocation("view[5]"), 1, GL_FALSE, glm::value_ptr(proj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0))));
	glUniform3fv(depthShader->GetUniformLocation("light_pos"), 1, glm::value_ptr(lightPos));
	glUniform1f(depthShader->GetUniformLocation("far"), camera->GetProjectionInfo().zFar);

	glm::mat4 modelMat;

	if (object->GetTransform()->ShouldUseModel()) {
		modelMat = object->GetTransform()->GetModel();
	}
	else {
		glm::mat4 trans = glm::translate(glm::mat4(1), object->GetTransform()->GetPos());

		glm::mat4 rot = glm::mat4(glm::quat(object->GetTransform()->GetRot()));

		glm::mat4 scale = glm::scale(glm::mat4(1), object->GetTransform()->GetScale());

		modelMat = trans * rot * scale;
	}

	glUniformMatrix4fv(depthShader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMat));
}

void PointShadowRendererInstanced::RenderLight() {
	Shader* lightShader = ShaderCache::GetInstance().GetShader(shaderName);
	lightShader->Use();

	glUniformMatrix4fv(lightShader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(lightShader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
	glUniformMatrix4fv(lightShader->GetUniformLocation("Light_View"), 1, GL_FALSE, glm::value_ptr(dirLightView));
	glUniformMatrix4fv(lightShader->GetUniformLocation("Light_Proj"), 1, GL_FALSE, glm::value_ptr(dirLightProj));
	
	glUniform3fv(lightShader->GetUniformLocation("light_pos"), 1, glm::value_ptr(lightPos));
	glUniform1f(lightShader->GetUniformLocation("far"), camera->GetProjectionInfo().zFar);
	glUniform1f(lightShader->GetUniformLocation("interior"), skybox ? 1.0f : 1.0f);
	glUniform3fv(lightShader->GetUniformLocation("view_pos"), 1, glm::value_ptr(camera->transform->GetWorldPosition()));

	glm::mat4 modelMat;

	if (object->GetTransform()->ShouldUseModel()) {
		modelMat = object->GetTransform()->GetModel();
	}
	else {
		glm::mat4 trans = glm::translate(glm::mat4(1), object->GetTransform()->GetPos());

		glm::mat4 rot = glm::mat4(glm::quat(object->GetTransform()->GetRot()));

		glm::mat4 scale = glm::scale(glm::mat4(1), object->GetTransform()->GetScale());

		modelMat = trans * rot * scale;
	}

	glUniformMatrix4fv(lightShader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMat));
}

void PointShadowRendererInstanced::SetMesh(std::string meshName) {
	this->meshName = meshName;
}