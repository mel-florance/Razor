#include "rzpch.h"
#include "TasksManager.h"

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
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			std::unique_lock<std::mutex> lock(mutex);
			condition.wait(lock, [=] { return !tasks.empty(); });

			Task task = tasks.top();
			tasks.pop();
			
			Log::trace("Executing task \"%s\" with priority %d in thread %d",
				task.getName().c_str(),
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