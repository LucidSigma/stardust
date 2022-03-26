#pragma once
#ifndef STARDUST_WINDOW_H
#define STARDUST_WINDOW_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/graphics/display/Display.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class Window final
        : private INoncopyable
    {
    public:
        using ID = u32;

        enum class Position
            : decltype(SDL_WINDOWPOS_UNDEFINED)
        {
            Undefined = SDL_WINDOWPOS_UNDEFINED,
            Centred = SDL_WINDOWPOS_CENTERED,
        };

        enum class CreateFlag
            : std::underlying_type_t<SDL_WindowFlags>
        {
            HardFullscreen = SDL_WINDOW_FULLSCREEN,
            SoftFullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
            Shown = SDL_WINDOW_SHOWN,
            Hidden = SDL_WINDOW_HIDDEN,
            Borderless = SDL_WINDOW_BORDERLESS,
            Resizable = SDL_WINDOW_RESIZABLE,
            Minimised = SDL_WINDOW_MINIMIZED,
            Maximised = SDL_WINDOW_MAXIMIZED,
            InputGrabbed = SDL_WINDOW_INPUT_GRABBED,
            InputFocus = SDL_WINDOW_INPUT_FOCUS,
            MouseFocus = SDL_WINDOW_MOUSE_FOCUS,
            Foreign = SDL_WINDOW_FOREIGN,
            AllowHighDPI = SDL_WINDOW_ALLOW_HIGHDPI,
            MouseCapture = SDL_WINDOW_MOUSE_CAPTURE,
            OpenGL = SDL_WINDOW_OPENGL,
            Vulkan = SDL_WINDOW_VULKAN,
        };

        enum class FullscreenType
            : std::underlying_type_t<SDL_WindowFlags>
        {
            Hard = SDL_WINDOW_FULLSCREEN,
            Soft = SDL_WINDOW_FULLSCREEN_DESKTOP,
        };

        enum class FlashOperation
            : std::underlying_type_t<SDL_FlashOperation>
        {
            Briefly = SDL_FLASH_BRIEFLY,
            UntilFocus = SDL_FLASH_UNTIL_FOCUSED,
        };

        struct CreateInfo final
        {
            String title;

            Variant<i32, Position> x = Position::Centred;
            Variant<i32, Position> y = Position::Centred;
            UVector2 size;

            List<CreateFlag> flags;

            Optional<UVector2> minimumSize = None;
            Optional<UVector2> maximumSize = None;
        };

    private:
        struct WindowDestroyer final
        {
            auto operator ()(SDL_Window* const window) const noexcept -> void;
        };

        UniquePointer<SDL_Window, WindowDestroyer> m_handle = nullptr;

        UVector2 m_size = UVector2Zero;
        Optional<UVector2> m_sizeBeforeFullscreen = None;

        FullscreenType m_fullscreenType = FullscreenType::Hard;
        bool m_isFullscreen = false;
        bool m_hasUpdatedFullscreen = false;

    public:
        static auto SetMinimiseOnFullscreenFocusLoss(const bool minimiseOnFocusLoss) -> void;

        Window() = default;
        explicit Window(const CreateInfo& createInfo);

        Window(Window&& other) noexcept;
        auto operator =(Window&& other) noexcept -> Window&;

        ~Window() noexcept;

        auto Initialise(const CreateInfo& createInfo) -> void;
        auto Destroy() noexcept -> void;

        [[nodiscard]] auto GetID() const noexcept -> ID;

        auto Present() const -> void;

        auto ToggleFullscreen() -> void;

        auto ChangeSize(const UVector2 newSize) -> void;
        auto ProcessResize(const UVector2 newSize) -> void;

        [[nodiscard]] auto GetOpacity() const noexcept -> f32;
        auto SetOpacity(const f32 opacity) const noexcept -> void;

        [[nodiscard]] auto SetIcon(const StringView iconFilepath) const -> Status;
        auto WarpCursor(const IVector2 coordinates) const -> void;

        [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_handle != nullptr; }

        [[nodiscard]] inline auto GetSize() const noexcept -> const UVector2 { return m_size; }
        [[nodiscard]] auto GetDrawableSize() const noexcept -> UVector2;
        [[nodiscard]] inline auto GetAspectRatio() const noexcept -> f32 { return static_cast<f32>(m_size.x) / static_cast<f32>(m_size.y); }

        [[nodiscard]] auto GetMinimumSize() const noexcept -> UVector2;
        auto SetMinimumSize(const UVector2 minimumSize) const noexcept -> void;
        [[nodiscard]] auto GetMaximumSize() const noexcept -> UVector2;
        auto SetMaximumSize(const UVector2 maximumSize) const noexcept -> void;

        [[nodiscard]] auto GetPosition() const noexcept -> IVector2;
        auto SetPosition(const IVector2 position) const noexcept -> void;

        [[nodiscard]] auto GetTitle() const noexcept -> String;
        auto SetTitle(const StringView title) const noexcept -> void;

        [[nodiscard]] inline auto IsFullscreen() const noexcept -> bool { return m_isFullscreen; }
        [[nodiscard]] inline auto GetFullscreenType() const noexcept -> FullscreenType { return m_fullscreenType; }
        inline auto SetFullscreenType(const FullscreenType fullscreenType) noexcept -> void { m_fullscreenType = fullscreenType; }

        [[nodiscard]] auto IsBorderless() const noexcept -> bool;
        auto SetBorderless(const bool isBorderless) const noexcept -> void;

        [[nodiscard]] inline auto GetRawHandle() const noexcept -> SDL_Window* { return m_handle.get(); }
        [[nodiscard]] auto GetDisplayIndex() const noexcept -> display::Index;

        [[nodiscard]] auto IsMinimised() const noexcept -> bool;
        auto Minimise() const noexcept -> void;
        [[nodiscard]] auto IsMaximised() const noexcept -> bool;
        auto Maximise() const noexcept -> void;

        [[nodiscard]] auto IsResizable() const noexcept -> bool;
        auto SetResizable(const bool isResizable) const noexcept -> void;

        [[nodiscard]] auto IsShown() const noexcept -> bool;
        auto Show() const noexcept -> void;
        [[nodiscard]] auto IsHidden() const noexcept -> bool;
        auto Hide() const noexcept -> void;

        auto Raise() const noexcept -> void;
        auto Restore() const noexcept -> void;

        [[nodiscard]] auto IsGrabbed() const noexcept -> bool;
        auto SetGrabbed(const bool isGrabbed) const noexcept -> void;

        auto Flash(const FlashOperation flashOperation) const -> void;
        auto StopFlashing() const -> void;

    private:
        [[nodiscard]] auto GetWindowCoordinate(const Variant<i32, Position>& windowCoordinate) const -> i32;
    };
}

#endif
