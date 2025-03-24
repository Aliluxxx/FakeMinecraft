#include "fmpch.h"

#include "Core/Base/Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace fm {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_VulkanLogger;

	void Log::Init() {

		s_CoreLogger = spdlog::stdout_color_mt("CORE");
		s_CoreLogger->set_pattern("[%T.%e][%n][%s:%#][%^%l%$]: %v");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_pattern("[%T.%e][%s:%#][%^%l%$]: %v");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_VulkanLogger = spdlog::stdout_color_mt("VULKAN");
		s_VulkanLogger->set_pattern("[%T.%e][%n][%^%l%$]: %v");
		s_VulkanLogger->set_level(spdlog::level::warn);
	}
}
