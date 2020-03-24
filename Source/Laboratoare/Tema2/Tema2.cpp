#include "Tema2.h"

#include <vector>
#include <iostream>
#include <map>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

Tema2::Tema2()
{
	outputMode = 0;
	gpuProcessing = false;
	saveScreenToImage = false;
	threshold = 0.5f;
	nrRegions = 8;
	window->SetSize(600, 600);
}

Tema2::~Tema2()
{
}

FrameBuffer *processed;

void Tema2::Init()
{
	// Load default texture fore imagine processing 
	originalImage = TextureManager::LoadTexture(RESOURCE_PATH::TEXTURES + "Cube/posx.png", nullptr, "image", true, true);
	processedImage = TextureManager::LoadTexture(RESOURCE_PATH::TEXTURES + "Cube/posx.png", nullptr, "newImage", true, true);

	{
		Mesh* mesh = new Mesh("quad");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "quad.obj");
		mesh->UseMaterials(false);
		meshes[mesh->GetMeshID()] = mesh;
	}

	static std::string shaderPath = "Source/Laboratoare/Tema2/Shaders/";

	// Create a shader program for particle system
	{
		Shader *shader = new Shader("ImageProcessing");
		shader->AddShader((shaderPath + "VertexShader.glsl").c_str(), GL_VERTEX_SHADER);
		shader->AddShader((shaderPath + "FragmentShader.glsl").c_str(), GL_FRAGMENT_SHADER);

		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Tema2::FrameStart()
{
}

void Tema2::Update(float deltaTimeSeconds)
{
	ClearScreen();

	auto shader = shaders["ImageProcessing"];
	shader->Use();

	if (saveScreenToImage)
	{
		window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
	}

	int flip_loc = shader->GetUniformLocation("flipVertical");
	glUniform1i(flip_loc, saveScreenToImage ? 0 : 1);

	int screenSize_loc = shader->GetUniformLocation("screenSize");
	glm::ivec2 resolution = window->GetResolution();
	glUniform2i(screenSize_loc, resolution.x, resolution.y);

	int outputMode_loc = shader->GetUniformLocation("outputMode");
	glUniform1i(outputMode_loc, outputMode);

	int gpuProcessing_loc = shader->GetUniformLocation("outputMode");
	glUniform1i(outputMode_loc, outputMode);

	int locTexture = shader->GetUniformLocation("textureImage");
	glUniform1i(locTexture, 0);

	int locThreshold = shader->GetUniformLocation("threshold");
	glUniform1f(locThreshold, threshold);

	int locNrRegions = shader->GetUniformLocation("nrRegions");
	glUniform1i(locNrRegions, nrRegions);

	auto textureImage = (gpuProcessing == true) ? originalImage : processedImage;
	textureImage->BindToTextureUnit(GL_TEXTURE0);

	RenderMesh(meshes["quad"], shader, glm::mat4(1));

	if (saveScreenToImage)
	{
		saveScreenToImage = false;
		GLenum format = GL_RGB;
		if (originalImage->GetNrChannels() == 4)
		{
			format = GL_RGBA;
		}
		glReadPixels(0, 0, originalImage->GetWidth(), originalImage->GetHeight(), format, GL_UNSIGNED_BYTE, processedImage->GetImageData());
		processedImage->UploadNewData(processedImage->GetImageData());
		SaveImage("shader_processing_" + std::to_string(outputMode));

		float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
		window->SetSize(static_cast<int>(600 * aspectRatio), 600);
	}
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Tema2::OnFileSelected(std::string fileName)
{
	if (fileName.size())
	{
		std::cout << fileName << endl;
		originalImage = TextureManager::LoadTexture(fileName.c_str(), nullptr, "image", true, true);
		processedImage = TextureManager::LoadTexture(fileName.c_str(), nullptr, "newImage", true, true);

		float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
		window->SetSize(static_cast<int>(600 * aspectRatio), 600);
	}
}

void Tema2::GrayScale()
{
	unsigned int channels = originalImage->GetNrChannels();
	unsigned char* data = originalImage->GetImageData();
	unsigned char* newData = processedImage->GetImageData();

	if (channels < 3)
		return;

	glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());

	for (int i = 0; i < imageSize.y; i++)
	{
		for (int j = 0; j < imageSize.x; j++)
		{
			int offset = channels * (i * imageSize.x + j);

			// Reset save image data
			char value = static_cast<char>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07);
			memset(&newData[offset], value, 3);
		}
	}

	processedImage->UploadNewData(newData);
}

