#include "rzpch.h"
#include "BillboardManager.h"
#include <glm/gtx/transform.hpp>
#include "Razor/Scene/Node.h"
#include "Razor/Lighting/Directional.h"

namespace Razor
{

	BillboardManager::BillboardManager() :
		billboards({})
	{
		
	}

	BillboardManager::~BillboardManager()
	{
	}

	void BillboardManager::addBillboard(unsigned int node_id, const glm::vec3& position, const std::string& icon_name)
	{
		if (billboards.find(node_id) == billboards.end())
			billboards[node_id] = std::make_pair(position, icon_name);
	}

	bool BillboardManager::removeBillboard(unsigned int node_id)
	{
		BillboardMap::iterator it = billboards.find(node_id);

		if (it != billboards.end())
		{
			billboards.erase(it);
			return true;
		}

		return false;
	}

	void BillboardManager::updatePosition(std::shared_ptr<Node> node, Camera* camera, const glm::vec4& viewport)
	{
		BillboardMap::iterator it = billboards.find(node->id);
		Transform transform = node->transform;

		if (it != billboards.end())
		{
			if (node->lights.size() > 0) 
			{
				switch (node->lights[0]->getType()) 
				{
				case Light::Type::DIRECTIONAL:
					std::shared_ptr<Directional> directional = std::dynamic_pointer_cast<Directional>(node->lights[0]);
					transform.setPosition(directional->getPosition());
					break;
				}
			}

			it->second.first = glm::project(
				glm::vec3(0.0f, 0.0f, 0.0f),
				camera->getViewMatrix() * transform.getMatrix(),
				camera->getProjectionMatrix(),
				viewport
			);
		}
	}

}
