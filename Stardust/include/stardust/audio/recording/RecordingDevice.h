#pragma once
#ifndef STARDUST_RECORDING_DEVICE_H
#define STARDUST_RECORDING_DEVICE_H

#include <functional>
#include <limits>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
	class RecordingDevice
	{
	public:
		using Callback = std::function<void(Vector<ubyte>, usize)>;

		struct Info
		{
			i32 index;
			String name;
		};

	private:
		static constexpr SDL_AudioDeviceID s_InvalidDeviceID = 0u;

		static constexpr u32 s_MaxRecordingSeconds = 1u;
		static constexpr u32 s_RecordingBufferSeconds = s_MaxRecordingSeconds + 1u;

		SDL_AudioDeviceID m_internalID = s_InvalidDeviceID;

		i32 m_index;
		String m_name;

		u32 m_frequency = 0u;
		u32 m_channelCount = 0u;
		u32 m_previousFrequency = std::numeric_limits<u32>::max();
		u32 m_previousChannelCount = std::numeric_limits<u32>::max();

		bool m_isRecording = false;

		Vector<ubyte> m_recordingBuffer{ };
		usize m_bufferByteSize = 0u;
		usize m_currentBufferByteOffset = 0u;
		usize m_maxBufferByteOffset = 0u;

		Callback m_callback{ };

	public:
		[[nodiscard]] static Vector<Info> GetAllDeviceInfos();

		RecordingDevice(const Info& info);
		~RecordingDevice() noexcept;

		Status Open(const u32 frequency = 44'100u, const u32 channelCount = 2u);
		void Close() noexcept;
		inline bool IsOpen() const { return m_internalID != s_InvalidDeviceID; }

		void StartRecording();
		Pair<Vector<ubyte>, usize> StopRecording();
		inline bool IsRecording() const noexcept { return m_isRecording; }

		inline void SetCallback(const Callback& callback) noexcept { m_callback = callback; }

		inline i32 GetIndex() const noexcept { return m_index; }
		inline const String& GetName() const noexcept { return m_name; }

		inline u32 GetFrequency() const noexcept { return m_frequency; }
		inline u32 GetChannelCount() const noexcept { return m_channelCount; }

	private:
		static void AudioRecordingCallback(void* const userData, u8* const stream, const i32 length);
	};
}

#endif