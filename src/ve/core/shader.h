#pragma once

#include "base.h"

#define VE_DELETED_SHADER UINT32_MAX

class Shader {
public:
    Shader(const stdfs::path& vertPath, const stdfs::path& fragPath, bool compileOnLoad);
    ~Shader();

    void use();
    bool compile();
    void clear();
    inline uint32_t getProgramId() const { return m_id; }
    inline bool isUsable() const { return m_isUsable; };

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

    bool m_isUsable;

    bool m_isCreated;
    bool m_isCompiled;
    bool m_isLinked;

    bool m_isDeleteMarked;
};
