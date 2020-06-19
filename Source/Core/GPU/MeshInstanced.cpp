#include "MeshInstanced.h"
#include <Core/GPU/GPUBuffers.h>
#include <GameEngine/Utils/ShaderCache.h>

MeshInstanced::MeshInstanced(std::string meshName) : Mesh(meshName) {
	
}

void MeshInstanced::AddInstance(glm::mat4 model) {
	this->models.push_back(model);
}

void MeshInstanced::Render() const {
	glBindVertexArray(buffers->VAO);
	Shader* shader = ShaderCache::GetInstance().GetShader("DungeonPack");
	for (unsigned int i = 0; i < meshEntries.size(); i++)
	{
		if (useMaterial)
		{
			auto materialIndex = meshEntries[i].materialIndex;

			if (materialIndex != INVALID_MATERIAL && materials[materialIndex]->texture)
			{
				(materials[materialIndex]->texture)->BindToTextureUnit(GL_TEXTURE0);
			}
			else {
				TextureManager::GetTexture(static_cast<unsigned int>(0))->BindToTextureUnit(GL_TEXTURE0);
			}

			if (useShader) {
				if (materialIndex != INVALID_MATERIAL)
				{
					glUniform3fv(shader->GetUniformLocation("diff"), 1, glm::value_ptr(materials[materialIndex]->diffuse));
				}
			}
		}

		glDrawElementsInstancedBaseVertex(glDrawMode, meshEntries[i].nrIndices,
			GL_UNSIGNED_SHORT, (void*)(sizeof(unsigned short) * meshEntries[i].baseIndex), models.size(),
			meshEntries[i].baseVertex);
	}
	glBindVertexArray(0);
}

void MeshInstanced::Reconstruct() {
	glBindVertexArray(buffers->VAO);

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}