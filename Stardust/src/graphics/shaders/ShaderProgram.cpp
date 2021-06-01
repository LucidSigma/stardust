#include "stardust/graphics/shaders/ShaderProgram.h"

#include <utility>

#include "stardust/debug/logging/Log.h"

namespace stardust
{
    ShaderProgram::ShaderProgram(const Vector<ObserverPtr<const Shader>>& shaders)
    {
        Initialise(shaders);
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    {
        Destroy();

        std::swap(m_id, other.m_id);
        std::swap(m_isValid, other.m_isValid);
        std::swap(m_uniformCache, other.m_uniformCache);
    }

    ShaderProgram& ShaderProgram::operator =(ShaderProgram&& other) noexcept
    {
        Destroy();

        std::swap(m_id, other.m_id);
        std::swap(m_isValid, other.m_isValid);
        std::swap(m_uniformCache, other.m_uniformCache);

        return *this;
    }

    ShaderProgram::~ShaderProgram() noexcept
    {
        Destroy();
    }

    void ShaderProgram::Initialise(const Vector<ObserverPtr<const Shader>>& shaders)
    {
        m_id = glCreateProgram();

        for (const auto& shader : shaders)
        {
            glAttachShader(m_id, shader->GetID());
        }

        glLinkProgram(m_id);

        if (CheckLinkingStatus() != Status::Success)
        {
            Destroy();

            return;
        }

        glValidateProgram(m_id);

        if (CheckValidationStatus() != Status::Success)
        {
            Destroy();

            return;
        }

        for (const auto& shader : shaders)
        {
            glDetachShader(m_id, shader->GetID());
        }

        m_isValid = true;
    }

    void ShaderProgram::Destroy() noexcept
    {
        if (m_id != 0u)
        {
            Disuse();

            glDeleteProgram(m_id);

            m_id = 0u;
            m_isValid = false;
            m_uniformCache.clear();
        }
    }


    void ShaderProgram::Use() const
    {
        glUseProgram(m_id);
    }

    void ShaderProgram::Disuse() const
    {
        glUseProgram(0u);
    }

    void ShaderProgram::SetTextureUniform(const String& name, const i32 index) const
    {
        const GLint location = GetUniformLocation(name);
        glUniform1i(location, index);
    }

    [[nodiscard]] Status ShaderProgram::CheckLinkingStatus() const
    {
        if (CheckStatus(GL_LINK_STATUS) != Status::Success)
        {
            GLint infoLogLength = 0;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

            Vector<char> infoLog(infoLogLength);
            glGetProgramInfoLog(m_id, infoLogLength, nullptr, infoLog.data());

            Log::EngineError("Shader program {} failed to link: {}", m_id, infoLog.data());

            return Status::Fail;
        }

        return Status::Success;
    }

    [[nodiscard]] Status ShaderProgram::CheckValidationStatus() const
    {
        if (CheckStatus(GL_VALIDATE_STATUS) != Status::Success)
        {
            GLint infoLogLength = 0;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

            Vector<char> infoLog(infoLogLength);
            glGetProgramInfoLog(m_id, infoLogLength, nullptr, infoLog.data());

            Log::EngineError("Shader program {} failed to validate: {}", m_id, infoLog.data());

            return Status::Fail;
        }

        return Status::Success;
    }

    [[nodiscard]] Status ShaderProgram::CheckStatus(const GLenum status) const
    {
        GLint isSuccessful = 0;
        glGetProgramiv(m_id, status, &isSuccessful);

        return isSuccessful == GL_TRUE ? Status::Success : Status::Fail;
    }

    [[nodiscard]] i32 ShaderProgram::GetUniformLocation(const String& uniformName) const
    {
        static_assert(sizeof(i32) == sizeof(GLint));

        if (m_uniformCache.contains(uniformName))
        {
            return static_cast<i32>(m_uniformCache[uniformName]);
        }

        const GLint location = glGetUniformLocation(m_id, uniformName.c_str());

    #ifndef NDEBUG
        if (location == GL_INVALID_INDEX)
        {
            Log::EngineWarn("Uniform {} not found in shader program {}.", uniformName.c_str(), m_id);
        }
    #endif

        m_uniformCache[uniformName] = location;

        return static_cast<i32>(location);
    }
}