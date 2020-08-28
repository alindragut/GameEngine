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
	mm.AddModel("model", "Source/GameEngine/Models/Player", "Erika_archer.dae");
	mm.AddModel("model_bow", "Source/GameEngine/Models/Player", "Erika_archer_bow_arrow.dae");
	mm.AddModel("skeleton", "Source/GameEngine/Models/NPC", "Skeleton.fbx");
	mm.AddModel("bruiser", "Source/GameEngine/Models/NPC", "Bruiser.dae");
	mm.AddModel("mage", "Source/GameEngine/Models/NPC", "Mage.dae");

	mm.AddAnimation("npc", "npc_idle", "Source/GameEngine/Animations", "NPCIdle.dae");
	mm.AddAnimation("npc", "npc_walk", "Source/GameEngine/Animations", "NPCWalking.dae");
	mm.AddAnimation("npc", "npc_attack_1", "Source/GameEngine/Animations", "NPCPunching.dae");

	mm.AddAnimation("skeleton", "skeleton_idle", "Source/GameEngine/Animations/NPC/Skeleton", "Idle.dae");
	mm.AddAnimation("skeleton", "skeleton_walk", "Source/GameEngine/Animations/NPC/Skeleton", "Walk.dae");
	mm.AddAnimation("skeleton", "skeleton_attack_1", "Source/GameEngine/Animations/NPC/Skeleton", "Attack01.dae");

	mm.AddAnimation("bruiser", "bruiser_idle", "Source/GameEngine/Animations/NPC/Bruiser", "Idle.dae");
	mm.AddAnimation("bruiser", "bruiser_walk", "Source/GameEngine/Animations/NPC/Bruiser", "Walk.dae");
	mm.AddAnimation("bruiser", "bruiser_attack_1", "Source/GameEngine/Animations/NPC/Bruiser", "Attack01.dae");
	mm.AddAnimation("bruiser", "bruiser_die", "Source/GameEngine/Animations/NPC/Bruiser", "Death.dae");

	mm.AddAnimation("mage", "mage_idle", "Source/GameEngine/Animations/NPC/Mage", "Idle.dae");
	mm.AddAnimation("mage", "mage_walk", "Source/GameEngine/Animations/NPC/Mage", "Walk.dae");
	mm.AddAnimation("mage", "mage_attack_1", "Source/GameEngine/Animations/NPC/Mage", "Attack01.dae");
	mm.AddAnimation("mage", "mage_die", "Source/GameEngine/Animations/NPC/Mage", "Death.dae");

	mm.AddAnimation("model", "player_walk", "Source/GameEngine/Animations/Player/No_Weapon", "Catwalk Walk Forward.dae");
	mm.AddAnimation("model", "player_run", "Source/GameEngine/Animations/Player/No_Weapon", "Medium Run.dae");
	mm.AddAnimation("model", "player_sprint", "Source/GameEngine/Animations/Player/No_Weapon", "Fast Run.dae");
	mm.AddAnimation("model", "player_idle", "Source/GameEngine/Animations/Player/No_Weapon", "Unarmed Idle 01.dae");
	mm.AddAnimation("model", "player_idle_2", "Source/GameEngine/Animations/Player/No_Weapon", "Standing Idle 02 Looking.dae");
	mm.AddAnimation("model", "player_die", "Source/GameEngine/Animations/Player/No_Weapon", "Standing Death Right.dae");
	mm.AddAnimation("model", "player_dead", "Source/GameEngine/Animations/Player/No_Weapon", "Dead.dae");

	mm.AddAnimation("model_bow", "player_b_walk", "Source/GameEngine/Animations/Player/Weapon", "Catwalk Walk Forward.dae");
	mm.AddAnimation("model_bow", "player_b_run", "Source/GameEngine/Animations/Player/Weapon", "Medium Run.dae");
	mm.AddAnimation("model_bow", "player_b_sprint", "Source/GameEngine/Animations/Player/Weapon", "Standing Sprint Forward.dae");
	mm.AddAnimation("model_bow", "player_b_idle", "Source/GameEngine/Animations/Player/Weapon", "Standing Idle Looking.dae");
	mm.AddAnimation("model_bow", "player_b_die", "Source/GameEngine/Animations/Player/Weapon", "Standing Death Right.dae");
	mm.AddAnimation("model_bow", "player_b_dead", "Source/GameEngine/Animations/Player/Weapon", "Dead.dae");
	mm.AddAnimation("model_bow", "player_b_disarm", "Source/GameEngine/Animations/Player/Weapon", "Standing Disarm Bow.dae");
	mm.AddAnimation("model_bow", "player_b_equip", "Source/GameEngine/Animations/Player/Weapon", "Standing Equip Bow.dae");
	mm.AddAnimation("model_bow", "player_b_shoot", "Source/GameEngine/Animations/Player/Weapon", "Standing Aim Recoil.dae");



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