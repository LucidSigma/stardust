#include "stardust/ai/state_machine/StateMachine.h"

#include <iterator>

namespace stardust
{
    namespace ai
    {
        void StateMachine::FixedUpdate(const f32 fixedDeltaTime)
        {
            if (m_currentState != nullptr) [[likely]]
            {
                m_currentState->FixedUpdate(fixedDeltaTime);
            }
        }

        void StateMachine::Update(const f32 deltaTime)
        {
            if (m_currentState != nullptr) [[likely]]
            {
                m_currentState->Update(deltaTime);
            }
        }

        void StateMachine::LateUpdate(const f32 deltaTime)
        {
            if (m_currentState != nullptr) [[likely]]
            {
                m_currentState->LateUpdate(deltaTime);
            }
        }

        void StateMachine::SetState(const String& name)
        {
            if (m_currentState != nullptr) [[likely]]
            {
                m_currentState->OnExit();
            }

            if (const auto stateLocation = m_states.find(name);
                stateLocation != std::cend(m_states))
            {
                m_currentState = stateLocation->second.get();
                m_currentState->OnEnter();
            }
            else
            {
                m_currentState = nullptr;
            }
        }

        void StateMachine::DeregisterState(const String& name)
        {
            m_states.erase(name);
        }
    }
}