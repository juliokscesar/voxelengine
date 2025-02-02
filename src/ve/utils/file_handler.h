#pragma once

#include <string>
#include <filesystem>

namespace utils {
    bool readFileContent(const std::filesystem::path& path, std::string& out);
}
