#include "stardust/window/Window.h"

#include <algorithm>
#include <utility>

#include <stb/stb_image.h>

#ifdef WIN32
#include <SDL2/SDL_syswm.h>
#include <windows.h>
#endif

#include "stardust/debug/logging/Log.h"
#include "stardust/debug/message_box/MessageBox.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/graphics/display/Display.h"
#include "stardust/graphics/surface/PixelSurface.h"
#include "stardust/utility/Utility.h"

namespace stardust
{
    void Window::WindowDestroyer::operator ()(SDL_Window* const window) const noexcept
    {
        SDL_DestroyWindow(window);
    }

    void Window::SetMinimiseOnFullscreenFocusLoss(const bool minimiseOnFocusLoss)
    {
        SDL_SetHintWithPriority(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, minimiseOnFocusLoss ? "1" : "0", SDL_HINT_OVERRIDE);
    }

    Status Window::SetVSync(const VSyncType vSyncType)
    {
        return SDL_GL_SetSwapInterval(static_cast<i32>(vSyncType)) == 0
            ? Status::Success
            : Status::Fail;
    }

    Window::Window(const CreateInfo& createInfo)
    {
        Initialise(createInfo);
    }

    Window::Window(Window&& other) noexcept
        : m_handle(nullptr), m_size(UVec2Zero), m_sizeBeforeFullscreen(NullOpt), m_fullscreenType(FullscreenType::Hard), m_isFullscreen(false), m_hasUpdatedFullscreen(false)
    {
        std::swap(m_handle, other.m_handle);

        std::swap(m_size, other.m_size);
        std::swap(m_sizeBeforeFullscreen, other.m_sizeBeforeFullscreen);

        std::swap(m_fullscreenType, other.m_fullscreenType);
        std::swap(m_isFullscreen, other.m_isFullscreen);
        std::swap(m_hasUpdatedFullscreen, other.m_hasUpdatedFullscreen);
    }

    Window& Window::operator =(Window&& other) noexcept
    {
        m_handle = std::exchange(other.m_handle, nullptr);

        m_size = std::exchange(other.m_size, UVec2Zero);
        m_sizeBeforeFullscreen = std::exchange(other.m_sizeBeforeFullscreen, NullOpt);

        m_fullscreenType = std::exchange(other.m_fullscreenType, FullscreenType::Hard);
        m_isFullscreen = std::exchange(other.m_isFullscreen, false);
        m_hasUpdatedFullscreen = std::exchange(other.m_hasUpdatedFullscreen, false);

        return *this;
    }

    Window::~Window() noexcept
    {
        Destroy();
    }

    void Window::Initialise(const CreateInfo& createInfo)
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

        m_handle = UniquePtr<SDL_Window, WindowDestroyer>(SDL_CreateWindow(createInfo.title.data(), x, y, createInfo.size.x, createInfo.size.y, windowFlags));

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

    void Window::Destroy() noexcept
    {
        if (m_handle != nullptr)
        {
            m_handle = nullptr;

            m_size = UVec2Zero;
            m_sizeBeforeFullscreen = NullOpt;
            m_isFullscreen = false;
            m_hasUpdatedFullscreen = false;
        }
    }

    void Window::Present() const
    {
        SDL_GL_SwapWindow(GetRawHandle());
    }

