#pragma once
#ifndef STARDUST_CANVAS_H
#define STARDUST_CANVAS_H

#include <concepts>
#include <type_traits>
#include <utility>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
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
			HashMap<String, UniquePtr<Component>> m_components{ };

			bool m_isEnabled = true;
			bool m_isVisible = true;

			UVec2 m_drawingSize = UVec2Zero;

		public:
			Canvas() = default;
			explicit Canvas(const UVec2& drawingSize);
			explicit Canvas(const Renderer& renderer);
			~Canvas() noexcept = default;

			void Initialise(const UVec2& drawingSize);
			void Initialise(const Renderer& renderer);

			void PollEvent(const SDL_Event& event);
			void ProcessInput();
			void Update(const f32 deltaTime);
			void Render(Renderer& renderer);

			template <std::derived_from<Component> T, typename... Args, typename = std::enable_if_t<std::is_constructible_v<T, Canvas, Args...>>>
			ObserverPtr<Component> AttachComponent(const String name, Args&&... args)
			{
				m_components[name] = std::make_unique<T>(*this, std::forward<Args>(args)...);
				m_components[name]->OnAttach();

				return m_components[name].get();
			}

			void DetachComponent(const String& name);
			inline HashMap<String, UniquePtr<Component>>& GetComponents() noexcept { return m_components; }
			inline const HashMap<String, UniquePtr<Component>>& GetComponents() const noexcept { return m_components; }
			[[nodiscard]] ObserverPtr<Component> GetComponent(const String& name);
			[[nodiscard]] bool HasComponent(const String& name) const { return m_components.contains(name); }

			[[nodiscard]] IVec2 GetPositionFromAnchor(const Anchor anchor, const UVec2& componentSize, const IVec2& anchorOffset = IVec2Zero) const;

			inline bool IsEnabled() const noexcept { return m_isEnabled; }
			void SetEnabled(const bool isEnabled);

			inline bool IsVisible() const noexcept { return m_isVisible; }
			inline void SetVisibility(const bool isVisible) noexcept { m_isVisible = isVisible; }

			inline const UVec2& GetDrawingSize() const noexcept { return m_drawingSize; }
			void SetDrawingSize(const UVec2& drawingSize);
		};
	}
}

#endif