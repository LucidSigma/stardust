#include "stardust/graphics/render_pass/subpasses/ColourSubpass.h"

namespace stardust
{
    namespace graphics
    {
        ColourSubpass::ColourSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto ColourSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_clearColour = createInfo.clearColour;
        }

        auto ColourSubpass::Begin(Renderer& renderer) const -> void
        {
            renderer.SetClearColour(m_clearColour);
        }
    }
}
