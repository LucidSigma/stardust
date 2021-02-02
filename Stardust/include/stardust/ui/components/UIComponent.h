#pragma once
#ifndef STARDUST_UI_COMPONENT_H
#define STARDUST_UI_COMPONENT_H

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/math/Math.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/ui/Anchor.h"

namespace stardust
{
	namespace ui
	{
		class Component
		{
		protected:
			bool m_isEnabled = true;
			bool m_isVisible = true;

			Anchor m_anchor = Anchor::Centre;
			IVec2 m_anchorOffset = IVec2Zero;

			ObserverPtr<const class Canvas> m_owningCanvas = nullptr;

		public:
			Component(const class Canvas& canvas);
			virtual ~Component() noexcept = default;

			virtual void PollEvent(const SDL_Event& event) = 0;
			inline virtual void Update() { }
			virtual void Render(Renderer& renderer) = 0;

			inline virtual void OnAttach() { }
			inline virtual void OnDetach() { }

			inline virtual void OnEnable() { }
			inline virtual void OnDisable() { }
			inline virtual void OnCanvasEnable() { }
			inline virtual void OnCanvasDisable() { }

			inline bool IsEnabled() const noexcept { return m_isEnabled; }
			void SetEnabled(const bool isEnabled);

			inline bool IsVisible() const noexcept { return m_isVisible; }
			inline void SetVisibility(const bool isVisible) noexcept { m_isVisible = isVisible; }

			inline Anchor GetAnchor() const noexcept { return m_anchor; }
			inline void SetAnchor(const Anchor anchor) noexcept { m_anchor = anchor; }
			inline const IVec2& GetAnchorOffset() const noexcept { return m_anchorOffset; }
			inline void GetAnchorOffset(const IVec2& anchorOffset) noexcept { m_anchorOffset = anchorOffset; }

			inline const class Canvas& GetOwningCanvas() const noexcept { return *m_owningCanvas; }
		};
	}
}

#endif