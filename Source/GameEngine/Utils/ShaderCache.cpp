#include "ShaderCache.h"

Shader* ShaderCache::GetShader(std::string name) {
	if (shaders.find(name) == shaders.end()) {
		return nullptr;
	}

	return shaders[name];
}

void ShaderCache::AddShader(std::string name, std::string shaderPath, std::string VSName, std::string FSName, bool useGeometry, std::string GSName) {
	if (shaders.find(name) != shaders.end()) {
		return;
	}
	
	Shader* shader = new Shader(name.c_str());
	shader->AddShader(shaderPath + "/" + VSName, GL_VERTEX_SHADER);
	if (useGeometry) {
		shader->AddShader(shaderPath + "/" + GSName, GL_GEOMETRY_SHADER);
	}
	shader->AddShader(shaderPath + "/" + FSName, GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
}