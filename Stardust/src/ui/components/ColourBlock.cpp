#include "stardust/ui/components/ColourBlock.h"

#include "stardust/graphics/renderer/FlipType.h"
#include "stardust/input/Input.h"

namespace stardust
{
	namespace ui
	{
		ColourBlock::ColourBlock(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Component(canvas, anchor, anchorOffset), m_transform(IVec2Zero, createInfo.size, FlipType::None, createInfo.rotation, createInfo.pivot),
			  m_colour(createInfo.enabledColour), m_enabledColour(createInfo.enabledColour), m_disabledColour(createInfo.disabledColour), m_hoverColour(createInfo.hoverColour),
			  m_renderer(createInfo.renderer)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, createInfo.size, m_anchorOffset);
		}

		void ColourBlock::ProcessInput()
		{
			const IVec2 mouseCoordinates = IVec2(Input::GetMouseState().GetProportionalCoordinates(*m_renderer));

			m_previousIsHoveredOver = m_isHoveredOver;
			m_isHoveredOver = mouseCoordinates.x >= m_transform.position.x && mouseCoordinates.x <= m_transform.position.x + m_transform.size.x
				&& mouseCoordinates.y >= m_transform.position.y && mouseCoordinates.y <= m_transform.position.y + m_transform.size.y;
		}

		void ColourBlock::Update(const f32)
		{
			if (m_previousIsHoveredOver != m_isHoveredOver)
			{
				if (m_isHoveredOver && m_hoverColour.has_value())
				{
					m_colour = m_hoverColour.value();
				}
				else
				{
					m_colour = m_enabledColour;
				}
			}
		}

		void ColourBlock::Render(Renderer& renderer)
		{
			renderer.DrawScreenRect(m_transform, m_colour);
		}

		void ColourBlock::OnEnable()
		{
			if (m_isHoveredOver && m_hoverColour.has_value())
			{
				m_colour = m_hoverColour.value();
			}
			else
			{
				m_colour = m_enabledColour;
			}
		}

		void ColourBlock::OnDisable()
		{
			if (m_disabledColour.has_value())
			{
				m_colour = m_disabledColour.value();
			}
		}

		void ColourBlock::OnCanvasResize(const UVec2&)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void ColourBlock::SetAnchor(const Anchor anchor) noexcept
		{
			m_anchor = anchor;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void ColourBlock::SetAnchorOffset(const IVec2& anchorOffset) noexcept
		{
			m_anchorOffset = anchorOffset;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void ColourBlock::SetSize(const UVec2& size)
		{
			m_transform.size = size;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void ColourBlock::SetEnabledColour(const Colour& colour) noexcept
		{
			m_enabledColour = colour;

			if (m_isEnabled)
			{
				m_colour = m_enabledColour;
			}
		}

		void ColourBlock::SetDisabledColour(const Optional<Colour>& colour) noexcept
		{
			m_disabledColour = colour;

			if (m_disabledColour.has_value() && !m_isEnabled)
			{
				m_colour = m_disabledColour.value();
			}
		}

		void ColourBlock::SetHoverColour(const Optional<Colour>& colour) noexcept
		{
			m_hoverColour = colour;

			if (m_isHoveredOver && m_hoverColour.has_value())
			{
				m_colour = m_hoverColour.value();
			}
		}
	}
}