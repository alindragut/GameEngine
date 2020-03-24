#include "Tema1.h"

#include <vector>
#include <iostream>

#include <stb/stb_image.h>

#include <Core/Engine.h>


using namespace std;

Texture2D* aux1;

struct Particle
{
	glm::vec4 position;
	glm::vec4 speed;
	glm::vec3 rotation;
	float inv;

	Particle() {};

	Particle(const glm::vec4 &pos, const glm::vec4 &speed, const glm::vec3 &rot, const float inv)
	{
		SetInitial(pos, speed, rot, inv);
	}

	void SetInitial(const glm::vec4 &pos, const glm::vec4 &speed, const glm::vec3 &rot, const float inv)
	{
		this->position = pos;
		this->rotation = rot;

		this->speed = speed;
		this->inv = inv;
	}
};

ParticleEffect<Particle> *particleEffect;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(0, 8, 8), glm::quat(glm::vec3(-40 * TO_RADIANS, 0, 0)));
	camera->Update();

	objSpeed = 0.0f;
	boom = 0;
	wireframe = 0;
	normale = 0;
	timeScale = 1.0f;
	once = true;
	impactSpeed = 0;
	triangleSize = 0.25f;
	rotateAngle = 0;
	cubeMapSize = 720;
	reflect = 1;

	std::string texturePath = RESOURCE_PATH::TEXTURES + "Cube/";

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("bamboo");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation/Bamboo", "bamboo.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("car");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "car.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Load textures
	{
		aux1 = TextureManager::LoadTexture(RESOURCE_PATH::TEXTURES, name);
	}

	LoadShader("Simple", false);
	LoadShader("Particle");
	std::string shaderPath = "Source/Laboratoare/Tema1/Shaders/";
	{
		Shader *shader = new Shader("ShaderNormal");
		shader->AddShader(shaderPath + "Normal.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "Normal.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	nrParticles = 50;
	nrRows = 10;

	float offsetX = nrParticles * triangleSize / 2.0f;
	float offsetY = nrRows * triangleSize;


	particleEffect = new ParticleEffect<Particle>();
	particleEffect->Generate(nrParticles * nrRows, true);

	auto particleSSBO = particleEffect->GetParticleBuffer();
	Particle* data = const_cast<Particle*>(particleSSBO->GetBuffer());

	int cubeSize = 20;
	int hSize = cubeSize / 2;

	for (unsigned int i = 0; i < nrRows; i++)
	{
		for (unsigned int j = 0; j < nrParticles; j += 2)
		{
			glm::vec4 pos(j * triangleSize + triangleSize - offsetX, i * 2 * triangleSize + triangleSize - offsetY, 0, -triangleSize / 8);

			glm::vec4 speed1(0);
			speed1.x = (rand() % 20 - 10) / 10.0f;
			speed1.z = (rand() % 20 - 10) / 10.0f;

			glm::vec4 speed2(0);
			speed2.x = (rand() % 20 - 10) / 10.0f;
			speed2.z = (rand() % 20 - 10) / 10.0f;

			glm::vec3 rot1(0);
			rot1.x = (rand() % 628) / 100.0f;
			rot1.y = (rand() % 628) / 100.0f;
			rot1.z = (rand() % 628) / 100.0f;

			glm::vec3 rot2(0);
			rot2.x = (rand() % 628) / 100.0f;
			rot2.y = (rand() % 628) / 100.0f;
			rot2.z = (rand() % 628) / 100.0f;

			data[i * nrParticles + j].SetInitial(pos, speed1, rot1, 0.0f);
			data[i * nrParticles + j + 1].SetInitial(pos, speed2, rot2, 1.0f);
		}
	}
	particleSSBO->SetBufferData(data);

	auto resolution = window->GetResolution();
	auto width = resolution.x;
	auto height = resolution.y;

	

	camera->SetPosition(glm::vec3(0, 0, -0.25));

	initPos = glm::vec3(0, -1, -10);

	cubeMapTextureID = UploadCubeMapTexture(
		texturePath + "posx.png",
		texturePath + "posy.png",
		texturePath + "posz.png",
		texturePath + "negx.png",
		texturePath + "negy.png",
		texturePath + "negz.png"
	);

	mirrorCubeMapTextureID = CreateCubeMap();

	frameBuffer = new FrameBuffer();
	frameBuffer->Generate(width, height, 1, true);

	
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	if (wireframe == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


void Tema1::Update(float deltaTimeSeconds)
{
	glLineWidth(3);

	float dt = deltaTimeSeconds * timeScale;

	float ff1 = 0.03f * objSpeed * timeScale;

	float ff2 = 0.03f * impactSpeed * timeScale;

	objSpeed -= ff1;

	impactSpeed -= ff2;

	if (objSpeed < 0.05f) {
		objSpeed = 0.0f;
	}

	if (impactSpeed < 0.05f) {
		impactSpeed = 0.0f;
	}

	initPos += glm::vec3(0, 0, objSpeed) * dt;
	
	if (initPos.z > -2.5f - triangleSize / 4) {
		boom = 1;
		if (once) {
			once = false;
			impactSpeed = objSpeed;
		}
	}

	auto camera = GetSceneCamera();

	glm::vec3 camPosition = camera->transform->GetWorldPosition();
	glm::quat camRotation = camera->transform->GetWorldRotation();

	if (wireframe == 0)
	{
		frameBuffer->Bind();
		camera->SetPerspective(90, 1, 0.01f, 1000);
		camera->SetPosition(glm::vec3(0, 0, 0));
		camera->SetRotation(glm::quat(0, sqrt(2) / 2, 0, sqrt(2) / 2));
		DrawScene();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mirrorCubeMapTextureID);
		glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, 0, 0, cubeMapSize, cubeMapSize);

		frameBuffer->Bind();
		camera->SetRotation(glm::quat(0, 0, sqrt(2) / 2, -sqrt(2) / 2));
		DrawScene();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mirrorCubeMapTextureID);
		glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, 0, 0, cubeMapSize, cubeMapSize);
		
		frameBuffer->Bind();
		camera->SetRotation(glm::quat(0, -1, 0, 0));
		DrawScene();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mirrorCubeMapTextureID);
		glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, 0, 0, cubeMapSize, cubeMapSize);

		frameBuffer->Bind();
		camera->SetRotation(glm::quat(0, sqrt(2) / 2, 0, -sqrt(2) / 2));
		DrawScene();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mirrorCubeMapTextureID);
		glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, 0, 0, cubeMapSize, cubeMapSize);

		frameBuffer->Bind();
		camera->SetRotation(glm::quat(-sqrt(2) / 2, sqrt(2) / 2, 0, 0));
		DrawScene();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mirrorCubeMapTextureID);
		glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, 0, 0, cubeMapSize, cubeMapSize);

		frameBuffer->Bind();
		camera->SetRotation(glm::quat(0, 0, 0, -1));
		DrawScene();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mirrorCubeMapTextureID);
		glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, 0, 0, cubeMapSize, cubeMapSize);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		FrameBuffer::BindDefault();

		camera->SetPerspective(60, window->props.aspectRatio, 0.01f, 200);

		camera->SetPosition(camPosition);
		camera->SetRotation(camRotation);
	}

	DrawScene();

	{
		auto shader = shaders["Particle"];
		if (shader->GetProgramID())
		{
			shader->Use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, mirrorCubeMapTextureID);
			
			int loc_texture = shader->GetUniformLocation("texture_cubemap");
			glUniform1i(loc_texture, 0);

			int loc_camera = shader->GetUniformLocation("camera_position");
			glUniform3f(loc_camera, camPosition.x, camPosition.y, camPosition.z);
			
			loc_texture = shader->GetUniformLocation("shatter");
			glUniform1i(loc_texture, boom);
			loc_texture = shader->GetUniformLocation("toggleNormals");
			glUniform1i(loc_texture, normale);
			loc_texture = shader->GetUniformLocation("impactSpeed");
			glUniform1f(loc_texture, impactSpeed);
			loc_texture = shader->GetUniformLocation("timeScale");
			glUniform1f(loc_texture, timeScale);
			loc_texture = shader->GetUniformLocation("triangleSize");
			glUniform1f(loc_texture, triangleSize);
			loc_texture = shader->GetUniformLocation("wireframe");
			glUniform1i(loc_texture, wireframe);
			loc_texture = shader->GetUniformLocation("ref");
			glUniform1i(loc_texture, reflect);

			particleEffect->Render(GetSceneCamera(), shader);
		}
	}
}

