#pragma once

#include "Core.h"

#include "Window.h"
#include "Razor/LayerStack.h"
#include "Razor/Events/Event.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/ImGui/ImGuiLayer.h"

namespace Razor {

	class RZ_API Application
	{
	public:
		Application();	
		virtual ~Application();
		void run();	
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		class Editor* m_Editor;
		bool m_Running = true;

		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	Application* createApplication();
}

