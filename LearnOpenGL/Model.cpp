#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <unordered_map>

#include "stb_image.h"

unsigned int TextureFromFile(
	const char* path,
	const std::string& directory,
	bool gamma = false
);

Model::Model(char* path) { loadModel(path); }

std::unordered_map<aiTextureType, TextureType> TYPENAME = {
	{aiTextureType_DIFFUSE, TextureType::diffuse},
	{aiTextureType_SPECULAR, TextureType::specualar},
};


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

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMPT::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	using std::vector;
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		auto position = mesh->mVertices[i];
		vertex.position = glm::vec3(position.x, position.y, position.z);

		auto normal = mesh->mNormals[i];
		vertex.normal = glm::vec3(normal.x, normal.y, normal.z);

		if (mesh->mTextureCoords[0])
		{
			auto texCoords = mesh->mTextureCoords[0][i];
			vertex.texCoords = glm::vec2(texCoords.x, texCoords.y);
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		auto face = mesh->mFaces[i];
		for (size_t j = 0; i < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		auto material = scene->mMaterials[mesh->mMaterialIndex];
		auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type)
{
	std::vector<Texture> textures;
	for (size_t i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString path;
		material->GetTexture(type, i, &path);
		bool skip = false;
		for (size_t loadedIndex = 0; i < textures_loaded.size(); loadedIndex++)
		{
			if (std::strcmp(textures_loaded[loadedIndex].path.data(), path.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[loadedIndex]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture = {
			TextureFromFile(path.C_Str(), directory),
			TYPENAME[type],
			path.C_Str(),
			};
			textures.push_back(texture);
		}
	}
	return textures;
}

unsigned int TextureFromFile(
	const char* path,
	const std::string& directory,
	bool gamma
) {
	auto filename = directory + '/' + path;

	GLuint textureId;
	glGenTextures(1, &textureId);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format;
		switch (nrChannels)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path" << path << std::endl;
	}
	stbi_image_free(data);
	return textureId;
}