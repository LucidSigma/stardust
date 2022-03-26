#include "stardust/ai/State.h"

#include "stardust/ai/StateMachine.h"

namespace stardust
{
    namespace ai
    {
        State::State(StateMachine& owner, const StringView name)
            : m_owner(owner), m_name(name)
        { }
    }
}
