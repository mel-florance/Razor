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

#include <set>
#include <any>
#include <iterator>
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
#include "Razor/Core/Log.h"
#include "Razor/Core/Utils.h"

#ifdef RZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
