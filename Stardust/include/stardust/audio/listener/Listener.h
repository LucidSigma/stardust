#pragma once
#ifndef STARDUST_LISTENER_H
#define STARDUST_LISTENER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace audio
    {
        class Listener final
            : private INoncopyable
        {
        private:
            ObserverPointer<class SoundSystem> m_soundSystem = nullptr;

            Vector2 m_position = Vector2Zero;
            Vector3 m_lookAt = Vector3Forward;

            f32 m_rotation = 0.0f;
            Vector3 m_upVector = Vector3Up;

            Vector2 m_velocity = Vector2Zero;

        public:
            Listener() = default;
            Listener(Listener&& other) noexcept;
            auto operator =(Listener&& other) noexcept -> Listener&;
            ~Listener() noexcept = default;

            auto Initialise(class SoundSystem& soundSystem) -> void;
            auto Reset() noexcept -> void;

            [[nodiscard]] inline auto GetPosition() const noexcept -> Vector2 { return m_position; }
            auto SetPosition(const f32 x, const f32 y) noexcept -> void;
            auto SetPosition(const Vector2 position) noexcept -> void;

            [[nodiscard]] inline auto GetRotation() const noexcept -> f32 { return m_rotation; }
            auto SetRotation(const f32 rotation) noexcept -> void;

            [[nodiscard]] inline auto GetVelocity() const noexcept -> Vector2 { return m_velocity; }
            auto SetVelocity(const Vector2 velocity) noexcept -> void;

            [[nodiscard]] inline auto GetLookAt() const noexcept -> Vector3 { return m_lookAt; }
            [[nodiscard]] inline auto GetUpVector() const noexcept -> Vector3 { return m_upVector; }
        };
    }
}

#endif
