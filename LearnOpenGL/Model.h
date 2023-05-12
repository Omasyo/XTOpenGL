#pragma once

#include <vector>
#include <assimp/scene.h>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
	Model(char* path);
	void draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(
		aiMaterial* material, 
		aiTextureType type
	);
};

