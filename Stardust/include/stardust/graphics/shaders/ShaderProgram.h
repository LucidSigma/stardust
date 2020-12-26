#pragma once
#ifndef STARDUST_SHADER_PROGRAM_H
#define STARDUST_SHADER_PROGRAM_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/shaders/Shader.h"
#include "stardust/math/Math.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
	class ShaderProgram
		: private INoncopyable
	{
	private:
		GLuint m_id = 0u;
		bool m_isValid = false;

		mutable HashMap<String, GLint> m_uniformCache{ };

	public:
		ShaderProgram() = default;
		ShaderProgram(const Vector<ObserverPtr<const Shader>>& shaders);

		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator =(ShaderProgram&& other) noexcept;

		~ShaderProgram() noexcept;

		void Initialise(const Vector<ObserverPtr<const Shader>>& shaders);
		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_id != 0u && m_isValid; }

		void Use() const;
		void Disuse() const;

		template <typename T>
		void SetUniform(const String& name, const T& value) const
		{
			static_assert(false, "Invalid uniform type.");
		}

		template <>
		void SetUniform<bool>(const String& name, const bool& value) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform1i(location, static_cast<int>(value));
		}

		template <>
		void SetUniform<i32>(const String& name, const i32& value) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform1i(location, value);
		}

		template <>
		void SetUniform<f32>(const String& name, const f32& value) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform1f(location, value);
		}

		template <>
		void SetUniform<f64>(const String& name, const f64& value) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform1f(location, static_cast<float>(value));
		}

		template <>
		void SetUniform<Vec2>(const String& name, const Vec2& value) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform2fv(location, 1, glm::value_ptr(value));
		}

		template <>
		void SetUniform<Vec3>(const String& name, const Vec3& value) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform3fv(location, 1, glm::value_ptr(value));
		}

		template <>
		void SetUniform<Vec4>(const String& name, const Vec4& value) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform4fv(location, 1, glm::value_ptr(value));
		}

		template <>
		void SetUniform<Mat4>(const String& name, const Mat4& value) const
		{
			const GLint location = GetUniformLocation(name);
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
		}

		template <typename T>
		void SetUniformVector(const String& name, const Vector<T>& values) const
		{
			static_assert(false, "Invalid uniform vector type.");
		}

		template<>
		void SetUniformVector<i32>(const String& name, const Vector<i32>& values) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform1iv(location, static_cast<GLsizei>(values.size()), values.data());
		}

		template<>
		void SetUniformVector<f32>(const String& name, const Vector<f32>& values) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform1fv(location, static_cast<GLsizei>(values.size()), values.data());
		}

		template <>
		void SetUniformVector<Vec2>(const String& name, const Vector<Vec2>& values) const
		{
			const GLint location = GetUniformLocation(name);
			glUniform2fv(location, static_cast<GLsizei>(values.size()), reinterpret_cast<const float*>(values.data()));
		}

		void SetTextureUniform(const String& name, const i32 index) const;

		inline u32 GetID() const noexcept { return static_cast<u32>(m_id); }

	private:
		[[nodiscard]] Status CheckLinkingStatus() const;
		[[nodiscard]] Status CheckValidationStatus() const;
		[[nodiscard]] Status CheckStatus(const GLenum status) const;

		[[nodiscard]] i32 GetUniformLocation(const String& uniformName) const;
	};
}

#endif