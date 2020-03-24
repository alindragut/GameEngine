#pragma once
#include <Core/Engine.h>
class Bone
{
public:
	Bone();
	~Bone();

protected:
	std::string name;
	int id;
	glm::mat4 transformMat;
	std::vector<Bone*> children;
public:
	void SetName(const std::string& name);
	std::string GetName() const;

	void SetID(int id);
	int GetID() const;

	void SetTransformMat(const glm::mat4& transformMat);
	glm::mat4 GetTransformMat() const;

	void AddChild(Bone* child);
	std::vector<Bone*> GetChildren();
};

