#include "stardust/graphics/render_pass/subpasses/StencilSubpass.h"

namespace stardust
{
    namespace graphics
    {
        StencilSubpass::StencilSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto StencilSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_enableStencilTest = createInfo.enableStencilTest;

            m_stencilParameters = createInfo.stencilParameters;
            m_stencilOperations = createInfo.stencilOperations;
        }

        auto StencilSubpass::Begin(Renderer& renderer) const -> void
        {
            if (m_stencilParameters.has_value())
            {
                renderer.SetStencilParameters(m_stencilParameters.value());
            }

            if (m_stencilOperations.has_value())
            {
                renderer.SetStencilOperations(m_stencilOperations.value());
            }

            if (m_enableStencilTest.has_value())
            {
                renderer.EnableStencilTest(m_enableStencilTest.value());
            }
        }
    }
}
