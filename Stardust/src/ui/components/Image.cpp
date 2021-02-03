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

		void Image::SetEnabledTexture(const ObserverPtr<const Texture> texture) noexcept
		{
			m_enabledTexture = texture;

			if (m_isEnabled)
			{
				m_sprite.texture = m_enabledTexture;
			}
		}

		void Image::SetDisabledTexture(const Optional<ObserverPtr<const Texture>>& texture) noexcept
		{
			m_disabledTexture = texture;

			if (m_disabledTexture.has_value() && !m_isEnabled)
			{
				m_sprite.texture = m_disabledTexture.value();
			}
		}

		void Image::SetEnabledSubTextureArea(const Optional<TextureCoordinatePair>& textureCoordinates) noexcept
		{
			m_enabledSubTextureArea = textureCoordinates;

			if (m_isEnabled)
			{
				m_sprite.subTextureArea = m_enabledSubTextureArea;
			}
		}

		void Image::SetDisabledSubTextureArea(const Optional<Optional<TextureCoordinatePair>>& textureCoordinates) noexcept
		{
			m_disabledSubTextureArea = textureCoordinates;

			if (m_disabledSubTextureArea.has_value() && !m_isEnabled)
			{
				m_sprite.subTextureArea = m_disabledSubTextureArea.value();
			}
		}

		void Image::SetEnabledColourMod(const Colour& colourMod) noexcept
		{
			m_enabledColourMod = colourMod;

			if (m_isEnabled)
			{
				m_sprite.colourMod = m_enabledColourMod;
			}
		}

		void Image::SetDisabledColourMod(const Optional<Colour>& colourMod) noexcept
		{
			m_disabledColourMod = colourMod;

			if (m_disabledColourMod.has_value() && !m_isEnabled)
			{
				m_sprite.colourMod = m_disabledColourMod.value();
			}
		}
	}
}