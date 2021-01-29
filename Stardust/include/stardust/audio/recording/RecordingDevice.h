#pragma once
#ifndef STARDUST_RECORDING_DEVICE_H
#define STARDUST_RECORDING_DEVICE_H

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	class RecordingDevice
	{
	public:
		struct Info
		{
			i32 index;
			String name;
		};

	private:
		static constexpr SDL_AudioDeviceID s_InvalidDeviceID = 0u;

		SDL_AudioDeviceID m_interalID = s_InvalidDeviceID;

		i32 m_index;
		String m_name;

		u32 m_frequency = 0u;
		u32 m_channelCount = 0u;

		bool m_isRecording = false;

	public:
		[[nodiscard]] Vector<Info> GetAllDeviceInfos();

		RecordingDevice(const Info& info);
		~RecordingDevice() noexcept;

		void Open(const u32 frequency = 44'100u, const u32 channelCount = 2u);
		void Close() noexcept;
		inline bool IsOpen() const { return m_interalID != s_InvalidDeviceID; }

		void StartRecording();
		void StopRecording();
		inline bool IsRecording() const noexcept { return m_isRecording; }

		void SetCallback();

		inline i32 GetIndex() const noexcept { return m_index; }
		inline const String& GetName() const noexcept { return m_name; }

		inline u32 GetFrequency() const noexcept { return m_frequency; }
		inline u32 GetChannelCount() const noexcept { return m_channelCount; }
	};
}

#endif