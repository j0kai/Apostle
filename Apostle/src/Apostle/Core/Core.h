#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64 */
		#define AP_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "Windows x86 builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/*  TARGET_OS_MAC exists on all the platforms
	so we must check all of them (in this order) 
	to ensure that we're running on MAC and not some
	other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "iOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define AP_PLATFORM_IOS
		#error "iOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define AP_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else 
		#error "Unknown Apple platform!"
	#endif

/* We also have to check __ANDRIOD__ before __LINUX__
sicne android is based off of the linux kernal and 
thus has __LINUX__ defined */
#elif defined(__ANDROID__)
	#define AP_PLATFORM_ANDROID
	#error "Android not supported!"
#elif defined(__LINUX__)
	#define AP_PLATFORM_LINUX
	#error "Linux not supported!"
#else
/* Unknown compiler / platform */
	#error "Unknown platform!"
#endif // end of platform detection

// DLL Support
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
#endif // End of DLL support


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

#define AP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

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