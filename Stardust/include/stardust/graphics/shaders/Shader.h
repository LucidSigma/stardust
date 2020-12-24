#pragma once
#ifndef STARDUST_SHADER_H
#define STARDUST_SHADER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/enums/Status.h"

namespace stardust
{
	class Shader
		: private INoncopyable
	{
	public:
		enum class Type
			: GLenum
		{
			Vertex = GL_VERTEX_SHADER,
			TessellationControl = GL_TESS_CONTROL_SHADER,
			TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
			Geometry = GL_GEOMETRY_SHADER,
			Fragment = GL_FRAGMENT_SHADER,
			Compute = GL_COMPUTE_SHADER,
		};

		enum class Language
		{
			GLSL,
			SPIR_V,
		};

	private:
		GLuint m_id = 0u;
		bool m_isValid = false;

	public:
		Shader() = default;
		Shader(const Type type, const StringView& filepath, const Language language = Language::GLSL);

		Shader(Shader&& other) noexcept;
		Shader& operator =(Shader&& other) noexcept;

		~Shader() noexcept;

		void Initialise(const Type type, const StringView& filepath, const Language language = Language::GLSL);
		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_id != 0u && m_isValid; }
		inline u32 GetID() const noexcept { return static_cast<u32>(m_id); }

	private:
		void CompileGLSL(const char* shaderCodeData);
		void CompileSpirV(const Vector<ubyte>& shaderCode);
		Status CheckCompilationErrors() const;
	};
}

#endif