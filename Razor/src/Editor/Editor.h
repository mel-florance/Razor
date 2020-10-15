#pragma once

#include "Razor/Input/KeyCodes.h"
#include "Razor/Input/MouseButtons.h"
#include "Razor/Application/Layer.h"
#include "Razor/Input/Input.h"
#include "Razor/ImGui/ImGuiLayer.h"

#include "Importers/AssimpImporter.h"

#include "Managers/ComponentsManager.h"
#include "Managers/ToolsManager.h"
#include "Managers/ModalsManager.h"
#include "Managers/IconsManager.h"

namespace Razor 
{

	class Engine;
	class Texture;
	class TextureAtlas;
	class TPSCamera;
	class FPSCamera;

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

		inline ModalsManager* getModalsManager() { return modals_manager; }
		inline ToolsManager* getToolsManager() { return tools_manager; }
		inline static ComponentsManager* getComponentsManager() { return components_manager; }

		static std::shared_ptr<Node> import(const std::string& path);
		static void importFinished(Node* result);

		inline void showImportModal(bool value) {
			show_import_modal = value;
		}

		enum class PreferencesCategory
		{
			SYSTEM_INFORMATION,
			SHORTCUTS,
			ENGINE,
			EDITOR,
			THEME
		};
		
		static IconsManager* icons_manager;
		static ComponentsManager* components_manager;

	private:
		Engine* m_Engine;
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;

		std::unique_ptr<AssimpImporter> assimpImporter;

		ToolsManager* tools_manager;
		ModalsManager* modals_manager;

		float delta;

		TPSCamera* tps_camera;
		FPSCamera* fps_camera;

		bool system_audio_device_opened;
		bool system_graphic_device_opened;
		bool system_capabilities_opened;
		bool system_features_opened;

		bool editor_memory_opened;

		bool show_import_modal;

		PreferencesCategory current_category;

		int cam_type;
	};

}
