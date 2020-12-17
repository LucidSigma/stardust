#include "stardust/window/Window.h"

#include <algorithm>
#include <cstdint>
#include <utility>

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

	Window::Window()
	{ }

	Window::Window(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	Window::Window(Window&& other) noexcept
		: m_handle(nullptr), m_size(glm::uvec2{ 0u, 0u }), m_sizeBeforeFullscreen(NullOpt), m_fullscreenType(FullscreenType::Hard), m_isFullscreen(false), m_hasUpdatedFullscreen(false)
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

		m_size = std::exchange(other.m_size, glm::uvec2{ 0u, 0u });
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
				windowFlags |= static_cast<std::uint32_t>(windowFlag);
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

			m_size = glm::uvec2{ 0u, 0u };
			m_sizeBeforeFullscreen = NullOpt;
			m_isFullscreen = false;
			m_hasUpdatedFullscreen = false;
		}
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
			// const display::DisplayData displayData = display::GetDisplayData(GetDisplayIndex());
			// 
			// m_sizeBeforeFullscreen = m_size;
			// m_size = displayData.size;
			// 
			// SDL_SetWindowSize(GetRawHandle(), m_size.x, m_size.y);
			// SDL_SetWindowFullscreen(GetRawHandle(), static_cast<std::uint32_t>(m_fullscreenType));
			// 
			// m_hasUpdatedFullscreen = true;
		}

		m_isFullscreen = !m_isFullscreen;
	}

	void Window::ChangeSize(const glm::uvec2& newSize)
	{
		SDL_SetWindowSize(GetRawHandle(), newSize.x, newSize.y);
		ProcessResize(newSize);
	}

	void Window::ProcessResize(const glm::uvec2& newSize)
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

	f32 Window::GetOpacity() const noexcept
	{
		f32 opacity = 0.0f;
		SDL_GetWindowOpacity(GetRawHandle(), &opacity);

		return opacity;
	}

	void Window::SetOpacity(const f32 opacity) const noexcept
	{
		SDL_SetWindowOpacity(GetRawHandle(), std::clamp(opacity, 0.0f, 1.0f));
	}

	//void Window::SetIcon(const std::string_view& iconFilepath, const Locale& locale) const
	//{
	//	const std::vector<std::byte> rawIconData = vfs::ReadFileData(iconFilepath);
	//
	//	if (rawIconData.empty())
	//	{
	//		message_box::Show(locale["warnings"]["titles"]["window"], locale["warnings"]["bodies"]["window-icon-load"], message_box::Type::Warning);
	//		Log::EngineWarn("Failed to open window icon file at {}.", iconFilepath);
	//
	//		return;
	//	}
	//
	//	int iconWidth = 0;
	//	int iconHeight = 0;
	//	stbi_uc* iconData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(rawIconData.data()), static_cast<int>(rawIconData.size()), &iconWidth, &iconHeight, nullptr, STBI_rgb_alpha);
	//
	//	if (iconData == nullptr)
	//	{
	//		message_box::Show(locale["warnings"]["titles"]["window"], locale["warnings"]["bodies"]["window-icon-load"], message_box::Type::Warning);
	//		Log::EngineWarn("Failed to open window icon file at {}.", iconFilepath);
	//
	//		return;
	//	}
	//
	//	const PixelSurface iconSurface(iconWidth, iconHeight, 4u, iconData);
	//
	//	if (!iconSurface.IsValid())
	//	{
	//		message_box::Show(locale["warnings"]["titles"]["window"], locale["warnings"]["bodies"]["window-icon-convert"], message_box::Type::Warning);
	//		Log::EngineWarn("Could not convert window icon image: {}.", SDL_GetError());
	//
	//		stbi_image_free(iconData);
	//		iconData = nullptr;
	//
	//		return;
	//	}
	//
	//	SDL_SetWindowIcon(GetRawHandle(), iconSurface.GetRawHandle());
	//
	//	stbi_image_free(iconData);
	//	iconData = nullptr;
	//}

	[[nodiscard]] glm::uvec2 Window::GetMinimumSize() const noexcept
	{
		i32 minimumX = 0;
		i32 minimumY = 0;
		SDL_GetWindowMinimumSize(GetRawHandle(), &minimumX, &minimumY);

		return glm::uvec2{ minimumX, minimumY };
	}

	[[nodiscard]] glm::uvec2 Window::GetMaximumSize() const noexcept
	{
		i32 maximumX = 0;
		i32 maximumY = 0;
		SDL_GetWindowMaximumSize(GetRawHandle(), &maximumX, &maximumY);

		return glm::uvec2{ maximumX, maximumY };
	}

	[[nodiscard]] glm::ivec2 Window::GetPosition() const noexcept
	{
		i32 x = 0;
		i32 y = 0;
		SDL_GetWindowPosition(GetRawHandle(), &x, &y);

		return glm::ivec2{ x, y };
	}

	i32 Window::GetWindowCoordinate(const Variant<i32, Position>& windowCoordinate) const
	{
		if (const Position* windowPosition = eastl::get_if<Position>(&windowCoordinate);
			windowPosition != nullptr) [[likely]]
		{
			return static_cast<int>(*windowPosition);
		}
		else
		{
			return eastl::get<int>(windowCoordinate);
		}
	}
}