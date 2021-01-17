#pragma once
#ifndef STARDUST_AUDIO_DEVICE_H
#define STARDUST_AUDIO_DEVICE_H

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	class AudioDevice
	{
	private:
		static constexpr SDL_AudioDeviceID s_InvalidDeviceID = 0u;

		SDL_AudioDeviceID m_internalID = s_InvalidDeviceID;

		i32 m_index;
		String m_name;

		bool m_isRecordingDevice;

	public:
		[[nodiscard]] static Vector<AudioDevice> GetPlaybackDevices();
		[[nodiscard]] static Vector<AudioDevice> GetRecordingDevices();

		AudioDevice(const i32 index, const bool isRecording);
		~AudioDevice() noexcept;

		void Open();
		void Close() noexcept;

		inline bool IsOpen() const noexcept { return m_internalID != s_InvalidDeviceID; }

		inline i32 GetIndex() const noexcept { return m_index; }
		inline const String& GetName() const noexcept { return m_name; }
		inline bool IsRecordingDevice() const noexcept { return m_isRecordingDevice; }

	private:
		static void AudioPlaybackCallback(void* const userData, u8* const stream, const i32 length);
		static void AudioRecordingCallback(void* const userData, u8* const stream, const i32 length);
	};
}

#endif