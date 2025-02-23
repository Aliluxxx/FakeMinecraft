#pragma once

#include <string>
#include <filesystem>

#include "Core/Base/Base.h"

namespace fm {

	class FM_API FileSystem {

	public:

		static std::filesystem::path GetCurrentWorkingDirectory();

		static std::vector<std::filesystem::path> GetList(const std::filesystem::path& dir, const bool only_files = false);
		static std::vector<std::filesystem::path> GetList(const std::filesystem::path& dir, const char* extension);
	};
}
