#include "Proiect.h"

#include <vector>
#include <iostream>
#include <map>

using namespace std;
Proiect::Proiect():
	time(0),
	whichAnimation(0)
{
}

Proiect::~Proiect()
{
}

void PrintTree(Bone* bone) {
	printf("%s\n", bone->GetName().c_str());
	std::vector<Bone*> children = bone->GetChildren();
	for (Bone* aux : children) {
		PrintTree(aux);
	}
}

void Proiect::Init()
{
	Assimp::Importer importer;

	string aux = string("Source/Jumping.dae");

	Shader *shader = new Shader("ShaderProiect");
	shader->AddShader("Source/Laboratoare/Proiect/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Proiect/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	model = new Model();
	bob = new Model();
	animation1 = new Animation();
	animation2 = new Animation();
	animation3 = new Animation();
	animation4 = new Animation();

	const aiScene* scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		Bone* root = new Bone();
		root = model->CreateBoneHierarchy(root, scene->mRootNode);
		model->SetRootBone(root);
		//PrintTree(root);
		model->LoadModel(scene);
		animation1->Init(scene->mAnimations[0], model->GetBones());
	}
	else {
		printf("Error\n");
		return;
	}

	aux = string("Source/NoSkinDance.dae");

	scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		animation2->Init(scene->mAnimations[0], model->GetBones());
	}
	else {
		printf("Error\n");
		return;
	}

	aux = string("Source/NoSkinShoved.dae");

	scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		animation3->Init(scene->mAnimations[0], model->GetBones());
	}
	else {
		printf("Error\n");
		return;
	}

	aux = string("Source/boblampclean.md5mesh");

	scene = importer.ReadFile(aux,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (scene) {
		Bone* root = new Bone();
		root = bob->CreateBoneHierarchy(root, scene->mRootNode);
		bob->SetRootBone(root);
		bob->LoadModel(scene);
		animation4->Init(scene->mAnimations[0], bob->GetBones());
	}
	else {
		printf("Error\n");
		return;
	}
}

void Proiect::FrameStart()
{
	ClearScreen();
}

void Proiect::Update(float deltaTimeSeconds)
{
	time += deltaTimeSeconds;
	auto camera = GetSceneCamera();
	if (whichAnimation == 0) {
		model->Render(shaders["ShaderProiect"], camera->GetViewMatrix(), camera->GetProjectionMatrix(), glm::mat4(1), time, animation1);
	}
	else if (whichAnimation == 1) {
		model->Render(shaders["ShaderProiect"], camera->GetViewMatrix(), camera->GetProjectionMatrix(), glm::mat4(1), time, animation2);
	}
	else if (whichAnimation == 2) {
		model->Render(shaders["ShaderProiect"], camera->GetViewMatrix(), camera->GetProjectionMatrix(), glm::mat4(1), time, animation3);
	}
	else {
		bob->Render(shaders["ShaderProiect"], camera->GetViewMatrix(), camera->GetProjectionMatrix(), glm::mat4(1), time, animation4);
	}
	
}

void Proiect::FrameEnd()
{
	DrawCoordinatSystem();
}

void Proiect::OnInputUpdate(float deltaTime, int mods)
{
}

void Proiect::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_1) {
		time = 0;
		whichAnimation = 0;
	}

	if (key == GLFW_KEY_2) {
		time = 0;
		whichAnimation = 1;
	}

	if (key == GLFW_KEY_3) {
		time = 0;
		whichAnimation = 2;
	}

	if (key == GLFW_KEY_4) {
		time = 0;
		whichAnimation = 3;
	}
}

void Proiect::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Proiect::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Proiect::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Proiect::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Proiect::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Proiect::OnWindowResize(int width, int height)
{
	// treat window resize event
}
