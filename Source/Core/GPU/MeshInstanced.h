#pragma once
#include <string>
#include <vector>

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

#include <include/glm.h>
#include "Mesh.h"


class MeshInstanced : public Mesh
{
public:
	MeshInstanced(std::string meshName);
	void AddInstance(glm::mat4 model);
	void Render() const;
	void Reconstruct();

private:
	
	std::vector<glm::mat4> models;
};