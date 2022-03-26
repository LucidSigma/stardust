#pragma once
#ifndef STARDUST_SUBPASS_H
#define STARDUST_SUBPASS_H

#include "stardust/graphics/renderer/Renderer.h"

namespace stardust
{
    namespace graphics
    {
        class Subpass
        {
        public:
            Subpass() = default;
            virtual ~Subpass() noexcept = default;

            virtual auto Begin(Renderer& renderer) const -> void = 0;
        };
    }
}

#endif
