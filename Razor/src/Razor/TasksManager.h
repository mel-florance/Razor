#pragma once

#include "Razor/Core.h"
#include "Razor/Task.h"

namespace Razor {
	
	class RZ_API TasksManager
	{
	public:
		TasksManager();
		~TasksManager();
		
		void run();

		void add(void* result, TaskCallback fn, TaskFinished tf,  Variant opts, const std::string& name, unsigned int priority);

		inline unsigned int getMaxThreads() { return std::thread::hardware_concurrency(); };

	private:
		inline void emplace(const Task& task) { tasks.emplace(task); }
		inline Task top() { return tasks.top(); }
		inline void pop() { tasks.pop(); }
		inline size_t size() { return tasks.size(); }

		std::priority_queue<Task> tasks;
		std::vector<std::thread*> pool;

		std::condition_variable condition;
		std::mutex mutex;
	};

}
