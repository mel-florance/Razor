#include "rzpch.h"
#include "Editor/Editor.h"
#include "Application.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/Core/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Razor/Core/Engine.h"
#include "Razor/Input/Input.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Scene/ScenesManager.h"

namespace Razor {

	Application* Application::s_Instance = nullptr;

	Application::Application(bool headless) : headless(headless), m_Running(true)
	{
		RZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if (!headless)
		{
			m_Window = std::unique_ptr<Window>(Window::Create());
			m_Window->SetEventCallback(RZ_BIND_EVENT_FN(Application::OnEvent));

			m_Engine = new Engine(this);
			m_log = new Log();
			m_Editor = new Editor(m_Engine);

			m_log->s_editorLogger = (Logger*)m_Editor->getComponents()["Logger"];

			m_ImGuiLayer = m_Editor->getLayer().get();
			PushLayer(m_Editor);
			PushOverlay(m_ImGuiLayer);
		}
	}

	Application::~Application()
	{
		delete m_log;
		delete m_Editor;
	}

	void Application::close()
	{
		glfwSetWindowShouldClose((GLFWwindow*)m_Window->GetNativeWindow(), GLFW_TRUE);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(RZ_BIND_EVENT_FN(Application::OnWindowClose));

		//RZ_CORE_INFO("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			m_Engine->OnEvent(event);

		
			if (event.Handled)
				break;
		}
	}

	ScenesManager* Application::getScenesManager()
	{
		return m_Engine->getScenesManager();
	}

	DeferredRenderer* Application::getDeferredRenderer()
	{
		return m_Engine->getDeferredRenderer();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		m_Engine->stop();
		return true;
	}

	void Application::run()
	{
		m_Engine->start();
	}

}
