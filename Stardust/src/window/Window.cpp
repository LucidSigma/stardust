#include "stardust/window/Window.h"

#include <algorithm>
#include <utility>
#include <variant>

#include <stb/stb_image.h>

#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/utility/Utility.h"

namespace stardust
{
    auto Window::WindowDestroyer::operator ()(SDL_Window* const window) const noexcept -> void
    {
        SDL_DestroyWindow(window);
    }

    auto Window::SetMinimiseOnFullscreenFocusLoss(const bool minimiseOnFocusLoss) -> void
    {
        SDL_SetHintWithPriority(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, minimiseOnFocusLoss ? "1" : "0", SDL_HINT_OVERRIDE);
    }

    Window::Window(const CreateInfo& createInfo)
    {
        Initialise(createInfo);
    }

    Window::Window(Window&& other) noexcept
        : m_handle(nullptr), m_size(UVector2Zero), m_sizeBeforeFullscreen(None), m_fullscreenType(FullscreenType::Hard), m_isFullscreen(false), m_hasUpdatedFullscreen(false)
    {
        std::swap(m_handle, other.m_handle);

        std::swap(m_size, other.m_size);
        std::swap(m_sizeBeforeFullscreen, other.m_sizeBeforeFullscreen);

        std::swap(m_fullscreenType, other.m_fullscreenType);
        std::swap(m_isFullscreen, other.m_isFullscreen);
        std::swap(m_hasUpdatedFullscreen, other.m_hasUpdatedFullscreen);
    }

    auto Window::operator =(Window&& other) noexcept -> Window&
    {
        m_handle = std::exchange(other.m_handle, nullptr);

        m_size = std::exchange(other.m_size, UVector2Zero);
        m_sizeBeforeFullscreen = std::exchange(other.m_sizeBeforeFullscreen, None);

        m_fullscreenType = std::exchange(other.m_fullscreenType, FullscreenType::Hard);
        m_isFullscreen = std::exchange(other.m_isFullscreen, false);
        m_hasUpdatedFullscreen = std::exchange(other.m_hasUpdatedFullscreen, false);

        return *this;
    }

    Window::~Window() noexcept
    {
        Destroy();
    }

    auto Window::Initialise(const CreateInfo& createInfo) -> void
    {
        const i32 x = GetWindowCoordinate(createInfo.x);
        const i32 y = GetWindowCoordinate(createInfo.y);

        u32 windowFlags = 0u;
        bool startWithFullscreen = false;
        bool startBorderless = false;

        for (const auto windowFlag : createInfo.flags)
        {
            if (windowFlag == CreateFlag::HardFullscreen && !startWithFullscreen)
            {
                m_fullscreenType = FullscreenType::Hard;
                startWithFullscreen = true;
            }
            else if (windowFlag == CreateFlag::SoftFullscreen && !startWithFullscreen)
            {
                m_fullscreenType = FullscreenType::Soft;
                startWithFullscreen = true;
            }
            else if (windowFlag == CreateFlag::Borderless && !startBorderless)
            {
                startBorderless = true;
            }
            else [[likely]]
            {
                windowFlags |= static_cast<u32>(windowFlag);
            }
        }

        m_handle = UniquePointer<SDL_Window, WindowDestroyer>(SDL_CreateWindow(createInfo.title.data(), x, y, createInfo.size.x, createInfo.size.y, windowFlags));

        if (m_handle != nullptr)
        {
            m_size = createInfo.size;

            if (createInfo.minimumSize.has_value())
            {
                SetMinimumSize(createInfo.minimumSize.value());
            }

            if (createInfo.maximumSize.has_value())
            {
                SetMaximumSize(createInfo.maximumSize.value());
            }

            if (startBorderless)
            {
                SetBorderless(true);
            }

            if (startWithFullscreen)
            {
                ToggleFullscreen();
            }
        }
    }

    auto Window::Destroy() noexcept -> void
    {
        if (m_handle != nullptr)
        {
            m_handle = nullptr;

            m_size = UVector2Zero;
            m_sizeBeforeFullscreen = None;

            m_fullscreenType = FullscreenType::Hard;
            m_isFullscreen = false;
            m_hasUpdatedFullscreen = false;
        }
    }

    [[nodiscard]] auto Window::GetID() const noexcept -> ID
    {
        return static_cast<ID>(SDL_GetWindowID(GetRawHandle()));
    }

    auto Window::Present() const -> void
    {
        SDL_GL_SwapWindow(GetRawHandle());
    }

