#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

public:
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH
	) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSIVITY), zoom(ZOOM),
		position(position), worldUp(up), yaw(yaw), pitch(pitch)
	{
		updateCameraVectors();
	}

	Camera(
		float posX, float posY, float posZ, 
		float upX, float upY, float upZ, 
		float yaw, float pitch
	) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSIVITY), zoom(ZOOM),
		position(glm::vec3(posX, posY, posZ)), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch)
	{
		updateCameraVectors();
	}

	glm::mat4 getViewMatrix();
	void processKeyboard(Camera_Movement direction, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void processMouseScroll(float yOffset);

private:
	void updateCameraVectors();
};
