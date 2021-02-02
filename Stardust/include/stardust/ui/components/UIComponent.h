#pragma once
#ifndef STARDUST_UI_COMPONENT_H
#define STARDUST_UI_COMPONENT_H

#include "stardust/data/Pointers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/math/Math.h"
#include "stardust/graphics/renderer/Renderer.h"

namespace stardust
{
	namespace ui
	{
		class Component
		{
		protected:
			bool m_isVisible = true;
			IVec2 m_position = IVec2Zero;

			ObserverPtr<const class Canvas> m_owningCanvas = nullptr;

		public:
			Component(const class Canvas& canvas);
			virtual ~Component() noexcept = default;

			inline virtual void Update() { }
			virtual void Render(Renderer& renderer) = 0;

			inline virtual void OnClick() { }
			inline virtual void OnClickRelease() { }

			inline virtual void OnMouseEnter() { }
			inline virtual void OnMouseExit() { }
			inline virtual void DoesContainMouse() const = 0;

			inline virtual void OnKeyUp() { }
			inline virtual void OnKeyDown() { }
			inline virtual void OnTextInput() { }

			inline virtual void OnAttach() { }
			inline virtual void OnDetach() { }

			inline virtual void OnCanvasEnable() { }
			inline virtual void OnCanvasDisable() { }

			inline bool IsVisible() const noexcept { return m_isVisible; }
			inline void SetVisibility(const bool isVisible) noexcept { m_isVisible = isVisible; }

			inline const IVec2& GetPosition() const noexcept { return m_position; }
			inline void SetPosition(const IVec2& position) noexcept { m_position = position; }

			inline const class Canvas& GetOwningCanvas() const noexcept { return *m_owningCanvas; }
		};
	}
}

#endif