#pragma once
#ifndef STARDUST_LABEL_H
#define STARDUST_LABEL_H

#include "stardust/ui/components/UIComponent.h"

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/FlipType.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/Colour.h"
#include "stardust/math/Math.h"
#include "stardust/scene/components/ScreenTransformComponent.h"
#include "stardust/scene/components/SpriteComponent.h"
#include "stardust/text/TextCache.h"
#include "stardust/text/Text.h"
#include "stardust/ui/Anchor.h"
#include "stardust/ui/canvas/Canvas.h"

namespace stardust
{
	namespace ui
	{
		class Label
			: public Component
		{
		public:
			struct CreateInfo
			{
				ObserverPtr<TextCache> textCache;
				f32 textScale;

				FlipType flip;
				f32 rotation;
				Optional<IVec2> pivot;

				Colour enabledColour;
				Optional<Colour> disabledColour;
			};

		private:
			components::ScreenTransform m_transform;
			components::Sprite m_sprite;

			ObserverPtr<TextCache> m_textCache;
			f32 m_textScale;

			Colour m_enabledColour;
			Optional<Colour> m_disabledColour;

			Optional<text::TextInfo> m_textInfo = NullOpt;
			Optional<text::UTF16TextInfo> m_UTF16textInfo = NullOpt;

		public:
			Label(const Canvas& canvas, const CreateInfo& createInfo, const String& text, const Anchor anchor = Anchor::Centre, const IVec2& anchorOffset = IVec2Zero);
			Label(const Canvas& canvas, const CreateInfo& createInfo, const text::TextInfo& textInfo, const Anchor anchor = Anchor::Centre, const IVec2& anchorOffset = IVec2Zero);
			Label(const Canvas& canvas, const CreateInfo& createInfo, const UTF16String& text, const Anchor anchor = Anchor::Centre, const IVec2& anchorOffset = IVec2Zero);
			Label(const Canvas& canvas, const CreateInfo& createInfo, const text::UTF16TextInfo& textInfo, const Anchor anchor = Anchor::Centre, const IVec2& anchorOffset = IVec2Zero);
			virtual ~Label() noexcept override = default;

			virtual void Render(Renderer& renderer) override;

			virtual void OnEnable() override;
			virtual void OnDisable() override;

			virtual void OnCanvasResize(const UVec2&) override;

			virtual void SetAnchor(const Anchor anchor) noexcept override;
			virtual void SetAnchorOffset(const IVec2& anchorOffset) noexcept override;

			const Optional<text::TextInfo>& GetText() const noexcept { return m_textInfo; }
			const Optional<text::UTF16TextInfo>& GetUTF16Text() const noexcept { return m_UTF16textInfo; }

			void SetText(const String& text);
			void SetText(const text::TextInfo& textInfo);
			void SetText(const UTF16String& text);
			void SetText(const text::UTF16TextInfo& textInfo);

			inline f32 GetTextScale() const noexcept { return m_textScale; }
			void SetTextScale(const f32 textScale) noexcept;

			inline const IVec2& GetPosition() const noexcept { return m_transform.position; }
			inline const UVec2& GetSize() const noexcept { return m_transform.size; }

			inline FlipType GetFlip() const noexcept { return m_transform.flip; }
			inline void SetFlip(const FlipType flip) noexcept { m_transform.flip = flip; }

			inline f32 GetRotation() const noexcept { return m_transform.rotation; }
			inline void SetRotation(const f32 rotation) noexcept { m_transform.rotation = rotation; }

			inline const Optional<IVec2>& GetPivot() const noexcept { return m_transform.pivot; }
			inline void SetPivot(const Optional<IVec2>& pivot) noexcept { m_transform.pivot = pivot; }

			inline ObserverPtr<const Texture> GetTexture() const noexcept { return m_sprite.texture; }

			inline const Colour& GetEnabledColour() const noexcept { return m_enabledColour; }
			void SetEnabledColour(const Colour& colour) noexcept;

			inline const Optional<Colour>& GetDisabledColour() const noexcept { return m_disabledColour; }
			void SetDisabledColour(const Optional<Colour>& colour) noexcept;

		private:
			Label(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor, const IVec2& anchorOffset);

			void ResetPosition();
			void ResetSize();
		};
	}
}

#endif