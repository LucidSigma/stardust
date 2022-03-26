#pragma once
#ifndef SUBPASS_TEMPLATE_H
#define SUBPASS_TEMPLATE_H

#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"

class SubpassTemplate final
    : public sd::gfx::Subpass
{
public:
    struct CreateInfo final
    {

    };

private:


public:
    SubpassTemplate() = default;
    SubpassTemplate(const CreateInfo& createInfo);
    virtual ~SubpassTemplate() noexcept override = default;

    auto Initialise(const CreateInfo& createInfo) -> void;

    auto Begin(Renderer& renderer) const -> void override;
};

#endif
