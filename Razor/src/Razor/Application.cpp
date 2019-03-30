#include "rzpch.h"

#include "Application.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/Log.h"
#include "GLFW/glfw3.h"

namespace Razor {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(RZ_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(RZ_BIND_EVENT_FN(Application::OnWindowClose));

		RZ_CORE_INFO("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

}

