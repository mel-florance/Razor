#include "rzpch.h"
#include "UniformBuffer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Razor {

	struct ShaderData {
		std::vector<glm::vec3> cameras_positions;
		std::vector<glm::vec3> lights_positions;
	};

	UniformBuffer::UniformBuffer(const void* data)
	{
		glGenBuffers(1, &id);
		bind();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(data), &data, GL_DYNAMIC_COPY);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void UniformBuffer::link(unsigned int program)
	{
		unsigned int block_index = glGetUniformBlockIndex(program, "shader_data");
	}

	void UniformBuffer::update(const void* data)
	{
		bind();
	}

	void UniformBuffer::bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, id);
	}

	void UniformBuffer::unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

}
