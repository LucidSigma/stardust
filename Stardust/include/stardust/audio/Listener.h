#pragma once
#ifndef STARDUST_LISTENER_H
#define STARDUST_LISTENER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/data/MathTypes.h"
#include "stardust/math/Math.h"

namespace stardust
{
    class Listener
        : private INoncopyable
    {
    private:
        class SoundSystem* m_soundSystem = nullptr;

        Vec3 m_position = Vec3Zero;
        Vec3 m_lookAt = Vec3Zero;
        Vec3 m_upVector = Vec3Up;

        Vec3 m_velocity = Vec3Zero;

    public:
        explicit Listener(class SoundSystem& soundSystem);
        Listener(Listener&& other) noexcept;
        Listener& operator =(Listener&& other) noexcept;
        ~Listener() noexcept = default;

        void Reset() noexcept;

        inline const Vec3& GetPosition() const noexcept { return m_position; }
        void SetPosition(const Vec2& position) noexcept;
        void SetPosition(const Vec3& position) noexcept;

        inline const Vec3& GetLookAt() const noexcept { return m_lookAt; }
        void SetLookAt(const Vec3& lookAt) noexcept;

        inline const Vec3& GeUpVector() const noexcept { return m_upVector; }
        void SetUpVector(const Vec3& upVector) noexcept;

        inline const Vec3& GetVelocity() const noexcept { return m_velocity; }
        void SetVelocity(const Vec3& velocity) noexcept;
    };
}

#endif