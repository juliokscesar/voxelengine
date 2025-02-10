#include "texture.h"


Texture2D::Texture2D() {}

Texture2D::Texture2D(uint32_t id, uint32_t glArray,
                     const Texture2DProperties& props) 
    : m_gpuTex(id, glArray)
    , m_props(props) {}

Texture2D::Texture2D(const Texture2D& other)
    : m_gpuTex(other.m_gpuTex)
    , m_props(other.m_props) {}

Texture2D::Texture2D(Texture2D&& other)
    : m_gpuTex(std::move(other.m_gpuTex))
    , m_props(std::move(other.m_props)) {}
