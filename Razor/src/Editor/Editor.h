#pragma once

#include "Razor/Input/KeyCodes.h"
#include "Razor/Input/MouseButtons.h"
#include "Razor/Application/Layer.h"
#include "Razor/Input/Input.h"
#include "Razor/ImGui/ImGuiLayer.h"
#include "Razor/Core/TasksManager.h"

#include "Importers/AssimpImporter.h"
#include "Components/AssetsManager.h"
#include "Components/MainMenu.h"
#include "Components/Console.h"
#include "Components/PropertiesEditor.h"
#include "Components/Tools.h"
#include "Components/Viewport.h"
#include "Components/Outliner.h"
#include "Components/Logger.h"
#include "Components/ProjectsManager.h"

namespace Razor {

	class Engine;

	typedef std::map<std::string, EditorComponent*> ComponentsMap;

	class RAZOR_API Editor : public Razor::Layer
	{
	public:
		Editor(Engine* Engine);
		~Editor();

		void OnUpdate(float delta) override;
		void OnEvent(Razor::Event& event) override;
		void OnImGuiRender() override;

		inline Engine* getEngine() { return m_Engine; }
		inline std::shared_ptr<ImGuiLayer> getLayer() { return m_ImGuiLayer;  }
		inline ComponentsMap& getComponents() { return components; }
		inline TasksManager* getTasksManager() { return tasksManager; }

		template<class T>
		inline T getComponent(const std::string& name) {
			auto it = components.find(name);

			if (it != components.end())
				return it->second;
		}

		static void importFinished(void* result);
		static void setupMeshBuffers(Node* node);

	private:
		Engine* m_Engine;
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;
		TasksManager* tasksManager;
		std::unique_ptr<AssimpImporter> assimpImporter;
		ComponentsMap components;
		float delta;
	};

}
