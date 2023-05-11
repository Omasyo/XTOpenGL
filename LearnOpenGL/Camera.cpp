#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
	glm::vec3 position,
	glm::vec3 up,
	float yaw,
	float pitch,
	float roll
) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSIVITY), zoom(ZOOM),
rotationSpeed(ROTATIONSPEED), position(position), worldUp(up), yaw(yaw), pitch(pitch), roll(roll)
{
	updateCameraVectors();
}

Camera::Camera(
	float posX, float posY, float posZ,
	float upX, float upY, float upZ,
	float yaw, float pitch
) : Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch) {}

glm::mat4 Camera::getViewMatrix() { return glm::lookAt(position, position + front, up); }

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		position += front * velocity;
		break;
	case BACKWARD:
		position -= front * velocity;
		break;
	case LEFT:
		position -= right * velocity;
		break;
	case RIGHT:
		position += right * velocity;
		break;
	}
}

void Camera::processRotate(Camera_Movement direction, float deltatTime, bool constrainPitch)
{

	switch (direction)
	{
	case FORWARD:
		pitch += rotationSpeed;
		break;
	case BACKWARD:
		pitch -= rotationSpeed;
		break;
	case LEFT:
		yaw -= rotationSpeed;
		break;
	case RIGHT:
		yaw += rotationSpeed;
		break;
	}

	if (constrainPitch)
	{
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
	zoom -= yOffset;
	if (zoom < 1.0f) zoom = 1.0f;
	if (zoom > 45.0f) zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}