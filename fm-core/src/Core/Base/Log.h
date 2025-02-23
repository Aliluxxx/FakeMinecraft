#pragma once

#include "Core/Base/Base.h"

#define FMT_UNICODE 0
#undef SPDLOG_ACTIFM_LEFML
#define SPDLOG_ACTIFM_LEFML SPDLOG_LEFML_TRACE
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace fm {

	class FM_API Log {

	public:

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetVulkanLogger() { return s_VulkanLogger; }

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_VulkanLogger;
	};
}

#if defined(FM_DIST)
#define FM_TRACE(...)
#define FM_INFO(...)
#define FM_WARN(...)
#define FM_ERROR(...)
#define FM_CRITICAL(...)
#else
#define FM_TRACE(...)         SPDLOG_LOGGER_TRACE(::fm::Log::GetClientLogger(), __VA_ARGS__)
#define FM_INFO(...)          SPDLOG_LOGGER_INFO(::fm::Log::GetClientLogger(), __VA_ARGS__)
#define FM_WARN(...)          SPDLOG_LOGGER_WARN(::fm::Log::GetClientLogger(), __VA_ARGS__)
#define FM_ERROR(...)         SPDLOG_LOGGER_ERROR(::fm::Log::GetClientLogger(), __VA_ARGS__)
#define FM_CRITICAL(...)      SPDLOG_LOGGER_CRITICAL(::fm::Log::GetClientLogger(), __VA_ARGS__)
#endif

#if defined(FM_ENGINE) && defined(FM_DIST)
#define FM_CORE_TRACE(...)
#define FM_CORE_INFO(...)
#define FM_CORE_WARN(...)
#define FM_CORE_ERROR(...)
#define FM_CORE_CRITICAL(...)
#elif defined(FM_ENGINE)
#define FM_CORE_TRACE(...)    SPDLOG_LOGGER_TRACE(::fm::Log::GetCoreLogger(), __VA_ARGS__)
#define FM_CORE_INFO(...)     SPDLOG_LOGGER_INFO(::fm::Log::GetCoreLogger(), __VA_ARGS__)
#define FM_CORE_WARN(...)     SPDLOG_LOGGER_WARN(::fm::Log::GetCoreLogger(), __VA_ARGS__)
#define FM_CORE_ERROR(...)    SPDLOG_LOGGER_ERROR(::fm::Log::GetCoreLogger(), __VA_ARGS__)
#define FM_CORE_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::fm::Log::GetCoreLogger(), __VA_ARGS__)
#endif
