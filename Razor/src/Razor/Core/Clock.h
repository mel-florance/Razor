#pragma once

namespace Razor {

	class Clock
	{
	public:
		Clock();
		~Clock();

		double getTime();

	private:
		std::chrono::steady_clock::time_point m_epoch;
	};

}
