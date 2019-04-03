#pragma once

#include "Razor/Core.h"

namespace Razor {

	class RAZOR_API Shader
	{
	public:
		Shader(const std::string& name);
		~Shader();

		bool load();
		bool compile();
		bool link();
		inline int getId() { return id; }
		inline std::string& getName() { return name; }

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

	private:
		int id;
		int program;
		std::string name;
		SourcesMap sources;
		ShadersMap shaders;
		char status;
	};

}
