#include "Mesh.h"

#include <string>
#include <format>
#include <iostream>

Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<GLuint> indices,
	std::vector<Texture> textures
) : vertices(vertices), indices(indices), textures(textures) 
{
	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(VAO, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(VAO, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(VAO, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
}

void Mesh::draw(Shader& shader)
{
	using std::string;

	GLuint diffuseIndex = 0;
	GLuint specularIndex = 0;

	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		string suffix;

		switch (textures[i].type)
		{
		case diffuse:
			suffix = "diffuse" + diffuseIndex++;
			break;
		case specualar:
			suffix = "specular" + specularIndex++;
			break;
		default :
			std::cout << "Invalid texture type" << std::endl;
			break;
		}

		shader.setInt("material.texture_" + suffix, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}