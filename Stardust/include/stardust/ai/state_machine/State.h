#pragma once
#ifndef STARDUST_STATE_H
#define STARDUST_STATE_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        class State
        {
        protected:
            class StateMachine& m_owner;

            String m_name;

        public:
            explicit State(class StateMachine& owner, const String& name);
            ~State() noexcept = default;

            virtual void OnEnter() = 0;
            virtual void OnExit() = 0;

            virtual void FixedUpdate(const f32 fixedDeltaTime) { }
            virtual void Update(const f32 deltaTime) = 0;
            virtual void LateUpdate(const f32 deltaTime) { }

            [[nodiscard]] inline const String& GetName() const noexcept { return m_name; }
        };
    }
}

#endif