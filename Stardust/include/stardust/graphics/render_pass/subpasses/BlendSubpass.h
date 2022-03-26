#pragma once
#ifndef STARDUST_BLEND_SUBPASS_H
#define STARDUST_BLEND_SUBPASS_H

#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/Blending.h"

namespace stardust
{
    namespace graphics
    {
        class BlendSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                BlendMode blendMode = blend_modes::Alpha;
            };

        private:
            BlendMode m_blendMode = blend_modes::Alpha;

        public:
            BlendSubpass() = default;
            BlendSubpass(const CreateInfo& createInfo);
            virtual ~BlendSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer& renderer) const -> void override;
        };
    }
}

#endif
