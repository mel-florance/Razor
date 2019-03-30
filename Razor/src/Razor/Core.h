#pragma once

#ifdef RZ_PLATFORM_WINDOWS
	#ifdef RZ_BUILD_DLL
		#define RZ_API __declspec(dllexport)
	#else
		#define RZ_API __declspec(dllimport)
	#endif
#else
	#error Razor only supports Windows ! 
#endif