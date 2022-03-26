#pragma once
#ifndef STARDUST_DEPTH_SUBPASS_H
#define STARDUST_DEPTH_SUBPASS_H

#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/Graphics.h"
#include "stardust/types/Containers.h"

namespace stardust
{
    namespace graphics
    {
        class DepthSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                Optional<bool> enableDepthTest = None;
                Optional<DepthFunction> depthFunction = None;
            };

        private:
            Optional<bool> m_enableDepthTest = None;
            Optional<DepthFunction> m_depthFunction = None;

        public:
            DepthSubpass() = default;
            DepthSubpass(const CreateInfo& createInfo);
            virtual ~DepthSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer& renderer) const -> void override;
        };
    }
}

#endif
