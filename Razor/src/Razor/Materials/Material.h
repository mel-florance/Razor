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
		virtual void bindLights(Shader* shader, const std::vector<Light*>& lights);
		virtual void setTextureMap(TextureType type, unsigned int id);
		virtual void removeTextureMap(TextureType type);

		inline void setName(const std::string& name) { this->name = name; }
		inline std::string getName() { return name; }

		inline bool hasDiffuseMap() { return has_diffuse; }
		inline unsigned int getDiffuseMap() { return textures_maps[TextureType::Diffuse]; }

		inline bool hasSpecularMap() { return has_specular; }
		inline unsigned int getSpecularMap() { return textures_maps[TextureType::Specular]; }

		inline bool hasNormalMap() { return has_normal; }
		inline unsigned int getNormalMap() { return textures_maps[TextureType::Normal]; }

		std::string formatParamName(const std::string& type, unsigned int index, const std::string& name);

	protected:
		std::string name;
		TexturesMap textures_maps;

		bool has_diffuse;
		bool has_specular;
		bool has_normal;
	};

}
