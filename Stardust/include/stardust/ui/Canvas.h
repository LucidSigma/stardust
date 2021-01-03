#pragma once
#ifndef STARDUST_CANVAS_H
#define STARDUST_CANVAS_H

#include "stardust/data/Pointers.h"
#include "stardust/graphics/renderer/Renderer.h"

namespace stardust
{
	namespace ui
	{
		class Canvas
		{
		private:
			ObserverPtr<Renderer> m_renderer = nullptr;

			bool m_isEnabled = true;

		public:
			Canvas() = default;
			Canvas(Renderer& renderer);
			~Canvas() noexcept = default;

			void Initialise(Renderer& renderer);

			inline bool IsEnabled() const noexcept { return m_isEnabled; }
			inline void Enable() noexcept { m_isEnabled = true; }
			inline void Disable() noexcept { m_isEnabled = false; }
		};
	}
}

#endif