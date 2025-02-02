#pragma once

#include "base.h"

class Shader {
public:
    Shader(const stdfs::path& vertPath, const stdfs::path& fragPath);
    ~Shader();

    void use();
    inline uint32_t getProgramId() const { return m_id; }

private:
    uint32_t m_id;
};
