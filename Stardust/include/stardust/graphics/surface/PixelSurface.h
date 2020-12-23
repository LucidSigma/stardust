#pragma once
#ifndef PIXEL_SURFACE_H
#define PIXEL_SURFACE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/rect/Rect.h"

namespace stardust
{
	class PixelSurface
		: private INoncopyable
	{
	private:
		struct SurfaceDestroyer
		{
			void operator ()(SDL_Surface* const surface) const noexcept;
		};

		static constexpr u32 s_RedMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'00'FF : 0xFF'00'00'00;
		static constexpr u32 s_GreenMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'FF'00 : 0x00'FF'00'00;
		static constexpr u32 s_BlueMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'FF'00'00 : 0x00'00'FF'00;
		static constexpr u32 s_AlphaMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0xFF'00'00'00 : 0x00'00'00'FF;

		UniquePtr<SDL_Surface, SurfaceDestroyer> m_handle = nullptr;
		bool m_isLocked = false;

	public:
		PixelSurface() = default;
		PixelSurface(const u32 width, const u32 height, const u32 channelCount);
		PixelSurface(const u32 width, const u32 height, const u32 channelCount, void* const pixels);

		PixelSurface(PixelSurface&& other) noexcept;
		PixelSurface& operator =(PixelSurface&& other) noexcept;

		~PixelSurface() noexcept;

		void Initialise(const u32 width, const u32 height, const u32 channelCount);
		void Initialise(const u32 width, const u32 height, const u32 channelCount, void* const pixels);
		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_handle != nullptr; }

		void Lock();
		void Unlock();
		inline bool IsLocked() const noexcept { return m_isLocked; }

		void Blit(const PixelSurface& source, const Optional<Pair<IVec2, UVec2>>& sourceArea, const IVec2& destinationPosition) const;

		[[nodiscard]] UVec2 GetSize() const;
		u32 GetPitch() const;
		u32 GetFormat() const;
		void* GetPixels() const;

		[[nodiscard]] inline SDL_Surface* const GetRawHandle() const noexcept { return m_handle.get(); }
	};
}

#endif