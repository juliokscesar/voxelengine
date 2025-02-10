#include "image.h"

#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>

namespace utils {
ImageData::ImageData() : width(0), height(0), nChannels(0), data(nullptr) {}

ImageData::ImageData(uint32_t width, uint32_t height, uint32_t nChannels)
    : width(width), height(height), nChannels(nChannels), data(nullptr) {}
    
ImageData::ImageData(const stdfs::path& path) { load(path); }

ImageData::~ImageData() { clear(); }

void ImageData::load(const stdfs::path& path) {
    int w, h, nc;
    data      = stbi_load(path.string().c_str(), &w, &h, &nc, 0);
    width     = w;
    height    = h;
    nChannels = nc;
}

void ImageData::clear() {
    if (data)
        stbi_image_free(data);
}

void fillSingleColor(ImageData& img, const glm::u8vec4& rgba) {
    const uint32_t pixelCount = img.width * img.height;
    img.data = new uint8_t[pixelCount];
    const uint8_t* colorData = reinterpret_cast<const uint8_t*>(glm::value_ptr(rgba));
    for (size_t i = 0; i < pixelCount; i++)
        std::memcpy(img.data + i * 4, colorData, 4 * sizeof(uint8_t)); 
}

}