#pragma once

#include <GameEngine/Components/BaseComponents/GraphicsComponent.h>

class TextureRenderer : public GraphicsComponent
{
public:
	GAME_COMPONENT(TextureRenderer)

	TextureRenderer();
	~TextureRenderer();

	virtual void update(float deltaTimeSeconds) override;
	virtual void render() override;

	Texture2D* GetTexture() { return this->texture;  }

	void SetShader(std::string shaderName);
	void SetMesh(std::string meshName);
	void SetTexture(Texture2D* texture);
	void LoadCubemap();

private:
	std::string meshName;
	std::string shaderName;
	Texture2D* texture;
	unsigned int cubeMapId;
};