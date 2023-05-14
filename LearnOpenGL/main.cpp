#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <format>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vShaderPath = "shaders/lighting_shader.vert";
const char* fShaderPath = "shaders/lighting_shader.frag";
const char* modelPath = "backpack/backpack.obj";

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void  framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLuint loadTexture(const char* path);

Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_WIDTH / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float ratio = 0.5f;

GLfloat genRand(GLfloat low, GLfloat high) {

	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "XTOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader lightingShader(vShaderPath, fShaderPath);

	srand(static_cast <unsigned> (time(0)));

	Model ourModel(modelPath);

	//lightingCubeShader.use();
	//lightingShader.use();
	//lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//lightingCubeShader.setInt("boxTexture", 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	camera.mouseSensitivity = 0.5f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.03f, 0.232f, 0.035f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		lightingShader.use();
		//lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);


		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		//lightingShader.setFloat("material.shininess", 64.0f);

		//lightingShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
		//lightingShader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		//lightingShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
		//lightingShader.setFloat("material.shininess", 32.0f);



		//auto dirLightColor = glm::vec3(0.30f, 0.35f, 0.40f);

		//glm::vec3 dirDiffuseColor = dirLightColor * glm::vec3(0.1f);
		//glm::vec3 dirAmbientColor = dirDiffuseColor * glm::vec3(0.1f);


		//auto torchLightColor = glm::vec3(1.0f);

		//glm::vec3 torchDiffuseColor = torchLightColor * glm::vec3(1.0f);
		//glm::vec3 torchAmbientColor = torchDiffuseColor * glm::vec3(0.05f);


		//lightingShader.setVec3("viewPos", camera.position);

		//lightingShader.setVec3("dirLight.direction", -1.0f, -1.0f, -1.0f);
		//lightingShader.setVec3("dirLight.ambient", dirAmbientColor);
		//lightingShader.setVec3("dirLight.diffuse", dirDiffuseColor);
		//lightingShader.setVec3("dirLight.specular", dirLightColor);


		//lightingShader.use();
		//lightingShader.setVec3("torchLight.position", camera.position);
		//lightingShader.setVec3("torchLight.direction", camera.front);
		//lightingShader.setFloat("torchLight.cutOff", glm::cos(glm::radians(5.0f)));
		//lightingShader.setFloat("torchLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		//lightingShader.setVec3("torchLight.ambient", torchAmbientColor);
		//lightingShader.setVec3("torchLight.diffuse",torchDiffuseColor);
		//lightingShader.setVec3("torchLight.specular", torchLightColor);
		//lightingShader.setFloat("torchLight.constant", 1.0f);
		//lightingShader.setFloat("torchLight.linear", 0.09f);
		//lightingShader.setFloat("torchLight.quadratic", 0.032f);

		ourModel.draw(lightingShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	//	ratio = std::min(ratio += 0.001f, 1.0f);
	//}
	//if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	//	ratio = std::max(ratio -= 0.001f, 0.5f);
	//}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.processKeyboard(RIGHT, deltaTime);
	}
	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
	//	yCoord -= speed;
	//}
	//if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
	//	yCoord += speed;
	//}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera.processRotate(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera.processRotate(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera.processRotate(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera.processRotate(RIGHT, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(static_cast<float>(yOffset));
}


void  framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLuint loadTexture(char const* path) {
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
		std::cout << "Texture failed to load at path" << path <<std::endl;
	}
	stbi_image_free(data);
	return textureId;
}