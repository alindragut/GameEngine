#pragma once

#include <Core/Engine.h>

class ShaderCache
{
public:
	Shader* GetShader(std::string name);
	void AddShader(std::string name, std::string shaderPath, std::string VSName, std::string FSName, bool useGeometry = false, std::string GSName = "");

	ShaderCache(ShaderCache const&) = delete;
	void operator=(ShaderCache const&) = delete;

	static ShaderCache& GetInstance() {
		static ShaderCache instance;
		return instance;
	}

private:
	std::unordered_map<std::string, Shader*> shaders;
	ShaderCache() {};
};