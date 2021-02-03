#pragma once
#ifndef STARDUST_LABEL_H
#define STARDUST_LABEL_H

#include "stardust/ui/components/UIComponent.h"

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/Colour.h"
#include "stardust/math/Math.h"
#include "stardust/scene/components/ScreenTransformComponent.h"
#include "stardust/scene/components/SpriteComponent.h"
#include "stardust/text/TextCache.h"
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
				String text;
				ObserverPtr<TextCache> textCache;
				f32 textScale;

				Colour enabledColour;
				Optional<Colour> disabledColour;
			};

		private:
			components::ScreenTransform m_transform;
			components::Sprite m_sprite;

			String m_text;
			ObserverPtr<TextCache> m_textCache;
			f32 m_textScale;

			Colour m_enabledColour;
			Optional<Colour> m_disabledColour;

		public:
			Label(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor = Anchor::Centre, const IVec2& anchorOffset = IVec2Zero);
			virtual ~Label() noexcept override = default;

			virtual void ProcessInput() override;
			virtual void Update(const f32) override;
			virtual void Render(Renderer& renderer) override;

			virtual void OnEnable() override;
			virtual void OnDisable() override;

			virtual void OnCanvasResize(const UVec2&) override;

			virtual void SetAnchor(const Anchor anchor) noexcept override;
			virtual void SetAnchorOffset(const IVec2& anchorOffset) noexcept override;

			inline f32 GetTextScale() const noexcept { return m_textScale; }
			void SetTextScale(const f32 textScale) noexcept;

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
		};
	}
}

#endif