#pragma once

#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <chrono>
#include <thread>
#include <future>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <deque>

#include <any>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Razor/Log.h"

#ifdef RZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
