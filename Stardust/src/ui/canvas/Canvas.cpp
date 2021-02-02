#include "stardust/ui/canvas/Canvas.h"

#include "stardust/data/Types.h"

namespace stardust
{
	namespace ui
	{
		Canvas::Canvas(const Renderer& renderer)
			: m_drawingSize(renderer.GetVirtualSize())
		{ }

		void Canvas::PollEvent(const SDL_Event& event)
		{
			if (m_isEnabled)
			{
				for (auto& [component, type] : m_components)
				{
					component->PollEvent(event);
				}
			}
		}

		void Canvas::Update()
		{
			if (m_isEnabled)
			{
				for (auto& [component, type] : m_components)
				{
					component->Update();
				}
			}
		}

		void Canvas::Render(Renderer & renderer)
		{
			if (m_isEnabled && m_isVisible)
			{
				for (auto& [component, type] : m_components)
				{
					component->Render(renderer);
				}
			}
		}

		void Canvas::AttachComponent(Component& component, const String type)
		{
			m_components.emplace(&component, type);
			component.OnAttach();
			component.SetOwningCanvas(this);
		}

		void Canvas::DetachComponent(Component& component)
		{
			if (m_components.contains(&component))
			{
				component.OnDetach();
				component.SetOwningCanvas(nullptr);
				m_components.erase(&component);
			}
		}

		[[nodiscard]] Vector<ObserverPtr<Component>> Canvas::GetComponents(const String& type)
		{
			Vector<ObserverPtr<Component>> components{ };

			for (const auto& [component, componentType] : m_components)
			{
				if (componentType == type)
				{
					components.push_back(component);
				}
			}

			return components;
		}

		[[nodiscard]] IVec2 Canvas::GetPositionFromAnchor(const Anchor anchor, const UVec2& componentSize, const IVec2& anchorOffset) const
		{
			switch (anchor)
			{
			case Anchor::TopLeft:
				return IVec2Zero + anchorOffset;

			case Anchor::TopCentre:
				return IVec2{
					static_cast<i32>(m_drawingSize.x / 2 - componentSize.x / 2),
					0,
				} + anchorOffset;

			case Anchor::TopRight:
				return IVec2{
					static_cast<i32>(m_drawingSize.x - componentSize.x),
					0,
				} + anchorOffset;

			case Anchor::CentreLeft:
				return IVec2{
					0,
					static_cast<i32>(m_drawingSize.y / 2 - componentSize.y / 2),
				} + anchorOffset;

			case Anchor::Centre:
				return IVec2{
					static_cast<i32>(m_drawingSize.x / 2 - componentSize.x / 2),
					static_cast<i32>(m_drawingSize.y / 2 - componentSize.y / 2),
				} + anchorOffset;

			case Anchor::CentreRight:
				return IVec2{
					static_cast<i32>(m_drawingSize.x - componentSize.x),
					static_cast<i32>(m_drawingSize.y / 2 - componentSize.y / 2),
				} + anchorOffset;

			case Anchor::BottomLeft:
				return IVec2{
					0,
					static_cast<i32>(m_drawingSize.y - componentSize.y),
				} + anchorOffset;

			case Anchor::BottomCentre:
				return IVec2{
					static_cast<i32>(m_drawingSize.x / 2 - componentSize.x / 2),
					static_cast<i32>(m_drawingSize.y - componentSize.y),
				} + anchorOffset;

			case Anchor::BottomRight:
				return IVec2{
					static_cast<i32>(m_drawingSize.x - componentSize.x),
					static_cast<i32>(m_drawingSize.y - componentSize.y),
				} + anchorOffset;

			default:
				return IVec2Zero;
			}
		}

		void Canvas::SetEnabled(const bool isEnabled)
		{
			m_isEnabled = isEnabled;

			if (m_isEnabled)
			{
				for (auto& [component, type] : m_components)
				{
					component->OnCanvasEnable();
				}
			}
			else
			{
				for (auto& [component, type] : m_components)
				{
					component->OnCanvasDisable();
				}
			}
		}
	}
}