#pragma once

#include <memory>

#ifdef AP_PLATFORM_WINDOWS
#if AP_DYNAMIC_LINK
	#ifdef AP_BUILD_DLL
		#define APOSTLE_API __declspec(dllexport)
	#else
		#define APOSTLE_API __declspec(dllimport)
	#endif
#else
	#define APOSTLE_API
#endif
#else
	#error Apostle only supports Windows!
#endif

#ifdef AP_DEBUG
	#define AP_ENABLE_ASSERTS
#endif

#ifdef AP_ENABLE_ASSERTS
	#define AP_ASSERT(x, ...) { if(!(x)) { AP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AP_CORE_ASSERT(x, ...) { if(!(x)) { AP_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AP_ASSERT(x, ...)
	#define AP_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define AP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Apostle {

	template<typename T>	
	using Scope = std::unique_ptr<T>;
	
	template<typename T>
	using Ref = std::shared_ptr<T>;

}