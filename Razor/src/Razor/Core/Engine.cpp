#include "rzpch.h"
#include "Engine.h"
#include "Razor/Rendering/Renderer.h"
#include "Razor/Materials/ShadersManager.h"
#include "Razor/Core/TasksManager.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Application/Application.h"
#include "Razor/Application/LayerStack.h"
#include "Razor/Events/Event.h"
#include "Razor/Input/KeyCodes.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include "Razor/Physics/World.h"
#include "Razor/Audio/SoundsManager.h"
#include "Razor/Audio/Sound.h"
#include "Razor/Core/ThreadPool.h"
#include "Razor/Core/System.h"
#include "Editor/Editor.h"

namespace Razor
{
	Engine::SystemInfos Engine::system_infos = Engine::SystemInfos();

	Engine::Engine(Application* application) : 
		application(application)
	{
		system = new System();
		//thread_pool = new ThreadPool(8);

		gameLoop = new GameLoop(this);
		gameLoop->setUpdateCallback(&Engine::update);
		gameLoop->setRenderCallback(&Engine::render);

		physics_world = new World();

		scenes_manager  = new ScenesManager();
		sounds_manager  = new SoundsManager();
		shaders_manager = new ShadersManager();
		//tasks_manager   = new TasksManager();

		renderer = new Renderer(&application->GetWindow(), this, scenes_manager, shaders_manager);

		//sounds_manager->loadSound("./data/audio/EpicTrailerFinal.wav", "trailer_music");

		extractSystemInfos();
	}

	Engine::~Engine()
	{
		delete system;
		delete gameLoop;
		delete renderer;
		delete sounds_manager;
		delete scenes_manager;
		delete shaders_manager;
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
		//forward_renderer->onEvent(event);

		Camera* camera = scenes_manager->getActiveScene()->getActiveCamera();

		if (camera != nullptr)
		{
			if (event.GetEventType() == EventType::MouseButtonPressed) {
				MouseButtonEvent& e = (MouseButtonEvent&)event;
				camera->onMouseDown(e.GetMouseButton());
				//event.Handled = true;
			}

			if (event.GetEventType() == EventType::MouseButtonReleased) {
				MouseButtonEvent& e = (MouseButtonEvent&)event;
				camera->onMouseUp(e.GetMouseButton());
				//event.Handled = true;
			}

			if (event.GetEventType() == EventType::MouseMoved) {
				MouseMovedEvent& e = (MouseMovedEvent&)event;
				//event.Handled = true;
				camera->onMouseMoved(glm::vec2(e.GetX(), e.GetY()));
			}

			if (event.GetEventType() == EventType::MouseScrolled) {
				MouseScrolledEvent& e = (MouseScrolledEvent&)event;
				camera->onMouseScrolled(glm::vec2(e.GetXOffset(), e.GetYOffset()));
				//event.Handled = true;
			}

			if (event.GetEventType() == EventType::WindowResize) {
				WindowResizeEvent& e = (WindowResizeEvent&)event;
				glm::vec2 size = glm::vec2(e.GetWidth(), e.GetHeight());
				camera->onWindowResized(size);
				//renderer->onResize(size);
				//event.Handled = true;
			}

			if (event.GetEventType() == EventType::KeyPressed) {
				KeyPressedEvent& e = (KeyPressedEvent&)event;
				camera->onKeyDown(e.GetKeyCode());
				//event.Handled = true;

			/*	if (e.GetKeyCode() == RZ_KEY_H)
				{
					sounds_manager->playSound("trailer_music");
				}

				if (e.GetKeyCode() == RZ_KEY_P)
				{
					Sound* trailer = sounds_manager->getSounds()["trailer_music"];

					trailer->isPlaying() ? trailer->pause() : trailer->unpause();
				}*/
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

		camera->update(delta);
		camera->onEvent(&self->application->GetWindow());

		self->getPhysicsWorld()->tick((float)loop->getPassedTime());
		self->getPhysicsWorld()->updateNodes();

		for (Layer* layer : self->application->getLayerStack())
			layer->OnUpdate((float)delta);

		//self->forward_renderer->setViewport(0, 0, window.GetWidth(), window.GetHeight());
		//self->forward_renderer->update((float)loop->getPassedTime());
	}

	void Engine::render(GameLoop* loop, Engine* self)
	{
		self->renderer->render();
			
		self->application->getImGuiLayer()->Begin();

		for (Layer* layer : self->application->getLayerStack())
			layer->OnImGuiRender();

		self->application->getImGuiLayer()->End();

		self->application->GetWindow().OnUpdate();

		glfwPollEvents();
	}

	void Engine::extractSystemInfos()
	{
		system_infos.opengl_version = glGetString(GL_VERSION);
		system_infos.vendor = glGetString(GL_VENDOR);
		system_infos.renderer = glGetString(GL_RENDERER);
		system_infos.shading_language_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
		
		const unsigned char* extensions = glGetString(GL_EXTENSIONS);
		char const* ext = reinterpret_cast<char const*>(extensions);
		system_infos.extensions = Utils::splitString(std::string(ext), " ");

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,          &system_infos.max_texture_image_units);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &system_infos.max_combined_texture_image_units);
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,   &system_infos.max_vertex_texture_image_units);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,                 &system_infos.max_texture_size);
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,        &system_infos.max_cubemap_texture_size);
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,            &system_infos.max_render_buffer_size);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,               &system_infos.max_vertex_attributes);
		glGetIntegerv(GL_MAX_VARYING_VECTORS,              &system_infos.max_varying_vectors);
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,     &system_infos.max_fragment_uniform_vectors);
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,       &system_infos.max_vertex_uniform_vectors);
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,            &system_infos.max_color_attachments);

		glGetIntegerv(GL_ALIASED_LINE_WIDTH_RANGE,         &system_infos.aliased_line_width_range);
		glGetIntegerv(GL_SMOOTH_LINE_WIDTH_RANGE,          &system_infos.smooth_line_width_range);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING,             &system_infos.array_buffer_binding);
		glGetBooleanv(GL_BLEND,                            &system_infos.blend);
		glGetIntegerv(GL_ACTIVE_TEXTURE,                   &system_infos.active_texture_unit);

		system_infos.active_texture_unit = activeUnitToDecimal();
	}

}
