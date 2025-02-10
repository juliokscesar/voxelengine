#pragma once

#include <filesystem>

#include <glm/vec4.hpp>

namespace stdfs = std::filesystem;

namespace utils {
struct ImageData {
    uint32_t width;
    uint32_t height;
    uint32_t nChannels;
    uint8_t* data;

    ImageData();
    ImageData(uint32_t width, uint32_t height, uint32_t nChannels);
    ImageData(const stdfs::path& path);
    ~ImageData();

    void load(const stdfs::path& path);
    void clear();
};

void fillSingleColor(ImageData& img, const glm::u8vec4& rgba);
} // namespace utils