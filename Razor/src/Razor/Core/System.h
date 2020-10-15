#pragma once

namespace Razor
{

	class System
	{
	public:
		System();
		~System();

		struct VirtualMemory
		{
			uint64 total;
			uint64 system_usage;
			uint64 process_usage;
		};

		struct PhysicalMemory
		{
			uint64 total;
			uint64 system_usage;
			uint64 process_usage;
		};

		struct CPU
		{
			uint64 system_usage;
			uint64 process_usage;
		};

		void getStats();

		inline VirtualMemory& getVirtualMemory() { return virtual_memory; }
		inline PhysicalMemory& getPhysicalMemory() { return physical_memory; }
		inline CPU& getCPU() { return cpu; }

	private:
		VirtualMemory virtual_memory;
		PhysicalMemory physical_memory;
		CPU cpu;
	};

}
