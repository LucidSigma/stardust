#pragma once
#ifndef STARDUST_SCISSOR_SUBPASS_H
#define STARDUST_SCISSOR_SUBPASS_H

#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/RenderArea.h"
#include "stardust/types/Containers.h"

namespace stardust
{
    namespace graphics
    {
        class ScissorSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                Optional<bool> enableScissorTest = None;
                Optional<RenderArea> scissorArea = None;
            };

        private:
            Optional<bool> m_enableScissorTest = None;
            Optional<RenderArea> m_scissorArea = None;

        public:
            ScissorSubpass() = default;
            ScissorSubpass(const CreateInfo& createInfo);
            virtual ~ScissorSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer& renderer) const -> void override;
        };
    }
}

#endif
