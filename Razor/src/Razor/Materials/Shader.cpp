#include "rzpch.h"
#include "Shader.h"
#include "Razor/Filesystem/File.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Razor {

	Shader::Shader(const std::string& name) : name(name)
	{
		load();
		compile();
		link();
	}

	Shader::~Shader()
	{
	}

	void Shader::bind()
	{
		glUseProgram(program);
	}

	void Shader::unbind()
	{
		glUseProgram(0);
	}

	bool Shader::load()
	{
		status &= ~(Status::Loaded);
		std::string shadersDir = "F:/Razor/Razor/Razor/src/Shaders/";

		auto frag_data = File::read(shadersDir + name + ".frag");
		auto vert_data = File::read(shadersDir + name + ".vert");

		if (frag_data.empty() || vert_data.empty())
			return false;

		sources[Type::VERTEX] = vert_data;
		sources[Type::FRAGMENT] = frag_data;

		RZ_INFO("Loaded shader: {0}", name);

		return status & Status::Loaded;
	}

	bool Shader::compile()
	{
		status &= ~(Status::Compiled);

		SourcesMap::iterator it = sources.begin();

		for (; it != sources.end(); it++)
		{
			if (!it->second.empty())
			{
				int shader = glCreateShader((GLenum)it->first);
				shaders[it->first] = shader;

				const char* str = it->second.c_str();
				glShaderSource(shader, 1, &str, NULL);

				glCompileShader(shader);	

				int status;
				char error[512];
				glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

				if (!status) {
					glGetShaderInfoLog(shader, 512, NULL, error);
					RZ_ERROR("Shader compilation failed: {0}.", error);
				}
				else {
					std::string type;
					switch (it->first) {
						case Type::VERTEX: type = "Vertex"; break;
						case Type::FRAGMENT: type = "Fragment";  break;
					}
					RZ_INFO("Compiled {0} shader: {1}.", type, name);
					status |= Status::Compiled;
				}
			}
		}

		return status & Status::Compiled;
	}

	bool Shader::link()
	{
		status &= ~(Status::Linked);

		program = glCreateProgram();
		ShadersMap::iterator it = shaders.begin();

		for (; it != shaders.end(); it++)
			glAttachShader(program, it->second);

		glLinkProgram(program);

		int status;
		char error[512];
		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (!status) {
			glGetProgramInfoLog(program, 512, NULL, error);
			RZ_ERROR("Shader program linking failed: {0}.", error);
		}
		else {
			RZ_INFO("Linked shader: {0}.", name);
			status |= Status::Linked;
		}

		for (; it != shaders.end(); it++)
			glDeleteShader(it->second);

		return status & Status::Linked;
	}

	void Shader::setUniform4f(const std::string& name, const glm::vec4& value) {
		glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
	}
	void Shader::setUniform4f(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(getUniformLocation(name), x, y, z, w);
	}
	void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void Shader::setUniform1i(const std::string & name, int value){
		glUniform1i(getUniformLocation(name), value);
	}
	void Shader::setUniform1f(const std::string& name, float value) {
		glUniform1f(getUniformLocation(name), value);
	}
	void Shader::setUniform2f(const std::string& name, float x, float y) {
		glUniform2f(getUniformLocation(name), x, y);
	}
	void Shader::setUniform2f(const std::string& name, const glm::vec2 & value) {
		glUniform2f(getUniformLocation(name), value.x, value.y);
	}
	void Shader::setUniform3f(const std::string& name, const glm::vec3& value) {
		glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
	}
	void Shader::setUniform3f(const std::string& name, float x, float y, float z) {
		glUniform3f(getUniformLocation(name), x, y, z);
	}

	unsigned int Shader::getUniformLocation(const std::string& name)
	{

		int location = glGetUniformLocation(program, name.c_str());

		if (location == -1)
			RZ_WARN("Invalid uniform location: {0}", name);

		return location;
	}

}
