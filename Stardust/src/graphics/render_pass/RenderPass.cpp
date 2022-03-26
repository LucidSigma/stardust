#include "stardust/graphics/render_pass/RenderPass.h"

namespace stardust
{
    namespace graphics
    {
        RenderPass::RenderPass(Renderer& renderer)
        {
            Initialise(renderer);
        }

        auto RenderPass::Initialise(Renderer& renderer) -> void
        {
            m_renderer = &renderer;
        }

        auto RenderPass::Begin() const -> void
        {
            for (const auto& subpass : m_subpasses)
            {
                subpass->Begin(*m_renderer);
            }
        }
    }
}
