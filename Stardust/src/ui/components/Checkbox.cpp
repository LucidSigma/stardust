#include "stardust/ui/components/Checkbox.h"

#include "stardust/input/mouse/MouseButtonCodes.h"
#include "stardust/input/Input.h"

namespace stardust
{
	namespace ui
	{
		Checkbox::Checkbox(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Component(canvas, anchor, anchorOffset),
			  m_transform(IVec2Zero, createInfo.size, createInfo.flip, createInfo.rotation, createInfo.pivot),
			  m_sprite(createInfo.texture, createInfo.subTextureArea, createInfo.enabledColourMod), m_checkedSprite(createInfo.checkOverlayTexture, createInfo.checkOverlaySubTextureArea, createInfo.enabledColourMod),
			  m_enabledColourMod(createInfo.enabledColourMod), m_disabledColourMod(createInfo.disabledColourMod),
			  m_renderer(createInfo.renderer)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Checkbox::ProcessInput()
		{
			const IVec2 mouseCoordinates = IVec2(Input::GetMouseState().GetProportionalCoordinates(*m_renderer));

			m_isHoveredOver = mouseCoordinates.x >= m_transform.position.x && mouseCoordinates.x <= m_transform.position.x + m_transform.size.x
				&& mouseCoordinates.y >= m_transform.position.y && mouseCoordinates.y <= m_transform.position.y + m_transform.size.y;

			if (m_isHoveredOver && Input::GetMouseState().IsButtonDown(MouseButton::Left))
			{
				m_isChecked = !m_isChecked;

				const auto& callbacks = m_isChecked ? m_onCheckCallbacks : m_onUncheckCallbacks;

				for (const auto& callback : callbacks)
				{
					callback();
				}
			}
		}

		void Checkbox::Render(Renderer& renderer)
		{
			renderer.DrawScreenRect(m_transform, m_sprite);

			if (m_isChecked)
			{
				renderer.DrawScreenRect(m_transform, m_checkedSprite);
			}
		}

		void Checkbox::OnEnable()
		{
			m_sprite.colourMod = m_enabledColourMod;
			m_checkedSprite.colourMod = m_enabledColourMod;
		}

		void Checkbox::OnDisable()
		{
			if (m_disabledColourMod.has_value())
			{
				m_sprite.colourMod = m_disabledColourMod.value();
				m_checkedSprite.colourMod = m_disabledColourMod.value();
			}
		}

		void Checkbox::OnCanvasResize(const UVec2&)
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Checkbox::AddOnCheckCallback(const Callback& callback)
		{
			m_onCheckCallbacks.push_back(callback);
		}

		void Checkbox::AddOnUncheckCallback(const Callback& callback)
		{
			m_onUncheckCallbacks.push_back(callback);
		}

		void Checkbox::ToggleChecked()
		{
			SetChecked(!m_isChecked);
		}

		void Checkbox::SetChecked(const bool isChecked)
		{
			if (m_isChecked == isChecked)
			{
				return;
			}

			m_isChecked = isChecked;
			const auto& callbacks = m_isChecked ? m_onCheckCallbacks : m_onUncheckCallbacks;

			for (const auto& callback : callbacks)
			{
				callback();
			}
		}

		void Checkbox::SetAnchor(const Anchor anchor) noexcept
		{
			m_anchor = anchor;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Checkbox::SetAnchorOffset(const IVec2& anchorOffset) noexcept
		{
			m_anchorOffset = anchorOffset;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Checkbox::SetSize(const UVec2& size)
		{
			m_transform.size = size;
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Checkbox::SetTexture(const ObserverPtr<const Texture> texture) noexcept
		{
			m_sprite.texture = texture;
		}

		void Checkbox::SetCheckedTexture(const ObserverPtr<const Texture> texture) noexcept
		{
			m_checkedSprite.texture = texture;
		}

		void Checkbox::SetSubTextureArea(const Optional<TextureCoordinatePair>& textureCoordinates) noexcept
		{
			m_sprite.subTextureArea = textureCoordinates;
		}

		void Checkbox::SetCheckedSubTextureArea(const Optional<TextureCoordinatePair>& textureCoordinates) noexcept
		{
			m_checkedSprite.subTextureArea = textureCoordinates;
		}

		void Checkbox::SetEnabledColourMod(const Colour& colourMod) noexcept
		{
			m_enabledColourMod = colourMod;

			if (m_isEnabled)
			{
				m_sprite.colourMod = m_enabledColourMod;
				m_checkedSprite.colourMod = m_enabledColourMod;
			}
		}

		void Checkbox::SetDisabledColourMod(const Optional<Colour>& colourMod) noexcept
		{
			m_disabledColourMod = colourMod;

			if (m_disabledColourMod.has_value() && !m_isEnabled)
			{
				m_sprite.colourMod = m_disabledColourMod.value();
				m_checkedSprite.colourMod = m_disabledColourMod.value();
			}
		}
	}
}