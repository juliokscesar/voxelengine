#include "texture.h"

Texture2D::Texture2D() {}

Texture2D::Texture2D(uint8_t* data, const Texture2DProperties& props, uint32_t unit) {
    assignRenderData(data, props, unit);
}

Texture2D::Texture2D(const Texture2D& other)
    : m_glID(other.m_glID)
    , m_glUnit(other.m_glUnit)
    , m_props(other.m_props) {}

Texture2D::Texture2D(Texture2D&& other)
    : m_glID(std::move(other.m_glID))
    , m_glUnit(std::move(other.m_glUnit))
    , m_props(std::move(other.m_props)) {}

void Texture2D::setWrapParams(GLint s, GLint t) {
    m_wrapS = s;
    m_wrapT = t;
    updateTexParams(/* wrap */ true, /* filter */ false);
}

void Texture2D::setFilterParams(GLint min, GLint mag) {
    m_minFilter = min;
    m_magFilter = mag;
    updateTexParams(/* wrap */ false, /* filteir */ true);
}

void Texture2D::updateTexParams(bool updateWrap, bool updateFilter) {
    this->bind();
    if (updateWrap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    }
    if (updateFilter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
    }
}

void Texture2D::assignRenderData(uint8_t *data, const Texture2DProperties &props, uint32_t unit) {
    m_glUnit = unit;
    m_props = props;

    if (m_glID != GL_NONE)
        clear();

    glGenTextures(1, &m_glID);
    bind();

    glTexImage2D(GL_TEXTURE_2D, 0, /* mipmap level */
                props.storeFmt, props.width, props.height,
                0, /* always 0, legacy stuff */
                props.pixelFmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    updateTexParams(/* wrap */ true, /* filter */ true);
}

void Texture2D::bind() const {
    glActiveTexture(GL_TEXTURE0 + m_glUnit);
    glBindTexture(GL_TEXTURE_2D, m_glID);
}

void Texture2D::clear() {
    glDeleteTextures(1, &m_glID);
    m_glID = 0;
}
