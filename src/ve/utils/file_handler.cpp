#include "file_handler.h"

#include <fstream>
#include <stb/stb_image.h>

namespace utils {
    bool readFileToStr(const std::filesystem::path& path, std::string& out) {
        out.clear();

        std::ifstream file(path);
        if (!file.is_open())
            return false;       

        out = std::string(
            std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()
        );
        return !out.empty();
    }


    ImageData::ImageData()
        : width(0), height(0), nChannels(0), data(nullptr) {}

    ImageData::ImageData(const stdfs::path& path) {
        load(path);
    }

    ImageData::~ImageData() {
        clear();
    }

    void ImageData::load(const stdfs::path& path) {
        int w, h, nc;
        data = stbi_load(path.string().c_str(), &w, &h, &nc, 0);
        width = w;
        height = h;
        nChannels = nc;
    }

    void ImageData::clear() {
        if (data)
            stbi_image_free(data);
    }
}
