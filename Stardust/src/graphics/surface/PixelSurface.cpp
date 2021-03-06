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

    void PixelSurface::Blit(const PixelSurface& source, const Optional<Rect>& sourceArea, const IVec2& destinationPosition) const
    {
        const SDL_Rect* sourceAreaPointer = nullptr;

        if (sourceArea.has_value())
        {
            sourceAreaPointer = &sourceArea.value().GetRawRect();
        }

        Rect destinationRect(destinationPosition.x, destinationPosition.y, 0u, 0u);

        SDL_BlitSurface(source.GetRawHandle(), sourceAreaPointer, GetRawHandle(), &destinationRect.GetRawRect());
    }

    [[nodiscard]] UVec2 PixelSurface::GetSize() const
    {
        return UVec2{
            GetRawHandle()->w,
            GetRawHandle()->h,
        };
    }

    [[nodiscard]] u32 PixelSurface::GetPitch() const
    {
        return static_cast<u32>(GetRawHandle()->pitch);
    }

    [[nodiscard]] u32 PixelSurface::GetFormat() const
    {
        return GetRawHandle()->format->format;
    }

    [[nodiscard]] void* PixelSurface::GetPixels() const
    {
        return GetRawHandle()->pixels;
    }
}