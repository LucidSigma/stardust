#include "stardust/ui/components/Image.h"

namespace stardust
{
	namespace ui
	{
		Image::Image(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Component(canvas, anchor, anchorOffset),
			  m_transform(IVec2Zero, createInfo.size, createInfo.flip, createInfo.rotation, createInfo.pivot),
			  m_sprite(createInfo.enabledTexture, createInfo.enabledSubTextureArea, createInfo.enabledColourMod),
			  m_enabledTexture(createInfo.enabledTexture), m_disabledTexture(createInfo.disabledTexture),
			  m_enabledSubTextureArea(createInfo.enabledSubTextureArea), m_disabledSubTextureArea(createInfo.disabledSubTextureArea),
			  m_enabledColourMod(createInfo.enabledColourMod), m_disabledColourMod(createInfo.disabledColourMod)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Image::Render(Renderer& renderer)
		{
			renderer.DrawScreenRect(m_transform, m_sprite);
		}

		void Image::OnEnable()
		{
			m_sprite.texture = m_enabledTexture;
			m_sprite.subTextureArea = m_enabledSubTextureArea;
			m_sprite.colourMod = m_enabledColourMod;
		}

		void Image::OnDisable()
		{
			if (m_disabledTexture.has_value())
			{
				m_sprite.texture = m_disabledTexture.value();
			}

			if (m_disabledSubTextureArea.has_value())
			{
				m_sprite.subTextureArea = m_disabledSubTextureArea.value();
			}

			if (m_disabledColourMod.has_value())
			{
				m_sprite.colourMod = m_disabledColourMod.value();
			}
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