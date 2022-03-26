#include "stardust/graphics/framebuffer/Framebuffer.h"

#include <utility>
#include <variant>

#include "stardust/utility/Utility.h"

namespace stardust
{
    namespace graphics
    {
        auto Framebuffer::BindDefault() -> void
        {
            glBindFramebuffer(GL_FRAMEBUFFER, s_DefaultID);
        }

        Framebuffer::Framebuffer(Framebuffer&& other) noexcept
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_attachments, other.m_attachments);
            std::swap(m_isComplete, other.m_isComplete);
        }

        auto Framebuffer::operator =(Framebuffer&& other) noexcept -> Framebuffer&
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_attachments, other.m_attachments);
            std::swap(m_isComplete, other.m_isComplete);

            return *this;
        }

        Framebuffer::~Framebuffer() noexcept
        {
            Destroy();
        }

        auto Framebuffer::Initialise() -> void
        {
            glGenFramebuffers(1, &m_id);
        }

        auto Framebuffer::Destroy() noexcept -> void
        {
            if (m_id != s_InvalidID)
            {
                Unbind();

                glDeleteFramebuffers(1, &m_id);

                m_id = s_InvalidID;
                m_attachments.clear();
                m_isComplete = false;
            }
        }

        auto Framebuffer::Bind() const noexcept -> void
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        }

        auto Framebuffer::Unbind() const noexcept -> void
        {
            glBindFramebuffer(GL_FRAMEBUFFER, s_InvalidID);
        }

        auto Framebuffer::BindAttachment(const AttachmentType attachmentType, const Texture::BindingIndex textureBindingIndex) const -> void
        {
            if (m_attachments.contains(attachmentType))
            {
                std::visit(
                    utility::VariantOverloader{
                        [textureBindingIndex](const ObserverPointer<const Texture> texture) {
                            texture->Bind(textureBindingIndex);
                        },
                        [](const ObserverPointer<const RenderBuffer> renderBuffer) {
                            renderBuffer->Bind();
                        },
                    },
                    m_attachments.at(attachmentType)
                );
            }
        }

        auto Framebuffer::UnbindAttachment(const AttachmentType attachmentType) const noexcept -> void
        {
            if (m_attachments.contains(attachmentType))
            {
                std::visit(
                    utility::VariantOverloader{
                        [](const ObserverPointer<const Texture> texture) {
                            texture->Unbind();
                        },
                        [](const ObserverPointer<const RenderBuffer> renderBuffer) {
                            renderBuffer->Unbind();
                        },
                    },
                    m_attachments.at(attachmentType)
                );
            }
        }

        auto Framebuffer::UnbindAllAttachments() const noexcept -> void
        {
            for (const auto& [attachmentType, attachment] : m_attachments)
            {
                std::visit(
                    utility::VariantOverloader{
                        [](const ObserverPointer<const Texture> texture) {
                            texture->Unbind();
                        },
                        [](const ObserverPointer<const RenderBuffer> renderBuffer) {
                            renderBuffer->Unbind();
                        },
                    },
                    attachment
                );
            }
        }

        auto Framebuffer::Attach(const AttachmentType attachmentType, const Texture& texture) -> void
        {
            texture.Bind();
            Bind();

            glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attachmentType), GL_TEXTURE_2D, texture.GetID(), 0);

            m_attachments[attachmentType] = &texture;
            UpdateCompleteness();

            Unbind();
            texture.Unbind();
        }

        auto Framebuffer::Attach(const AttachmentType attachmentType, const RenderBuffer& renderBuffer) -> void
        {
            renderBuffer.Bind();
            Bind();

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(attachmentType), GL_RENDERBUFFER, renderBuffer.GetID());

            m_attachments[attachmentType] = &renderBuffer;
            UpdateCompleteness();

            Unbind();
            renderBuffer.Unbind();
        }

        [[nodiscard]] auto Framebuffer::HasAttachment(const AttachmentType attachmentType) const -> bool
        {
            return m_attachments.contains(attachmentType);
        }

        [[nodiscard]] auto Framebuffer::GetAttachment(const AttachmentType attachmentType) const -> const Variant<ObserverPointer<const Texture>, ObserverPointer<const RenderBuffer>>&
        {
            return m_attachments.at(attachmentType);
        }

        auto Framebuffer::UpdateCompleteness() -> void
        {
            m_isComplete = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }
    }
}
