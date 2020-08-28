#include "Model.h"

void VertexWeight::AddBoneData(int Id, float w)
{
	for (unsigned int i = 0; i < 4; i++) {
		if (weight[i] == 0.0f) {
			boneId[i] = Id;
			weight[i] = w;
			return;
		}
	}
}

Model::Model()
{
	VAO = 0;
	ZERO_MEM(buffers);
	setLocations = false;
}


Model::~Model()
{
}

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
	glm::mat4 to;


	to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
	to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
	to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
	to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

	return to;
}

Bone* Model::CreateBoneHierarchy(Bone* bone, const aiNode* node) {
	bone->SetName(node->mName.data);

	bone->SetTransformMat(aiMatrix4x4ToGlm(&node->mTransformation));

	for (unsigned i = 0; i < node->mNumChildren; i++) {
		Bone* child = new Bone();

		bone->AddChild(CreateBoneHierarchy(child, node->mChildren[i]));
	}
	return bone;
}
Bone* Model::GetRootBone() {
	return rootBone;
}

void Model::SetRootBone(Bone* rootBone) {
	this->rootBone = rootBone;
}

std::map<std::string, Bone*>* Model::GetBones() {
	return &bones;
}

GLenum Model::LoadModel(const aiScene* scene) {
	inverseGlobalMat = glm::inverse(rootBone->GetTransformMat());
	Init(scene->mRootNode, scene);
	InitTextures(scene);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(5, buffers);
	glBindVertexArray(VAO);
	

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBones[0]) * vertexBones.size(), &vertexBones[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexWeight), (const GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexWeight), (const GLvoid*)16);
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	return glGetError();
}

void Model::VisitBoneTree(float time, Animation* anim, Bone* bone, glm::mat4 parentTransform) {
	if (anim == nullptr) {
		return;
	}

	std::string boneName = bone->GetName();
	glm::mat4 transform = parentTransform * bone->GetTransformMat();

	if (bones.find(boneName) != bones.end()) {
		glm::vec3 interpScaling = anim->InterpolateScaling(time, boneName);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1), interpScaling);

		glm::quat interpRot = anim->InterpolateRotation(time, boneName);
		glm::mat4 rotMat = glm::toMat4(interpRot);

		glm::vec3 interpPos = anim->InterpolatePosition(time, boneName);
		glm::mat4 transMat = glm::translate(glm::mat4(1), interpPos);

		transform = parentTransform * transMat * rotMat * scaleMat;
		transforms[bones[boneName]->GetID()] = transform * bones[boneName]->GetTransformMat();
	}

	for (Bone* child : bone->GetChildren()) {
		VisitBoneTree(time, anim, child, transform);
	}
}

void Model::Render(Shader* shader, glm::mat4 ViewMat, glm::mat4 ProjectionMat, glm::mat4 ModelMat, float time, Animation* anim, bool depth, float farPlane, glm::vec3 lightPos) {
	if (!shader || !shader->GetProgramID()) {
		printf("Shader invalid\n");
		return;
	}

	transforms.clear();
	transforms.resize(bones.size());
	VisitBoneTree(time, anim, rootBone, glm::mat4(1));

	shader->Use();

	if (depth) {
		glUniformMatrix4fv(shader->GetUniformLocation("view[0]"), 1, GL_FALSE, glm::value_ptr(ProjectionMat * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))));
		glUniformMatrix4fv(shader->GetUniformLocation("view[1]"), 1, GL_FALSE, glm::value_ptr(ProjectionMat * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))));
		glUniformMatrix4fv(shader->GetUniformLocation("view[2]"), 1, GL_FALSE, glm::value_ptr(ProjectionMat * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0))));
		glUniformMatrix4fv(shader->GetUniformLocation("view[3]"), 1, GL_FALSE, glm::value_ptr(ProjectionMat * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0))));
		glUniformMatrix4fv(shader->GetUniformLocation("view[4]"), 1, GL_FALSE, glm::value_ptr(ProjectionMat * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0))));
		glUniformMatrix4fv(shader->GetUniformLocation("view[5]"), 1, GL_FALSE, glm::value_ptr(ProjectionMat * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))));
		glUniform3fv(shader->GetUniformLocation("light_pos"), 1, glm::value_ptr(lightPos));
		glUniform1f(shader->GetUniformLocation("far"), farPlane);
	}
	int View_loc = shader->GetUniformLocation("View");
	glUniformMatrix4fv(View_loc, 1, GL_FALSE, glm::value_ptr(ViewMat));
	int Proj_loc = shader->GetUniformLocation("Projection");
	glUniformMatrix4fv(Proj_loc, 1, GL_FALSE, glm::value_ptr(ProjectionMat));

	int Model_loc = shader->GetUniformLocation("Model");
	glUniformMatrix4fv(Model_loc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	if (!setLocations) {
		setLocations = true;
		for (unsigned int i = 0; i < 206; i++) {
			char loc[15];
			memset(loc, 0, 15);
			sprintf(loc, "Bones[%d]", i);
			locations[i] = shader->GetUniformLocation(loc);
		}
	}

	for (unsigned int i = 0; i < transforms.size(); i++) {
		glUniformMatrix4fv(locations[i], 1, GL_FALSE, glm::value_ptr(transforms[i]));
	}

	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < entries.size(); i++)
	{
		unsigned int materialIndex = entries[i].materialIndex;

		if (materialIndex < textures.size() && textures[materialIndex] != NULL)
			textures[materialIndex]->BindToTextureUnit(GL_TEXTURE0);
		
		glDrawElementsBaseVertex(GL_TRIANGLES, entries[i].nrIndices,
			GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * entries[i].baseIndex),
			entries[i].baseVertex);
	}
	glBindVertexArray(0);
	
}

