#pragma once
#ifndef STARDUST_AUDIO_DEVICE_H
#define STARDUST_AUDIO_DEVICE_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	class AudioDevice
	{
	private:
		i32 m_id;
		String m_name;
		bool m_isRecordingDevice;

	public:
		[[nodiscard]] static Vector<AudioDevice> GetPlaybackDevices();
		[[nodiscard]] static Vector<AudioDevice> GetRecordingDevices();

		AudioDevice(const i32 id, const bool isRecording);
		~AudioDevice() noexcept = default;

		inline i32 GetID() const noexcept { return m_id; }
		inline const String& GetName() const noexcept { return m_name; }
		inline bool IsRecordingDevice() const noexcept { return m_isRecordingDevice; }
	};
}

#endif