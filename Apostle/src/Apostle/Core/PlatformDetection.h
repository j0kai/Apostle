#pragma once

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
#endif
