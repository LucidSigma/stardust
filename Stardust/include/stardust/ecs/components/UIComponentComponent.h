#pragma once
#ifndef STARDUST_UI_COMPONENT_COMPONENT_H
#define STARDUST_UI_COMPONENT_COMPONENT_H

#include "stardust/types/Pointers.h"
#include "stardust/ui/UIComponent.h"

namespace stardust
{
    namespace components
    {
        struct UIComponent final
        {
            ObserverPointer<ui::Component> component = nullptr;
        };
    }
}

#endif
