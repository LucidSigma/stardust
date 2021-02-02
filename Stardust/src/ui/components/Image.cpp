#include "stardust/ui/components/Image.h"

namespace stardust
{
	namespace ui
	{
		Image::Image(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Component(canvas, anchor, anchorOffset),
			  m_transform(IVec2Zero, createInfo.size, createInfo.flip, createInfo.rotation, createInfo.pivot),
			  m_sprite(createInfo.texture, createInfo.subTextureArea, createInfo.colourMod)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Image::Render(Renderer& renderer)
		{
			renderer.DrawScreenRect(m_transform, m_sprite);
		}

		void Image::OnCanvasResize(const UVec2&)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Image::SetAnchor(const Anchor anchor) noexcept
		{
			m_anchor = anchor;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Image::SetAnchorOffset(const IVec2& anchorOffset) noexcept
		{
			m_anchorOffset = anchorOffset;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Image::SetSize(const UVec2& size)
		{
			m_transform.size = size;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}
	}
}