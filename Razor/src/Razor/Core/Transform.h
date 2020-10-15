#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Razor
{
	class Transform
	{
	private:
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_rotation = glm::vec3(0.0f);
		glm::vec3 m_scale = glm::vec3(1.0f);
		glm::mat4 m_matrix = glm::mat4(1.0f);
		bool m_dirty;

	public:
		Transform(
			const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)
		) :
			m_position(position),
			m_rotation(rotation),
			m_scale(scale),
			m_dirty(true) {}

		virtual ~Transform() {};

		inline glm::mat4& getMatrix()
		{
			glm::mat4 positionMatrix = glm::translate(m_position);
			glm::mat4 rotationXMatrix = glm::rotate(m_rotation.x, glm::vec3(1, 0, 0));
			glm::mat4 rotationYMatrix = glm::rotate(m_rotation.y, glm::vec3(0, 1, 0));
			glm::mat4 rotationZMatrix = glm::rotate(m_rotation.z, glm::vec3(0, 0, 1));
			glm::mat4 scaleMatrix = glm::scale(m_scale);

			glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;
			m_matrix = positionMatrix * rotationMatrix * scaleMatrix;

			return m_matrix;
		}

		inline glm::mat4 getPositionMatrix() { return glm::translate(m_position); }
		inline glm::mat4 getScaleMatrix() { return glm::scale(m_scale); }
		inline glm::mat4 getRotationMatrix() {
			glm::mat4 rotationXMatrix = glm::rotate(m_rotation.x, glm::vec3(1, 0, 0));
			glm::mat4 rotationYMatrix = glm::rotate(m_rotation.y, glm::vec3(0, 1, 0));
			glm::mat4 rotationZMatrix = glm::rotate(m_rotation.z, glm::vec3(0, 0, 1));

			return rotationZMatrix * rotationYMatrix * rotationXMatrix;
		}

		inline glm::vec3& getPosition() { return m_position; };
		inline float& getPositionX() { return m_position.x; };
		inline float& getPositionY() { return m_position.y; };
		inline float& getPositionZ() { return m_position.z; };
		inline glm::vec3& getRotation() { return m_rotation; };
		inline glm::vec3& getScale() { return m_scale; };

		inline void setPosition(const glm::vec3& position) { m_dirty = true; m_position = position; }
		inline void setPositionX(float position) { m_dirty = true; m_position.x = position; }
		inline void setPositionY(float position) { m_dirty = true; m_position.y = position; }
		inline void setPositionZ(float position) { m_dirty = true; m_position.z = position; }
		inline void setRotation(const glm::vec3& rotation) { m_dirty = true; m_rotation = rotation; }
		inline void setScale(const glm::vec3& scale) { m_dirty = true; m_scale = scale; }
		inline void setMatrix(const glm::mat4& mat) { m_dirty = true; m_matrix = mat; }
	};
}