void Tema1::DrawScene()
{

	auto camera = GetSceneCamera();
	{
		Shader *shader = shaders["ShaderNormal"];
		shader->Use();

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
		int loc_texture = shader->GetUniformLocation("texture_cubemap");
		glUniform1i(loc_texture, 0);

		meshes["box"]->Render();
	}


	glm::mat4 model = glm::translate(glm::mat4(1), initPos);
	TextureManager::GetTexture(name)->BindToTextureUnit(GL_TEXTURE0);
	RenderMesh(meshes["car"], shaders["Simple"], model);

	rotateAngle += 0.01f;

	for (int i = 1; i < 16; i++) {
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(-2.5f + 5.0f * (i % 2), 0, -i));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["bamboo"], shaders["Simple"], modelMatrix);
	}
}

unsigned int Tema1::UploadCubeMapTexture(const std::string &posx, const std::string &posy, const std::string &posz, const std::string& negx, const std::string& negy, const std::string& negz)
{
	int width, height, chn;

	unsigned char* data_posx = stbi_load(posx.c_str(), &width, &height, &chn, 0);
	unsigned char* data_posy = stbi_load(posy.c_str(), &width, &height, &chn, 0);
	unsigned char* data_posz = stbi_load(posz.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negx = stbi_load(negx.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negy = stbi_load(negy.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negz = stbi_load(negz.c_str(), &width, &height, &chn, 0);

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posx);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posy);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posz);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negx);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negy);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negz);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	SAFE_FREE(data_posx);
	SAFE_FREE(data_posy);
	SAFE_FREE(data_posz);
	SAFE_FREE(data_negx);
	SAFE_FREE(data_negy);
	SAFE_FREE(data_negz);

	return textureID;
}

