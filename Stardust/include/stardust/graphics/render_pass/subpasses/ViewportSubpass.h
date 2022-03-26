#pragma once
#ifndef STARDUST_VIEWPORT_SUBPASS_H
#define STARDUST_VIEWPORT_SUBPASS_H

#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/RenderArea.h"

namespace stardust
{
    namespace graphics
    {
        class ViewportSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                RenderArea viewport{ };
            };

        private:
            RenderArea m_viewport{ };

        public:
            ViewportSubpass() = default;
            ViewportSubpass(const CreateInfo& createInfo);
            virtual ~ViewportSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer& renderer) const -> void override;
        };
    }
}

#endif
