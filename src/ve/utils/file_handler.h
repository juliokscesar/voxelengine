#pragma once

#include <string>
#include <filesystem>

namespace stdfs = std::filesystem;

namespace utils {
    bool readFileToStr(const std::filesystem::path& path, std::string& out);

    constexpr bool existPaths() {
        return true;
    }
    template<typename... Paths>
    constexpr bool existPaths(const stdfs::path& first, const Paths&... rest) {
        return stdfs::exists(first) && existPaths(rest...);
    }

    struct ImageData {
        uint32_t width;
        uint32_t height;
        uint32_t nChannels;
        uint8_t* data;

        ImageData();
        ImageData(const stdfs::path& path);
        ~ImageData();

        void load(const stdfs::path& path);
        void clear();
    };
}
