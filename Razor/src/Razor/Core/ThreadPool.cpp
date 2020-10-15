#include "rzpch.h"
#include "ThreadPool.h"

namespace Razor
{

	ThreadPool::ThreadPool(size_t thread_count) :
		stopped(false)
	{
		start(thread_count);
	}

	ThreadPool::~ThreadPool()
	{
		stop();
	}

	void ThreadPool::start(size_t thread_count)
	{
		for (unsigned int i = 0; i < thread_count; ++i)
		{
			threads.emplace_back([=] 
			{
				while (true)
				{
					Task task;

					{
						std::unique_lock<std::mutex> lock{ event_mutex };
						event_condition.wait(lock, [=] { return stopped || !tasks.empty(); });

						if (stopped)
							break;

						task = std::move(tasks.front());
						tasks.pop();
					}

					task();
				}
			});
		}
	}

	void ThreadPool::stop() noexcept
	{
		{
			std::unique_lock<std::mutex> lock{ event_mutex };
			stopped = true;
		}

		event_condition.notify_all();

		for (auto& thread : threads)
			thread.join();
	}

}
