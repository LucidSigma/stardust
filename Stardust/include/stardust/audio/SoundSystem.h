#pragma once
#ifndef STARDUST_SOUND_SYSTEM_H
#define STARDUST_SOUND_SYSTEM_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <soloud/soloud.h>
#undef min
#undef max

#include "stardust/audio/sounds/Sounds.h"
#include "stardust/audio/Listener.h"
#include "stardust/audio/SoundSource.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/math/Math.h"

namespace stardust
{
	class SoundSystem
		: private INoncopyable, private INonmovable
	{
	private:
		UniquePtr<SoLoud::Soloud> m_soLoudHandle;
		Listener m_listener;

		bool m_didInitialiseSuccessfully = false;

	public:
		SoundSystem();
		~SoundSystem() noexcept;

		void Update() const;

		inline bool DidInitialiseSuccessfully() const noexcept { return m_didInitialiseSuccessfully; }

		SoundSource PlaySound(Sound& sound, const bool startPaused = false);
		SoundSource PlaySound(SoundStream& soundStream, const bool startPaused = false);
		SoundSource Play3DSound(Sound& sound, const Vec3& position, const Vec3& velocity = Vec3Zero, const bool startPaused = false);
		SoundSource Play3DSound(SoundStream& soundStream, const Vec3& position, const Vec3& velocity = Vec3Zero, const bool startPaused = false);
		
		SoundSource PlaySoundWithDelay(Sound& sound, const f32 secondsDelay);
		SoundSource PlaySoundWithDelay(SoundStream& soundStream, const f32 secondsDelay);
		SoundSource Play3DSoundWithDelay(Sound& sound, const f32 secondsDelay, const Vec3& position, const Vec3& velocity = Vec3Zero);
		SoundSource Play3DSoundWithDelay(SoundStream& soundStream, const f32 secondsDelay, const Vec3& position, const Vec3& velocity = Vec3Zero);
		
		SoundSource PlaySoundInBackground(Sound& sound, const bool startPaused = false);
		SoundSource PlaySoundInBackground(SoundStream& soundStream, const bool startPaused = false);

		void PauseAllSounds() const;
		void ResumeAllSounds() const;
		void StopAllSounds() const;

		void FadeVolumeGlobal(const f32 volumeToFadeTo, const f32 seconds) const;

		void OscillateGlobalVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency) const;
		void ResetGlobalVolumeOscillation() const;

		[[nodiscard]] unsigned int GetPlayingSoundCount() const;
		[[nodiscard]] bool IsSoundPlaying() const;

		[[nodiscard]] f32 GetGlobalVolume() const noexcept;
		void SetGlobalVolume(const f32 globalVolume) const noexcept;

		[[nodiscard]] f32 GetPostClipScaler() const noexcept;
		void SetPostClipScaler(const f32 postClipScaler) const noexcept;

		[[nodiscard]] f32 GetSpeedOfSound() const noexcept;
		void SetSpeedOfSound(const f32 speedOfSound) const noexcept;

		[[nodiscard]] SoLoud::Soloud& GetRawHandle() noexcept { return *m_soLoudHandle.get(); }
		Listener& GetListener() noexcept { return m_listener; }

	private:
		void Initialise();
		void Destroy();
	};
}

#endif