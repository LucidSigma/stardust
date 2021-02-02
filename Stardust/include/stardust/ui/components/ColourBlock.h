#pragma once
#ifndef COLOUR_BLOCK_H
#define COLOUR_BLOCK_H

#include "stardust/ui/components/UIComponent.h"

#include "stardust/data/Types.h"
#include "stardust/data/MathTypes.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/Colour.h"
#include "stardust/math/Math.h"
#include "stardust/scene/components/ScreenTransformComponent.h"
#include "stardust/ui/Anchor.h"
#include "stardust/ui/canvas/Canvas.h"

namespace stardust
{
	namespace ui
	{
		class ColourBlock
			: public Component
		{
		public:
			struct CreateInfo
			{
				UVec2 size;

				Colour enabledColour;
				Optional<Colour> disabledColour;
			};

		private:
			components::ScreenTransform m_transform;

			Colour m_colour;
			Colour m_enabledColour;
			Optional<Colour> m_disabledColour;

		public:
			ColourBlock(const Canvas& canvas, const CreateInfo& createInfo, const Anchor anchor = Anchor::Centre, const IVec2& anchorOffset = IVec2Zero);
			virtual ~ColourBlock() noexcept override = default;

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
		};
	}
}

#endif