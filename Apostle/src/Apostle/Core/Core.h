#pragma once

#include "PlatformDetection.h"

#include <memory>

// Asserts
#ifdef AP_DEBUG
	#define AP_ENABLE_ASSERTS
#endif

// TODO: Make a version of this macro that doesn't take in arguments (except condition).
#ifdef AP_ENABLE_ASSERTS
	#define AP_ASSERT(x, ...) { if(!(x)) { AP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AP_CORE_ASSERT(x, ...) { if(!(x)) { AP_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AP_ASSERT(x, ...)
	#define AP_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define AP_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Apostle {

	template<typename T>	
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}