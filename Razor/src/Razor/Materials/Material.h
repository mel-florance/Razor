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
			Normal,
			Metallic,
			Roughness,
			Ao,
			Orm,
			Opacity,
			Emissive
		};

		typedef std::map<TextureType, unsigned int> TexturesMap;
		
		virtual void bind(Shader* shader) {}
		virtual void bindLights(Shader* shader, const std::vector<std::shared_ptr<Light>>& lights);
		virtual void setTextureMap(TextureType type, unsigned int id);
		virtual void removeTextureMap(TextureType type);

		inline unsigned int getId() { return id; }

		inline void setName(const std::string& name) { this->name = name; }
		inline std::string getName() { return name; }

		inline bool hasDiffuseMap() { return has_diffuse; }
		inline bool hasSpecularMap() { return has_specular; }
		inline bool hasNormalMap() { return has_normal; }
		inline bool hasMetallicMap() { return has_metallic; }
		inline bool hasRoughnessMap() { return has_roughness; }
		inline bool hasAoMap() { return has_ao; }
		inline bool hasOrmMap() { return has_orm; }
		inline bool hasOpacityMap() { return has_opacity; }
		inline bool hasEmissiveMap() { return has_emissive; }

		inline unsigned int getDiffuseMap() { return textures_maps[TextureType::Diffuse]; }
		inline unsigned int getSpecularMap() { return textures_maps[TextureType::Specular]; }
		inline unsigned int getNormalMap() { return textures_maps[TextureType::Normal]; }
		inline unsigned int getMetallicMap() { return textures_maps[TextureType::Metallic]; }
		inline unsigned int getRoughnessMap() { return textures_maps[TextureType::Roughness]; }
		inline unsigned int getAoMap() { return textures_maps[TextureType::Ao]; }
		inline unsigned int getOrmMap() { return textures_maps[TextureType::Orm]; }
		inline unsigned int getOpacityMap() { return textures_maps[TextureType::Opacity]; }
		inline unsigned int getEmissiveMap() { return textures_maps[TextureType::Emissive]; }

		std::string formatParamName(const std::string& type, unsigned int index, const std::string& name);

		inline std::string& getDiffusePath() { return diffuse_path; }
		inline std::string& getSpecularPath() { return specular_path; }
		inline std::string& getNormalPath() { return normal_path; }
		inline std::string& getMetallicPath() { return metallic_path; }
		inline std::string& getRoughnessPath() { return roughness_path; }
		inline std::string& getAoPath() { return ao_path; }
		inline std::string& getOrmPath() { return orm_path; }
		inline std::string& getOpacityPath() { return opacity_path; }
		inline std::string& getEmissivePath() { return emissive_path; }

		inline void setDiffusePath(const std::string& path) { diffuse_path = path; }
		inline void setSpecularPath(const std::string& path) { specular_path = path; }
		inline void setNormalPath(const std::string& path) { normal_path = path; }
		inline void setMetallicPath(const std::string& path) { metallic_path = path; }
		inline void setRoughnessPath(const std::string& path) { roughness_path = path; }
		inline void setAoPath(const std::string& path) { ao_path = path; }
		inline void setOrmPath(const std::string& path) { orm_path = path; }
		inline void setOpacityPath(const std::string& path) { opacity_path = path; }
		inline void setEmissivePath(const std::string& path) { emissive_path = path; }

	protected:
		unsigned int id;
		std::string name;
		TexturesMap textures_maps;

		bool has_diffuse;
		bool has_specular;
		bool has_normal;
		bool has_metallic;
		bool has_roughness;
		bool has_ao;
		bool has_orm;
		bool has_opacity;
		bool has_emissive;

		std::string diffuse_path;
		std::string specular_path;
		std::string normal_path;
		std::string metallic_path;
		std::string roughness_path;
		std::string ao_path;
		std::string orm_path;
		std::string opacity_path;
		std::string emissive_path;
	};

}
