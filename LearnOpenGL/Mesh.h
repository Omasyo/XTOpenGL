#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

enum TextureType { diffuse, specualar };

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	GLuint id;
	TextureType type;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Mesh(
		std::vector<Vertex> vertices,
		std::vector<GLuint> indices,
		std::vector<Texture> textures
	);

	void draw(Shader& shader);

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};

