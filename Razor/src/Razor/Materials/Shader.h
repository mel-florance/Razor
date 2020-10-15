#pragma once

#include "Razor/Core/Core.h"
#include <glm/glm.hpp>

namespace Razor 
{

	class Shader
	{
	public:
		Shader(const std::string& name, const std::string& frag_name, const std::string& vert_name, bool isInternal = false);
		~Shader();

		enum class State 
		{
			VERTEX    = 0x8B31,
			FRAGMENT  = 0x8B30,
			GEOMETRY  = 0x8DD9,
			COMPUTE   = 0x91B9,
			TESS_EVAL = 0x8E87,
			TESS_CTRL = 0x8E88
		};

		enum Status
		{
			Loaded   = 1 << 0,
			Compiled = 1 << 1,
			Linked   = 1 << 2
		};

		enum class Type
		{
			ATMOSPHERE,
			BACKGROUND,
			BLUR,
			DEFAULT,
			DEPTH,
			GRID,
			LANDSCAPE,
			OUTLINE,
			PARTICLE,
			PBR,
			SKYBOX,
			VIEWPORT
		};

		typedef std::map<State, int> ShadersMap;
		typedef std::map<State, std::string> SourcesMap;
		typedef std::map<std::pair<State, std::string>, int> ConstantsMap;

		bool load();
		void defineConstant(State state, const std::string& name, int value);
		void replaceConstants();
		void parseIncludes();
		bool compile();
		bool link();
		void bind();
		void unbind();

		inline int getId() { return id; }
		inline std::string& getName() { return name; }
		inline int getProgram() { return program; }
		inline bool isInternal() { return is_internal; }
		inline bool isDirty() { return dirty; }

		inline Status getStatus() 
		{
			for (char s = Status::Loaded; s != Status::Linked; s++)
				if (s == status)
					return static_cast<Status>(s);

			return Status::Loaded;
		}

		void setUniform1i(const std::string& name, int value);

		void setUniform1f(const std::string& name, float value);
		void setUniform2f(const std::string& name, float x, float y);
		void setUniform2f(const std::string& name, const glm::vec2& value);
		void setUniform3f(const std::string& name, const glm::vec3& value);
		void setUniform3f(const std::string& name, float x, float y, float z);
		void setUniform4f(const std::string& name, const glm::vec4& value);
		void setUniform4f(const std::string& name, float x, float y, float z, float w);

		void setUniform1d(const std::string& name, double value);
		void setUniform2d(const std::string& name, double x, double y);
		void setUniform2d(const std::string& name, const glm::dvec2& value);
		void setUniform3d(const std::string& name, const glm::dvec3& value);
		void setUniform3d(const std::string& name, double x, double y, double z);
		void setUniform4d(const std::string& name, const glm::dvec4& value);
		void setUniform4d(const std::string& name, double x, double y, double z, double w);

		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void setUniformMat4d(const std::string& name, const glm::dmat4& matrix);

	private:
		unsigned int getUniformLocation(const std::string& name);

		int id;
		int program;
		char status;
		bool dirty;
		bool is_internal;
		std::string name;
		std::string frag_name;
		std::string vert_name;
		SourcesMap sources;
		ShadersMap shaders;
		ConstantsMap constants;
	
		std::unordered_map<std::string, int> uniforms_cache;
	};

}
