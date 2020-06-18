#pragma once

#include <GameEngine/Components/Renderers/Model.h>

class MeshManager
{
public:
	Mesh* GetMesh(std::string name);
	void AddMesh(std::string meshName, std::string meshPath, std::string fileName, bool useMaterial = false, bool useTextureFolder = false, bool normalizePositions = false, bool useShader = false);

	Model* GetModel(std::string name);
	void AddModel(std::string modelName, std::string modelPath, std::string fileName);

	Animation* GetAnimation(std::string name);
	void AddAnimation(std::string modelName, std::string animationName, std::string animationPath, std::string fileName, bool notRepeatable = false);

	MeshManager(MeshManager const&) = delete;
	void operator=(MeshManager const&) = delete;

	static MeshManager& GetInstance() {
		static MeshManager instance;
		return instance;
	}

private:
	std::unordered_map<std::string, Mesh*> meshes;
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, Animation*> animations;
	MeshManager() {};
};