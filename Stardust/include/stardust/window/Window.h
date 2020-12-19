#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/locale/Locale.h"

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
			OpenGL = SDL_WINDOW_OPENGL,
			Vulkan = SDL_WINDOW_VULKAN,
		};

		enum class FullscreenType
			: std::underlying_type_t<SDL_WindowFlags>
		{
			Hard = SDL_WINDOW_FULLSCREEN,
			Soft = SDL_WINDOW_FULLSCREEN_DESKTOP,
		};

		struct CreateInfo
		{
			StringView title;

			Variant<i32, Position> x = Position::Centred;
			Variant<i32, Position> y = Position::Centred;
			UVec2 size;

			Vector<CreateFlag> flags;

			Optional<UVec2> minimumSize = NullOpt;
			Optional<UVec2> maximumSize = NullOpt;
		};

	private:
		struct WindowDestroyer
		{
			void operator ()(SDL_Window* const window) const noexcept;
		};

		UniquePtr<SDL_Window, WindowDestroyer> m_handle = nullptr;

		UVec2 m_size{ 0u, 0u };
		Optional<UVec2> m_sizeBeforeFullscreen = NullOpt;

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

		void ChangeSize(const UVec2& newSize);
		void ProcessResize(const UVec2& newSize);

		f32 GetOpacity() const noexcept;
		void SetOpacity(const f32 opacity) const noexcept;

		void SetIcon(const StringView& iconFilepath, const Locale& locale) const;

		inline bool IsValid() const noexcept { return m_handle != nullptr; }

		inline const UVec2& GetSize() const noexcept { return m_size; }
		[[nodiscard]] inline f32 GetAspectRatio() const noexcept { return static_cast<f32>(m_size.x) / static_cast<f32>(m_size.y); }

		[[nodiscard]] UVec2 GetMinimumSize() const noexcept;
		void SetMinimumSize(const UVec2& minimumSize) const noexcept;
		[[nodiscard]] UVec2 GetMaximumSize() const noexcept;
		void SetMaximumSize(const UVec2& maximumSize) const noexcept;

		[[nodiscard]] IVec2 GetPosition() const noexcept;
		void SetPosition(const IVec2& position) const noexcept;

		[[nodiscard]] String GetTitle() const noexcept;
		void SetTitle(const StringView& title) const noexcept;

		inline bool IsFullscreen() const noexcept { return m_isFullscreen; }
		inline FullscreenType GetFullscreenType() const noexcept { return m_fullscreenType; }
		inline void SetFullscreenType(const FullscreenType fullscreenType) noexcept { m_fullscreenType = fullscreenType; }

		bool IsBorderless() const noexcept;
		void SetBorderless(const bool isBorderless) const noexcept;

		[[nodiscard]] inline SDL_Window* const GetRawHandle() const noexcept { return m_handle.get(); }
		[[nodiscard]] SDL_Surface* GetSurface() const noexcept;
		[[nodiscard]] u32 GetPixelFormat() const noexcept;
		i32 GetDisplayIndex() const noexcept;

		bool IsMinimised() const noexcept;
		void Minimise() const noexcept;
		bool IsMaximised() const noexcept;
		void Maximise() const noexcept;

		bool IsResizable() const noexcept;
		void SetResizable(const bool isResizable) const noexcept;

		bool IsShown() const noexcept;
		void Show() const noexcept;
		bool IsHidden() const noexcept;
		void Hide() const noexcept;

		void Raise() const noexcept;
		void Restore() const noexcept;

		bool IsGrabbed() const noexcept;
		void SetGrabbed(const bool isGrabbed) const noexcept;

	private:
		i32 GetWindowCoordinate(const Variant<i32, Position>& windowCoordinate) const;
	};
}

#endif