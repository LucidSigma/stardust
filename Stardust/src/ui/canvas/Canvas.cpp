#include "stardust/ui/canvas/Canvas.h"

namespace stardust
{
	namespace ui
	{
		Canvas::Canvas(const UVec2& drawingSize)
		{
			Initialise(drawingSize);
		}

		Canvas::Canvas(const Renderer& renderer)
		{
			Initialise(renderer);
		}

		void Canvas::Initialise(const UVec2& drawingSize)
		{
			SetDrawingSize(drawingSize);
		}

		void Canvas::Initialise(const Renderer& renderer)
		{
			SetDrawingSize(renderer.GetVirtualSize());
		}

		void Canvas::PollEvent(const SDL_Event& event)
		{
			if (m_isEnabled)
			{
				for (auto& [name, component] : m_components)
				{
					if (component->IsEnabled())
					{
						component->PollEvent(event);
					}
				}
			}
		}

		void Canvas::ProcessInput()
		{
			if (m_isEnabled)
			{
				for (auto& [name, component] : m_components)
				{
					if (component->IsEnabled())
					{
						component->ProcessInput();
					}
				}
			}
		}

		void Canvas::Update(const f32 deltaTime)
		{
			if (m_isEnabled)
			{
				for (auto& [name, component] : m_components)
				{
					if (component->IsEnabled())
					{
						component->Update(deltaTime);
					}
				}
			}
		}

		void Canvas::Render(Renderer& renderer)
		{
			if (m_isVisible)
			{
				for (auto& [name, component] : m_components)
				{
					if (component->IsVisible())
					{
						component->Render(renderer);
					}
				}
			}
		}

		void Canvas::DetachComponent(const String& name)
		{
			if (const auto componentLocation = m_components.find(name);
				componentLocation != std::cend(m_components))
			{
				componentLocation->second->OnDetach();
				m_components.erase(componentLocation);
			}
		}

		[[nodiscard]] ObserverPtr<Component> Canvas::GetComponent(const String& name)
		{
			if (const auto componentLocation = m_components.find(name);
				componentLocation != std::cend(m_components))
			{
				return componentLocation->second.get();
			}
			else
			{
				return nullptr;
			}
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
				for (auto& [name, component] : m_components)
				{
					component->SetEnabled(true);
				}
			}
			else
			{
				for (auto& [name, component] : m_components)
				{
					component->SetEnabled(false);
				}
			}
		}

		void Canvas::SetDrawingSize(const UVec2& drawingSize)
		{
			m_drawingSize = drawingSize;

			for (auto& [name, component] : m_components)
			{
				component->OnCanvasResize(m_drawingSize);
			}
		}
	}
}