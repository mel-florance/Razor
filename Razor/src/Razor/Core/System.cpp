#include "rzpch.h"
#include "System.h"

namespace Razor
{

	System::System()
	{

	}

	System::~System()
	{
	}

	void System::getStats()
	{
		// Virtual memory
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		virtual_memory.total = memInfo.ullTotalPageFile;
		virtual_memory.system_usage = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)& pmc, sizeof(pmc));
		virtual_memory.process_usage = pmc.PrivateUsage;

		// Physical memory
		physical_memory.total = memInfo.ullTotalPhys;
		physical_memory.system_usage = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
		physical_memory.process_usage = pmc.WorkingSetSize;

		// CPU -> TODO
	}
}
