#include "camera.h"

GameEngine::Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const ProjectionMode projectionMode)
{
	updateViewMatrix();
	updateProjectionMatrix();
}

void GameEngine::Camera::setPosition(const glm::vec3& position)
{
	m_position = position;
	m_updateViewMatrix = true;
}

void GameEngine::Camera::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
	m_updateViewMatrix = true;
}

void GameEngine::Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
{
	m_position = position;
	m_rotation = rotation;
	m_updateViewMatrix = true;
}

void GameEngine::Camera::moveForward(const float delta) 
{
	m_position += m_direction * delta;
	m_updateViewMatrix = true;
}
void GameEngine::Camera::moveRight(const float delta) 
{
	m_position += m_right * delta;
	m_updateViewMatrix = true;
}
void GameEngine::Camera::moveUp(const float delta) 
{
	m_position += m_up * delta;
	m_updateViewMatrix = true;
}

void GameEngine::Camera::moveAndRotate(const glm::vec3& move_delta, const glm::vec3& rotate_delta)
{
	m_position += m_direction * move_delta.x;
	m_position += m_right * move_delta.y;
	m_position += m_up * move_delta.z;
	m_rotation += rotate_delta;

	m_updateViewMatrix = true;
}

void GameEngine::Camera::rotate(const glm::vec3& rotate_delta)
{
	m_rotation += rotate_delta;

	m_updateViewMatrix = true;
}

const glm::mat4& GameEngine::Camera::getViewMatrix()
{
	if (m_updateViewMatrix) {
		updateViewMatrix();
	}
	return m_viewMatrix;
}

const glm::mat4& GameEngine::Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

void GameEngine::Camera::setProjectionMode(const ProjectionMode projectionMode)
{
	m_projectionMode = projectionMode;
	updateProjectionMatrix();
}

void GameEngine::Camera::updateViewMatrix()
{
	const glm::mat3 rotationMatrix_x = {
		1, 0, 0,
		0, cos(m_rotation.x), -sin(m_rotation.x),
		0, sin(m_rotation.x), cos(m_rotation.x),
	};
	const glm::mat3 rotationMatrix_y = {
		cos(m_rotation.y), 0, -sin(m_rotation.y),
		0, 1, 0,
		sin(m_rotation.y), 0, cos(m_rotation.y),
	};
	const glm::mat3 rotationMatrix_z = {
		cos(m_rotation.z), -sin(m_rotation.z), 0,
		sin(m_rotation.z), cos(m_rotation.z), 0,
		0, 0, 1,
	};
	const glm::mat3 euler_rotateMatrix = rotationMatrix_z * rotationMatrix_y * rotationMatrix_x;
	m_direction = glm::normalize(euler_rotateMatrix * s_world_forward);
	m_right = glm::normalize(euler_rotateMatrix * s_world_right);
	m_up = glm::cross(m_right, m_direction);

	m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);
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
