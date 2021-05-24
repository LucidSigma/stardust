#pragma once
#ifndef STARDUST_STATE_MACHINE_H
#define STARDUST_STATE_MACHINE_H

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/ai/state_machine/State.h"

#include <concepts>
#include <type_traits>
#include <utility>

namespace stardust
{
    namespace ai
    {
        class StateMachine
        {
        private:
            HashMap<String, UniquePtr<State>> m_states{ };
            ObserverPtr<State> m_currentState = nullptr;
            
        public:
            StateMachine() = default;
            ~StateMachine() noexcept = default;

            void FixedUpdate(const f32 fixedDeltaTime);
            void Update(const f32 deltaTime);
            void LateUpdate(const f32 deltaTime);

            void SetState(const String& name);

            template <std::derived_from<State> T, typename... Args, typename = std::enable_if_t<std::is_constructible_v<T, StateMachine, const String, Args...>>>
            void RegisterState(const String& name, Args&&... args)
            {
                m_states[name] = std::make_unique<T>(*this, name, std::forward<Args>(args)...);
            }

            void DeregisterState(const String& name);

            [[nodiscard]] inline bool IsCurrentStateValid() const noexcept { return m_currentState != nullptr; }
            [[nodiscard]] inline bool HasState(const String& name) const noexcept { return m_states.contains(name); }
        };
    }
}

#endif