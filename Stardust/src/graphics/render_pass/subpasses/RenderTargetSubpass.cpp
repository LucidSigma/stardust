#include "stardust/graphics/render_pass/subpasses/RenderTargetSubpass.h"

#include <variant>

#include "stardust/utility/Utility.h"

namespace stardust
{
    namespace graphics
    {
        RenderTargetSubpass::RenderTargetSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto RenderTargetSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_renderTarget = createInfo.renderTarget;
        }

        auto RenderTargetSubpass::Begin(Renderer& renderer) const -> void
        {
            std::visit(
                utility::VariantOverloader{
                    [&renderer](const ObserverPointer<const Framebuffer> framebuffer)
                    {
                        renderer.SetRenderTarget(*framebuffer);
                    },
                    [&renderer](const DefaultFramebufferType)
                    {
                        renderer.SetRenderTarget(DefaultFramebuffer);
                    },
                },
                m_renderTarget
            );
        }
    }
}
