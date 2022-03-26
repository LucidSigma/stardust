#include "stardust/graphics/render_pass/subpasses/ScissorSubpass.h"

namespace stardust
{
    namespace graphics
    {
        ScissorSubpass::ScissorSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto ScissorSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_enableScissorTest = createInfo.enableScissorTest;
            m_scissorArea = createInfo.scissorArea;
        }

        auto ScissorSubpass::Begin(Renderer& renderer) const -> void
        {
            if (m_enableScissorTest.has_value())
            {
                renderer.EnableScissorTest(m_enableScissorTest.value());
            }

            if (m_scissorArea.has_value())
            {
                renderer.SetScissorArea(m_scissorArea.value());
            }
        }
    }
}
