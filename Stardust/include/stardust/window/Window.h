#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <type_traits>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	class Window
		: private INoncopyable, private INonmovable
	{
	public:
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
		};

		enum class FullscreenType
			: std::underlying_type_t<SDL_WindowFlags>
		{
			Hard = SDL_WINDOW_FULLSCREEN,
			Soft = SDL_WINDOW_FULLSCREEN_DESKTOP,
		};

		struct CreateInfo
		{
			String title;

			Variant<int, Position> x = Position::Centred;
			Variant<int, Position> y = Position::Centred;
			glm::uvec2 size;

			std::vector<CreateFlag> flags;

			Optional<glm::uvec2> minimumSize = NullOpt;
			Optional<glm::uvec2> maximumSize = NullOpt;
		};

	private:
		struct WindowDestroyer
		{
			void operator ()(SDL_Window* const window) const noexcept;
		};

		UniquePtr<SDL_Window, WindowDestroyer> m_handle = nullptr;

		glm::uvec2 m_size{ 0u, 0u };
		Optional<glm::uvec2> m_sizeBeforeFullscreen = NullOpt;

		FullscreenType m_fullscreenType = FullscreenType::Hard;
		bool m_isFullscreen = false;
		bool m_hasUpdatedFullscreen = false;

	public:
		static void SetMinimiseOnFullscreenFocusLoss(const bool minimiseOnFocusLoss);

		Window();
		explicit Window(const CreateInfo& createInfo);

		Window(Window&& other) noexcept;
		Window& operator =(Window&& other) noexcept;

		~Window() noexcept;

		void Initialise(const CreateInfo& createInfo);
		void Destroy() noexcept;

		void ToggleFullscreen();

		void ChangeSize(const glm::uvec2& newSize);
		void ProcessResize(const glm::uvec2& newSize);

		float GetOpacity() const noexcept;
		void SetOpacity(const float opacity) const noexcept;

		// void SetIcon(const std::string_view& iconFilepath, const Locale& locale) const;

		inline bool IsValid() const noexcept { return m_handle != nullptr; }
		inline void UpdateSurface() const noexcept { SDL_UpdateWindowSurface(GetRawHandle()); }

		inline const glm::uvec2& GetSize() const noexcept { return m_size; }
		[[nodiscard]] inline float GetAspectRatio() const noexcept { return static_cast<float>(m_size.x) / static_cast<float>(m_size.y); }

		[[nodiscard]] glm::uvec2 GetMinimumSize() const noexcept;
		inline void SetMinimumSize(const glm::uvec2& minimumSize) const noexcept { SDL_SetWindowMinimumSize(GetRawHandle(), minimumSize.x, minimumSize.y); }
		[[nodiscard]] glm::uvec2 GetMaximumSize() const noexcept;
		inline void SetMaximumSize(const glm::uvec2& maximumSize) const noexcept { SDL_SetWindowMaximumSize(GetRawHandle(), maximumSize.x, maximumSize.y); }

		[[nodiscard]] glm::ivec2 GetPosition() const noexcept;
		inline void SetPosition(const glm::ivec2& position) const noexcept { SDL_SetWindowPosition(GetRawHandle(), position.x, position.y); }

		[[nodiscard]] inline String GetTitle() const noexcept { return SDL_GetWindowTitle(GetRawHandle()); }
		inline void SetTitle(const String& title) const noexcept { SDL_SetWindowTitle(GetRawHandle(), title.c_str()); }

		inline bool IsFullscreen() const noexcept { return m_isFullscreen; }
		inline FullscreenType GetFullscreenType() const noexcept { return m_fullscreenType; }
		inline void SetFullscreenType(const FullscreenType fullscreenType) noexcept { m_fullscreenType = fullscreenType; }

		inline bool IsBorderless() const noexcept { return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_BORDERLESS; }
		inline void SetBorderless(const bool isBorderless) const noexcept { SDL_SetWindowBordered(GetRawHandle(), static_cast<SDL_bool>(!isBorderless)); }

		[[nodiscard]] inline SDL_Window* const GetRawHandle() const noexcept { return m_handle.get(); }
		[[nodiscard]] inline SDL_Surface* GetSurface() const noexcept { return SDL_GetWindowSurface(GetRawHandle()); }
		[[nodiscard]] inline u32 GetPixelFormat() const noexcept { return SDL_GetWindowPixelFormat(GetRawHandle()); }
		inline i32 GetDisplayIndex() const noexcept { return SDL_GetWindowDisplayIndex(GetRawHandle()); }

		inline bool IsMinimised() const noexcept { return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_MINIMIZED; }
		inline void Minimise() const noexcept { SDL_MinimizeWindow(GetRawHandle()); }
		inline bool IsMaximised() const noexcept { return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_MAXIMIZED; }
		inline bool Maximise() const noexcept { SDL_MaximizeWindow(GetRawHandle()); }

		inline bool IsResizable() const noexcept { return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_RESIZABLE; }
		inline void SetResizable(const bool isResizable) const noexcept { SDL_SetWindowResizable(GetRawHandle(), static_cast<SDL_bool>(isResizable)); }

		inline bool IsShown() const noexcept { return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_SHOWN; }
		inline void Show() const noexcept { SDL_ShowWindow(GetRawHandle()); }
		inline bool IsHidden() const noexcept { return SDL_GetWindowFlags(GetRawHandle()) & SDL_WINDOW_HIDDEN; }
		inline void Hide() const noexcept { SDL_HideWindow(GetRawHandle()); }

		inline void Raise() const noexcept { SDL_RaiseWindow(GetRawHandle()); }
		inline void Restore() const noexcept { SDL_RestoreWindow(GetRawHandle()); }

		inline bool IsGrabbed() const noexcept { return SDL_GetWindowGrab(GetRawHandle()); }
		inline void SetGrabbed(const bool isGrabbed) const noexcept { SDL_SetWindowGrab(GetRawHandle(), static_cast<SDL_bool>(isGrabbed)); }

	private:
		int GetWindowCoordinate(const Variant<int, Position>& windowCoordinate) const;
	};
}

#endif