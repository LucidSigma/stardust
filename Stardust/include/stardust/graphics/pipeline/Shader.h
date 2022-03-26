#pragma once
#ifndef STARDUST_SHADER_H
#define STARDUST_SHADER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <ANGLE/GLES3/gl3.h>

#include "stardust/types/Containers.h"

namespace stardust
{
    namespace graphics
    {
        class Shader final
            : private INoncopyable
        {
        public:
            using ID = GLuint;

            enum class Type
                : GLenum
            {
                Vertex = GL_VERTEX_SHADER,
                Fragment = GL_FRAGMENT_SHADER,
            };

        private:
            static constexpr ID s_InvalidID = 0u;

            ID m_id = s_InvalidID;
            bool m_isCompiled = false;

        public:
            [[nodiscard]] static constexpr auto InvalidID() noexcept -> ID { return s_InvalidID; }

            Shader() = default;
            Shader(const Type type, const StringView filepath);

            Shader(const Shader&) = delete;
            auto operator =(const Shader&) -> Shader& = delete;

            Shader(Shader&& other) noexcept;
            auto operator =(Shader&& other) noexcept -> Shader&;

            ~Shader() noexcept;

            auto Initialise(const Type type, const StringView filepath) -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_id != s_InvalidID && m_isCompiled; }
            [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }

        private:
            auto Compile(const char* glslCodeData) -> void;
            [[nodiscard]] auto DidCompileSuccessfully() const -> bool;
        };
    }
}

#endif
