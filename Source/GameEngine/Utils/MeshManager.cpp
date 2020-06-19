#include "MeshManager.h"
#include <Core/GPU/MeshInstanced.h>

Mesh* MeshManager::GetMesh(std::string name) {
	if (meshes.find(name) == meshes.end()) {
		return nullptr;
	}

	return meshes[name];
}

Model* MeshManager::GetModel(std::string name) {
	if (models.find(name) == models.end()) {
		return nullptr;
	}

	return models[name];
}

void MeshManager::AddMesh(std::string meshName, std::string meshPath, std::string fileName, bool useMaterial, bool useTextureFolder, bool normalizePositions, bool useShader) {
	if (meshes.find(meshName) != meshes.end()) {
		return;
	}

	Mesh* mesh = new Mesh(meshName);
	mesh->UseMaterials(useMaterial);
	mesh->UseTextureFolder(useTextureFolder);
	mesh->SetNormalizePositions(normalizePositions);
	mesh->UseShader(useShader);
	mesh->LoadMesh(meshPath, fileName);
	meshes[meshName] = mesh;
}

void MeshManager::AddInstancedMesh(std::string meshName, std::string meshPath, std::string fileName, bool useMaterial, bool useTextureFolder, bool normalizePositions, bool useShader) {
	if (meshes.find(meshName) != meshes.end()) {
		return;
	}
	printf("%s\n", meshName);
	Mesh* mesh = new MeshInstanced(meshName);
	mesh->UseMaterials(useMaterial);
	mesh->UseTextureFolder(useTextureFolder);
	mesh->SetNormalizePositions(normalizePositions);
	mesh->UseShader(useShader);
	mesh->LoadMesh(meshPath, fileName);
	meshes[meshName] = mesh;
}

void MeshManager::AddModel(std::string modelName, std::string modelPath, std::string fileName) {
	if (models.find(modelName) != models.end()) {
		return;
	}

	Assimp::Importer importer;

	std::string aux = std::string(modelPath + "/" + fileName);

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
		models[modelName] = model;
	}
	else {
		printf("Error\n");
		return;
	}
}

Animation* MeshManager::GetAnimation(std::string name) {
	if (animations.find(name) == animations.end()) {
		return nullptr;
	}

	return animations[name];
}

void MeshManager::AddAnimation(std::string modelName, std::string animationName, std::string animationPath, std::string fileName, bool notRepeatable) {
	if (animations.find(animationName) != animations.end()) {
		return;
	}

	Model* model = GetModel(modelName);

	if (model == nullptr) {
		printf("Error\n");
		return;
	}
	
	Assimp::Importer importer;

	std::string aux = std::string(animationPath + "/" + fileName);

	const aiScene* scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		Animation* anim = new Animation();
		anim->Init(scene->mAnimations[0], model->GetBones());
		anim->SetNotRepeatable(notRepeatable);
		animations[animationName] = anim;
	}
	else {
		printf("Error\n");
		return;
	}
}