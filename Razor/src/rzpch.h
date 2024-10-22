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
#include <bitset>
#include <typeinfo>
#include <stdint.h>
#include <cassert>
#include <type_traits>
#include <iomanip>
#include <locale>
#include <regex>
#include <array>
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
#include <streambuf>

#include "Razor/Types/Array.h"
#include "Razor/Types/Variant.h"
#include "Razor/Core/Log.h"
#include "Razor/Core/Utils.h"

#ifdef RZ_PLATFORM_WINDOWS
	#pragma comment (lib, "ws2_32.lib")
	#include <WS2tcpip.h>
	#include <winsock2.h>

	#include <Windows.h>
	#include <psapi.h>
#endif
