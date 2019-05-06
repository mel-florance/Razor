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

#include "Tools/Selection.h"
#include "Tools/Gizmo.h"
#include "Tools/GridAxis.h"

namespace Razor {

	class Engine;
	class TPSCamera;
	class FPSCamera;

	typedef std::map<std::string, EditorComponent*> ComponentsMap;
	typedef std::map<std::string, EditorTool*> ToolsMap;

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
		inline ToolsMap& getTools() { return tools; }
		inline TasksManager* getTasksManager() { return tasksManager; }

		template<class T>
		inline T getComponent(const std::string& name) {
			auto it = components.find(name);

			if (it != components.end())
				return dynamic_cast<T>(it->second);

			return nullptr;
		}

		template<class T>
		inline T getTool(const std::string& name) {
			auto it = tools.find(name);

			if (it != tools.end())
				return dynamic_cast<T>(it->second);

			return nullptr;
		}

		static void importFinished(void* result);
		static void setupMeshBuffers(Node* node);

	private:
		Engine* m_Engine;
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;
		TasksManager* tasksManager;
		std::unique_ptr<AssimpImporter> assimpImporter;
		ComponentsMap components;
		ToolsMap tools;
		float delta;

		TPSCamera* tps_camera;
		FPSCamera* fps_camera;

		int cam_type;
	};

}
