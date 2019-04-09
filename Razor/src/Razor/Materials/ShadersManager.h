#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Materials/Shader.h"

namespace Razor {

	class RAZOR_API ShadersManager
	{
	public:
		ShadersManager();
		~ShadersManager();

		std::shared_ptr<Shader> createShader(const std::string& name);
		bool removeShader(const std::string& name);
		std::shared_ptr<Shader> getShader(const std::string& name);

	private:
		std::map<std::string, std::shared_ptr<Shader>> shaders;
	};

}
