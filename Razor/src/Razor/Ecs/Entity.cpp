#include "rzpch.h"

#include "Manager.h"
#include "Entity.h"
#include "Component.h"

namespace Razor {
	namespace ECS {

		Entity::Entity(Manager& manager) :
			manager(manager), 
			alive(true)
		{
		}

		Entity::~Entity()
		{
		}

		void Entity::update(float delta)
		{
			for (auto& c : components)
				c->update(delta);
		}

		void Entity::render()
		{
			for (auto& c : components)
				c->render();
		}

		void Entity::addGroup(size_t group) noexcept
		{
			groupBitset[group] = true;
			manager.addToGroup(this, group);
		}

		void Entity::removeGroup(size_t group) noexcept
		{
			groupBitset[group] = false;
		}

	}
}
