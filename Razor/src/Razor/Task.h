#pragma once

#include "Razor/Core.h"
#include "Razor/Variant.h"

namespace Razor {

	typedef std::function<void(void*)> TaskFinished;
	typedef std::function<void(void*, TaskFinished, Variant)> TaskCallback;

	class RZ_API Task
	{
	public:
		Task(void* result, TaskFinished tf, TaskCallback fn, Variant opts, const std::string& name, unsigned int priority = 0) :
			result(result),
			tf(std::bind(tf, result)),
			fn(std::bind(fn, result, tf, opts)),
			opts(opts),
			name(name),
			priority(priority) {}

		void execute() {
			run(fn, tf, opts);
		}

		~Task() {

		}

		inline std::string& getName() { return name; }
		inline unsigned int getPriority() { return priority; }

		void run(TaskCallback fn, TaskFinished tf, Variant opts) {
			fn(result, tf, opts);
		}

		inline bool operator < (const Task& task) const {
			return task.priority > priority;
		}

		void* result;
		Variant opts;
		TaskCallback fn;
		TaskFinished tf;

	private:
		std::string name;
		unsigned int priority;
	};
}

