#pragma once
#ifndef STARDUST_PIPELINE_H
#define STARDUST_PIPELINE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <ANGLE/GLES3/gl3.h>

#include "stardust/graphics/pipeline/Shader.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/Utility.h"

namespace stardust
{
    namespace graphics
    {
        class Pipeline final
            : private INoncopyable
        {
        public:
            using ID = GLuint;

        private:
            using UniformID = GLint;

            enum class BuildStatus
                : GLenum
            {
                Link = GL_LINK_STATUS,
                Validate = GL_VALIDATE_STATUS,
            };

            static constexpr ID s_InvalidID = 0u;

            ID m_id = s_InvalidID;
            mutable HashMap<String, UniformID> m_uniformCache{ };

        public:
            [[nodiscard]] static constexpr auto InvalidID() noexcept -> ID { return s_InvalidID; }

            Pipeline() = default;
            Pipeline(const Shader& vertexShader, const Shader& fragmentShader);

            Pipeline(const Pipeline&) = delete;
            auto operator =(const Pipeline&) -> Pipeline& = delete;

            Pipeline(Pipeline&& other) noexcept;
            auto operator =(Pipeline&& other) noexcept -> Pipeline&;

            ~Pipeline() noexcept;

            auto Initialise(const Shader& vertexShader, const Shader& fragmentShader) -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_id != s_InvalidID; }

            auto Use() const -> void;
            auto Disuse() const -> void;

            template <typename T>
            auto SetUniform(const String& name, const T & value) const -> void
            {
                static_assert(utility::DependentFalse<T>, "Invalid uniform type.");
            }

            template <>
            auto SetUniform<bool>(const String& name, const bool& value) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform1i(uniformID, static_cast<i32>(value));
            }

            template <>
            auto SetUniform<i32>(const String& name, const i32& value) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform1i(uniformID, value);
            }

            template <>
            auto SetUniform<f32>(const String& name, const f32& value) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform1f(uniformID, value);
            }

            template <>
            auto SetUniform<f64>(const String& name, const f64& value) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform1f(uniformID, static_cast<f32>(value));
            }

            template <>
            auto SetUniform<Vector2>(const String& name, const Vector2& value) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform2fv(uniformID, 1, glm::value_ptr(value));
            }

            template <>
            auto SetUniform<Vector3>(const String& name, const Vector3& value) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform3fv(uniformID, 1, glm::value_ptr(value));
            }

            template <>
            auto SetUniform<Vector4>(const String& name, const Vector4& value) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform4fv(uniformID, 1, glm::value_ptr(value));
            }

            template <>
            auto SetUniform<Matrix4>(const String& name, const Matrix4& value) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(value));
            }

            template <typename T>
            auto SetUniformVector(const String& name, const List<T>& values) const -> void
            {
                static_assert(utility::DependentFalse<T>, "Invalid uniform vector type.");
            }

            template<>
            auto SetUniformVector<i32>(const String& name, const List<i32>& values) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform1iv(uniformID, static_cast<GLsizei>(values.size()), values.data());
            }

            template<>
            auto SetUniformVector<f32>(const String& name, const List<f32>& values) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform1fv(uniformID, static_cast<GLsizei>(values.size()), values.data());
            }

            template <>
            auto SetUniformVector<Vector2>(const String& name, const List<Vector2>& values) const -> void
            {
                const UniformID uniformID = GetUniformID(name);
                glUniform2fv(uniformID, static_cast<GLsizei>(values.size()), reinterpret_cast<const GLfloat*>(values.data()));
            }

            auto SetTextureUniform(const String& name, const Texture::BindingIndex bindingIndex) const -> void;
            auto SetTextureUniformVector(const String& name, const List<Texture::BindingIndex>& bindingIndices) const -> void;

            [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }

        private:
            [[nodiscard]] auto DidLinkSuccessfully() const -> bool;
            [[nodiscard]] auto DidValidateSuccessfully() const -> bool;
            [[nodiscard]] auto IsStatusSuccessful(const BuildStatus status) const -> bool;

            [[nodiscard]] auto GetUniformID(const String& uniformName) const -> UniformID;
        };
    }
}

#endif
