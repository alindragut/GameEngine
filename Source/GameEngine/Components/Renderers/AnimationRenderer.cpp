#include "AnimationRenderer.h"
#include <GameEngine/Utils/ShaderCache.h>
#include <GameEngine/Utils/MeshManager.h>

AnimationRenderer::AnimationRenderer(std::string defaultModelName, std::string defaultAnimName) {
	this->time = 0;
	this->shaderName = "Animation";
	this->lightPos = glm::vec3(0);
	this->depth = false;

	ShaderCache& sc = ShaderCache::GetInstance();
	sc.AddShader("Animation", "Source/GameEngine/Shaders", "VertexShaderAnim.glsl", "FragmentShaderAnim.glsl");
	sc.AddShader("AnimationDepth", "Source/GameEngine/Shaders", "VertexShaderPointShadowsAnim.glsl", "FragmentShaderPointShadows.glsl", true, "GeometryShaderPointShadows.glsl");

	MeshManager& mm = MeshManager::GetInstance();
	mm.AddModel("model", "Source/GameEngine/Models", "erika_archer.dae");
	mm.AddModel("npc", "Source/GameEngine/Models", "pirate_p_konstantinov.dae");
	mm.AddAnimation("model", "player_walk", "Source/GameEngine/Animations", "Walking.dae");
	mm.AddAnimation("model", "player_run", "Source/GameEngine/Animations", "Running.dae");
	mm.AddAnimation("model", "player_idle", "Source/GameEngine/Animations", "Idle.dae");
	mm.AddAnimation("model", "player_die", "Source/GameEngine/Animations", "Falling Forward Death.dae", true);
	mm.AddAnimation("model", "player_dead", "Source/GameEngine/Animations", "Dead.dae", true);
	mm.AddAnimation("npc", "npc_idle", "Source/GameEngine/Animations", "NPCIdle.dae");
	mm.AddAnimation("npc", "npc_walk", "Source/GameEngine/Animations", "NPCWalking.dae");
	mm.AddAnimation("npc", "npc_attack_1", "Source/GameEngine/Animations", "NPCPunching.dae");

	animInfo = new AnimationInfo(mm.GetModel(defaultModelName), mm.GetAnimation(defaultAnimName), time);
}

AnimationRenderer::~AnimationRenderer() {
	delete animInfo;
}

void AnimationRenderer::update(float deltaTimeSeconds) {
	time += deltaTimeSeconds;
}

void AnimationRenderer::render() {
	Model* currentModel = animInfo->GetModel();
	Animation* currentAnim = animInfo->GetAnimation();

	if (currentModel != nullptr && currentAnim != nullptr) {
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

		float dt = time - animInfo->GetTimeStamp();

		if (currentAnim->IsBlending() && dt >= currentAnim->GetBlendingTime()) {
			currentAnim->SetBlending(false);
			animInfo->SetTimeStamp(time);
			dt = 0;
		}

		Shader* shader = ShaderCache::GetInstance().GetShader(shaderName);

		currentModel->Render(shader, camera->GetViewMatrix(), camera->GetProjectionMatrix(), modelMat,
			dt, animInfo->GetAnimation(), depth, camera->GetProjectionInfo().zFar, lightPos);
	}
}

void AnimationRenderer::SetAnimation(std::string animationName) {
	animInfo->SetTimeStamp(time);
	animInfo->SetAnimation(MeshManager::GetInstance().GetAnimation(animationName));
}

void AnimationRenderer::SetModel(std::string modelName) {
	animInfo->SetModel(MeshManager::GetInstance().GetModel(modelName));
}

void AnimationRenderer::SetShader(std::string shaderName) {
	this->shaderName = shaderName;
}

void AnimationRenderer::SetLightPos(glm::vec3 lightPos) {
	this->lightPos = lightPos;
}

void AnimationRenderer::SetDepth(bool depth) {
	this->depth = depth;
}