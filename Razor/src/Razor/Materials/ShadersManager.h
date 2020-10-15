#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Materials/Shader.h"

namespace fs = std::experimental::filesystem;

namespace Razor {

	class RAZOR_API ShadersManager
	{
	public:
		ShadersManager();
		~ShadersManager();

		typedef std::unordered_map<std::string, Shader*> ShaderMap;

		inline static ShaderMap& getShaders() { return shaders; }

		static Shader* addShader(const std::string& name, const std::string& vert_name, const std::string& frag_name = std::string(), bool isInternal = false);
		static bool removeShader(const std::string& name);
		static Shader* getShader(const std::string& name);
		static ShaderMap shaders;

		static void compileShaders();
		static std::string shaders_dir;

	private:

	};

}
