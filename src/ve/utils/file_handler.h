#pragma once

#include <string>
#include <filesystem>

namespace utils {
    bool readFileToStr(const std::filesystem::path& path, std::string& out);
}
