#include "stardust/graphics/render_pass/subpasses/PipelineSubpass.h"

namespace stardust
{
    namespace graphics
    {
        PipelineSubpass::PipelineSubpass(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        auto PipelineSubpass::Initialise(const CreateInfo& createInfo) -> void
        {
            m_pipeline = createInfo.pipeline;
        }

        auto PipelineSubpass::Begin(Renderer&) const -> void
        {
            m_pipeline->Use();
        }
    }
}
