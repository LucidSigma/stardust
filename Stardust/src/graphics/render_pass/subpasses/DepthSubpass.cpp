#include "stardust/graphics/render_pass/subpasses/DepthSubpass.h"

namespace stardust
{
    namespace graphics
    {
        DepthSubpass::DepthSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto DepthSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_enableDepthTest = createInfo.enableDepthTest;
            m_depthFunction = createInfo.depthFunction;
        }

        auto DepthSubpass::Begin(Renderer& renderer) const -> void
        {
            if (m_depthFunction.has_value())
            {
                renderer.SetDepthFunction(m_depthFunction.value());
            }

            if (m_enableDepthTest.has_value())
            {
                renderer.EnableDepthTest(m_enableDepthTest.value());
            }
        }
    }
}
