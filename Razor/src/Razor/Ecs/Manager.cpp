#include "rzpch.h"
#include "Manager.h"

namespace Razor {
	namespace ECS {

		Manager::Manager()
		{
		}

		Manager::~Manager()
		{
		}

		void Manager::update(float delta)
		{
			for (auto& e : entities)
				e->update(delta);
		}

		void Manager::render()
		{
			for (auto& e : entities)
				e->render();
		}

		void Manager::refresh()
		{
			for (auto i = 0; i < MAX_GROUPS; i++)
			{
				auto& group = groupedEntities[i];

				group.erase(std::remove_if(std::begin(group), std::end(group),
				[i](Entity* entity)
				{
					return !entity->isAlive() || !entity->hasGroup(i);
				}), std::end(group));
			}

			entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& entity)
			{
				return !entity->isAlive();
			}), std::end(entities));
		}

		Entity& Manager::createEntity()
		{
			Entity* entity = new Entity(*this);
			std::unique_ptr<Entity> ptr{ entity };
			entities.emplace_back(std::move(ptr));

			return *entity;
		}

		void Manager::addToGroup(Entity* entity, size_t group)
		{
			groupedEntities[group].emplace_back(entity);
		}

		std::vector<Entity*>& Manager::getEntitiesByGroup(size_t group)
		{
			return groupedEntities[group];
		}

	}
}