    auto Window::ToggleFullscreen() -> void
    {
        if (m_isFullscreen)
        {
            SDL_SetWindowSize(GetRawHandle(), m_sizeBeforeFullscreen->x, m_sizeBeforeFullscreen->y);
            SDL_SetWindowFullscreen(GetRawHandle(), 0u);

            m_size = m_sizeBeforeFullscreen.value();
            m_sizeBeforeFullscreen = None;
            m_hasUpdatedFullscreen = true;
        }
        else
        {
            const display::DisplayData displayData = display::GetDisplayData(GetDisplayIndex());

            m_sizeBeforeFullscreen = m_size;
            m_size = displayData.size;

            SDL_SetWindowSize(GetRawHandle(), m_size.x, m_size.y);
            SDL_SetWindowFullscreen(GetRawHandle(), static_cast<u32>(m_fullscreenType));

            m_hasUpdatedFullscreen = true;
        }

        m_isFullscreen = !m_isFullscreen;
    }

    auto Window::ChangeSize(const UVector2 newSize) -> void
    {
        SDL_SetWindowSize(GetRawHandle(), newSize.x, newSize.y);
        ProcessResize(newSize);
    }

    auto Window::ProcessResize(const UVector2 newSize) -> void
    {
        if (!m_hasUpdatedFullscreen)
        {
            m_size = newSize;
        }
        else
        {
            m_hasUpdatedFullscreen = false;
        }
    }

    [[nodiscard]] auto Window::GetOpacity() const noexcept -> f32
    {
        f32 opacity = 0.0f;
        SDL_GetWindowOpacity(GetRawHandle(), &opacity);

        return opacity;
    }

    auto Window::SetOpacity(const f32 opacity) const noexcept -> void
    {
        SDL_SetWindowOpacity(GetRawHandle(), std::clamp(opacity, 0.0f, 1.0f));
    }

    [[nodiscard]] auto Window::SetIcon(const StringView iconFilepath) const -> Status
    {
        auto iconFileResult = vfs::ReadFileBytes(iconFilepath);

        if (iconFileResult.is_err())
        {
            return Status::Fail;
        }

        const List<ubyte> rawIconFileData = std::move(iconFileResult).unwrap();

        i32 iconWidth = 0;
        i32 iconHeight = 0;
        stbi_uc* iconData = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc*>(rawIconFileData.data()),
            static_cast<i32>(rawIconFileData.size()),
            &iconWidth,
            &iconHeight,
            nullptr,
            STBI_rgb_alpha
        );

        if (iconData == nullptr)
        {
            return Status::Fail;
        }

        static constexpr u32 RedMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'00'FFu : 0xFF'00'00'00u;
        static constexpr u32 GreenMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'FF'00u : 0x00'FF'00'00u;
        static constexpr u32 BlueMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'FF'00'00u : 0x00'00'FF'00u;
        static constexpr u32 AlphaMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0xFF'00'00'00u : 0x00'00'00'FFu;

        SDL_Surface* iconPixelSurface = SDL_CreateRGBSurfaceFrom(
            iconData,
            iconWidth,
            iconHeight,
            32,
            iconWidth * 4,
            RedMask,
            GreenMask,
            BlueMask,
            AlphaMask
        );

        if (iconPixelSurface == nullptr)
        {
            stbi_image_free(iconData);
            iconData = nullptr;

            return Status::Fail;
        }

        SDL_SetWindowIcon(GetRawHandle(), iconPixelSurface);

        SDL_FreeSurface(iconPixelSurface);
        iconPixelSurface = nullptr;

        stbi_image_free(iconData);
        iconData = nullptr;

