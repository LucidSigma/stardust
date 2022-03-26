#pragma once
#ifndef STARDUST_PIPELINE_SUBPASS_H
#define STARDUST_PIPELINE_SUBPASS_H

#include "stardust/graphics/pipeline/Pipeline.h"
#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace graphics
    {
        class PipelineSubpass final
            : public Subpass
        {
        public:
            struct CreateInfo final
            {
                ObserverPointer<const Pipeline> pipeline = nullptr;
            };

        private:
            ObserverPointer<const Pipeline> m_pipeline = nullptr;

        public:
            PipelineSubpass() = default;
            PipelineSubpass(const CreateInfo& createInfo);
            virtual ~PipelineSubpass() noexcept override = default;

            auto Initialise(const CreateInfo& createInfo) -> void;

            auto Begin(Renderer&) const -> void override;
        };
    }
}

#endif
