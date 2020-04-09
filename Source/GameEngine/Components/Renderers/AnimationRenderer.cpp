#include "AnimationRenderer.h"
#include <GameEngine/IncludeList.h>

AnimationRenderer::AnimationRenderer(std::string defaultModelName, std::string defaultAnimName) {
	EngineManager& em = EngineManager::GetInstance();
	this->engine = em.GetGameEngine();
	this->time = 0;

	Shader *shader = new Shader("AnimationShader");
	shader->AddShader("Source/GameEngine/Shaders/VertexShaderAnim.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/GameEngine/Shaders/FragmentShaderAnim.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	Assimp::Importer importer;

	string aux = string("Source/GameEngine/Models/erika_archer.dae");

	const aiScene* scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		Bone* root = new Bone();
		Model* model = new Model();
		root = model->CreateBoneHierarchy(root, scene->mRootNode);
		model->SetRootBone(root);
		model->LoadModel(scene);
		models["model"] = model;
	}
	else {
		printf("Error\n");
		return;
	}

	aux = string("Source/GameEngine/Animations/Walking.dae");

	scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		Animation* anim_1 = new Animation();
		anim_1->Init(scene->mAnimations[0], models["model"]->GetBones());
		animations["anim_1"] = anim_1;
	}
	else {
		printf("Error\n");
		return;
	}

	aux = string("Source/GameEngine/Animations/Running.dae");

	scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		Animation* anim_2 = new Animation();
		anim_2->Init(scene->mAnimations[0], models["model"]->GetBones());
		animations["anim_2"] = anim_2;
	}
	else {
		printf("Error\n");
		return;
	}

	aux = string("Source/GameEngine/Animations/Idle.dae");

	scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		Animation* anim_3 = new Animation();
		anim_3->Init(scene->mAnimations[0], models["model"]->GetBones());
		animations["anim_3"] = anim_3;
	}
	else {
		printf("Error\n");
		return;
	}


	animInfo = new AnimationInfo(models[defaultModelName], animations[defaultAnimName], time);
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

		glm::mat4 trans = glm::translate(glm::mat4(1), object->GetTransform()->GetPos());
		glm::mat4 rot = glm::rotate(glm::mat4(1), object->GetTransform()->GetRot().y, glm::vec3(0, 1, 0));
		glm::mat4 scale = glm::scale(glm::mat4(1), object->GetTransform()->GetScale());

		glm::mat4 modelMat = trans * rot * scale;

		float dt = time - animInfo->GetTimeStamp();

		if (currentAnim->IsBlending() && dt >= currentAnim->GetBlendingTime()) {
			currentAnim->SetBlending(false);
			animInfo->SetTimeStamp(time);
			dt = 0;
		}

		currentModel->Render(shaders["AnimationShader"], engine->GetCamera()->GetViewMatrix(), engine->GetCamera()->GetProjectionMatrix(), modelMat,
			dt, animInfo->GetAnimation());
	}
}

void AnimationRenderer::SetAnimation(std::string animationName) {
	animInfo->SetTimeStamp(time);
	animInfo->SetAnimation(animations[animationName]);
}

void AnimationRenderer::SetModel(std::string modelName) {
	animInfo->SetModel(models[modelName]);
}