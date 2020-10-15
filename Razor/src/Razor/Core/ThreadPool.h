#pragma once

namespace Razor
{

	class ThreadPool
	{
	public:
		using Task = std::function<void()>;

		ThreadPool(size_t thread_count);
		~ThreadPool();

		template<class T>
		auto addTask(T task) -> std::future<decltype(task())>
		{
			auto wrapper = std::make_shared<std::packaged_task<decltype(task()) () >>(std::move(task));

			{
				std::unique_lock<std::mutex> lock{ event_mutex };
				tasks.emplace([=]
				{
					(*wrapper)();
				});
			}

			event_condition.notify_one();

			return wrapper->get_future();
		}

	private:
		void start(size_t thread_count);
		void stop() noexcept;

		std::queue<Task> tasks;
		std::vector<std::thread> threads;
		std::condition_variable event_condition;
		std::mutex event_mutex;
		bool stopped;
	};
}