    void Window::ToggleFullscreen()
    {
        if (m_isFullscreen)
        {
            SDL_SetWindowSize(GetRawHandle(), m_sizeBeforeFullscreen->x, m_sizeBeforeFullscreen->y);
            SDL_SetWindowFullscreen(GetRawHandle(), 0u);

            m_size = m_sizeBeforeFullscreen.value();
            m_sizeBeforeFullscreen = NullOpt;
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

    void Window::ChangeSize(const UVec2& newSize)
    {
        SDL_SetWindowSize(GetRawHandle(), newSize.x, newSize.y);
        ProcessResize(newSize);
    }

    void Window::ProcessResize(const UVec2& newSize)
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

    [[nodiscard]] f32 Window::GetOpacity() const noexcept
    {
        f32 opacity = 0.0f;
        SDL_GetWindowOpacity(GetRawHandle(), &opacity);

        return opacity;
    }

    void Window::SetOpacity(const f32 opacity) const noexcept
    {
        SDL_SetWindowOpacity(GetRawHandle(), std::clamp(opacity, 0.0f, 1.0f));
    }

    void Window::SetIcon(const StringView& iconFilepath, const Locale& locale) const
    {
        const Vector<ubyte> rawIconData = vfs::ReadFileData(iconFilepath);
    
        if (rawIconData.empty())
        {
            message_box::Show(
                locale["engine"]["warnings"]["titles"]["window"],
                locale["engine"]["warnings"]["bodies"]["window-icon-load"],
                message_box::Type::Warning
            );
            Log::EngineWarn("Failed to open window icon file at {}.", iconFilepath);
    
            return;
        }
    
        i32 iconWidth = 0;
        i32 iconHeight = 0;
        stbi_uc* iconData = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc*>(rawIconData.data()),
            static_cast<i32>(rawIconData.size()),
            &iconWidth, &iconHeight, nullptr,
            STBI_rgb_alpha
        );
    
        if (iconData == nullptr)
        {
            message_box::Show(
                locale["engine"]["warnings"]["titles"]["window"],
                locale["engine"]["warnings"]["bodies"]["window-icon-load"],
                message_box::Type::Warning
            );
            Log::EngineWarn("Failed to open window icon file at {}.", iconFilepath);
    
            return;
        }
    
        const PixelSurface iconSurface(iconWidth, iconHeight, 4u, iconData);
    
        if (!iconSurface.IsValid())
        {
            message_box::Show(
                locale["engine"]["warnings"]["titles"]["window"],
                locale["engine"]["warnings"]["bodies"]["window-icon-convert"],
                message_box::Type::Warning
            );
            Log::EngineWarn("Could not convert window icon image: {}.", SDL_GetError());
    
            stbi_image_free(iconData);
            iconData = nullptr;
    
            return;
        }
    
        SDL_SetWindowIcon(GetRawHandle(), iconSurface.GetRawHandle());
    
        stbi_image_free(iconData);
        iconData = nullptr;
    }

    [[nodiscard]] UVec2 Window::GetDrawableSize() const noexcept
    {
        i32 drawableX = 0;
        i32 drawableY = 0;
        SDL_GL_GetDrawableSize(GetRawHandle(), &drawableX, &drawableY);

        return UVec2{ drawableX, drawableY };
    }

    [[nodiscard]] UVec2 Window::GetMinimumSize() const noexcept
    {
        i32 minimumX = 0;
        i32 minimumY = 0;
        SDL_GetWindowMinimumSize(GetRawHandle(), &minimumX, &minimumY);

        return UVec2{ minimumX, minimumY };
    }

    void Window::SetMinimumSize(const UVec2& minimumSize) const noexcept
    {
        SDL_SetWindowMinimumSize(GetRawHandle(), minimumSize.x, minimumSize.y);
    }

    [[nodiscard]] UVec2 Window::GetMaximumSize() const noexcept
    {
        i32 maximumX = 0;
        i32 maximumY = 0;
        SDL_GetWindowMaximumSize(GetRawHandle(), &maximumX, &maximumY);

        return UVec2{ maximumX, maximumY };
    }

    void Window::SetMaximumSize(const UVec2& maximumSize) const noexcept
    {
        SDL_SetWindowMaximumSize(GetRawHandle(), maximumSize.x, maximumSize.y);
    }

    [[nodiscard]] IVec2 Window::GetPosition() const noexcept
    {
        i32 x = 0;
        i32 y = 0;
        SDL_GetWindowPosition(GetRawHandle(), &x, &y);

        return IVec2{ x, y };
    }

    void Window::SetPosition(const IVec2& position) const noexcept
    {
        SDL_SetWindowPosition(GetRawHandle(), position.x, position.y);
    }

    [[nodiscard]] String Window::GetTitle() const noexcept
    {
        return SDL_GetWindowTitle(GetRawHandle());
    }

    void Window::SetTitle(const StringView& title) const noexcept
    {
        SDL_SetWindowTitle(GetRawHandle(), title.data());
    }

    [[nodiscard]] bool Window::IsBorderless() const noexcept
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_BORDERLESS;
    }

