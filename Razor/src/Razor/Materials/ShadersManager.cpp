#include "rzpch.h"
#include "ShadersManager.h"

namespace Razor 
{

	std::string ShadersManager::shaders_dir = std::string();
	ShadersManager::ShaderMap ShadersManager::shaders = {};

	ShadersManager::ShadersManager()
	{
		shaders_dir = (fs::current_path() / fs::path("shaders/")).string();

		shaders["atmosphere"] = ShadersManager::addShader("atmosphere", "atmosphere");
		shaders["blur"]       = ShadersManager::addShader("blur", "blur");
		shaders["default"]    = ShadersManager::addShader("default", "default", "default", true);
		shaders["depth"]      = ShadersManager::addShader("depth", "depth");
		shaders["grid"]       = ShadersManager::addShader("grid", "grid");
		shaders["landscape"]  = ShadersManager::addShader("landscape", "landscape", "landscape", true);
		shaders["outline"]    = ShadersManager::addShader("outline", "outline");
		shaders["particle"]   = ShadersManager::addShader("particle", "particle");
		shaders["skybox"]     = ShadersManager::addShader("skybox", "skybox");
		shaders["viewport"]   = ShadersManager::addShader("viewport", "viewport");
		shaders["deferred"]   = ShadersManager::addShader("deferred", "deferred", "deferred", true);
		shaders["g_buffer"]   = ShadersManager::addShader("g_buffer", "g_buffer", "g_buffer", true);
		shaders["fbo_debug"]  = ShadersManager::addShader("fbo_debug", "fbo_debug", "fbo_debug", true);

		shaders["pbr"]        = ShadersManager::addShader("pbr", "pbr", "pbr", true);
		shaders["cubemap"]    = ShadersManager::addShader("cubemap", "cubemap", "eqToCubemap", true);
		shaders["irradiance"] = ShadersManager::addShader("irradiance", "cubemap", "irradiance", true);
		shaders["prefilter"]  = ShadersManager::addShader("prefilter", "cubemap", "prefilter", true);
		shaders["brdf"]       = ShadersManager::addShader("brdf", "brdf", "brdf", true);
		shaders["background"] = ShadersManager::addShader("background", "background", "background", true);

		compileShaders();
	}

	ShadersManager::~ShadersManager()
	{
	}

	Shader* ShadersManager::addShader(const std::string& name, const std::string& vert_name, const std::string& frag_name, bool isInternal)
	{
		Shader* shader = new Shader(name, vert_name, frag_name, isInternal);
		shaders[name] = shader;
		
		return shader;
	}

	bool ShadersManager::removeShader(const std::string& name)
	{
		auto shader = shaders.find(name);

		if (shader != shaders.end())
			return shaders.erase(shader->first) == 1;

		return false;
	}

	Shader* ShadersManager::getShader(const std::string& name)
	{
		auto shader = shaders.find(name);

		if (shader != shaders.end())
			return shader->second;

		return nullptr;
	}

	void ShadersManager::compileShaders()
	{
		ShaderMap::iterator it = shaders.begin();

		for (; it != shaders.end(); it++)
		{
			if (it->second->isInternal())
			{
				it->second->parseIncludes();
				it->second->replaceConstants();
			}

			it->second->compile();
			it->second->link();
		}
	}

}
