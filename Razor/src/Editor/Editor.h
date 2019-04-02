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

namespace Razor {

	class RAZOR_API Editor : public Razor::Layer
	{
	public:
		Editor();
		~Editor();

		static void import(void* result, TaskFinished tf, Variant opts);
		static void finished(void* result);
		void OnUpdate() override;
		void OnEvent(Razor::Event& event) override;
		void OnImGuiRender() override;

		inline ImGuiLayer* getLayer() { return m_ImGuiLayer;  }

	private:
		ImGuiLayer* m_ImGuiLayer;
		TasksManager* tasksManager;
		AssimpImporter* assimpImporter;
		AssetsManager* assetsManager;
	};

}
