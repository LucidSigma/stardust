#include "stardust/graphics/render_pass/subpasses/ClearSubpass.h"

namespace stardust
{
    namespace graphics
    {
        ClearSubpass::ClearSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto ClearSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_clearColourBuffer = createInfo.clearColourBuffer;
            m_clearDepthBuffer = createInfo.clearDepthBuffer;
            m_clearStencilBuffer = createInfo.clearStencilBuffer;
        }

        auto ClearSubpass::Begin(Renderer& renderer) const -> void
        {
            if (m_clearColourBuffer)
            {
                renderer.ClearColour();
            }

            if (m_clearDepthBuffer)
            {
                renderer.ClearDepthBuffer();
            }

            if (m_clearStencilBuffer)
            {
                renderer.ClearStencilBuffer();
            }
        }
    }
}
