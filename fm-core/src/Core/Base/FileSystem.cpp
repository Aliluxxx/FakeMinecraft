#include "fmpch.h"

#include "FileSystem.h"

namespace fm {

	std::filesystem::path FileSystem::GetCurrentWorkingDirectory() {

		return std::filesystem::current_path();
	}

	std::vector<std::filesystem::path> FileSystem::GetList(const std::filesystem::path& dir, const bool only_files) {

		FM_CORE_ASSERT(std::filesystem::is_directory(dir), "The provided path is not a directory");

		std::vector<std::filesystem::path> list;
		const auto& tmp = std::filesystem::directory_iterator(dir);
		std::size_t length = 0;
		for (const auto& entry : tmp)
			if (!only_files || !std::filesystem::is_directory(entry.path()))
				length++;
		list.reserve(length);

		const auto& entries = std::filesystem::directory_iterator(dir);
		for (const auto& entry : entries)
			if (!only_files || !std::filesystem::is_directory(entry.path()))
				list.emplace_back(entry.path());

		return list;
	}

	std::vector<std::filesystem::path> FileSystem::GetList(const std::filesystem::path& dir, const char* extension) {

		FM_CORE_ASSERT(std::filesystem::is_directory(dir), "The provided path is not a directory");


		std::vector<std::filesystem::path> list;
		const auto& tmp = std::filesystem::directory_iterator(dir);
		std::size_t length = 0;
		for (const auto& entry : tmp)
			if (!std::filesystem::is_directory(entry.path()) && (entry.path().extension().compare(extension) == 0))
				length++;
		list.reserve(length);

		const auto& entries = std::filesystem::directory_iterator(dir);
		for (const auto& entry : entries)
			if (!std::filesystem::is_directory(entry.path()) && entry.path().extension().compare(extension) == 0)
				list.emplace_back(entry.path());

		return list;
	}
}
