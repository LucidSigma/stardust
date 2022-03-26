#include "stardust/graphics/render_pass/subpasses/BlendSubpass.h"

namespace stardust
{
    namespace graphics
    {
        BlendSubpass::BlendSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto BlendSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_blendMode = createInfo.blendMode;
        }

        auto BlendSubpass::Begin(Renderer& renderer) const -> void
        {
            renderer.SetBlendMode(m_blendMode);
        }
    }
}
