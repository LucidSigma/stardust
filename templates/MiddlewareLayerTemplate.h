#pragma once
#ifndef MIDDLEWARE_LAYER_TEMPLATE_H
#define MIDDLEWARE_LAYER_TEMPLATE_H

#include <stardust/Stardust.h>

class MiddlewareLayerTemplate
    : public sd::Layer
{
protected:
    MiddlewareLayerTemplate(sd::Scene& scene);
    virtual ~MiddlewareLayerTemplate() noexcept override = default;
};

#endif
