#pragma once
#ifndef STARDUST_CANVAS_H
#define STARDUST_CANVAS_H

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/math/Math.h"
#include "stardust/ui/Anchor.h"
#include "stardust/ui/components/UIComponent.h"

namespace stardust
{
	namespace ui
	{
		class Canvas
		{
		private:
			HashMap<ObserverPtr<Component>, String> m_components{ };

			bool m_isEnabled = true;
			bool m_isVisible = true;

			UVec2 m_drawingSize = UVec2Zero;

		public:
			Canvas(const Renderer& renderer);
			~Canvas() noexcept = default;

			void PollEvent(const SDL_Event& event);
			void Update();
			void Render(Renderer& renderer);

			void AttachComponent(Component& component, const String type);
			void DetachComponent(Component& component);
			inline HashMap<ObserverPtr<Component>, String>& GetComponents() noexcept { return m_components; }
			inline const HashMap<ObserverPtr<Component>, String>& GetComponents() const noexcept { return m_components; }
			[[nodiscard]] Vector<ObserverPtr<Component>> GetComponents(const String& type);

			[[nodiscard]] IVec2 GetPositionFromAnchor(const Anchor anchor, const UVec2& componentSize, const IVec2& anchorOffset = IVec2Zero) const;

			inline bool IsEnabled() const noexcept { return m_isEnabled; }
			void SetEnabled(const bool isEnabled);

			inline bool IsVisible() const noexcept { return m_isVisible; }
			inline void SetVisibility(const bool isVisible) noexcept { m_isVisible = isVisible; }

			inline const UVec2& GetDrawingSize() const noexcept { return m_drawingSize; }
			inline void SetDrawingSize(const UVec2& drawingSize) noexcept { m_drawingSize = drawingSize; }
		};
	}
}

#endif