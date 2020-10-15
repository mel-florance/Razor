#pragma once

#include "GameLoop.h"

namespace Razor 
{

	class Renderer;

	class ScenesManager;
	class SoundsManager;
	class ShadersManager;
	class TasksManager;
	class Window;
	class Application;
	class Event;
	class World;
	class ThreadPool;
	class System;

	class Engine
	{
	public:
		Engine(Application* application);
		~Engine();

		void start();
		void stop();
		void OnEvent(Event& event);
		static void update(GameLoop* loop, Engine* self);
		static void render(GameLoop* loop, Engine* self);

		inline ScenesManager* getScenesManager() { return scenes_manager; }
		inline SoundsManager* getSoundsManager() { return sounds_manager; }
		inline ShadersManager* getShadersManager() { return shaders_manager; }
		inline TasksManager* getTasksManager() { return tasks_manager; }
		inline ThreadPool* getThreadPool() { return thread_pool; }
		inline System* getSystem() { return system; }

		inline Renderer* getRenderer() { return renderer; }

		inline World* getPhysicsWorld() { return physics_world; }

		inline float getFrameTiming()  { return (float)gameLoop->getProfiler()->getReport("frame");  }
		inline float getUpdateTiming() { return (float)gameLoop->getProfiler()->getReport("update");  }
		inline float getRenderTiming() { return (float)gameLoop->getProfiler()->getReport("render");  }
		inline float getSleepTiming()  { return (float)gameLoop->getProfiler()->getReport("sleep");  }
		
		inline GameLoop* getGameLoop() { return gameLoop; }
		inline float getFPS() { return gameLoop->getFps(); }
		inline std::map<int, float>& getFpsArray() { return gameLoop->getFpsList(); }

		inline unsigned int activeUnitToDecimal() 
		{
			int active_unit;
			std::stringstream max_str;
			max_str << std::dec << system_infos.active_texture_unit;
			max_str >> active_unit;

			return active_unit;
		}

		struct SystemInfos 
		{
			SystemInfos() :
				vendor(nullptr),
				renderer(nullptr),
				opengl_version(nullptr),
				shading_language_version(nullptr),
				extensions({}),
				max_texture_image_units(0),
				max_combined_texture_image_units(0),
				max_vertex_texture_image_units(0),
				max_texture_size(0),
				max_cubemap_texture_size(0),
				max_render_buffer_size(0),
				max_vertex_attributes(0),
				max_varying_vectors(0),
				max_fragment_uniform_vectors(0),
				max_vertex_uniform_vectors(0),
				max_color_attachments(0),
				active_texture_unit(0),
				aliased_line_width_range(0),
				smooth_line_width_range(0),
				array_buffer_binding(0),
				blend(0)
			{}

			const unsigned char* vendor;
			const unsigned char* renderer;
			const unsigned char* opengl_version;
			const unsigned char* shading_language_version;

			std::vector<std::string> extensions;

			int max_texture_image_units;
			int max_combined_texture_image_units;
			int max_vertex_texture_image_units;
			int max_texture_size;
			int max_cubemap_texture_size;
			int max_render_buffer_size;
			int max_vertex_attributes;
			int max_varying_vectors;
			int max_fragment_uniform_vectors;
			int max_vertex_uniform_vectors;
			int max_color_attachments;

			int active_texture_unit;
			int aliased_line_width_range;
			int smooth_line_width_range;
			int array_buffer_binding;
			unsigned char blend;
		};

		inline SystemInfos& getSystemInfos() { return system_infos; }

		inline static int getMaxColorAttachments() { return system_infos.max_color_attachments; }

		void extractSystemInfos();

		inline bool hasExtension(const std::string& extension)
		{
			return std::find(system_infos.extensions.begin(), system_infos.extensions.end(), extension) != system_infos.extensions.end();
		}

		static SystemInfos system_infos;

	private:
		Application* application;
		GameLoop* gameLoop;
		ThreadPool* thread_pool;

		Renderer* renderer;

		ScenesManager* scenes_manager;
		SoundsManager* sounds_manager;
		ShadersManager* shaders_manager;
		TasksManager* tasks_manager;

		World* physics_world;

		System* system;
	
	};

}
