#pragma once
#ifndef STARDUST_TAG_COMPONENT_H
#define STARDUST_TAG_COMPONENT_H

#include "stardust/types/Containers.h"

namespace stardust
{
    namespace components
    {
        struct Tag final
        {
            HashSet<String> tags{ };
        };
    }
}

#endif
