#pragma once

#include "Razor/Core/Core.h"
#include "Razor/Core/Window.h"
#include "Razor/Scene/SceneGraph.h"

#include "Razor/Application/LayerStack.h"
#include "Razor/Events/Event.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/ImGui/ImGuiLayer.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Razor/Rendering/DeferredRenderer.h"

namespace Razor {

	class Editor;

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
		inline Camera* getCamera() { return camera; }
		inline Editor* getEditor() { return m_Editor; }

		inline DeferredRenderer* getForwardRenderer() { return dRenderer; }

	private:
		
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		Editor* m_Editor;
		bool m_Running;
		Log* m_log;
		LayerStack m_LayerStack;
		DeferredRenderer* dRenderer;
		SceneGraph* sceneGraph;
		Camera* camera;

		static Application* s_Instance;
		bool headless;

		double currentFrame;
		double deltaTime;
		double lastFrame;
	};

	Application* createApplication();
}

