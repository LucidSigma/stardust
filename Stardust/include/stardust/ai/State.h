#pragma once
#ifndef STARDUST_STATE_H
#define STARDUST_STATE_H

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace ai
    {
        class State
        {
        private:
            class StateMachine& m_owner;

            String m_name;

        public:
            State(class StateMachine& owner, const StringView name);
            virtual ~State() noexcept = default;

            virtual auto OnEnter() -> void = 0;
            inline virtual auto OnExit() -> void { }

            inline virtual auto FixedUpdate(const f32 fixedDeltaTime) -> void { }
            inline virtual auto PreUpdate(const f32 deltaTime) -> void { }
            inline virtual auto Update(const f32 deltaTime) -> void { }
            inline virtual auto PostUpdate(const f32 deltaTime) -> void { }

            [[nodiscard]] inline auto GetName() const noexcept -> const String& { return m_name; }
        };
    }
}

#endif
