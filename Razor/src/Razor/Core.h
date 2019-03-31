#pragma once
#pragma warning (disable : 4251)


#ifdef RZ_PLATFORM_WINDOWS
	#ifdef RZ_BUILD_DLL
		#define RZ_API __declspec(dllexport)
	#else
		#define RZ_API __declspec(dllimport)
	#endif
#else
	#error Razor only supports Windows ! 
#endif

#ifdef RZ_DEBUG
	#define RZ_ENABLE_ASSERTS
#endif

#ifdef RZ_ENABLE_ASSERTS
	#define RZ_ASSERT(x, ...) { if(!(x)) { RZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RZ_CORE_ASSERT(x, ...) { if(!(x)) { RZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RZ_ASSERT(x, ...)
	#define RZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
