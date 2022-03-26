#pragma once
#ifndef STARDUST_FRAMEBUFFER_H
#define STARDUST_FRAMEBUFFER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <ANGLE/GLES3/gl3.h>

#include "stardust/graphics/framebuffer/RenderBuffer.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace graphics
    {
        class Framebuffer final
            : private INoncopyable
        {
        public:
            using ID = GLuint;

            enum class AttachmentType
                : GLenum
            {
                Colour0 = GL_COLOR_ATTACHMENT0,
                Colour1 = GL_COLOR_ATTACHMENT1,
                Colour2 = GL_COLOR_ATTACHMENT2,
                Colour3 = GL_COLOR_ATTACHMENT3,
                DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
            };

        private:
            static constexpr ID s_InvalidID = 0u;
            static constexpr ID s_DefaultID = 0u;

            ID m_id = s_InvalidID;

            HashMap<AttachmentType, Variant<ObserverPointer<const Texture>, ObserverPointer<const RenderBuffer>>> m_attachments{ };
            bool m_isComplete = false;

        public:
            [[nodiscard]] static constexpr auto InvalidID() noexcept -> ID { return s_InvalidID; }
            [[nodiscard]] static constexpr auto DefaultID() noexcept -> ID { return s_DefaultID; }
            static auto BindDefault() -> void;

            Framebuffer() = default;
            Framebuffer(Framebuffer&& other) noexcept;
            auto operator =(Framebuffer&& other) noexcept -> Framebuffer&;
            ~Framebuffer() noexcept;

            auto Initialise() -> void;
            auto Destroy() noexcept -> void;

            auto Bind() const noexcept -> void;
            auto Unbind() const noexcept -> void;

            auto BindAttachment(const AttachmentType attachmentType, const Texture::BindingIndex textureBindingIndex = 0u) const -> void;
            auto UnbindAttachment(const AttachmentType attachmentType) const noexcept -> void;
            auto UnbindAllAttachments() const noexcept -> void;

            auto Attach(const AttachmentType attachmentType, const Texture& texture) -> void;
            auto Attach(const AttachmentType attachmentType, const RenderBuffer& renderBuffer) -> void;

            [[nodiscard]] auto HasAttachment(const AttachmentType attachmentType) const -> bool;
            [[nodiscard]] auto GetAttachment(const AttachmentType attachmentType) const -> const Variant<ObserverPointer<const Texture>, ObserverPointer<const RenderBuffer>>&;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_id != s_InvalidID && m_isComplete; }
            [[nodiscard]] inline auto IsComplete() const noexcept -> bool { return m_isComplete; }

            [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }

        private:
            [[nodiscard]] auto UpdateCompleteness() -> void;
        };

        struct DefaultFramebufferType final
        { };

        constexpr DefaultFramebufferType DefaultFramebuffer{ };
    }
}

#endif
