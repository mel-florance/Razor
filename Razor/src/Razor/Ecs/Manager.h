#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace Razor {
	namespace ECS {

		class Manager
		{
		public:
			Manager();
			~Manager();

			template<typename T>
			inline static size_t getComponentTypeId() noexcept
			{
				static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

				static size_t typeId { getUniqueComponentId() };
				return typeId;
			}

			void update(float delta);
			void render();
			void refresh();
			Entity& createEntity();
			void addToGroup(Entity* entity, size_t group);
			std::vector<Entity*>& getEntitiesByGroup(size_t group);

		private:
			inline static size_t getUniqueComponentId() noexcept
			{
				static size_t lastId{ 0u };
				return lastId++;
			}

			std::vector<std::unique_ptr<Entity>> entities;
			std::array<std::vector<Entity*>, MAX_GROUPS> groupedEntities;
		};

	}
}