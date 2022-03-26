#pragma once
#ifndef STARDUST_TEXTURE_H
#define STARDUST_TEXTURE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <ANGLE/GLES3/gl3.h>

#include "stardust/graphics/texture/Sampler.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    namespace graphics
    {
        struct TextureCoordinatePair final
        {
            Vector2 lowerLeft = Vector2Zero;
            Vector2 upperRight = Vector2One;

            auto Inset(const class Texture& texture, const f32 texelCount = 1.0f) -> void;
            auto Inset(const class Texture& texture, const Vector2 texelCount) -> void;
            auto Inset(const class Texture& texture, const f32 uTexelCount, const f32 vTexelCount) -> void;

            auto ShiftLowerLeft(const class Texture& texture, const f32 texelCount = 1.0f) -> void;
            auto ShiftLowerLeft(const class Texture& texture, const Vector2 texelCount) -> void;
            auto ShiftLowerLeft(const class Texture& texture, const f32 uTexelCount, const f32 vTexelCount) -> void;

            auto ShiftUpperRight(const class Texture& texture, const f32 texelCount = 1.0f) -> void;
            auto ShiftUpperRight(const class Texture& texture, const Vector2 texelCount) -> void;
            auto ShiftUpperRight(const class Texture& texture, const f32 uTexelCount, const f32 vTexelCount) -> void;
        };

        class Texture final
            : private INoncopyable
        {
        public:
            using ID = GLuint;
            using BindingIndex = u32;

        private:
            enum class InternalComponentFormat
                : GLint
            {
                OneByte = GL_R8,
                TwoBytes = GL_RG8,
                ThreeBytes = GL_RGB8,
                FourBytes = GL_RGBA8,
                DepthStencil = GL_DEPTH24_STENCIL8,
            };

            enum class PixelFormat
                : GLenum
            {
                R = GL_RED,
                RG = GL_RG,
                RGB = GL_RGB,
                RGBA = GL_RGBA,
                DepthStencil = GL_DEPTH_STENCIL,
            };

            static constexpr ID s_InvalidID = 0u;

            inline static const HashMap<u32, Pair<InternalComponentFormat, PixelFormat>> s_componentMap{
                { 1u, { InternalComponentFormat::OneByte, PixelFormat::R } },
                { 2u, { InternalComponentFormat::TwoBytes, PixelFormat::RG } },
                { 3u, { InternalComponentFormat::ThreeBytes, PixelFormat::RGB } },
                { 4u, { InternalComponentFormat::FourBytes, PixelFormat::RGBA } },
            };

            ID m_id = s_InvalidID;
            UVector2 m_size = UVector2Zero;

            bool m_isValid = false;

        public:
            [[nodiscard]] static constexpr auto InvalidID() noexcept -> ID { return s_InvalidID; }

            static auto ResetActiveTexture() -> void;

            Texture() = default;
            explicit Texture(const StringView filepath, const Sampler& sampler = DefaultSampler);
            Texture(const UVector2 extent, const u32 channelCount, const Sampler& sampler = DefaultSampler);
            Texture(const List<ubyte>& data, const UVector2 extent, const u32 channelCount, const Sampler& sampler = DefaultSampler);
            Texture(const ubyte* const data, const UVector2 extent, const u32 channelCount, const Sampler& sampler = DefaultSampler);

            Texture(Texture&& other) noexcept;
            auto operator =(Texture&& other) noexcept -> Texture&;

            ~Texture() noexcept;

            auto Initialise(const StringView filepath, const Sampler& sampler = DefaultSampler) -> void;
            auto Initialise(const UVector2 extent, const u32 channelCount, const Sampler& sampler = DefaultSampler) -> void;
            auto Initialise(const List<ubyte>& data, const UVector2 extent, const u32 channelCount, const Sampler& sampler = DefaultSampler) -> void;
            auto Initialise(const ubyte* const data, const UVector2 extent, const u32 channelCount, const Sampler& sampler = DefaultSampler) -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_id != s_InvalidID && m_isValid; }

            auto Bind(const BindingIndex bindingIndex = 0u) const -> void;
            auto Unbind() const -> void;

            auto SetData(const List<ubyte>& data, const UVector2 extent, const u32 channelCount) -> void;
            auto SetData(const ubyte* const data, const UVector2 extent, const u32 channelCount) -> void;

            auto SetHorizontalWrapMode(const TextureWrap horizontalWrap) const -> void;
            auto SetVerticalWrapMode(const TextureWrap verticalWrap) const -> void;
            auto SetMinFilter(const TextureMinFilter minFilter) const -> void;
            auto SetMagFilter(const TextureMagFilter magFilter) const -> void;

            [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }
            [[nodiscard]] inline auto GetSize() const noexcept -> const UVector2 { return m_size; }

            [[nodiscard]] auto GetSizeFromCoordinates(const TextureCoordinatePair& textureCoordinates) const noexcept -> UVector2;
            [[nodiscard]] auto GetWidthFromCoordinates(const TextureCoordinatePair& textureCoordinates) const noexcept -> u32;
            [[nodiscard]] auto GetHeightFromCoordinates(const TextureCoordinatePair& textureCoordinates) const noexcept -> u32;

        private:
            [[nodiscard]] auto LoadFromImageFile(const StringView filepath, const Sampler& sampler) -> Status;
            auto SetupParameters(const InternalComponentFormat internalComponentFormat, const PixelFormat pixelFormat, const ubyte* const data, const Sampler& sampler) -> void;
        };
    }
}

#endif
