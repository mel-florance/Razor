#pragma once

namespace Razor {
	namespace ECS {

		class Manager;
		class Component;

		constexpr std::size_t MAX_COMPONENTS{ 32 };
		constexpr std::size_t MAX_GROUPS{ 32 };

		using GroupBitset = std::bitset<MAX_GROUPS>;
		using ComponentBitset = std::bitset<MAX_COMPONENTS>;
		using ComponentArray = std::array<Component*, MAX_COMPONENTS>;

		class Entity
		{
		public:
			Entity(Manager& manager);
			~Entity();

			void update(float delta);
			void render();

			inline bool isAlive() const { return alive; }
			inline void destroy() { alive = false; }

			template<typename T>
			inline bool hasComponent() const {
				return componentBitset[Manager::getComponentTypeId<T>()];
			}

			inline bool hasGroup(size_t group) const noexcept {
				return groupBitset[group];
			}

			void addGroup(size_t group) noexcept;
			void removeGroup(size_t group) noexcept;

			template<typename T, typename... TArgs>
			T& addComponent(TArgs&&... args)
			{
				assert(!hasComponent<T>());

				T* c(new T(std::forward<TArgs>(args)...));
				c->entity = this;
				std::unique_ptr<Component> ptr{ c };
				components.emplace_back(std::move(ptr));

				componentArray[Manager::getComponentTypeId<T>()] = c;
				componentBitset[Manager::getComponentTypeId<T>()] = true;

				c->init();

				return *c;
			}

			template<typename T>
			T& getComponent() const
			{
				assert(hasComponent<T>());
				auto ptr = componentArray[Manager::getComponentTypeId<T>()];

				return *reinterpret_cast<T*>(ptr);
			}

		private:
			Manager& manager;

			bool alive;
			std::vector<std::unique_ptr<Component>> components;

			ComponentArray componentArray;
			ComponentBitset componentBitset;
			GroupBitset groupBitset;
		};

	}
}