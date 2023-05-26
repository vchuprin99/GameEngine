#include "camera.h"

GameEngine::Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const ProjectionMode projectionMode)
{
	updateViewMatrix();
	updateProjectionMatrix();
}

void GameEngine::Camera::setPosition(const glm::vec3& position)
{
	m_position = position;
	updateViewMatrix();
}

void GameEngine::Camera::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
	updateViewMatrix();
}

void GameEngine::Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
{
	m_position = position;
	m_rotation = rotation;
	updateViewMatrix();
}

void GameEngine::Camera::setProjectionMode(const ProjectionMode projectionMode)
{
	m_projectionMode = projectionMode;
	updateProjectionMatrix();
}

void GameEngine::Camera::updateViewMatrix()
{
	glm::mat4 translateMatrix = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-m_position[0], -m_position[1], -m_position[2], 1
	};
	float rotateRadians_x = glm::radians(m_rotation.x);
	glm::mat4 rotationMatrix_x = {
		1, 0, 0, 0, 
		0, -cos(rotateRadians_x), -sin(rotateRadians_x), 0,
		0, sin(rotateRadians_x), -cos(rotateRadians_x), 0, 
		0, 0, 0, 1
	};
	float rotateRadians_y = glm::radians(m_rotation.y);
	glm::mat4 rotationMatrix_y = {
		-cos(rotateRadians_y), 0, sin(rotateRadians_y), 0,
		0, 1, 0, 0,
		-sin(rotateRadians_y), 0, -cos(rotateRadians_y), 0,
		0, 0, 0, 1
	};

	m_viewMatrix = rotationMatrix_x * rotationMatrix_y * translateMatrix;
}

void GameEngine::Camera::updateProjectionMatrix()
{
	if (m_projectionMode == ProjectionMode::Perspective) {
		m_projectionMatrix = glm::perspective(glm::radians(45.0f), 1.f, 0.f, 100.0f);
	}
	else {
		float r = 2;
		float t = 2;
		float f = 100;
		float n = 0.1f;
		m_projectionMatrix = glm::mat4(1 / r, 0, 0, 0,
			0, 1 / t, 0, 0,
			0, 0, -2 / (f - n), 0,
			0, 0, (-f - n) / (f - n), 1);
	}
}
