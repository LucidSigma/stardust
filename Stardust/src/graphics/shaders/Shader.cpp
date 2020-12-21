#include "stardust/graphics/shaders/Shader.h"

#include <utility>

#include "stardust/debug/logging/Log.h"
#include "stardust/vfs/VFS.h"

namespace stardust
{
	Shader::Shader(const Type type, const StringView& filepath, const bool isSpirV)
		: m_id(glCreateShader(static_cast<GLenum>(type)))
	{
		const auto shaderCode = vfs::ReadFileData(filepath);

		if (shaderCode.empty())
		{
			return;
		}

		if (isSpirV)
		{
			CompileSpirV(shaderCode);
		}
		else
		{
			const String shaderCodeString(
				reinterpret_cast<const char*>(shaderCode.data()),
				reinterpret_cast<const char*>(shaderCode.data() + shaderCode.size())
			);
			const char* shaderCodeData = shaderCodeString.data();
			
			CompileGLSL(shaderCodeData);
		}

		m_isValid = CheckCompilationErrors() == Status::Success;
	}

	Shader::Shader(Shader&& other) noexcept
	{
		Destroy();

		std::swap(m_id, other.m_id);
		std::swap(m_isValid, other.m_isValid);
	}

	Shader& Shader::operator =(Shader&& other) noexcept
	{
		Destroy();

		std::swap(m_id, other.m_id);
		std::swap(m_isValid, other.m_isValid);

		return *this;
	}

	Shader::~Shader() noexcept
	{
		Destroy();
	}

	void Shader::Destroy() noexcept
	{
		if (m_id != 0)
		{
			glDeleteShader(m_id);
			m_id = 0;
			m_isValid = false;
		}
	}

	void Shader::CompileGLSL(const char* shaderCodeData)
	{
		glShaderSource(m_id, 1, &shaderCodeData, nullptr);
		glCompileShader(m_id);
	}

	void Shader::CompileSpirV(const Vector<ubyte>& shaderCode)
	{
		glShaderBinary(1, &m_id, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderCode.data(), static_cast<GLsizei>(shaderCode.size()));
		glSpecializeShader(m_id, "main", 0, nullptr, nullptr);
	}

	Status Shader::CheckCompilationErrors() const
	{
		GLint isSuccessful = 0;
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &isSuccessful);

		if (isSuccessful == GL_FALSE)
		{
			GLint infoLogLength = 0;
			glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

			Vector<char> infoLog(infoLogLength);
			glGetShaderInfoLog(m_id, infoLogLength, nullptr, infoLog.data());

			Log::EngineError("Shader {} failed to compile: {}", m_id, infoLog.data());

			return Status::Fail;
		}

		return Status::Success;
	}
}