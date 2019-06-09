#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Types/Variant.h"
#include "Razor/Materials/Shader.h"

namespace Razor {

	class Light;

	class RAZOR_API Material
	{
	public:
		Material();
		virtual ~Material();

		enum class TextureType
		{
			Diffuse,
			Specular,
			Normal
		};

		typedef std::map<TextureType, unsigned int> TexturesMap;
		
		virtual void bind(Shader* shader) {}
		virtual void bindLights(Shader* shader, const std::vector<std::shared_ptr<Light>>& lights);
		virtual void setTextureMap(TextureType type, unsigned int id);
		virtual void removeTextureMap(TextureType type);

		inline void setName(const std::string& name) { this->name = name; }
		inline std::string getName() { return name; }

		inline bool hasDiffuseMap() { return has_diffuse; }
		inline bool hasSpecularMap() { return has_specular; }
		inline bool hasNormalMap() { return has_normal; }

		inline unsigned int getDiffuseMap() { return textures_maps[TextureType::Diffuse]; }
		inline unsigned int getSpecularMap() { return textures_maps[TextureType::Specular]; }
		inline unsigned int getNormalMap() { return textures_maps[TextureType::Normal]; }

		std::string formatParamName(const std::string& type, unsigned int index, const std::string& name);

		inline std::string& getDiffusePath() { return diffuse_path; }
		inline std::string& getSpecularPath() { return specular_path; }
		inline std::string& getNormalPath() { return normal_path; }

		inline void setDiffusePath(const std::string& path) { diffuse_path = path; }
		inline void setSpecularPath(const std::string& path) { specular_path = path; }
		inline void setNormalPath(const std::string& path) { normal_path = path; }

	protected:
		std::string name;
		TexturesMap textures_maps;

		bool has_diffuse;
		bool has_specular;
		bool has_normal;

		std::string diffuse_path;
		std::string specular_path;
		std::string normal_path;
	};

}
