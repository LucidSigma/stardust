#pragma once
#ifndef STARDUST_STATE_BEHAVIOUR_COMPONENT_H
#define STARDUST_STATE_BEHAVIOUR_COMPONENT_H

#include "stardust/ai/state_machine/StateMachine.h"
#include "stardust/data/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct StateBehaviour
        {
            ObserverPtr<ai::StateMachine> stateMachine;

            StateBehaviour()
                : stateMachine(nullptr)
            { }

            StateBehaviour(ai::StateMachine& stateMachine)
                : stateMachine(&stateMachine)
            { }

            ~StateBehaviour() noexcept = default;
        };
    }
}

#endif