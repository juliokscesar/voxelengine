#include "file_handler.h"

#include <fstream>

namespace utils {
    bool readFileContent(const std::filesystem::path& path, std::string& out) {
        out.clear();

        std::ifstream file(path);
        if (!file.is_open())
            return false;       

        out = std::string(
            std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()
        );
        return !out.empty();
    }
}
