#pragma once

#include "core/base.h"

#include <glm/vec4.hpp>

struct Texture2DProperties {
    uint32_t width;
    uint32_t height;
    
    GLenum storeFmt;
    GLenum pixelFmt;

    Texture2DProperties()
        : width(0), height(0), storeFmt(GL_NONE), pixelFmt(GL_NONE) {}

    Texture2DProperties(uint32_t width, uint32_t height, GLenum storeFmt, GLenum pixelFmt)
        : width(width), height(height), storeFmt(storeFmt), pixelFmt(pixelFmt) {}
};

struct GPUTexture {
    uint32_t ID;
    uint32_t arrayID;

    GPUTexture()
        : ID(0), arrayID(0) {}

    GPUTexture(uint32_t ID, uint32_t arrayID)
        : ID(ID), arrayID(arrayID) {}
};

class Texture2D {
public:
    Texture2D();
    Texture2D(uint32_t id, uint32_t glArray, const Texture2DProperties& props);
    Texture2D(const Texture2D& other);
    Texture2D(Texture2D&& other);

    Texture2D& operator=(const Texture2D& other) {
        if (this != &other) {
            this->m_gpuTex = other.m_gpuTex;
            this->m_props  = other.m_props;
        }
        return *this;
    }

    Texture2D& operator=(Texture2D&& other) {
        if (this != &other) {
            this->m_gpuTex = std::move(other.m_gpuTex);
            this->m_props  = std::move(other.m_props);
        }
        return *this;
    }

    bool operator==(const Texture2D& other) {
        return (this->m_gpuTex.ID == other.m_gpuTex.ID) && (this->m_gpuTex.arrayID == other.m_gpuTex.arrayID);
    }

    inline uint32_t getID()         const     { return m_gpuTex.ID; }
    inline uint32_t getTexArray()   const     { return m_gpuTex.arrayID; }

    inline Texture2DProperties getProperties() const { return m_props; }

private:
    GPUTexture m_gpuTex;
    Texture2DProperties m_props;
};
