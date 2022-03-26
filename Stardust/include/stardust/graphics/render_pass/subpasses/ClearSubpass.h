#pragma once
#ifndef STARDUST_CLEAR_SUBPASS_H
#define STARDUST_CLEAR_SUBPASS_H

#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"

namespace stardust
{
    namespace graphics
    {
        class ClearSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                bool clearColourBuffer = true;
                bool clearDepthBuffer = true;
                bool clearStencilBuffer = true;
            };

        private:
            bool m_clearColourBuffer = true;
            bool m_clearDepthBuffer = true;
            bool m_clearStencilBuffer = true;

        public:
            ClearSubpass() = default;
            ClearSubpass(const CreateInfo& createInfo);
            virtual ~ClearSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer& renderer) const -> void override;
        };
    }
}

#endif
