#pragma once
#ifndef STARDUST_LISTENER_H
#define STARDUST_LISTENER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/data/MathTypes.h"

namespace stardust
{
	class Listener
		: private INoncopyable
	{
	private:
		class SoundSystem* m_soundSystem = nullptr;

		Vec3 m_position{ 0.0f, 0.0f, 0.0f };
		Vec3 m_lookAt{ 0.0f, 0.0f, 0.0f };
		Vec3 m_upVector{ 0.0f, 1.0f, 0.0f };

		Vec3 m_velocity{ 0.0f, 0.0f, 0.0f };

	public:
		Listener(class SoundSystem& soundSystem);
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