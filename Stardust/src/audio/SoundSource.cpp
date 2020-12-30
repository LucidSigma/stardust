#include "stardust/audio/SoundSource.h"

#include <algorithm>
#include <utility>

#include "stardust/audio/SoundSystem.h"

namespace stardust
{
	SoundSource::SoundSource(const SoLoud::handle handle, SoundSystem& soundSystem, const Vec3& position, const Vec3& velocity)
		: m_soundSystem(&soundSystem), m_handle(handle), m_position(position), m_velocity(velocity)
	{ }

	SoundSource::SoundSource(SoundSource&& other) noexcept
		: m_soundSystem(other.m_soundSystem), m_handle(other.m_handle),
		m_position(other.m_position), m_velocity(other.m_velocity),
		m_minDistance(other.m_minDistance), m_maxDistance(other.m_maxDistance),
		m_inaudibleBehaviour(other.m_inaudibleBehaviour)
	{
		other.m_soundSystem = nullptr;
		other.m_handle = 0u;

		other.m_position = Vec3{ 0.0f, 0.0f, 0.0f };
		other.m_velocity = Vec3{ 0.0f, 0.0f, 0.0f };

		other.m_minDistance = 1.0f;
		other.m_maxDistance = 1'000'000.0f;

		other.m_inaudibleBehaviour = InaudibleBehaviour::Pause;
	}

	SoundSource& SoundSource::operator=(SoundSource&& other) noexcept
	{
		m_soundSystem = std::exchange(other.m_soundSystem, nullptr);
		m_handle = std::exchange(other.m_handle, 0u);

		m_position = std::exchange(other.m_position, Vec3{ 0.0f, 0.0f, 0.0f });
		m_velocity = std::exchange(other.m_velocity, Vec3{ 0.0f, 0.0f, 0.0f });

		m_minDistance = std::exchange(other.m_minDistance, 1.0f);
		m_maxDistance = std::exchange(other.m_maxDistance, 1'000'000.0f);

		m_inaudibleBehaviour = std::exchange(other.m_inaudibleBehaviour, InaudibleBehaviour::Pause);

		return *this;
	}

	void SoundSource::Pause() const
	{
		m_soundSystem->GetRawHandle().setPause(m_handle, true);
	}

	void SoundSource::Resume() const
	{
		m_soundSystem->GetRawHandle().setPause(m_handle, false);
	}

	void SoundSource::Stop() const
	{
		m_soundSystem->GetRawHandle().stop(m_handle);
	}

	[[nodiscard]] bool SoundSource::IsPlaying() const
	{
		return m_soundSystem->GetRawHandle().isValidVoiceHandle(m_handle) && !IsPaused();
	}

	[[nodiscard]] bool SoundSource::IsPaused() const
	{
		return m_soundSystem->GetRawHandle().getPause(m_handle);
	}

	[[nodiscard]] bool SoundSource::IsStopped() const
	{
		return !m_soundSystem->GetRawHandle().isValidVoiceHandle(m_handle);
	}

	void SoundSource::SchedulePause(const f32 secondsDelay)
	{
		m_soundSystem->GetRawHandle().schedulePause(m_handle, secondsDelay);
	}

	void SoundSource::ScheduleStop(const f32 secondsDelay)
	{
		m_soundSystem->GetRawHandle().scheduleStop(m_handle, secondsDelay);
	}

	[[nodiscard]] bool SoundSource::IsLooping() const
	{
		return m_soundSystem->GetRawHandle().getLooping(m_handle);
	}

	void SoundSource::SetLooping(const bool isLooping)
	{
		m_soundSystem->GetRawHandle().setLooping(m_handle, isLooping);
	}

	[[nodiscard]] f32 SoundSource::GetLoopPoint() const
	{
		return static_cast<f32>(m_soundSystem->GetRawHandle().getLoopPoint(m_handle));
	}

	void SoundSource::SetLoopPoint(const f32 loopPoint)
	{
		m_soundSystem->GetRawHandle().setLoopPoint(m_handle, loopPoint);
	}

	[[nodiscard]] unsigned int SoundSource::GetLoopCount() const
	{
		return m_soundSystem->GetRawHandle().getLoopCount(m_handle);
	}

	[[nodiscard]] f64 SoundSource::GetStreamTime() const
	{
		return m_soundSystem->GetRawHandle().getStreamTime(m_handle);
	}

	void SoundSource::Seek(const f64 seconds)
	{
		m_soundSystem->GetRawHandle().seek(m_handle, seconds);
	}

	void SoundSource::Rewind()
	{
		m_soundSystem->GetRawHandle().seek(m_handle, 0.0);
	}

	[[nodiscard]] f32 SoundSource::GetVolume() const
	{
		return m_soundSystem->GetRawHandle().getVolume(m_handle);
	}

	[[nodiscard]] f32 SoundSource::GetOverallVolume() const
	{
		return m_soundSystem->GetRawHandle().getOverallVolume(m_handle);;
	}

	void SoundSource::SetVolume(const f32 volume)
	{
		m_soundSystem->GetRawHandle().setVolume(m_handle, volume);
	}

