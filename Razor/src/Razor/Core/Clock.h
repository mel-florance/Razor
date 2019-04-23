#pragma once

namespace Razor {

	class Clock
	{
	public:
		Clock();
		~Clock();

		 double getTime();

	private:
		double time;
	};

}
