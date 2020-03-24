#pragma once
#include <Core/Engine.h>
#include "Bone.h"
#include "Animation.h"
#include <map>

struct VertexWeight {
	int boneId[4];
	float weight[4];

	VertexWeight()
	{
		Reset();
	};

	void Reset()
	{
		ZERO_MEM(boneId);
		ZERO_MEM(weight);
	}

	void AddBoneData(int Id, float w);
};

struct Entry {
	Entry()
	{
		materialIndex = 0;
		nrIndices = 0;
		baseVertex = 0;
		baseIndex = 0;
	}

	unsigned int materialIndex;
	unsigned int nrIndices;
	unsigned int baseVertex;
	unsigned int baseIndex;
};

class Model
{
public:
	Model();
	~Model();

protected:
	std::map<std::string, Bone*> bones;
	std::vector<VertexWeight> vertexBones;
	Bone* rootBone;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> positions;
	std::vector<unsigned int> indices;
	std::vector<Texture2D*> textures;
	glm::mat4 inverseGlobalMat;
	std::vector<glm::mat4> transforms;
	GLuint locations[206];
	bool setLocations;

public:
	Bone* CreateBoneHierarchy(Bone* bone, const aiNode* node);
	Bone* GetRootBone();
	void SetRootBone(Bone* rootBone);
	std::map<std::string, Bone*>* GetBones();
	GLenum LoadModel(const aiScene* scene);
	void Render(Shader* shader, glm::mat4 ViewMat, glm::mat4 ProjectionMat, glm::mat4 ModelMat, float time, Animation* anim);



private:
	void VisitBoneTree(float time, Animation* anim, Bone* bone, glm::mat4 parentTransform);
	void InitModel(unsigned int, const aiMesh* mesh);
	void Init(const aiNode* node, const aiScene* scene);
	void InitTextures(const aiScene* scene);
	GLuint VAO;
	GLuint buffers[6];
	std::vector<Entry> entries;
};

