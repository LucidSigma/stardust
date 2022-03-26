#pragma once
#ifndef STARDUST_STATE_MACHINE_H
#define STARDUST_STATE_MACHINE_H

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

#include "stardust/ai/State.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace ai
    {
        class StateMachine final
        {
        private:
            HashMap<String, UniquePointer<State>> m_states{ };
            ObserverPointer<State> m_currentState = nullptr;

        public:
            auto FixedUpdate(const f32 fixedDeltaTime) -> void;
            auto PreUpdate(const f32 deltaTime) -> void;
            auto Update(const f32 deltaTime) -> void;
            auto PostUpdate(const f32 deltaTime) -> void;

            auto SetState(const String& name) -> void;

            template <std::derived_from<State> S, typename... Args>
                requires std::is_constructible_v<S, StateMachine, const String&, Args...>
            auto RegisterState(const String& name, Args&&... args) -> void
            {
                m_states[name] = std::make_unique<S>(*this, name, std::forward<Args>(args)...);
            }

            auto DeregisterState(const String& name) -> void;

            [[nodiscard]] inline auto IsCurrentStateValid() const noexcept -> bool { return m_currentState != nullptr; }
            [[nodiscard]] inline auto GetCurrentState() const noexcept -> ObserverPointer<const State> { return m_currentState; }
            [[nodiscard]] inline auto HasStateRegistered(const String& name) const noexcept -> bool { return m_states.contains(name); }
        };
    }
}

#endif
