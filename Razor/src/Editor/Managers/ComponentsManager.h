#pragma once

#include "Editor/EditorManager.h"
#include "Editor/EditorComponent.h"

namespace Razor
{

	class Editor;

	class ComponentsManager : public EditorManager
	{
	public:
		ComponentsManager(Editor* editor);
		~ComponentsManager();

		typedef std::unordered_map<const char*, EditorComponent*> ComponentsMap;

		inline ComponentsMap& getComponents() { return components; }

		void onEvent(Razor::Event& event);

		template<typename T>
		T* getComponent(const char* name) {
			auto it = components.find(name);

			if (it != components.end())
				return (T*)it->second;

			return nullptr;
		}

	private:
		ComponentsMap components;
	};

}
