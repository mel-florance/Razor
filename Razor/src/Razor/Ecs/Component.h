#pragma once

#include "Entity.h"

namespace Razor {
	namespace ECS {

		class Component
		{
		public:
			Component();
			virtual ~Component();

			virtual void init() {}
			virtual void update(float delta) {}
			virtual void render() {}

			Entity* entity;
		};

	}
}
