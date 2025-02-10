#pragma once

#include <string>
#include <filesystem>

namespace stdfs = std::filesystem;

namespace utils {
bool readFileToStr(const std::filesystem::path& path, std::string& out);

constexpr bool existPaths() { return true; }
template <typename... Paths>
constexpr bool existPaths(const stdfs::path& first, const Paths&... rest) {
    return stdfs::exists(first) && existPaths(rest...);
}

} // namespace utils
