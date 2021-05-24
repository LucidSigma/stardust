#include "stardust/ai/state_machine/State.h"

#include "stardust/ai/state_machine/StateMachine.h"

namespace stardust
{
    namespace ai
    {
        State::State(StateMachine& owner, const String& name)
            : m_owner(owner), m_name(name)
        { }
    }
}