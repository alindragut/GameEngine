#pragma once

#include <Component/SimpleScene.h>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void DrawScene();

	void LoadShader(std::string name, bool hasGeomtery = true);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	unsigned int UploadCubeMapTexture(const std::string &posx, const std::string &posy, const std::string &posz, const std::string& negx, const std::string& negy, const std::string& negz);
	unsigned int CreateCubeMap();


protected:
	char* name = "particle2.png";
	FrameBuffer *frameBuffer;
	int cubeMapTextureID;
	int mirrorCubeMapTextureID;
	glm::vec3 initPos;
	float objSpeed;
	int boom;
	int wireframe;
	int normale;
	int reflect;
	float timeScale;
	bool once;
	float impactSpeed;
	float triangleSize;
	float rotateAngle;
	unsigned int nrRows;
	unsigned int nrParticles;
	unsigned int cubeMapSize;
};
