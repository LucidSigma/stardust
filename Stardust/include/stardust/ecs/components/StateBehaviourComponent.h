#pragma once
#ifndef STARDUST_STATE_BEHAVIOUR_COMPONENT_H
#define STARDUST_STATE_BEHAVIOUR_COMPONENT_H

#include "stardust/ai/StateMachine.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct StateBehaviour final
        {
            ObserverPointer<ai::StateMachine> stateMachine = nullptr;
        };
    }
}

#endif
