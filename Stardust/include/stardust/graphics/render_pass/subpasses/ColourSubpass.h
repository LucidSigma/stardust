#pragma once
#ifndef STARDUST_COLOUR_SUBPASS_H
#define STARDUST_COLOUR_SUBPASS_H

#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"

namespace stardust
{
    namespace graphics
    {
        class ColourSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                Colour clearColour = colours::Black;
            };

        private:
            Colour m_clearColour = colours::Black;

        public:
            ColourSubpass() = default;
            ColourSubpass(const CreateInfo& createInfo);
            virtual ~ColourSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer& renderer) const -> void override;
        };
    }
}

#endif