	void SoundSource::FadeVolume(const f32 toVolume, const f32 fadeTime)
	{
		m_soundSystem->GetRawHandle().fadeVolume(m_handle, toVolume, fadeTime);
	}

	void SoundSource::FadeRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fadeTime)
	{
		m_soundSystem->GetRawHandle().fadeRelativePlaySpeed(m_handle, toRelativePlaySpeed, fadeTime);
	}

	void SoundSource::FadePan(const f32 toPan, const f32 fadeTime)
	{
		m_soundSystem->GetRawHandle().fadePan(m_handle, std::clamp(toPan, -1.0f, 1.0f), fadeTime);
	}

	[[nodiscard]] f32 SoundSource::GetPan() const
	{
		return m_soundSystem->GetRawHandle().getPan(m_handle);
	}

	void SoundSource::SetPan(const f32 pan)
	{
		m_soundSystem->GetRawHandle().setPan(m_handle, std::clamp(pan, -1.0f, 1.0f));
	}

	void SoundSource::SetPanAbsolute(const f32 leftVolume, const f32 rightVolume)
	{
		m_soundSystem->GetRawHandle().setPanAbsolute(m_handle, leftVolume, rightVolume);
	}

	[[nodiscard]] f32 SoundSource::GetRelativePlaySpeed() const
	{
		return m_soundSystem->GetRawHandle().getRelativePlaySpeed(m_handle);
	}

	void SoundSource::SetRelativePlaySpeed(const f32 relativePlaySpeed)
	{
		m_soundSystem->GetRawHandle().setRelativePlaySpeed(m_handle, relativePlaySpeed);
	}

	[[nodiscard]] f32 SoundSource::GetSampleRate() const
	{
		return m_soundSystem->GetRawHandle().getSamplerate(m_handle);
	}

	void SoundSource::SetSampleRate(const f32 sampleRate)
	{
		m_soundSystem->GetRawHandle().setSamplerate(m_handle, sampleRate);
	}

	void SoundSource::OscillateVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency)
	{
		m_soundSystem->GetRawHandle().oscillateVolume(m_handle, fromVolume, toVolume, frequency);
	}

	void SoundSource::ResetVolumeOscillation()
	{
		m_soundSystem->GetRawHandle().oscillateVolume(m_handle, 1.0f, 1.0f, 0.0);
	}

	void SoundSource::OscillatePan(const f32 toPan, const f32 fromPan, const f32 frequency)
	{
		m_soundSystem->GetRawHandle().oscillatePan(m_handle, fromPan, toPan, frequency);
	}

	void SoundSource::ResetPanOscillation()
	{
		m_soundSystem->GetRawHandle().oscillatePan(m_handle, 0.0f, 0.0f, 0.0);
	}

	void SoundSource::OscillateRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fromRelativePlaySpeed, const f32 frequency)
	{
		m_soundSystem->GetRawHandle().oscillateRelativePlaySpeed(m_handle, fromRelativePlaySpeed, toRelativePlaySpeed, frequency);
	}

	void SoundSource::ResetRelativePlaySpeedOscillation()
	{
		m_soundSystem->GetRawHandle().oscillateRelativePlaySpeed(m_handle, 1.0f, 1.0f, 0.0);
	}

	void SoundSource::SetPosition(const Vec3& position)
	{
		m_soundSystem->GetRawHandle().set3dSourcePosition(m_handle, position.x, position.y, position.z);
		m_position = position;
	}

	void SoundSource::SetVelocity(const Vec3& velocity)
	{
		m_soundSystem->GetRawHandle().set3dSourceVelocity(m_handle, velocity.x, velocity.y, velocity.z);
		m_velocity = velocity;
	}

	void SoundSource::SetMinDistance(const f32 minDistance)
	{
		m_soundSystem->GetRawHandle().set3dSourceMinMaxDistance(m_handle, minDistance, m_maxDistance);
		m_minDistance = minDistance;
	}

	void SoundSource::SetMaxDistance(const f32 maxDistance)
	{
		m_soundSystem->GetRawHandle().set3dSourceMinMaxDistance(m_handle, m_minDistance, maxDistance);
		m_maxDistance = maxDistance;
	}

	void SoundSource::SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour)
	{
		switch (inaudibleBehaviour)
		{
		case InaudibleBehaviour::KeepTicking:
			m_soundSystem->GetRawHandle().setInaudibleBehavior(m_handle, true, false);

			break;

		case InaudibleBehaviour::Kill:
			m_soundSystem->GetRawHandle().setInaudibleBehavior(m_handle, false, true);

			break;

		case InaudibleBehaviour::Pause:
		default:
			m_soundSystem->GetRawHandle().setInaudibleBehavior(m_handle, false, false);

			break;
		}

		m_inaudibleBehaviour = inaudibleBehaviour;
	}

	[[nodiscard]] bool SoundSource::IsProtected() const
	{
		return m_soundSystem->GetRawHandle().getProtectVoice(m_handle);
	}

	void SoundSource::SetProtection(const bool isProtected)
	{
		m_soundSystem->GetRawHandle().setProtectVoice(m_handle, isProtected);
	}
}