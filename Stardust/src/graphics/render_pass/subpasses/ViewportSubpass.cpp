#include "stardust/graphics/render_pass/subpasses/ViewportSubpass.h"

namespace stardust
{
    namespace graphics
    {
        ViewportSubpass::ViewportSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto ViewportSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_viewport = createInfo.viewport;
        }

        auto ViewportSubpass::Begin(Renderer& renderer) const -> void
        {
            renderer.SetViewport(m_viewport);
        }
    }
}
