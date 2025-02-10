#pragma once

#include "core/base.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#define VE_DELETED_SHADER UINT32_MAX

class Shader {
public:
    Shader(const stdfs::path& vertPath, const stdfs::path& fragPath, bool compileOnLoad);
    Shader(const Shader& other);
    Shader(Shader&& other);
    ~Shader();

    Shader& operator=(const Shader& other) {
        if (this != &other) {
            m_id                = other.m_id;
            m_vertID            = other.m_vertID;
            m_fragID            = other.m_fragID;
            m_isUsable          = other.m_isUsable;
            m_isCreated         = other.m_isCreated;
            m_isCompiled        = other.m_isCompiled;
            m_isLinked          = other.m_isLinked;
            m_isDeleteMarked    = other.m_isDeleteMarked;
        }
        return *this;
    }
    Shader& operator=(Shader&& other) {
        if (this != &other) {
            m_id                = std::move(other.m_id);
            m_vertID            = std::move(other.m_vertID);
            m_fragID            = std::move(other.m_fragID);
            m_isUsable          = std::move(other.m_isUsable);
            m_isCreated         = std::move(other.m_isCreated);
            m_isCompiled        = std::move(other.m_isCompiled);
            m_isLinked          = std::move(other.m_isLinked);
            m_isDeleteMarked    = std::move(other.m_isDeleteMarked);
        }
        return *this;
    }
    void use();
    bool compile();
    void clear();
    inline uint32_t getProgramId() const { return m_id; }
    inline bool isUsable() const { return m_isUsable; };

    void setUniformUInt(const std::string& name, uint32_t value) const;
    void setUniformFloat(const std::string& name, float value) const;
    void setUniformVec3(const std::string& name, const glm::vec3& vec) const;
    void setUniformMat4(const std::string& name, const glm::mat4& mat) const;

    uint32_t getUniformLocation(const std::string& name) const;    

private:
    bool initShader(const stdfs::path& vertPath, const stdfs::path& fragPath);
    bool createShader(uint32_t& out, const stdfs::path& srcPath, GLenum glShaderType);

    bool checkCompileError(uint32_t shaderID, std::string& errOut);
    bool checkLinkError(std::string& errOut);

    void clearShaders();
    void clearProgram();

private:
    uint32_t m_id;

    uint32_t m_vertID, m_fragID;

    mutable std::unordered_map<std::string, uint32_t> m_uniformLocCache;

    bool m_isUsable;

    bool m_isCreated;
    bool m_isCompiled;
    bool m_isLinked;

    bool m_isDeleteMarked;
};
