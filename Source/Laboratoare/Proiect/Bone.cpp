#include "Bone.h"

Bone::Bone()
{
}


Bone::~Bone()
{
}

void Bone::SetName(const std::string& name)
{
	this->name = name;
}

std::string Bone::GetName() const
{
	return name;
}

void Bone::SetID(int id)
{
	this->id = id;
}

int Bone::GetID() const
{
	return id;
}

void Bone::SetTransformMat(const glm::mat4& transformMat)
{
	this->transformMat = transformMat;
}

glm::mat4 Bone::GetTransformMat() const
{
	return transformMat;
}

void Bone::AddChild(Bone* child) {
	children.push_back(child);
}

std::vector<Bone*> Bone::GetChildren() {
	return children;
}