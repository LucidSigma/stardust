#pragma once
#ifndef STARDUST_RENDER_TARGET_SUBPASS_H
#define STARDUST_RENDER_TARGET_SUBPASS_H

#include "stardust/graphics/framebuffer/Framebuffer.h"
#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace graphics
    {
        class RenderTargetSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                Variant<ObserverPointer<const Framebuffer>, DefaultFramebufferType> renderTarget = DefaultFramebuffer;
            };

        private:
            Variant<ObserverPointer<const Framebuffer>, DefaultFramebufferType> m_renderTarget = DefaultFramebuffer;

        public:
            RenderTargetSubpass() = default;
            RenderTargetSubpass(const CreateInfo& createInfo);
            virtual ~RenderTargetSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer& renderer) const -> void override;
        };
    }
}

#endif
