#pragma once
#ifndef STARDUST_STENCIL_SUBPASS_H
#define STARDUST_STENCIL_SUBPASS_H

#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/Graphics.h"
#include "stardust/types/Containers.h"

namespace stardust
{
    namespace graphics
    {
        class StencilSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                Optional<bool> enableStencilTest = None;

                Optional<StencilParameters> stencilParameters = None;
                Optional<StencilOperations> stencilOperations = None;
            };

        private:
            Optional<bool> m_enableStencilTest = None;

            Optional<StencilParameters> m_stencilParameters = None;
            Optional<StencilOperations> m_stencilOperations = None;

        public:
            StencilSubpass() = default;
            StencilSubpass(const CreateInfo& createInfo);
            virtual ~StencilSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer& renderer) const -> void override;
        };
    }
}

#endif
