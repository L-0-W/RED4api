#pragma once

#include <cstdint>
#include <unordered_map>
#include <filesystem>

namespace RED4api
{

	namespace Files
	{
		std::unordered_map<std::uint64_t, std::filesystem::path> files_path{};
	};

}
