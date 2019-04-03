#pragma once

#include "Core.h"

#include "Window.h"
#include "Razor/LayerStack.h"
#include "Razor/Events/Event.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/ImGui/ImGuiLayer.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Razor {

	class RAZOR_API Application
	{
	public:
		Application(bool headless = false);
		virtual ~Application();
		void run();	
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		inline void close();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		class Editor* m_Editor;
		bool m_Running;

		LayerStack m_LayerStack;

		static Application* s_Instance;
		bool headless;
	};

	Application* createApplication();
}

