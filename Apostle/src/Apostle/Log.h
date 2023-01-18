#pragma once

#include <memory>
#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Apostle {

	class APOSTLE_API Log
	{
	public: 
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define AP_CORE_TRACE(...)     ::Apostle::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AP_CORE_INFO(...)      ::Apostle::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AP_CORE_WARN(...)      ::Apostle::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AP_CORE_ERROR(...)	   ::Apostle::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AP_CORE_CRITICAL(...)  ::Apostle::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define AP_TRACE(...)      ::Apostle::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AP_INFO(...)       ::Apostle::Log::GetClientLogger()->info(__VA_ARGS__)
#define AP_WARN(...)       ::Apostle::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AP_ERROR(...)	   ::Apostle::Log::GetClientLogger()->error(__VA_ARGS__)
#define AP_CRITICAL(...)   ::Apostle::Log::GetClientLogger()->critical(__VA_ARGS__)