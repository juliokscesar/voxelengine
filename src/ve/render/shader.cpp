#include "shader.h"

#include "utils/file_handler.h"
#include "core/logging.h"


Shader::Shader(const stdfs::path& vertPath, const stdfs::path& fragPath, bool compileOnLoad)
    : m_id(VE_DELETED_SHADER)
    , m_vertID(VE_DELETED_SHADER)
    , m_fragID(VE_DELETED_SHADER)
    , m_isCreated(false)
    , m_isDeleteMarked(false) {
    
    m_isCreated = initShader(vertPath, fragPath);
    if (!m_isCreated)
        return;
    if (compileOnLoad)
        compile();
}

Shader::Shader(const Shader &other)
    : m_id(other.m_id)
    , m_vertID(other.m_vertID)
    , m_fragID(other.m_fragID)
    , m_isUsable(other.m_isUsable)
    , m_isCreated(other.m_isCreated)
    , m_isCompiled(other.m_isCompiled)
    , m_isLinked(other.m_isLinked)
    , m_isDeleteMarked(other.m_isDeleteMarked) {}

Shader::Shader(Shader &&other)
    : m_id(std::move(other.m_id))
    , m_vertID(std::move(other.m_vertID))
    , m_fragID(std::move(other.m_fragID))
    , m_isUsable(std::move(other.m_isUsable))
    , m_isCreated(std::move(other.m_isCreated))
    , m_isCompiled(std::move(other.m_isCompiled))
    , m_isLinked(std::move(other.m_isLinked))
    , m_isDeleteMarked(std::move(other.m_isDeleteMarked)) {}

Shader::~Shader() {
    // clear();
}

void Shader::use() {
    if (m_isUsable)
        glUseProgram(m_id);
    else
        grflog::warn("Trying to use a shader program that's not usable");
}

bool Shader::compile() {
    if (!m_isCreated)
        return false;

    std::string err;
    std::vector<uint32_t> compQueue({
        m_vertID, m_fragID
    });
    for (const auto& sID : compQueue) {
        glCompileShader(sID);

        if (!checkCompileError(sID, err)) {
            grflog::fatal("Error compiling shader: {}", err);
            return false;
        }

        glAttachShader(m_id, sID);
    }
    m_isCompiled = true;

    glLinkProgram(m_id);
    if (!checkLinkError(err)) {
        grflog::fatal("Error linking shader program: {}", err);
        return false;
    }   
    m_isLinked = true;

    m_isUsable = m_isCompiled && m_isLinked;
    clearShaders();

    return true;
}

void Shader::clear() {
    clearShaders();
    clearProgram();
}

bool Shader::initShader(const stdfs::path& vertPath, const stdfs::path& fragPath) {
    if (!createShader(m_vertID, vertPath, GL_VERTEX_SHADER)) {
        grflog::critical("Failed to create vertex shader. Probably unable to find {}", vertPath.string());
        return false;
    }

    if (!createShader(m_fragID, fragPath, GL_FRAGMENT_SHADER)) {
        grflog::critical("Failed to create fragment shader. Probably unable to find {}", fragPath.string());
        return false;
    }

    m_id = glCreateProgram();
    m_isCreated = true;
    return true;
}


bool Shader::createShader(uint32_t& out, const stdfs::path& srcPath, GLenum glShaderType) {
    std::string srcStr{};
    if (!utils::readFileToStr(srcPath, srcStr))
        return false;
    
    out = glCreateShader(glShaderType);
    const char* src = srcStr.c_str();
    glShaderSource(out, 1, &src, nullptr);
    return true;
}

bool Shader::checkCompileError(uint32_t shaderID, std::string& errOut) {
    int success{};
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
        errOut = infoLog;
    }

    return static_cast<bool>(success);
}

bool Shader::checkLinkError(std::string& errOut) {
    int success{};
    char infoLog[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, sizeof(infoLog), nullptr, infoLog);
        errOut = infoLog;
    }
    return static_cast<bool>(success);
}

void Shader::clearShaders() {
    if (!m_isCreated)
        return;

    std::vector<uint32_t*> deleteQueue({
        &m_vertID, &m_fragID
    });
    for (auto& id : deleteQueue) {
        if (*id != VE_DELETED_SHADER) {
            glDeleteShader(*id);
            *id = VE_DELETED_SHADER;
        }
    }
}

void Shader::clearProgram() {
    if (m_isCreated && !m_isDeleteMarked)
        glDeleteProgram(m_id);
    
    m_isCreated = false;
    m_isCompiled = false;
    m_isLinked = false;
    m_isDeleteMarked = true;
}
