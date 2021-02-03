#include "stardust/ui/components/Label.h"

namespace stardust
{
	namespace ui
	{
		Label::Label(const Canvas& canvas, const CreateInfo& createInfo, const String& text, const Anchor anchor, const IVec2& anchorOffset)
			: Label(canvas, createInfo, anchor, anchorOffset)
		{
			SetText(text);
		}

		Label::Label(const Canvas& canvas, const CreateInfo& createInfo, const text::TextInfo& textInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Label(canvas, createInfo, anchor, anchorOffset)
		{
			SetText(textInfo);
		}

		Label::Label(const Canvas& canvas, const CreateInfo& createInfo, const UTF16String& text, const Anchor anchor, const IVec2& anchorOffset)
			: Label(canvas, createInfo, anchor, anchorOffset)
		{
			SetText(text);
		}

		Label::Label(const Canvas& canvas, const CreateInfo& createInfo, const text::UTF16TextInfo& textInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Label(canvas, createInfo, anchor, anchorOffset)
		{
			SetText(textInfo);
		}

		void Label::Render(Renderer& renderer)
		{
			renderer.DrawScreenRect(m_transform, m_sprite);
		}

		void Label::OnEnable()
		{
			m_sprite.colourMod = m_enabledColour;
		}

		void Label::OnDisable()
		{
			if (m_disabledColour.has_value())
			{
				m_sprite.colourMod = m_disabledColour.value();
			}
		}

		void Label::OnCanvasResize(const UVec2&)
		{
			ResetPosition();
		}

		void Label::SetAnchor(const Anchor anchor) noexcept
		{
			m_anchor = anchor;
			ResetPosition();
		}

		void Label::SetAnchorOffset(const IVec2 & anchorOffset) noexcept
		{
			m_anchorOffset = anchorOffset;
			ResetPosition();
		}

		void Label::SetText(const String& text)
		{
			m_sprite.texture = m_textCache->Get(text);
			ResetSize();
			ResetPosition();

			m_UTF16textInfo = NullOpt;
			m_textInfo = text::TextInfo{
				.text = text,
				.outline = NullOpt,
				.wrapLength = NullOpt,
			};
		}

		void Label::SetText(const text::TextInfo& textInfo)
		{
			m_sprite.texture = m_textCache->Get(textInfo);
			ResetSize();
			ResetPosition();

			m_UTF16textInfo = NullOpt;
			m_textInfo = textInfo;
		}

		void Label::SetText(const UTF16String& text)
		{
			m_sprite.texture = m_textCache->Get(text);
			ResetSize();
			ResetPosition();

			m_textInfo = NullOpt;
			m_UTF16textInfo = text::UTF16TextInfo{
				.text = text,
				.outline = NullOpt,
				.wrapLength = NullOpt,
			};
		}

		void Label::SetText(const text::UTF16TextInfo& textInfo)
		{
			m_sprite.texture = m_textCache->Get(textInfo);
			ResetSize();
			ResetPosition();

			m_textInfo = NullOpt;
			m_UTF16textInfo = textInfo;
		}

		void Label::SetTextScale(const f32 textScale) noexcept
		{
			m_textScale = textScale;

			ResetSize();
			ResetPosition();
		}

		void Label::SetEnabledColour(const Colour& colour) noexcept
		{
			m_enabledColour = colour;

			if (m_isEnabled)
			{
				m_sprite.colourMod = m_enabledColour;
			}
		}

		void Label::SetDisabledColour(const Optional<Colour>&colour) noexcept
		{
			m_disabledColour = colour;

			if (m_disabledColour.has_value() && !m_isEnabled)
			{
				m_sprite.colourMod = m_disabledColour.value();
			}
		}

		Label::Label(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor, const IVec2& anchorOffset)
			: Component(canvas, anchor, anchorOffset), m_transform(IVec2Zero, UVec2Zero, createInfo.flip, createInfo.rotation, createInfo.pivot),
			  m_sprite(nullptr, NullOpt, createInfo.enabledColour),
			  m_textCache(createInfo.textCache), m_textScale(createInfo.textScale),
			  m_enabledColour(createInfo.enabledColour), m_disabledColour(createInfo.disabledColour)
		{ }

		void Label::ResetPosition()
		{
			m_transform.position = m_owningCanvas->GetPositionFromAnchor(m_anchor, m_transform.size, m_anchorOffset);
		}

		void Label::ResetSize()
		{
			m_transform.size = UVec2(Vec2(m_sprite.texture->GetSize()) * m_textScale);
		}
	}
}