#include "rzpch.h"
#include "Material.h"
#include "Razor/Lighting/Light.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Spot.h"
#include "Razor/Lighting/Point.h"

namespace Razor {

	Material::Material() :
		has_diffuse(false),
		has_specular(false),
		has_normal(false),
		has_metallic(false),
		has_roughness(false),
		has_ao(false),
		has_orm(false),
		has_opacity(false),
		has_emissive(false),
		diffuse_path("Not set"),
		specular_path("Not set"),
		normal_path("Not set"),
		metallic_path("Not set"),
		roughness_path("Not set"),
		ao_path("Not set"),
		orm_path("Not set"),
		opacity_path("Not set"),
		emissive_path("Not set")
	{

	}

	Material::~Material()
	{
	}

	void Material::bindLights(Shader* shader, const std::vector<std::shared_ptr<Light>>& lights)
	{
		if (shader != nullptr)
		{
			std::string paramType;
			unsigned int sIdx = 0, dIdx = 0, pIdx = 0;

			for (auto light : lights)
			{
				if (light->getType() == Light::Type::DIRECTIONAL)
				{
					std::shared_ptr<Directional> directional = std::dynamic_pointer_cast<Directional>(light);
					paramType = "directionalLights";

					shader->setUniform3f(formatParamName(paramType, dIdx, "position"), directional->getPosition());
					shader->setUniform3f(formatParamName(paramType, dIdx, "direction"), directional->getDirection());
					shader->setUniform3f(formatParamName(paramType, dIdx, "ambient"), directional->getAmbient());
					shader->setUniform3f(formatParamName(paramType, dIdx, "diffuse"), directional->getDiffuse());
					shader->setUniform3f(formatParamName(paramType, dIdx, "specular"), directional->getSpecular());
					shader->setUniform1f(formatParamName(paramType, dIdx, "intensity"), directional->getIntensity());

					dIdx++;
				}
				else if (light->getType() == Light::Type::POINT)
				{
					std::shared_ptr<Point> point = std::dynamic_pointer_cast<Point>(light);
					paramType = "pointLights";

					shader->setUniform3f(formatParamName(paramType, pIdx, "position"), point->getPosition());
					shader->setUniform1f(formatParamName(paramType, pIdx, "constant"), point->getLinear());
					shader->setUniform1f(formatParamName(paramType, pIdx, "linear"), point->getConstant());
					shader->setUniform1f(formatParamName(paramType, pIdx, "quadratic"), point->getQuadratic());
					shader->setUniform3f(formatParamName(paramType, pIdx, "ambient"), point->getAmbient());
					shader->setUniform3f(formatParamName(paramType, pIdx, "diffuse"), point->getDiffuse());
					shader->setUniform3f(formatParamName(paramType, pIdx, "specular"), point->getSpecular());
					shader->setUniform1f(formatParamName(paramType, pIdx, "intensity"), point->getIntensity());

					pIdx++;
				}
				else if (light->getType() == Light::Type::SPOT)
				{
					std::shared_ptr<Spot> spot = std::dynamic_pointer_cast<Spot>(light);
					paramType = "spotLights";

					shader->setUniform3f(formatParamName(paramType, sIdx, "direction"), spot->getDirection());
					shader->setUniform3f(formatParamName(paramType, sIdx, "position"), spot->getPosition());
					shader->setUniform1f(formatParamName(paramType, sIdx, "intensity"), spot->getIntensity());
					shader->setUniform1f(formatParamName(paramType, sIdx, "constant"), spot->getConstant());
					shader->setUniform1f(formatParamName(paramType, sIdx, "linear"), spot->getLinear());
					shader->setUniform1f(formatParamName(paramType, sIdx, "quadratic"), spot->getQuadratic());
					shader->setUniform1f(formatParamName(paramType, sIdx, "inner_cutoff"), spot->getInnerCutoff());
					shader->setUniform1f(formatParamName(paramType, sIdx, "outer_cutoff"), spot->getOuterCutoff());
					shader->setUniform3f(formatParamName(paramType, sIdx, "ambient"), spot->getAmbient());
					shader->setUniform3f(formatParamName(paramType, sIdx, "diffuse"), spot->getDiffuse());
					shader->setUniform3f(formatParamName(paramType, sIdx, "specular"), spot->getSpecular());

					sIdx++;
				}
				else if (light->getType() == Light::Type::AREA)
				{

				}
			}
		}
	}

	void Material::setTextureMap(TextureType type, unsigned int id)
	{
		switch (type)
		{
			case TextureType::Diffuse  : has_diffuse  = true; break;
			case TextureType::Specular : has_specular = true; break;
			case TextureType::Normal : has_normal = true; break;
			case TextureType::Metallic : has_metallic = true; break;
			case TextureType::Roughness : has_roughness = true; break;
			case TextureType::Ao : has_ao = true; break;
			case TextureType::Orm : has_orm = true; break;
			case TextureType::Opacity : has_opacity = true; break;
			case TextureType::Emissive : has_emissive = true; break;
		}

		textures_maps[type] = id;
	}

	void Material::removeTextureMap(TextureType type)
	{
		TexturesMap::iterator it = textures_maps.find(type);

		if (it != textures_maps.end()) 
		{
			switch (it->first)
			{
				case TextureType::Diffuse: has_diffuse = false; break;
				case TextureType::Specular: has_specular = false; break;
				case TextureType::Normal: has_normal = false; break;
				case TextureType::Metallic: has_metallic = false; break;
				case TextureType::Roughness: has_roughness = false; break;
				case TextureType::Ao: has_ao = false; break;
				case TextureType::Orm: has_orm = false; break;
				case TextureType::Opacity: has_opacity = false; break;
				case TextureType::Emissive: has_emissive = false; break;
			}

			textures_maps.erase(it);
		}
	}

	std::string Material::formatParamName(const std::string& type, unsigned int index, const std::string& name)
	{
		std::stringstream str;
		str << type << "[" << index << "]." << name;

		return str.str();
	}

}

