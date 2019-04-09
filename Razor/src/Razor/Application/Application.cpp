#include "rzpch.h"
#include "Editor/Editor.h"
#include "Application.h"
#include "Razor/Events/ApplicationEvent.h"
#include "Razor/Core/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Razor/Input/Input.h"
#include "Razor/Cameras/Camera.h"

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

			sceneGraph = new SceneGraph();
			camera = new Camera(m_Window.get(), glm::vec3(0.0f, 0.0f, 3.0f));
			dRenderer = new DeferredRenderer(camera, sceneGraph);

			m_log = new Log();
			m_Editor = new Editor();
			m_log->s_editorLogger = (Logger*)m_Editor->getComponents()["Logger"];

			m_ImGuiLayer = m_Editor->getLayer().get();
			PushLayer(m_Editor);
			PushOverlay(m_ImGuiLayer);
		}
	}

	Application::~Application()
	{
		delete dRenderer;
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

			if (event.GetEventType() == EventType::MouseButtonPressed) {
				MouseButtonEvent& e = (MouseButtonEvent&)event;
				camera->onMouseDown(e.GetMouseButton());
			}
			
			if (event.GetEventType() == EventType::MouseButtonReleased) {
				MouseButtonEvent& e = (MouseButtonEvent&)event;
				camera->onMouseUp(e.GetMouseButton());
			}

			if (event.GetEventType() == EventType::MouseMoved) {
				MouseMovedEvent& e = (MouseMovedEvent&)event;
				camera->onMouseMoved(glm::vec2(e.GetX(), e.GetY()));
			}

			if (event.GetEventType() == EventType::MouseScrolled) {
				MouseScrolledEvent& e = (MouseScrolledEvent&)event;
				camera->onMouseScrolled(glm::vec2(e.GetXOffset(), e.GetYOffset()));
			}

			if (event.Handled)
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
			currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glViewport(0, 0, (GLsizei)m_Window->GetWidth(), (GLsizei)m_Window->GetHeight());

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			camera->setDelta(deltaTime);
			camera->onEvent((GLFWwindow*)m_Window->GetNativeWindow());

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			dRenderer->update(deltaTime);

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();

			dRenderer->render(deltaTime);

			//auto[x, y] = Input::getMousePosition();
			//RZ_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	}

}

