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
const float ROLL = 0.0f;
const float SPEED = 2.5f;
const float SENSIVITY = 0.1f;
const float ZOOM = 45.0f;
const float ROTATIONSPEED = 0.2f;

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
	GLfloat roll;
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat rotationSpeed;
	GLfloat zoom;

public:
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH,
		float roll = ROLL
	) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSIVITY), zoom(ZOOM),
		rotationSpeed(ROTATIONSPEED), position(position), worldUp(up), yaw(yaw), pitch(pitch), roll(roll)
	{
		updateCameraVectors();
	}

	Camera(
		float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw, float pitch
	) : Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch) {}

	glm::mat4 getViewMatrix();
	void processKeyboard(Camera_Movement direction, float deltaTime);
	void processRotate(Camera_Movement direction, float deltaTime, bool constrainPitch = true);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void processMouseScroll(float yOffset);



private:
	void updateCameraVectors();
};

