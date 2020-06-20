#include "TextureRenderer.h"
#include <GameEngine/Utils/ShaderCache.h>
#include <GameEngine/Utils/MeshManager.h>
#include <include/gl.h>
#include <include/utils.h>
#include <stb/stb_image.h>


TextureRenderer::TextureRenderer() {
	meshName = "box";
	ShaderCache& sc = ShaderCache::GetInstance();
	sc.AddShader("Room", "Source/GameEngine/Shaders", "VertexShaderDefault.glsl", "FragmentShaderRoom.glsl");
	sc.AddShader("Skybox", "Source/GameEngine/Shaders", "VertexShaderSkybox.glsl", "SkyBox.glsl");
	SetShader("Room");
	cubeMapId = -1;
}

TextureRenderer::~TextureRenderer() {
}

void TextureRenderer::update(float deltaTimeSeconds) {

}
void TextureRenderer::LoadCubemap()
{
	std::vector<std::string> faces =
	{
		"DrkGft.tga",
		"DrkGbk.tga",
		
		
		
		"DrkGup.tga",
		"DrkGdn.tga",
		
		"DrkGrt.tga",
		"DrkGlf.tga",
	};
	
	glGenTextures(1, &cubeMapId);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	int width, height, nrChannels;

	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(("Source/GameEngine/Textures/" + faces[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	
}

void TextureRenderer::render() {
	Shader* shader = ShaderCache::GetInstance().GetShader(shaderName);
	shader->Use();

	
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
	
	
	glm::mat4 modelMat;

	if (object->GetTransform()->ShouldUseModel()) {
		modelMat = object->GetTransform()->GetModel();
	}
	else {
		glm::mat4 trans = glm::translate(glm::mat4(1), object->GetTransform()->GetPos());
		glm::mat4 rot = glm::rotate(glm::mat4(1), object->GetTransform()->GetRot().y, glm::vec3(0, 1, 0));
		glm::mat4 scale = glm::scale(glm::mat4(1), object->GetTransform()->GetScale());

		modelMat = trans * rot * scale;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (cubeMapId != -1) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera->GetViewMatrix()))));
		
	}
	else {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMat));
	}
	
	
	

	MeshManager::GetInstance().GetMesh(meshName)->Render();
}

void TextureRenderer::SetMesh(std::string meshName) {
	this->meshName = meshName;
}

void TextureRenderer::SetShader(std::string shaderName) {
	this->shaderName = shaderName;
}

void TextureRenderer::SetTexture(Texture2D* texture) {

	this->texture = texture;
}