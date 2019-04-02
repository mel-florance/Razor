#include "rzpch.h"
#include "TasksManager.h"
#include "Mesh.h"
#include "Editor/Importers/AssimpImporter.h"

namespace Razor {

	TasksManager::TasksManager()
	{
		for (unsigned int i = 0; i < this->getMaxThreads(); i++)
		{
			std::thread* thread = new std::thread(&TasksManager::run, this);
			thread->detach();
			pool.push_back(thread);
		}
	}

	void TasksManager::run()
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2));

			std::unique_lock<std::mutex> lock(mutex);
			condition.wait(lock, [=] { return !tasks.empty(); });

			Task task = tasks.top();
			tasks.pop();
			
			RZ_CORE_TRACE("Executing task \"{0}\" with priority {1} in thread {2}",
				task.getName(),
				task.getPriority(),
				std::this_thread::get_id());
		
			task.execute();
		}
	}

	void TasksManager::add(const TaskData& data)
	{
		std::unique_lock<std::mutex> lock(mutex);
		tasks.push(Task(data));
		condition.notify_one();
	}

	TasksManager::~TasksManager()
	{
		for (auto thread : pool)
			delete thread;
	}
}