    void Window::SetBorderless(const bool isBorderless) const noexcept
    {
        SDL_SetWindowBordered(GetRawHandle(), static_cast<SDL_bool>(!isBorderless));
    }

    [[nodiscard]] SDL_Surface* Window::GetSurface() const noexcept
    {
        return SDL_GetWindowSurface(GetRawHandle());
    }

    [[nodiscard]] u32 Window::GetPixelFormat() const noexcept
    {
        return SDL_GetWindowPixelFormat(GetRawHandle());
    }

    [[nodiscard]] i32 Window::GetDisplayIndex() const noexcept
    {
        return SDL_GetWindowDisplayIndex(GetRawHandle());
    }

    [[nodiscard]] bool Window::IsMinimised() const noexcept
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_MINIMIZED;
    }

    void Window::Minimise() const noexcept
    {
        SDL_MinimizeWindow(GetRawHandle());
    }

    [[nodiscard]] bool Window::IsMaximised() const noexcept
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_MAXIMIZED;
    }

    void Window::Maximise() const noexcept
    {
        SDL_MaximizeWindow(GetRawHandle());
    }

    [[nodiscard]] bool Window::IsResizable() const noexcept
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_RESIZABLE;
    }

    void Window::SetResizable(const bool isResizable) const noexcept
    {
        SDL_SetWindowResizable(GetRawHandle(), static_cast<SDL_bool>(isResizable));
    }

    [[nodiscard]] bool Window::IsShown() const noexcept
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_SHOWN;
    }

    void Window::Show() const noexcept
    {
        SDL_ShowWindow(GetRawHandle());
    }

    [[nodiscard]] bool Window::IsHidden() const noexcept
    {
        return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_HIDDEN;
    }

    void Window::Hide() const noexcept
    {
        SDL_HideWindow(GetRawHandle());
    }

    void Window::Raise() const noexcept
    {
        SDL_RaiseWindow(GetRawHandle());
    }

    void Window::Restore() const noexcept
    {
        SDL_RestoreWindow(GetRawHandle());
    }

    [[nodiscard]] bool Window::IsGrabbed() const noexcept
    {
        return SDL_GetWindowGrab(GetRawHandle());
    }

    void Window::SetGrabbed(const bool isGrabbed) const noexcept
    {
        SDL_SetWindowGrab(GetRawHandle(), static_cast<SDL_bool>(isGrabbed));
    }

    void Window::Flash() const
    {
    #ifdef WIN32
        SDL_SysWMinfo systemInfo{ };
        SDL_VERSION(&systemInfo.version);
        
        if (SDL_GetWindowWMInfo(GetRawHandle(), &systemInfo) == SDL_TRUE)
        {
            FLASHWINFO flashInfo{
                .cbSize = sizeof(FLASHWINFO),
                .hwnd = systemInfo.info.win.window,
                .dwFlags = FLASHW_TIMERNOFG,
                .uCount = 1u,
                .dwTimeout = 100u,
            };

            FlashWindowEx(&flashInfo);
        }
    #endif
    }

    [[nodiscard]] i32 Window::GetWindowCoordinate(const Variant<i32, Position>& windowCoordinate) const
    {
        return std::visit(utility::Overload{
            [](const Position windowPosition) { return static_cast<i32>(windowPosition); },
            [](const i32 coordinate) { return coordinate; },
        }, windowCoordinate);
    }
}