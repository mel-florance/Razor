#include "rzpch.h"
#include "Shader.h"
#include "Razor/File.h"

#include <glad/glad.h>

namespace Razor {

	Shader::Shader(const std::string& name) : name(name)
	{
		
	}

	Shader::~Shader()
	{
	}

	bool Shader::load()
	{
		status &= ~(Status::Loaded);


		auto data = File::read(name);


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
					RZ_ERROR("Shader compilation failed: {0}", error);
				}
				else
					status |= Status::Compiled;
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
			RZ_ERROR("Shader program linking failed: {0}", error);
		}
		else
			status |= Status::Linked;

		for (; it != shaders.end(); it++)
			glDeleteShader(it->second);

		return status & Status::Linked;
	}

}
