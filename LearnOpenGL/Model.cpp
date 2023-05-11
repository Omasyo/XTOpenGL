#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model(char* path) { loadModel(path); }

void Model::draw(Shader& shader)
{
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	auto  scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

}