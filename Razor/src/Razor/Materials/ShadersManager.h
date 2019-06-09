#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Materials/Shader.h"

namespace Razor {

	class RAZOR_API ShadersManager
	{
	public:
		ShadersManager();
		~ShadersManager();

		Shader* createShader(const std::string& name);
		bool removeShader(const std::string& name);
		Shader* getShader(const std::string& name);

	private:
		std::unordered_map<std::string, Shader*> shaders;
	};

}
