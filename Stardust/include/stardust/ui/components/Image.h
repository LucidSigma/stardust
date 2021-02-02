#pragma once
#ifndef STARDUST_IMAGE_H
#define STARDUST_IMAGE_H

#include "stardust/ui/components/UIComponent.h"

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/FlipType.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Colour.h"
#include "stardust/math/Math.h"
#include "stardust/scene/components/ScreenTransformComponent.h"
#include "stardust/scene/components/SpriteComponent.h"
#include "stardust/ui/canvas/Canvas.h"
#include "stardust/ui/Anchor.h"

namespace stardust
{
	namespace ui
	{
		class Image
			: public Component
		{
		public:
			struct CreateInfo
			{
				UVec2 size;
				FlipType flip;

				f32 rotation;
				Optional<IVec2> pivot;

				ObserverPtr<const Texture> enabledTexture;
				Optional<TextureCoordinatePair> enabledSubTextureArea;
				Colour enabledColourMod;

				Optional<ObserverPtr<const Texture>> disabledTexture;
				Optional<Optional<TextureCoordinatePair>> disabledSubTextureArea;
				Optional<Colour> disabledColourMod;
			};

		private:
			components::ScreenTransform m_transform;
			components::Sprite m_sprite;

			ObserverPtr<const Texture> m_enabledTexture;
			Optional<ObserverPtr<const Texture>> m_disabledTexture;

			Optional<TextureCoordinatePair> m_enabledSubTextureArea;
			Optional<Optional<TextureCoordinatePair>> m_disabledSubTextureArea;

			Colour m_enabledColourMod;
			Optional<Colour> m_disabledColourMod;

		public:
			Image(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor = Anchor::Centre, const IVec2& anchorOffset = IVec2Zero);
			virtual ~Image() noexcept override = default;

			virtual void Render(Renderer& renderer) override;

			virtual void OnEnable() override;
			virtual void OnDisable() override;

			virtual void OnCanvasResize(const UVec2&) override;

			virtual void SetAnchor(const Anchor anchor) noexcept override;
			virtual void SetAnchorOffset(const IVec2& anchorOffset) noexcept override;

			inline const UVec2& GetSize() const noexcept { return m_transform.size; }
			void SetSize(const UVec2& size);

			inline f32 GetRotation() const noexcept { return m_transform.rotation; }
			inline void SetRotation(const f32 rotation) noexcept { m_transform.rotation = rotation; }

			inline const Optional<IVec2>& GetPivot() const noexcept { return m_transform.pivot; }
			inline void SetPivot(const Optional<IVec2>& pivot) noexcept { m_transform.pivot = pivot; }

			//inline ObserverPtr<const Texture> GetTexture() const noexcept { return m_sprite.texture; }
			//inline void SetTexture(const Texture& texture) noexcept { m_sprite.texture = &texture; }
			//
			//inline const Optional<TextureCoordinatePair>& GetSubTextureArea() const noexcept { return m_sprite.subTextureArea; }
			//inline void SetSubTextureArea(const Optional<TextureCoordinatePair>& textureCoordinates) noexcept { m_sprite.subTextureArea = textureCoordinates; }
			//
			//inline const Colour& GetColourMod() const noexcept { return m_sprite.colourMod; }
			//inline void SetColourMod(const Colour& colourMod) noexcept { m_sprite.colourMod = colourMod; }
		};
	}
}

#endif