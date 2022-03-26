#include "stardust/audio/listener/Listener.h"

#include <utility>

#include "stardust/audio/SoundSystem.h"

namespace stardust
{
    namespace audio
    {
        Listener::Listener(Listener&& other) noexcept
            : m_soundSystem(nullptr), m_position(Vector3Zero), m_lookAt(Vector3Forward), m_rotation(0.0f), m_upVector(Vector3Up), m_velocity(Vector3Zero)
        {
            std::swap(m_soundSystem, other.m_soundSystem);

            std::swap(m_position, other.m_position);
            std::swap(m_lookAt, other.m_lookAt);

            std::swap(m_rotation, other.m_rotation);
            std::swap(m_upVector, other.m_upVector);

            std::swap(m_velocity, other.m_velocity);
        }

        auto Listener::operator =(Listener&& other) noexcept -> Listener&
        {
            m_soundSystem = std::exchange(other.m_soundSystem, nullptr);

            m_position = std::exchange(other.m_position, Vector3Zero);
            m_lookAt = std::exchange(other.m_lookAt, Vector3Forward);

            m_rotation = std::exchange(other.m_rotation, 0.0f);
            m_upVector = std::exchange(other.m_upVector, Vector3Up);

            m_velocity = std::exchange(other.m_velocity, Vector3Zero);

            return *this;
        }

        auto Listener::Initialise(SoundSystem& soundSystem) -> void
        {
            m_soundSystem = &soundSystem;

            m_soundSystem->GetRawHandle().set3dListenerPosition(m_position.x, m_position.y, 0.0f);
            m_soundSystem->GetRawHandle().set3dListenerAt(m_lookAt.x, m_lookAt.y, m_lookAt.z);
            m_soundSystem->GetRawHandle().set3dListenerUp(m_upVector.x, m_upVector.y, m_upVector.z);
            m_soundSystem->GetRawHandle().set3dListenerVelocity(m_velocity.x, m_velocity.y, 0.0f);
        }

        auto Listener::Reset() noexcept -> void
        {
            m_position = Vector2Zero;
            m_rotation = 0.0f;

            m_lookAt = Vector3Forward;
            m_upVector = Vector3Up;

            m_velocity = Vector3Zero;

            m_soundSystem->GetRawHandle().set3dListenerPosition(m_position.x, m_position.y, 0.0f);
            m_soundSystem->GetRawHandle().set3dListenerAt(m_lookAt.x, m_lookAt.y, m_lookAt.z);
            m_soundSystem->GetRawHandle().set3dListenerUp(m_upVector.x, m_upVector.y, m_upVector.z);
            m_soundSystem->GetRawHandle().set3dListenerVelocity(m_velocity.x, m_velocity.y, 0.0f);
        }

        auto Listener::SetPosition(const f32 x, const f32 y) noexcept -> void
        {
            m_position = Vector2{ x, y };
            m_soundSystem->GetRawHandle().set3dListenerPosition(x, y, 0.0f);
        }

        auto Listener::SetPosition(const Vector2 position) noexcept -> void
        {
            m_position = position;
            m_soundSystem->GetRawHandle().set3dListenerPosition(m_position.x, m_position.y, 0.0f);
        }

        auto Listener::SetRotation(const f32 rotation) noexcept -> void
        {
            m_rotation = rotation;
            m_upVector = glm::rotateZ(Vector3Up, -glm::radians(rotation));

            m_soundSystem->GetRawHandle().set3dListenerUp(m_upVector.x, m_upVector.y, m_upVector.z);
        }

        auto Listener::SetVelocity(const Vector2 velocity) noexcept -> void
        {
            m_velocity = velocity;
            m_soundSystem->GetRawHandle().set3dListenerVelocity(m_velocity.x, m_velocity.y, 0.0f);
        }
    }
}
