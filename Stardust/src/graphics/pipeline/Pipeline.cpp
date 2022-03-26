#include "stardust/graphics/pipeline/Pipeline.h"

#include <utility>

#include "stardust/debug/logging/Logging.h"

namespace stardust
{
    namespace graphics
    {
        Pipeline::Pipeline(const Shader& vertexShader, const Shader& fragmentShader)
        {
            Initialise(vertexShader, fragmentShader);
        }

        Pipeline::Pipeline(Pipeline&& other) noexcept
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_uniformCache, other.m_uniformCache);
        }

        auto Pipeline::operator =(Pipeline&& other) noexcept -> Pipeline&
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_uniformCache, other.m_uniformCache);

            return *this;
        }

        Pipeline::~Pipeline() noexcept
        {
            Destroy();
        }

        auto Pipeline::Initialise(const Shader& vertexShader, const Shader& fragmentShader) -> void
        {
            m_id = glCreateProgram();

            glAttachShader(m_id, vertexShader.GetID());
            glAttachShader(m_id, fragmentShader.GetID());

            glLinkProgram(m_id);

            if (!DidLinkSuccessfully())
            {
                Destroy();

                return;
            }

            glValidateProgram(m_id);

            if (!DidValidateSuccessfully())
            {
                Destroy();

                return;
            }

            glDetachShader(m_id, vertexShader.GetID());
            glDetachShader(m_id, fragmentShader.GetID());
        }

        auto Pipeline::Destroy() noexcept -> void
        {
            if (m_id != s_InvalidID)
            {
                Disuse();

                glDeleteProgram(m_id);

                m_id = s_InvalidID;
                m_uniformCache.clear();
            }
        }

        auto Pipeline::Use() const -> void
        {
            glUseProgram(m_id);
        }

        auto Pipeline::Disuse() const -> void
        {
            glUseProgram(s_InvalidID);
        }

        auto Pipeline::SetTextureUniform(const String& name, const Texture::BindingIndex bindingIndex) const -> void
        {
            const UniformID uniformID = GetUniformID(name);
            glUniform1i(uniformID, static_cast<GLint>(bindingIndex));
        }

        auto Pipeline::SetTextureUniformVector(const String& name, const List<Texture::BindingIndex>& bindingIndices) const -> void
        {
            const UniformID uniformID = GetUniformID(name);
            glUniform1iv(uniformID, static_cast<GLsizei>(bindingIndices.size()), reinterpret_cast<const GLint*>(bindingIndices.data()));
        }

        [[nodiscard]] auto Pipeline::DidLinkSuccessfully() const -> bool
        {
            if (!IsStatusSuccessful(BuildStatus::Link))
            {
                GLint infoLogLength = 0;
                glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

                List<char> infoLog(static_cast<usize>(infoLogLength));
                glGetProgramInfoLog(m_id, static_cast<GLsizei>(infoLogLength), nullptr, infoLog.data());

                Log::EngineError("Shader program {} failed to link: {}\n", m_id, infoLog.data());

                return false;
            }

            return true;
        }

        [[nodiscard]] auto Pipeline::DidValidateSuccessfully() const -> bool
        {
            if (!IsStatusSuccessful(BuildStatus::Validate))
            {
                GLint infoLogLength = 0;
                glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

                List<char> infoLog(static_cast<usize>(infoLogLength));
                glGetProgramInfoLog(m_id, static_cast<GLsizei>(infoLogLength), nullptr, infoLog.data());

                Log::EngineError("Shader program {} failed to validate: {}\n", m_id, infoLog.data());

                return false;
            }

            return true;
        }

        [[nodiscard]] auto Pipeline::IsStatusSuccessful(const BuildStatus status) const -> bool
        {
            GLint isSuccessful = GL_FALSE;
            glGetProgramiv(m_id, static_cast<GLenum>(status), &isSuccessful);

            return isSuccessful == GL_TRUE;
        }

        [[nodiscard]] auto Pipeline::GetUniformID(const String& uniformName) const -> UniformID
        {
            if (const auto uniformLocation = m_uniformCache.find(uniformName);
                uniformLocation != std::cend(m_uniformCache))
            {
                return uniformLocation->second;
            }

            const UniformID uniformID = glGetUniformLocation(m_id, uniformName.c_str());

        #ifndef NDEBUG
            if (uniformID == static_cast<UniformID>(GL_INVALID_INDEX))
            {
                Log::EngineWarn("Uniform \"{}\" not found in shader program {}.\n", uniformName.c_str(), m_id);
            }
        #endif

            m_uniformCache[uniformName] = uniformID;

            return uniformID;
        }
    }
}
