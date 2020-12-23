#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include "stardust/camera/Camera2D.h"
#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/Colour.h"
#include "stardust/graphics/renderer/objects/IndexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/graphics/shaders/ShaderProgram.h"
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
		enum class ShaderName
		{
			Quad,
		};

		inline static const Vector<f32> s_quadVertices{
			-0.5f, -0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 1.0f, 1.0f,
		};

		inline static const Vector<u32> s_quadIndices{
			0u, 2u, 1u,
			1u, 2u, 3u,
		};

		ObserverPtr<Window> m_window = nullptr;

		UVec2 m_virtualSize{ 0u, 0u };
		Vec2 m_virtualScale{ 1.0f, 1.0f };
		f32 m_virtualAspectRatio = 0.0f;

		HashMap<ShaderName, ShaderProgram> m_shaderPrograms{ };

		VertexLayout m_quadVertexLayout;
		VertexBuffer m_quadVBO;
		IndexBuffer m_quadIBO;

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

		void DrawWorldRect(const Camera2D& camera, const Vec2& position, const Colour& colour, const Vec2& scale = Vec2{ 1.0f, 1.0f }, const f32 rotation = 0.0f, const Optional<Vec2> rotationCentreOffset = NullOpt);

		[[nodiscard]] PixelReadData ReadPixels() const;

		inline const UVec2& GetVirtualSize() const noexcept { return m_virtualSize; }
		inline const Vec2& GetVirtualScale() const noexcept { return m_virtualScale; }
		inline f32 GetVirtualAspectRatio() const noexcept { return m_virtualAspectRatio; }

		inline const Mat4& GetScreenProjectionMatrix() const noexcept { return m_screenProjectionMatrix; }

		inline bool IsValid() const noexcept { return m_isValid; }

	private:
		void InitialiseVertexObjects();
		void InitialiseShaders();

		[[nodiscard]] Mat4 CreateModelMatrix(const Vec2& position, const Colour& colour, const Vec2& scale, const f32 rotation, const Optional<Vec2> rotationCentreOffset);

		void UpdateScreenProjectionMatrix();
	};
}

#endif