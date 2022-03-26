#include "stardust/ai/StateMachine.h"

namespace stardust
{
    namespace ai
    {
        auto StateMachine::FixedUpdate(const f32 fixedDeltaTime) -> void
        {
            if (m_currentState != nullptr) [[unlikely]]
            {
                m_currentState->FixedUpdate(fixedDeltaTime);
            }
        }

        auto StateMachine::PreUpdate(const f32 deltaTime) -> void
        {
            if (m_currentState != nullptr) [[unlikely]]
            {
                m_currentState->PreUpdate(deltaTime);
            }
        }

        auto StateMachine::Update(const f32 deltaTime) -> void
        {
            if (m_currentState != nullptr) [[unlikely]]
            {
                m_currentState->Update(deltaTime);
            }
        }

        auto StateMachine::PostUpdate(const f32 deltaTime) -> void
        {
            if (m_currentState != nullptr) [[unlikely]]
            {
                m_currentState->PostUpdate(deltaTime);
            }
        }

        auto StateMachine::SetState(const String& name) -> void
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
        
        auto StateMachine::DeregisterState(const String& name) -> void
        {
            m_states.erase(name);
        }
    }
}
