#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Core/Window.h"
#include "Razor/Scene/SceneGraph.h"

#include "Razor/Application/LayerStack.h"
#include "Razor/Events/Event.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/ImGui/ImGuiLayer.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Razor/Rendering/ForwardRenderer.h"

namespace Razor {

	class ScenesManager;
	class Editor;
	class Engine;
	class FPSCamera;

	class RAZOR_API Application
	{
	public:
		Application(bool headless = false);
		virtual ~Application();
		void run();	
		void OnEvent(Event& e);
		void close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		inline Editor* getEditor() { return m_Editor; }
		ScenesManager* getScenesManager();
		inline LayerStack& getLayerStack() { return m_LayerStack; }
		inline ImGuiLayer* getImGuiLayer() { return m_ImGuiLayer; }

		ForwardRenderer* getForwardRenderer();
		static Editor* m_Editor;

	private:
		
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
	
		Engine* m_Engine;
		bool m_Running;
		Log* m_log;
		LayerStack m_LayerStack;

		static Application* s_Instance;
		bool headless;
	};

	Application* createApplication();
}

