#include "stardust/graphics/pipeline/Shader.h"

#include <utility>

#include "stardust/debug/logging/Logging.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        Shader::Shader(const Type type, const StringView filepath)
        {
            Initialise(type, filepath);
        }

        Shader::Shader(Shader&& other) noexcept
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_isCompiled, other.m_isCompiled);
        }

        auto Shader::operator =(Shader&& other) noexcept -> Shader&
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_isCompiled, other.m_isCompiled);

            return *this;
        }

        Shader::~Shader() noexcept
        {
            Destroy();
        }

        auto Shader::Initialise(const Type type, const StringView filepath) -> void
        {
            m_id = glCreateShader(static_cast<GLenum>(type));
            const auto shaderCode = vfs::ReadFileString(filepath);

            if (shaderCode.is_err())
            {
                return;
            }

            Compile(shaderCode.value().c_str());
            m_isCompiled = DidCompileSuccessfully();
        }

        auto Shader::Destroy() noexcept -> void
        {
            if (m_id != s_InvalidID)
            {
                glDeleteShader(m_id);

                m_id = s_InvalidID;
                m_isCompiled = false;
            }
        }
        
        auto Shader::Compile(const char* glslCodeData) -> void
        {
            glShaderSource(m_id, 1, &glslCodeData, nullptr);
            glCompileShader(m_id);
        }
        
        [[nodiscard]] auto Shader::DidCompileSuccessfully() const -> bool
        {
            GLint isSuccessful = GL_FALSE;
            glGetShaderiv(m_id, GL_COMPILE_STATUS, &isSuccessful);

            if (isSuccessful == GL_FALSE)
            {
                GLint infoLogLength = 0;
                glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

                List<char> infoLog(static_cast<usize>(infoLogLength));
                glGetShaderInfoLog(m_id, static_cast<GLsizei>(infoLogLength), nullptr, infoLog.data());

                Log::EngineError("Shader{} failed to compile : {}\n", m_id, infoLog.data());

                return false;
            }

            return true;
        }
    }
}
