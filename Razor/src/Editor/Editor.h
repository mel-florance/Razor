#pragma once

#include "Razor/Input/KeyCodes.h"
#include "Razor/Input/MouseButtons.h"
#include "Razor/Layer.h"
#include "Razor/Input.h"
#include "Razor/ImGui/ImGuiLayer.h"
#include "Razor/TasksManager.h"

#include "Importers/AssimpImporter.h"
#include "Components/AssetsManager.h"
#include "Components/MainMenu.h"
#include "Components/Console.h"
#include "Components/PropertiesEditor.h"
#include "Components/Tools.h"
#include "Components/Viewport.h"
#include "Components/Outliner.h"
#include "Components/Logger.h"

namespace Razor {

	typedef std::map<std::string, EditorComponent*> ComponentsMap;

	class RAZOR_API Editor : public Razor::Layer
	{
	public:
		Editor();
		~Editor();

		void OnUpdate() override;
		void OnEvent(Razor::Event& event) override;
		void OnImGuiRender() override;

		inline std::shared_ptr<ImGuiLayer> getLayer() { return m_ImGuiLayer;  }
		inline ComponentsMap& getComponents() { return components; }

		template<class T>
		inline T getComponent(const std::string& name) {
			auto it = components.find(name);

			if (it != components.end())
				return it->second;
		}

	private:
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;
		std::unique_ptr<TasksManager> tasksManager;
		std::unique_ptr<AssimpImporter> assimpImporter;

		ComponentsMap components;
	};

}
