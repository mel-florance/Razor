#include "rzpch.h"
#include "Engine.h"
#include "Razor/Rendering/DeferredRenderer.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Application/Application.h"
#include "Razor/Application/LayerStack.h"
#include "Razor/Events/Event.h"
#include "GLFW/glfw3.h"

namespace Razor {

	Engine::Engine(Application* application) : application(application)
	{
		gameLoop = new GameLoop(this);
		gameLoop->setUpdateCallback(&Engine::update);
		gameLoop->setRenderCallback(&Engine::render);

		scenes_manager = new ScenesManager();
		deferred_renderer = new DeferredRenderer(&application->GetWindow(), scenes_manager);
	}

	Engine::~Engine()
	{
		delete gameLoop;
		delete deferred_renderer;
	}

	void Engine::start()
	{
		gameLoop->start();
	}

	void Engine::stop()
	{
		gameLoop->stop();
	}

	void Engine::OnEvent(Event& event)
	{
		Camera* camera = scenes_manager->getActiveScene()->getActiveCamera();

		if (camera != nullptr)
		{
			if (event.GetEventType() == EventType::MouseButtonPressed) {
				MouseButtonEvent& e = (MouseButtonEvent&)event;
				camera->onMouseDown(e.GetMouseButton());
				event.Handled = true;
			}

			if (event.GetEventType() == EventType::MouseButtonReleased) {
				MouseButtonEvent& e = (MouseButtonEvent&)event;
				camera->onMouseUp(e.GetMouseButton());
				event.Handled = true;
			}

			if (event.GetEventType() == EventType::MouseMoved) {
				MouseMovedEvent& e = (MouseMovedEvent&)event;
				event.Handled = true;
				camera->onMouseMoved(glm::vec2(e.GetX(), e.GetY()));
			}

			if (event.GetEventType() == EventType::MouseScrolled) {
				MouseScrolledEvent& e = (MouseScrolledEvent&)event;
				camera->onMouseScrolled(glm::vec2(e.GetXOffset(), e.GetYOffset()));
				event.Handled = true;
			}

			if (event.GetEventType() == EventType::WindowResize) {
				WindowResizeEvent& e = (WindowResizeEvent&)event;
				camera->onWindowResized(glm::vec2(e.GetWidth(), e.GetHeight()));
				event.Handled = true;
			}

			if (event.GetEventType() == EventType::KeyPressed) {
				KeyPressedEvent& e = (KeyPressedEvent&)event;
				camera->onKeyDown(e.GetKeyCode());
				//event.Handled = true;
			}

			if (event.GetEventType() == EventType::KeyReleased) {
				KeyReleasedEvent& e = (KeyReleasedEvent&)event;
				camera->onKeyReleased(e.GetKeyCode());
				//event.Handled = true;
			}
		}
	}

	void Engine::update(GameLoop* loop, Engine* self)
	{
		double delta = loop->getPassedTime();
		Window& window = self->application->GetWindow();
		Camera* camera = self->scenes_manager->getActiveScene()->getActiveCamera();

		self->deferred_renderer->clear(DeferredRenderer::ClearType::ALL);

		camera->update(delta);
		camera->onEvent(&self->application->GetWindow());

		for (Layer* layer : self->application->getLayerStack())
			layer->OnUpdate((float)delta);

		self->deferred_renderer->setViewport(0, 0, window.GetWidth(), window.GetHeight());
		self->deferred_renderer->update((float)loop->getPassedTime());
	}

	void Engine::render(GameLoop* loop, Engine* self)
	{
		self->application->getImGuiLayer()->Begin();

		for (Layer* layer : self->application->getLayerStack())
			layer->OnImGuiRender();

		self->application->getImGuiLayer()->End();

		self->deferred_renderer->render();
		self->application->GetWindow().OnUpdate();

		glfwPollEvents();
	}

}
