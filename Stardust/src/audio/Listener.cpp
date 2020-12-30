#include "stardust/audio/Listener.h"

#include <utility>

#include "stardust/audio/SoundSystem.h"

namespace stardust
{
	Listener::Listener(SoundSystem& soundSystem)
		: m_soundSystem(&soundSystem)
	{ }

	Listener::Listener(Listener&& other) noexcept
		: m_soundSystem(nullptr), m_position(Vec3{ 0.0f, 0.0f, 0.0f }), m_lookAt(Vec3{ 0.0f, 0.0f, 0.0f }), m_upVector(Vec3{ 0.0f, 1.0f, 0.0f })
	{
		std::swap(m_soundSystem, other.m_soundSystem);

		std::swap(m_position, other.m_position);
		std::swap(m_lookAt, other.m_lookAt);
		std::swap(m_upVector, other.m_upVector);
	}

	Listener& Listener::operator =(Listener&& other) noexcept
	{
		m_soundSystem = std::exchange(other.m_soundSystem, nullptr);

		m_position = std::exchange(other.m_position, Vec3{ 0.0f, 0.0f, 0.0f });
		m_lookAt = std::exchange(other.m_lookAt, Vec3{ 0.0f, 0.0f, 0.0f });
		m_upVector = std::exchange(other.m_upVector, Vec3{ 0.0f, 1.0f, 0.0f });

		return *this;
	}

	void Listener::Reset() noexcept
	{
		m_position = Vec3{ 0.0f, 0.0f, 0.0f };
		m_lookAt = Vec3{ 0.0f, 0.0f, 0.0f };
		m_upVector = Vec3{ 0.0f, 1.0f, 0.0f };
		m_velocity = Vec3{ 0.0f, 0.0f, 0.0f };
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