#pragma once
#ifndef STARDUST_RENDER_BUFFER_H
#define STARDUST_RENDER_BUFFER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <ANGLE/GLES3/gl3.h>

#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"

namespace stardust
{
    namespace graphics
    {
        class RenderBuffer final
        {
        public:
            using ID = GLuint;

            enum class StorageFormat
                : GLenum
            {
                R8 = GL_R8,
                RG8 = GL_RG8,
                RGB8 = GL_RGB8,
                RGBA8 = GL_RGBA8,
                DepthStencil = GL_DEPTH24_STENCIL8,
            };

        private:
            static constexpr ID s_InvalidID = 0u;

            ID m_id = s_InvalidID;

            UVector2 m_size = UVector2Zero;
            StorageFormat m_storageFormat = StorageFormat::RGBA8;

        public:
            [[nodiscard]] static constexpr auto InvalidID() noexcept -> ID { return s_InvalidID; }

            RenderBuffer() = default;
            RenderBuffer(const UVector2 extent, const StorageFormat storageFormat);

            RenderBuffer(RenderBuffer&& other) noexcept;
            auto operator =(RenderBuffer&& other) noexcept -> RenderBuffer&;

            ~RenderBuffer() noexcept;

            auto Initialise(const UVector2 extent, const StorageFormat storageFormat) -> void;
            auto Destroy() noexcept -> void;

            auto Resize(const UVector2 extent, const StorageFormat storageFormat) -> void;

            auto Bind() const noexcept -> void;
            auto Unbind() const noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_id != s_InvalidID; }

            [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }

            [[nodiscard]] inline auto GetSize() const noexcept -> UVector2 { return m_size; }
            [[nodiscard]] inline auto GetStorageFormat() const noexcept -> StorageFormat { return m_storageFormat; }
        };
    }
}

#endif
