#include "rzpch.h"
#include "Editor/Editor.h"
#include "Application.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Input.h"

namespace Razor {

	Application* Application::s_Instance = nullptr;

	Application::Application(bool headless) : headless(headless), m_Running(true)
	{
		RZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if (!headless) {
			m_Window = std::unique_ptr<Window>(Window::Create());
			m_Window->SetEventCallback(RZ_BIND_EVENT_FN(Application::OnEvent));

			m_Editor = new Editor();

			m_ImGuiLayer = m_Editor->getLayer().get();
			PushLayer(m_Editor);
			PushOverlay(m_ImGuiLayer);
		}


	}

	Application::~Application()
	{
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

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(RZ_BIND_EVENT_FN(Application::OnWindowClose));

		//RZ_CORE_INFO("{0}", e);

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
		while (!glfwWindowShouldClose((GLFWwindow*)m_Window->GetNativeWindow()))
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();

			//auto[x, y] = Input::getMousePosition();
			//RZ_CORE_TRACE("{0}, {1}", x, y);


			m_Window->OnUpdate();
		}
	}

}