void Tema2::Segmentation()
{
	unsigned int channels = originalImage->GetNrChannels();
	unsigned char* data = originalImage->GetImageData();
	unsigned char* newData = processedImage->GetImageData();

	int i, j, k;

	if (channels < 3)
		return;

	glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());
	int* pixelRegion = new int[imageSize.y * imageSize.x];
	vector < glm::u64vec4 > regions;
	float deviation = 0.7f;
	for (i = 0; i < imageSize.y; i++)
	{
		for (j = 0; j < imageSize.x; j++)
		{
			int offset = channels * (i * imageSize.x + j);

			int value = static_cast<int>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07f);

			int n = regions.size();

			int minDif = 999;
			int minIdx = -1;

			if (i == 0 && j == 0) {

			}
			else if (i == 0) {
				glm::u64vec4 aux = regions[pixelRegion[i * imageSize.x + j - 1]];

				int regionValue = static_cast<int>(aux.x / aux.w * 0.2f + aux.y / aux.w * 0.71f + aux.z / aux.w * 0.07f);

				int dif = abs(value - regionValue);
				int segmThreshold = static_cast<int>(regionValue * deviation);
				if (dif < segmThreshold) {
					minIdx = pixelRegion[i * imageSize.x + j - 1];
				}
			}
			else if (j == 0) {
				glm::u64vec4 aux = regions[pixelRegion[(i - 1) * imageSize.x + j]];

				int regionValue = static_cast<int>(aux.x / aux.w * 0.2f + aux.y / aux.w * 0.71f + aux.z / aux.w * 0.07f);

				int dif = abs(value - regionValue);
				int segmThreshold = static_cast<int>(regionValue * deviation);
				if (dif < segmThreshold) {
					minIdx = pixelRegion[(i - 1) * imageSize.x + j];
				}
			}
			else {
				glm::u64vec4 aux = regions[pixelRegion[i * imageSize.x + j - 1]];

				int regionValue = static_cast<int>(aux.x / aux.w * 0.2f + aux.y / aux.w * 0.71f + aux.z / aux.w * 0.07f);

				int dif = abs(value - regionValue);
				int segmThreshold = static_cast<int>(regionValue * deviation);
				if (dif < segmThreshold) {
					minDif = dif;
					minIdx = pixelRegion[i * imageSize.x + j - 1];
				}

				aux = regions[pixelRegion[(i - 1) * imageSize.x + j]];

				regionValue = static_cast<int>(aux.x / aux.w * 0.2f + aux.y / aux.w * 0.71f + aux.z / aux.w * 0.07f);

				dif = abs(value - regionValue);
				segmThreshold = static_cast<int>(regionValue * deviation);
				if (dif < minDif && dif < segmThreshold) {
					minIdx = pixelRegion[(i - 1) * imageSize.x + j];
					minDif = dif;
				}

				aux = regions[pixelRegion[(i - 1) * imageSize.x + j - 1]];

				regionValue = static_cast<int>(aux.x / aux.w * 0.2f + aux.y / aux.w * 0.71f + aux.z / aux.w * 0.07f);

				dif = abs(value - regionValue);
				segmThreshold = static_cast<int>(regionValue * deviation);
				if (dif < minDif && dif < segmThreshold) {
					minIdx = pixelRegion[(i - 1) * imageSize.x + j - 1];
				}
			}

			if (minIdx != -1) {
				glm::u64vec4 aux = regions[minIdx];
				regions[minIdx] = glm::u64vec4(aux.x + data[offset], aux.y + data[offset + 1], aux.z + data[offset + 2], aux.w + 1);
				pixelRegion[i * imageSize.x + j] = minIdx;
			}
			else {
				pixelRegion[i * imageSize.x + j] = regions.size();
				regions.push_back(glm::u64vec4(data[offset], data[offset + 1], data[offset + 2], 1));
			}
		}
	}

	for (int i = 0; i < imageSize.y; i++)
	{
		for (int j = 0; j < imageSize.x; j++)
		{
			int offset = channels * (i * imageSize.x + j);

			glm::u64vec4 aux = regions[pixelRegion[i * imageSize.x + j]];
			
			newData[offset] = static_cast<unsigned char>(aux.x / aux.w);
			newData[offset + 1] = static_cast<unsigned char>(aux.y / aux.w);
			newData[offset + 2] = static_cast<unsigned char>(aux.z / aux.w);
		}
	}

	processedImage->UploadNewData(newData);
	

	delete[] pixelRegion;
}

void Tema2::SaveImage(std::string fileName)
{
	cout << "Saving image! ";
	processedImage->SaveToFile((fileName + ".png").c_str());
	cout << "[Done]" << endl;
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
};

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
	{
		OpenDialogue();
	}

	if (key == GLFW_KEY_E)
	{
		gpuProcessing = !gpuProcessing;
		if (gpuProcessing == false)
		{
			outputMode = 0;
		}
		cout << "Processing on GPU: " << (gpuProcessing ? "true" : "false") << endl;
	}

	if (key - GLFW_KEY_0 >= 0 && key <= GLFW_KEY_5)
	{
		outputMode = key - GLFW_KEY_0;

		if (gpuProcessing == false)
		{
			outputMode = 0;
			GrayScale();
		}
	}

	if (key == GLFW_KEY_F1) {
		threshold = max(0.0f, threshold - 0.05f);
	}

	if (key == GLFW_KEY_F2) {
		threshold = min(1.0f, threshold + 0.05f);
	}

	if (key == GLFW_KEY_F3) {
		nrRegions = max(1, nrRegions >> 1);
	}

	if (key == GLFW_KEY_F4) {
		nrRegions = min(256, nrRegions << 1);
	}

	if (key == GLFW_KEY_F5) {
		if (gpuProcessing == false)
		{
			outputMode = 0;
			Segmentation();
		}
	}

	if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL)
	{
		if (!gpuProcessing)
		{
			SaveImage("processCPU_" + std::to_string(outputMode));
		}
		else
		{
			saveScreenToImage = true;
		}
	}
};

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Tema2::OnWindowResize(int width, int height)
{
	// treat window resize event
}
