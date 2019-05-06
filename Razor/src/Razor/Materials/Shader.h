#pragma once

#include "Razor/Core/Core.h"
#include <glm/glm.hpp>

namespace Razor {

	class RAZOR_API Shader
	{
	public:
		Shader(const std::string& name);
		~Shader();

		void bind();
		void unbind();

		bool load();
		bool compile();
		bool link();

		inline int getId() { return id; }
		inline std::string& getName() { return name; }
		inline int getProgram() { return program; }

		void setUniform1i(const std::string& name, int value);
		void setUniform1f(const std::string& name, float value);
		void setUniform2f(const std::string& name, float x, float y);
		void setUniform2f(const std::string& name, const glm::vec2& value);
		void setUniform3f(const std::string& name, const glm::vec3& value);
		void setUniform3f(const std::string& name, float x, float y, float z);
		void setUniform4f(const std::string& name, const glm::vec4& value);
		void setUniform4f(const std::string& name, float x, float y, float z, float w);
		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

		enum class Type {
			VERTEX    = 0x8B31,
			FRAGMENT  = 0x8B30,
			GEOMETRY  = 0x8DD9,
			COMPUTE   = 0x91B9,
			TESS_EVAL = 0x8E87,
			TESS_CTRL = 0x8E88
		};

		enum Status {
			Loaded   = 1 << 0,
			Compiled = 1 << 1,
			Linked   = 1 << 2
		};

		typedef std::map<Type, int> ShadersMap;
		typedef std::map<Type, std::string> SourcesMap;
		typedef std::map<std::pair<Type, std::string>, int> ConstantsMap;

		void defineConstant(Type type, const std::string& name, int value);
		void replaceConstants();

	private:
		unsigned int getUniformLocation(const std::string& name);

		int id;
		int program;
		std::string name;
		SourcesMap sources;
		ShadersMap shaders;
		ConstantsMap constants;
		char status;
		bool dirty;
	};

}
