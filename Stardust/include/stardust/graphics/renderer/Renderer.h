#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <glad/glad.h>

#include "stardust/camera/Camera2D.h"
#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/objects/IndexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/graphics/renderer/FlipType.h"
#include "stardust/graphics/shaders/ShaderProgram.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Colour.h"
#include "stardust/window/Window.h"

namespace stardust
{
	class Renderer
		: private INoncopyable, private INonmovable
	{
	public:
		enum class PolygonMode
			: GLenum
		{
			Filled = GL_FILL,
			Outline = GL_LINE,
		};

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
			TexturedQuad,
		};

		inline static const Vector<f32> s_quadVertices{
			-0.5f, -0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 1.0f, 0.0f,
		};

		inline static const Vector<u32> s_quadIndices{
			0u, 2u, 1u,
			0u, 2u, 3u,
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
		void Destroy() noexcept;

		void ProcessResize();
		void SetVirtualSize(const UVec2& virtualSize);

		void SetPolygonMode(const PolygonMode polygonMode) const;
		void SetClearColour(const Colour& colour) const;
		void Clear() const;

		void DrawWorldRect(const Camera2D& camera, const Vec2& position, const Vec2& size, const Colour& colour, const f32 rotation = 0.0f, const Optional<Vec2>& pivot = NullOpt) const;
		void DrawScreenRect(const IVec2& position, const UVec2& size, const Colour& colour, const f32 rotation = 0.0f, const Optional<IVec2>& pivot = NullOpt) const;

		// Note: These functions only work properly when the points are specified in a clockwise or counter-clockwise order.
		void DrawWorldQuad(const Camera2D& camera, const Array<Vec2, 4u>& points, const Colour& colour) const;
		void DrawScreenQuad(const Array<IVec2, 4u>& points, const Colour& colour) const;

		void DrawTexturedWorldRect(const Camera2D& camera, const Texture& texture, const Vec2& position, const Vec2& scale = Vec2{ 1.0f, 1.0f }, const Colour& colour = colours::White, const f32 rotation = 0.0f, const Optional<Vec2>& pivot = NullOpt) const;
		void DrawTexturedScreenRect(const Texture& texture, const IVec2& position, const Vec2& scale = Vec2{ 1.0f, 1.0f }, const FlipType flip = FlipType::None, const Colour& colour = colours::White, const f32 rotation = 0.0f, const Optional<IVec2>& pivot = NullOpt) const;
		// DrawTexturedWorldQuad
		// DrawTexturedScreenQuad

		// BeginFrame
		// SubmitWorldBatch (use camera here)
		// SubmitScreenBatch

		[[nodiscard]] PixelReadData ReadPixels() const;

		inline const UVec2& GetVirtualSize() const noexcept { return m_virtualSize; }
		inline const Vec2& GetVirtualScale() const noexcept { return m_virtualScale; }
		inline f32 GetVirtualAspectRatio() const noexcept { return m_virtualAspectRatio; }

		inline const Mat4& GetScreenProjectionMatrix() const noexcept { return m_screenProjectionMatrix; }

		inline bool IsValid() const noexcept { return m_isValid; }

	private:
		void InitialiseVertexObjects();
		void InitialiseShaders();

		[[nodiscard]] Mat4 CreateWorldModelMatrix(const Vec2& position, const Vec2& scale, const f32 rotation, const Optional<Vec2>& pivot) const;
		[[nodiscard]] Mat4 CreateScreenModelMatrix(const Vec2& position, const Vec2& size, const FlipType flip, const f32 rotation, const Optional<IVec2>& pivot) const;

		[[nodiscard]] Vec2 GetScaleFromFlipType(const FlipType flipType) const noexcept;

		void UpdateScreenProjectionMatrix();
	};
}

#endif