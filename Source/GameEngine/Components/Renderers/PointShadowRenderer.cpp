#include "PointShadowRenderer.h"
#include <GameEngine/Utils/ShaderCache.h>
#include <GameEngine/Utils/MeshManager.h>

PointShadowRenderer::PointShadowRenderer() {
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
	sc.AddShader("DungeonPack", "Source/GameEngine/Shaders", "VertexShaderDungeon.glsl", "FragmentShaderDungeon.glsl");
	sc.AddShader("PointShadowDepth", "Source/GameEngine/Shaders", "VertexShaderPointShadows.glsl", "FragmentShaderPointShadows.glsl", true, "GeometryShaderPointShadows.glsl");

	MeshManager& mm = MeshManager::GetInstance();

	mm.AddMesh("arrow", "Source/GameEngine/Models/Gameplay", "Arrow.fbx");
	mm.AddMesh("box", RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	mm.AddMesh("sphere", RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");

	mm.AddInstancedMesh("wall01", "Source/GameEngine/Models/Room", "ModularStoneWall.fbx", true, true, false, true);
	mm.AddInstancedMesh("fence01", "Source/GameEngine/Models/Room", "Fence_Straight_Modular.fbx", true, true, false, true);
	mm.AddInstancedMesh("fence02", "Source/GameEngine/Models/Room", "Fence_End_Modular.fbx", true, true, false, true);
	mm.AddInstancedMesh("fence03", "Source/GameEngine/Models/Room", "Fence_90_Modular.fbx", true, true, false, true);
	mm.AddInstancedMesh("door01", "Source/GameEngine/Models/Room", "SM_Bld_Rockwall_Archway_01.fbx", true, true, true);
	mm.AddInstancedMesh("floor01", "Source/GameEngine/Models/Room", "ModularFloor.fbx", true, true, true, true);

	mm.AddInstancedMesh("column", "Source/GameEngine/Models/Decorations", "Column.fbx", true, true, true, true);
	mm.AddInstancedMesh("column_broken", "Source/GameEngine/Models/Decorations", "Column_Broken.fbx", true, true, true, true);
	mm.AddInstancedMesh("banner", "Source/GameEngine/Models/Decorations", "Banner.fbx", true, true, false, true);
	mm.AddInstancedMesh("banner_wall", "Source/GameEngine/Models/Decorations", "Banner_wall.fbx", true, true, false, true);
	mm.AddInstancedMesh("barrel", "Source/GameEngine/Models/Decorations", "Barrel.fbx", true, true, false, true);
	mm.AddInstancedMesh("barrel_2", "Source/GameEngine/Models/Decorations", "Barrel2.fbx", true, true, false, true);
	mm.AddInstancedMesh("bones", "Source/GameEngine/Models/Decorations", "Bones.fbx", true, true, false, true);
	mm.AddInstancedMesh("bones_2", "Source/GameEngine/Models/Decorations", "Bones2.fbx", true, true, false, true);
	mm.AddInstancedMesh("book", "Source/GameEngine/Models/Decorations", "Book.fbx", true, true, false, true);
	mm.AddInstancedMesh("book_2", "Source/GameEngine/Models/Decorations", "Book2.fbx", true, true, false, true);
	mm.AddInstancedMesh("book_3", "Source/GameEngine/Models/Decorations", "Book3.fbx", true, true, false, true);
	mm.AddInstancedMesh("bucket", "Source/GameEngine/Models/Decorations", "Bucket.fbx", true, true, false, true);
	mm.AddInstancedMesh("candelabrum_tall", "Source/GameEngine/Models/Decorations", "Candelabrum_tall.fbx", true, true, false, true);
	mm.AddInstancedMesh("candle", "Source/GameEngine/Models/Decorations", "Candle.fbx", true, true, false, true);
	mm.AddInstancedMesh("carpet", "Source/GameEngine/Models/Decorations", "Carpet.fbx", true, true, false, true);
	mm.AddInstancedMesh("chair", "Source/GameEngine/Models/Decorations", "Chair.fbx", true, true, false, true);
	mm.AddInstancedMesh("chest", "Source/GameEngine/Models/Decorations", "Chest.fbx", true, true, false, true);
	mm.AddInstancedMesh("cobweb", "Source/GameEngine/Models/Decorations", "Cobweb.fbx", true, true, false, true);
	mm.AddInstancedMesh("cobweb_2", "Source/GameEngine/Models/Decorations", "Cobweb2.fbx", true, true, false, true);
	mm.AddInstancedMesh("crate", "Source/GameEngine/Models/Decorations", "Crate.fbx", true, true, false, true);
	mm.AddInstancedMesh("pedestal", "Source/GameEngine/Models/Decorations", "Pedestal.fbx", true, true, false, true);
	mm.AddInstancedMesh("pedestal_2", "Source/GameEngine/Models/Decorations", "Pedestal2.fbx", true, true, false, true);
	mm.AddInstancedMesh("potion", "Source/GameEngine/Models/Decorations", "Potion.fbx", true, true, false, true);
	mm.AddInstancedMesh("potion_2", "Source/GameEngine/Models/Decorations", "Potion2.fbx", true, true, false, true);
	mm.AddInstancedMesh("potion_3", "Source/GameEngine/Models/Decorations", "Potion3.fbx", true, true, false, true);
	mm.AddInstancedMesh("potion_4", "Source/GameEngine/Models/Decorations", "Potion4.fbx", true, true, false, true);
	mm.AddInstancedMesh("potion_5", "Source/GameEngine/Models/Decorations", "Potion5.fbx", true, true, false, true);
	mm.AddInstancedMesh("potion_6", "Source/GameEngine/Models/Decorations", "Potion6.fbx", true, true, false, true);
	mm.AddInstancedMesh("skull", "Source/GameEngine/Models/Decorations", "Skull.fbx", true, true, false, true);
	mm.AddInstancedMesh("table_big", "Source/GameEngine/Models/Decorations", "Table_Big.fbx", true, true, false, true);
	mm.AddInstancedMesh("table_small", "Source/GameEngine/Models/Decorations", "Table_Small.fbx", true, true, false, true);
	mm.AddInstancedMesh("torch_wall", "Source/GameEngine/Models/Decorations", "Torch_Wall.fbx", true, true, false, true);
	mm.AddInstancedMesh("vase", "Source/GameEngine/Models/Decorations", "Vase.fbx", true, true, false, true);
	mm.AddInstancedMesh("window", "Source/GameEngine/Models/Decorations", "Window.fbx", true, true, false, true);
	mm.AddInstancedMesh("woodfire", "Source/GameEngine/Models/Decorations", "Woodfire.fbx", true, true, false, true);

}

PointShadowRenderer::~PointShadowRenderer() {
}

void PointShadowRenderer::update(float deltaTimeSeconds) {
	if (pressed) {
		lightPos = camera->transform->GetWorldPosition();
	}
}

void PointShadowRenderer::render() {
	if (renderDepth) {
		RenderDepth();
	} else {
		RenderLight();
	}
	
	MeshManager::GetInstance().GetMesh(meshName)->Render();
}

void PointShadowRenderer::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_7) {
		pressed = !pressed;
	}
}

void PointShadowRenderer::RenderDepth() {
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

void PointShadowRenderer::RenderLight() {
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

void PointShadowRenderer::SetMesh(std::string meshName) {
	this->meshName = meshName;
}