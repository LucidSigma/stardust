#include "stardust/audio/Listener.h"

#include <utility>

#include "stardust/audio/SoundSystem.h"

namespace stardust
{
    namespace audio
    {
        Listener::Listener(SoundSystem& soundSystem)
            : m_soundSystem(&soundSystem)
        { }

        Listener::Listener(Listener&& other) noexcept
            : m_soundSystem(nullptr), m_position(Vec3Zero), m_lookAt(Vec3Zero), m_upVector(Vec3Up)
        {
            std::swap(m_soundSystem, other.m_soundSystem);

            std::swap(m_position, other.m_position);
            std::swap(m_lookAt, other.m_lookAt);
            std::swap(m_upVector, other.m_upVector);
        }

        Listener& Listener::operator =(Listener&& other) noexcept
        {
            m_soundSystem = std::exchange(other.m_soundSystem, nullptr);

            m_position = std::exchange(other.m_position, Vec3Zero);
            m_lookAt = std::exchange(other.m_lookAt, Vec3Zero);
            m_upVector = std::exchange(other.m_upVector, Vec3Up);

            return *this;
        }

        void Listener::Reset() noexcept
        {
            m_position = Vec3Zero;
            m_lookAt = Vec3Zero;
            m_upVector = Vec3Up;
            m_velocity = Vec3Zero;
        }

        void Listener::SetPosition(const Vec2& position) noexcept
        {
            m_position = Vec3{ position, m_position.z };
            m_soundSystem->GetRawHandle().set3dListenerPosition(position.x, position.y, m_position.z);
        }

        void Listener::SetPosition(const Vec3& position) noexcept
        {
            m_position = position;
            m_soundSystem->GetRawHandle().set3dListenerPosition(position.x, position.y, position.z);
        }

        void Listener::SetLookAt(const Vec3& lookAt) noexcept
        {
            m_lookAt = lookAt;
            m_soundSystem->GetRawHandle().set3dListenerAt(lookAt.x, lookAt.y, lookAt.z);
        }

        void Listener::SetUpVector(const Vec3& upVector) noexcept
        {
            m_upVector = upVector;
            m_soundSystem->GetRawHandle().set3dListenerUp(upVector.x, upVector.y, upVector.z);
        }

        void Listener::SetVelocity(const Vec3& velocity) noexcept
        {
            m_velocity = velocity;
            m_soundSystem->GetRawHandle().set3dListenerVelocity(velocity.x, velocity.y, velocity.z);
        }
    }
}