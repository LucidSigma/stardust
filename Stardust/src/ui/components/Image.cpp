#include "stardust/ui/components/Image.h"

#include "stardust/input/Input.h"

namespace stardust
{
	namespace ui
	{
		Image::Image(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Component(canvas, anchor, anchorOffset),
			  m_transform(IVec2Zero, createInfo.size, createInfo.flip, createInfo.rotation, createInfo.pivot),
			  m_sprite(createInfo.texture, createInfo.subTextureArea, createInfo.enabledColourMod),
			  m_enabledColourMod(createInfo.enabledColourMod), m_disabledColourMod(createInfo.disabledColourMod), m_hoverColourMod(createInfo.hoverColourMod),
			  m_renderer(createInfo.renderer)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Image::ProcessInput()
		{
			const IVec2 mouseCoordinates = IVec2(Input::GetMouseState().GetProportionalCoordinates(*m_renderer));

			m_previousIsHoveredOver = m_isHoveredOver;
			m_isHoveredOver = mouseCoordinates.x >= m_transform.position.x && mouseCoordinates.x <= m_transform.position.x + m_transform.size.x
				&& mouseCoordinates.y >= m_transform.position.y && mouseCoordinates.y <= m_transform.position.y + m_transform.size.y;
		}

		void Image::Update(const f32)
		{
			if (m_previousIsHoveredOver != m_isHoveredOver)
			{
				if (m_isHoveredOver && m_hoverColourMod.has_value())
				{
					m_sprite.colourMod = m_hoverColourMod.value();
				}
				else
				{
					m_sprite.colourMod = m_enabledColourMod;
				}
			}
		}

		void Image::Render(Renderer& renderer)
		{
			renderer.DrawScreenRect(m_transform, m_sprite);
		}

		void Image::OnEnable()
		{
			if (m_isHoveredOver && m_hoverColourMod.has_value())
			{
				m_sprite.colourMod = m_hoverColourMod.value();
			}
			else
			{
				m_sprite.colourMod = m_enabledColourMod;
			}
		}

		void Image::OnDisable()
		{
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

		void Image::SetHoverColourMod(const Optional<Colour>& colour) noexcept
		{
			m_hoverColourMod = colour;

			if (m_isHoveredOver && m_hoverColourMod.has_value())
			{
				m_sprite.colourMod = m_hoverColourMod.value();
			}
		}
	}
}