#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/Colour.h"
#include "stardust/window/Window.h"

namespace stardust
{
	class Renderer
		: private INoncopyable, private INonmovable
	{
	public:
		struct CreateInfo
		{
			ObserverPtr<Window> window = nullptr;
			Optional<UVec2> virtualSize = NullOpt;
		};

		struct PixelReadData
		{
			Vector<ubyte> pixels;

			UVec2 extent;
			usize channelCount;
		};

	private:
		ObserverPtr<Window> m_window = nullptr;

		UVec2 m_virtualSize{ 0u, 0u };
		Vec2 m_virtualScale{ 1.0f, 1.0f };
		f32 m_virtualAspectRatio = 0.0f;

		Mat4 m_screenProjectionMatrix{ 1.0f };

		bool m_isValid = false;

	public:
		Renderer() = default;
		Renderer(const CreateInfo& createInfo);
		~Renderer() noexcept = default;

		void Initialise(const CreateInfo& createInfo);
		// Add destroy function.

		void ProcessResize();
		void SetVirtualSize(const UVec2& virtualSize);

		void SetClearColour(const Colour& colour) const;
		void Clear() const;

		[[nodiscard]] PixelReadData ReadPixels() const;

		inline const UVec2& GetVirtualSize() const noexcept { return m_virtualSize; }
		inline const Vec2& GetVirtualScale() const noexcept { return m_virtualScale; }
		inline f32 GetVirtualAspectRatio() const noexcept { return m_virtualAspectRatio; }

		inline const Mat4& GetScreenProjectionMatrix() const noexcept { return m_screenProjectionMatrix; }

		inline bool IsValid() const noexcept { return m_isValid; }

	private:
		void UpdateScreenProjectionMatrix();
	};
}

#endif