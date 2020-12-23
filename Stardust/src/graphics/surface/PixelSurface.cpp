#include "stardust/graphics/surface/PixelSurface.h"

#include <utility>

namespace stardust
{
	void PixelSurface::SurfaceDestroyer::operator()(SDL_Surface* const surface) const noexcept
	{
		SDL_FreeSurface(surface);
	}

	PixelSurface::PixelSurface(const u32 width, const u32 height, const u32 channelCount)
	{
		Initialise(width, height, channelCount);
	}

	PixelSurface::PixelSurface(const u32 width, const u32 height, const u32 channelCount, void* const pixels)
	{
		Initialise(width, height, channelCount, pixels);
	}

	PixelSurface::PixelSurface(PixelSurface&& other) noexcept
		: m_handle(nullptr), m_isLocked(false)
	{
		std::swap(m_handle, other.m_handle);
		std::swap(m_isLocked, other.m_isLocked);
	}

	PixelSurface& PixelSurface::operator=(PixelSurface&& other) noexcept
	{
		m_handle = std::exchange(other.m_handle, nullptr);
		m_isLocked = std::exchange(other.m_isLocked, false);

		return *this;
	}

	PixelSurface::~PixelSurface() noexcept
	{
		Destroy();
	}

	void PixelSurface::Initialise(const u32 width, const u32 height, const u32 channelCount)
	{
		m_handle = UniquePtr<SDL_Surface, SurfaceDestroyer>(
			SDL_CreateRGBSurface(
				0u,
				static_cast<i32>(width),
				static_cast<i32>(height),
				static_cast<i32>(channelCount * 8u),
				s_RedMask,
				s_GreenMask,
				s_BlueMask,
				s_AlphaMask
			)
		);
	}

	void PixelSurface::Initialise(const u32 width, const u32 height, const u32 channelCount, void* const pixels)
	{
		m_handle = UniquePtr<SDL_Surface, SurfaceDestroyer>(
			SDL_CreateRGBSurfaceFrom(
				pixels,
				static_cast<i32>(width),
				static_cast<i32>(height),
				static_cast<i32>(channelCount * 8u),
				width * 4u,
				s_RedMask,
				s_GreenMask,
				s_BlueMask,
				s_AlphaMask
			)
		);

	}

	void PixelSurface::Destroy() noexcept
	{
		if (m_handle != nullptr)
		{
			Unlock();

			m_handle = nullptr;
		}
	}

	void PixelSurface::Lock()
	{
		m_isLocked = SDL_LockSurface(GetRawHandle()) == 0;
	}

	void PixelSurface::Unlock()
	{
		if (m_isLocked)
		{
			SDL_UnlockSurface(GetRawHandle());
			m_isLocked = false;
		}
	}

	void PixelSurface::Blit(const PixelSurface& source, const Optional<Pair<IVec2, UVec2>>& sourceArea, const IVec2& destinationPosition) const
	{
		SDL_Rect sourceAreaRect{ };
		const SDL_Rect* sourceAreaPointer = nullptr;

		if (sourceArea.has_value())
		{
			sourceAreaRect.x = sourceArea.value().first.x;
			sourceAreaRect.y = sourceArea.value().first.x;
			sourceAreaRect.w = static_cast<i32>(sourceArea.value().second.x);
			sourceAreaRect.h = static_cast<i32>(sourceArea.value().second.y);

			sourceAreaPointer = &sourceAreaRect;
		}

		SDL_Rect destinationRect{ destinationPosition.x, destinationPosition.y, 0, 0 };

		SDL_BlitSurface(source.GetRawHandle(), sourceAreaPointer, GetRawHandle(), &destinationRect);
	}

	[[nodiscard]] UVec2 PixelSurface::GetSize() const
	{
		return UVec2{
			GetRawHandle()->w,
			GetRawHandle()->h,
		};
	}

	u32 PixelSurface::GetPitch() const
	{
		return static_cast<u32>(GetRawHandle()->pitch);
	}

	u32 PixelSurface::GetFormat() const
	{
		return GetRawHandle()->format->format;
	}

	void* PixelSurface::GetPixels() const
	{
		return GetRawHandle()->pixels;
	}
}