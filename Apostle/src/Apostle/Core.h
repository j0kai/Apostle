#pragma once

#ifdef AP_PLATFORM_WINDOWS
	#ifdef AP_BUILD_DLL
		#define APOSTLE_API __declspec(dllexport)
	#else
		#define APOSTLE_API __declspec(dllimport)
	#endif
#else
	#error Apostle only supports Windows!
#endif