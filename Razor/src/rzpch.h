#pragma once

#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <experimental/filesystem>
#include <chrono>
#include <thread>
#include <future>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <deque>
#include <limits>
#include <typeinfo>

#include <any>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Razor/Types/Array.h"
#include "Razor/Types/Variant.h"
#include "Razor/Log.h"

#ifdef RZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
