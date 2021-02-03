#include "stardust\ui\components\ColourBlock.h"

namespace stardust
{
	namespace ui
	{
		ColourBlock::ColourBlock(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Component(canvas, anchor, anchorOffset), m_transform(IVec2Zero, createInfo.size),
			  m_colour(createInfo.enabledColour), m_enabledColour(createInfo.enabledColour), m_disabledColour(createInfo.disabledColour)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, createInfo.size, m_anchorOffset);
		}

		void ColourBlock::Render(Renderer& renderer)
		{
			renderer.DrawScreenRect(m_transform, m_colour);
		}

		void ColourBlock::OnEnable()
		{
			m_colour = m_enabledColour;
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
	}
}