unsigned int Tema1::CreateCubeMap()
{
	int width = cubeMapSize;
	int height = cubeMapSize;
	unsigned int textureID = 1;

	glGenTextures(1, &textureID);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return textureID;
}

void Tema1::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema1::LoadShader(std::string name, bool hasGeomtery)
{
	static std::string shaderPath = "Source/Laboratoare/Tema1/Shaders/";

	// Create a shader program for particle system
	{
		Shader *shader = new Shader(name.c_str());
		shader->AddShader((shaderPath + name + ".VS.glsl").c_str(), GL_VERTEX_SHADER);
		shader->AddShader((shaderPath + name + ".FS.glsl").c_str(), GL_FRAGMENT_SHADER);
		if (hasGeomtery)
		{
			shader->AddShader((shaderPath + name + ".GS.glsl").c_str(), GL_GEOMETRY_SHADER);
		}

		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (window->KeyHold(GLFW_KEY_1)) {
		objSpeed += 10.0f * deltaTime * timeScale;
	}
};

void Tema1::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_2) {
		timeScale += 0.1f;
		printf("%lf\n", timeScale);
	}
	if (key == GLFW_KEY_3) {
		timeScale -= 0.1f;
		printf("%lf\n", timeScale);
	}
	if (key == GLFW_KEY_4) {
		wireframe ^= 1;
	}
	if (key == GLFW_KEY_5) {
		normale ^= 1;
	}
	if (key == GLFW_KEY_6) {
		reflect ^= 1;
	}
	if (key == GLFW_KEY_R) {
		Init();
	}
};

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Tema1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