        return Status::Success;
    }

    auto Window::WarpCursor(const IVector2 coordinates) const -> void
    {
        SDL_WarpMouseInWindow(GetRawHandle(), coordinates.x, coordinates.y);
    }

    [[nodiscard]] auto Window::GetDrawableSize() const noexcept -> UVector2
    {
        i32 drawableX = 0;
        i32 drawableY = 0;
        SDL_GL_GetDrawableSize(GetRawHandle(), &drawableX, &drawableY);

        return UVector2{ drawableX, drawableY };
    }

    [[nodiscard]] auto Window::GetMinimumSize() const noexcept -> UVector2
    {
        i32 minimumX = 0;
        i32 minimumY = 0;
        SDL_GetWindowMinimumSize(GetRawHandle(), &minimumX, &minimumY);

        return UVector2{ minimumX, minimumY };
    }

    auto Window::SetMinimumSize(const UVector2 minimumSize) const noexcept -> void
    {
        SDL_SetWindowMinimumSize(GetRawHandle(), static_cast<i32>(minimumSize.x), static_cast<i32>(minimumSize.y));
    }

    [[nodiscard]] auto Window::GetMaximumSize() const noexcept -> UVector2
    {
        i32 maximumX = 0;
        i32 maximumY = 0;
        SDL_GetWindowMaximumSize(GetRawHandle(), &maximumX, &maximumY);

        return UVector2{ maximumX, maximumY };
    }

    auto Window::SetMaximumSize(const UVector2 maximumSize) const noexcept -> void
    {
        SDL_SetWindowMaximumSize(GetRawHandle(), static_cast<i32>(maximumSize.x), static_cast<i32>(maximumSize.y));
    }

    [[nodiscard]] auto Window::GetPosition() const noexcept -> IVector2
    {
        i32 x = 0;
        i32 y = 0;
        SDL_GetWindowPosition(GetRawHandle(), &x, &y);

        return IVector2{ x, y };
    }

    auto Window::SetPosition(const IVector2 position) const noexcept -> void
    {
        SDL_SetWindowPosition(GetRawHandle(), position.x, position.y);
    }

    [[nodiscard]] auto Window::GetTitle() const noexcept -> String
    {
        return SDL_GetWindowTitle(GetRawHandle());
    }

    auto Window::SetTitle(const StringView title) const noexcept -> void
    {
        SDL_SetWindowTitle(GetRawHandle(), title.data());
    }

    auto Window::IsBorderless() const noexcept -> bool
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_BORDERLESS;
    }

    auto Window::SetBorderless(const bool isBorderless) const noexcept -> void
    {
        SDL_SetWindowBordered(GetRawHandle(), static_cast<SDL_bool>(!isBorderless));
    }

    [[nodiscard]] auto Window::GetDisplayIndex() const noexcept -> display::Index
    {
        return static_cast<display::Index>(SDL_GetWindowDisplayIndex(GetRawHandle()));
    }

    [[nodiscard]] auto Window::IsMinimised() const noexcept -> bool
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_MINIMIZED;
    }

    auto Window::Minimise() const noexcept -> void
    {
        SDL_MinimizeWindow(GetRawHandle());
    }

    [[nodiscard]] auto Window::IsMaximised() const noexcept -> bool
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_MAXIMIZED;
    }

    auto Window::Maximise() const noexcept -> void
    {
        SDL_MaximizeWindow(GetRawHandle());
    }

    [[nodiscard]] auto Window::IsResizable() const noexcept -> bool
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_RESIZABLE;
    }

    auto Window::SetResizable(const bool isResizable) const noexcept -> void
    {
        SDL_SetWindowResizable(GetRawHandle(), static_cast<SDL_bool>(isResizable));
    }

    [[nodiscard]] auto Window::IsShown() const noexcept -> bool
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_SHOWN;
    }

    auto Window::Show() const noexcept -> void
    {
        SDL_ShowWindow(GetRawHandle());
    }

    [[nodiscard]] auto Window::IsHidden() const noexcept -> bool
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_HIDDEN;
    }

    auto Window::Hide() const noexcept -> void
    {
        SDL_HideWindow(GetRawHandle());
    }

    auto Window::Raise() const noexcept -> void
    {
        SDL_RaiseWindow(GetRawHandle());
    }

    auto Window::Restore() const noexcept -> void
    {
        SDL_RestoreWindow(GetRawHandle());
    }

    [[nodiscard]] auto Window::IsGrabbed() const noexcept -> bool
    {
        return SDL_GetWindowGrab(GetRawHandle());
    }

    auto Window::SetGrabbed(const bool isGrabbed) const noexcept -> void
    {
        SDL_SetWindowGrab(GetRawHandle(), static_cast<SDL_bool>(isGrabbed));
    }

    auto Window::Flash(const FlashOperation flashOperation) const -> void
    {
        SDL_FlashWindow(GetRawHandle(), static_cast<SDL_FlashOperation>(flashOperation));
    }

    auto Window::StopFlashing() const -> void
    {
        SDL_FlashWindow(GetRawHandle(), SDL_FLASH_CANCEL);
    }

    [[nodiscard]] auto Window::GetWindowCoordinate(const Variant<i32, Position>& windowCoordinate) const -> i32
    {
        return std::visit(
            utility::VariantOverloader{
                [](const Position windowPosition) { return static_cast<i32>(windowPosition); },
                [](const i32 coordinate) { return coordinate; },
            },
            windowCoordinate
        );
    }
}
