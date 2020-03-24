#pragma once

#include <GameEngine/Components/BaseComponents/GraphicsComponent.h>

class DefaultRenderer : public GraphicsComponent
{
public:
	GAME_COMPONENT(DefaultRenderer)

	DefaultRenderer();
	~DefaultRenderer();
	
	virtual void update(float deltaTimeSeconds);

	Shader* GetShader() { return this->shader; }
	Mesh* GetMesh() { return this->mesh; }

	void SetShader(std::string shaderPath, std::string VSName, std::string FSName, std::string shaderName);
	void SetMesh(std::string meshPath, std::string fileName, std::string meshName);

private:
	Mesh* mesh;
	Shader* shader;
};