void Model::Init(const aiNode* node, const aiScene* scene) {

	entries.resize(scene->mNumMeshes);
	textures.resize(scene->mNumMaterials);

	unsigned int nrVertices = 0;
	unsigned int nrIndices = 0;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		entries[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
		entries[i].nrIndices = scene->mMeshes[i]->mNumFaces * 3;
		entries[i].baseVertex = nrVertices;
		entries[i].baseIndex = nrIndices;

		nrVertices += scene->mMeshes[i]->mNumVertices;
		nrIndices += entries[i].nrIndices;
	}

	positions.reserve(nrVertices);
	normals.reserve(nrVertices);
	texCoords.reserve(nrVertices);
	indices.reserve(nrIndices);
	vertexBones.resize(nrVertices);

	for (unsigned i = 0; i < entries.size(); i++) {
		aiMesh* mesh = scene->mMeshes[i];
		InitModel(i, mesh);
	}
}

void Model::InitTextures(const aiScene* scene) {
	for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
		const aiMaterial* material = scene->mMaterials[i];

		aiString texture_path;

		textures[i] = NULL;

		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string path;
			int index = strlen(texture_path.data);
			while (texture_path.data[--index] != '/') {}
			path.append(&texture_path.data[index + 1]);
			printf("%s\n", path.c_str());
			textures[i] = TextureManager::LoadTexture("Source/GameEngine/Textures", path.c_str());
		}
	}
}

void Model::InitModel(unsigned int index, const aiMesh* mesh) {
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(mesh->mVertices[i]);
		positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		texCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
	}

	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		std::string boneName(mesh->mBones[i]->mName.data);
		int boneId = 0;

		if (bones.find(boneName) == bones.end()) {
			Bone* bone = new Bone();
			bone->SetName(boneName);
			bone->SetID(bones.size());
			boneId = bone->GetID();

			bone->SetTransformMat(aiMatrix4x4ToGlm(&(mesh->mBones[i]->mOffsetMatrix)));

			bones[boneName] = bone;
		}
		else {
			boneId = bones[boneName]->GetID();
		}

		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			unsigned int vertexID = entries[index].baseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			vertexBones[vertexID].AddBoneData(boneId, weight);
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
}

void Model::NormalizePositions() {
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

	for (auto pos : positions) {
		if (pos.x < minX) {
			minX = pos.x;
		}

		if (pos.y < minY) {
			minY = pos.y;
		}

		if (pos.z < minZ) {
			minZ = pos.z;
		}

		if (pos.x > maxX) {
			maxX = pos.x;
		}

		if (pos.y > maxY) {
			maxY = pos.y;
		}

		if (pos.z > maxZ) {
			maxZ = pos.z;
		}
	}

	for (auto it = positions.begin(); it != positions.end();) {
		glm::vec3 pos = *it;
		float newX = (pos.x - minX) / (maxX - minX) - 0.5f;
		float newY = (pos.y - minY) / (maxY - minY) - 0.5f;
		float newZ = (pos.z - minZ) / (maxZ - minZ) - 0.5f;

		*it = glm::vec3(newX, newY, newZ);
		++it;
	}
}

glm::vec3 Model::GetScaleDifference() {
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

	for (auto pos : positions) {
		if (pos.x < minX) {
			minX = pos.x;
		}

		if (pos.y < minY) {
			minY = pos.y;
		}

		if (pos.z < minZ) {
			minZ = pos.z;
		}

		if (pos.x > maxX) {
			maxX = pos.x;
		}

		if (pos.y > maxY) {
			maxY = pos.y;
		}

		if (pos.z > maxZ) {
			maxZ = pos.z;
		}
	}

	return glm::vec3(maxX, maxY, maxZ) - glm::vec3(minX, minY, minZ);
}