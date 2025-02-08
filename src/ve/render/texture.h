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

class Texture2D {
public:
    Texture2D();
    Texture2D(uint8_t* data, const Texture2DProperties& props, uint32_t unit = 0);
    Texture2D(const Texture2D& other);
    Texture2D(Texture2D&& other);

    Texture2D& operator=(const Texture2D& other) {
        if (this != &other) {
            this->m_glID    = other.m_glID;
            this->m_glUnit  = other.m_glUnit;
            this->m_props   = other.m_props;
        }
        return *this;
    }

    Texture2D& operator=(Texture2D&& other) {
        if (this != &other) {
            this->m_glID    = std::move(other.m_glID);
            this->m_glUnit  = std::move(other.m_glUnit);
            this->m_props   = std::move(other.m_props);
        }
        return *this;
    }

    inline uint32_t getID()   const     { return m_glID; }
    inline uint32_t getUnit() const     { return m_glUnit; } 
    inline void setUnit(uint32_t unit)  { m_glUnit = unit; }

    inline Texture2DProperties getProperties() const { return m_props; }

    void setWrapParams(GLint s, GLint t);
    void setFilterParams(GLint min, GLint mag);
    void updateTexParams(bool updateWrap, bool updateFilter);

    void assignRenderData(uint8_t* data, const Texture2DProperties& props, uint32_t unit = 0);
    void bind() const;
    void clear();

    static Texture2D fromColor(const glm::u8vec4& rgba, uint32_t width, uint32_t height, uint32_t unit = 0);

private:
    uint32_t m_glID     = 0;
    uint32_t m_glUnit   = 0;

    Texture2DProperties m_props;
    
    GLint m_wrapS       = GL_REPEAT;
    GLint m_wrapT       = GL_REPEAT;
    GLint m_minFilter   = GL_LINEAR_MIPMAP_NEAREST;
    GLint m_magFilter   = GL_LINEAR;
};
