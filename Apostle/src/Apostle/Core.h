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

#ifdef AP_ENABLE_ASSERTS
	#define AP_ASSERT(x, ...) { if(!(x)) { AP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AP_CORE_ASSERT(x, ...) { if(!(x)) { AP_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AP_ASSERT(x, ...)
	#define AP_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)