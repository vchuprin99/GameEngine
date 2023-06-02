#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GameEngine {
	class Camera {
	public:
		enum class ProjectionMode : uint8_t {
			Orthographic,
			Perspective
		};

		Camera(
			const glm::vec3& position = { 0, 0, 0 },
			const glm::vec3& rotation = { 0, 0, 0 },
			const ProjectionMode projectionMode = ProjectionMode::Orthographic
		);

		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setPositionRotation(const glm::vec3& position, const glm::vec3& rotation);
		void setProjectionMode(const ProjectionMode projectionMode);

		void moveForward(const float delta);
		void moveRight(const float delta);
		void moveUp(const float delta);

		void moveAndRotate(const glm::vec3& move_delta, const glm::vec3& rotate_delta);
		void rotate(const glm::vec3& rotate_delta);

		const glm::mat4& getViewMatrix();
		const glm::mat4& getProjectionMatrix() const;
	private:
		void updateViewMatrix();
		void updateProjectionMatrix();

		ProjectionMode m_projectionMode;

		glm::vec3 m_position;
		glm::vec3 m_rotation; // X - Roll, Y - Pitch, Z - Yaw

		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;

		static constexpr glm::vec3 s_world_forward = { 1.f, 0.f, 0.f };
		static constexpr glm::vec3 s_world_right = { 0.f, -1.f, 0.f };
		static constexpr glm::vec3 s_world_up = { 1.f, 0.f, 0.f };

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		bool m_updateViewMatrix = false;
	};
}