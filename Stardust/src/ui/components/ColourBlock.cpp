#include "stardust\ui\components\ColourBlock.h"

namespace stardust
{
	namespace ui
	{
		ColourBlock::ColourBlock(const Canvas& canvas, const Colour& colour, const UVec2& size, const Anchor anchor, const IVec2& anchorOffset)
			: Component(canvas, anchor, anchorOffset), m_transform(IVec2Zero, size), m_colour(colour)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, size, m_anchorOffset);
		}

		void ColourBlock::Render(Renderer& renderer)
		{
			renderer.DrawScreenRect(m_transform, m_colour);
		}

		void ColourBlock::OnCanvasResize(const UVec2&)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void ColourBlock::SetAnchorOffset(const IVec2& anchorOffset) noexcept
		{
			Component::SetAnchorOffset(anchorOffset);

			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void ColourBlock::SetAnchor(const Anchor anchor) noexcept
		{
			Component::SetAnchor(anchor);

			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void ColourBlock::SetSize(const UVec2& size)
		{
			m_transform.size = size;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}
	}
}