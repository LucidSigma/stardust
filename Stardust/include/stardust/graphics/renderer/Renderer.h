#pragma once
#ifndef STARDUST_RENDERER_H
#define STARDUST_RENDERER_H

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
			0u, 1u, 2u,
			2u, 3u, 0u,
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

		// Batching (will replace non-batched rendering when it is finished).
		struct BatchVertex
		{
			Vec2 position;
			Vec4 colour;
			Vec2 textureCoordinates;
			f32 textureIndex;
		};

		static constexpr usize s_MaxQuadsPerBatch = 4'000u;
		static constexpr usize s_VerticesPerBatch = s_MaxQuadsPerBatch * 4u;
		static constexpr usize s_IndicesPerBatch = s_MaxQuadsPerBatch * 6u;

		// TODO: Query from GPU instead of hard code.
		static constexpr usize s_MaxTextures = 32u;
		static constexpr u32 s_BlankTextureSlot = 0u;

		VertexLayout m_batchVertexLayout;
		VertexBuffer m_batchVertexBuffer;
		IndexBuffer m_batchIndexBuffer;

		ShaderProgram m_batchShader;
		Texture m_blankTexture;

		Vector<BatchVertex> m_quadBuffer{ };
		BatchVertex* m_quadBufferPtr = nullptr;

		u32 m_indexCount = 0u;

		Array<ObserverPtr<const Texture>, s_MaxTextures> m_textureSlots{ nullptr };
		usize m_textureSlotIndex = 1u;

		u32 m_quadsDrawnThisFrame = 0u;

	public:
		Renderer() = default;
		Renderer(const CreateInfo& createInfo);
		~Renderer() noexcept;

		void Initialise(const CreateInfo& createInfo);
		void Destroy() noexcept;

		void ProcessResize();
		void SetVirtualSize(const UVec2& virtualSize);

		void SetPolygonMode(const PolygonMode polygonMode) const;
		void SetClearColour(const Colour& colour) const;
		void Clear() const;

		void DrawScreenRect(const IVec2& position, const UVec2& size, const Colour& colour, const f32 rotation = 0.0f, const Optional<IVec2>& pivot = NullOpt) const;
		
		// Note: These functions only work properly when the points are specified in a clockwise or counter-clockwise order and centred around (0.0, 0.0).
		void DrawScreenQuad(const Array<IVec2, 4u>& points, const Colour& colour) const;

		void DrawTexturedScreenRect(const Texture& texture, const IVec2& position, const Vec2& scale = Vec2{ 1.0f, 1.0f }, const FlipType flip = FlipType::None, const Colour& colour = colours::White, const f32 rotation = 0.0f, const Optional<IVec2>& pivot = NullOpt) const;
		
		// TODO: Implement scaling and rotation for these functions.
		// Note: These functions only work properly when the points are specified in a clockwise or counter-clockwise order and centred around (0.0, 0.0).
		void DrawTexturedWorldQuad(const Camera2D& camera, const Texture& texture, const Array<Vec2, 4u>& points, const Vec2& translation = Vec2{ 0.0f, 0.0f }, const f32 rotation = 0.0f, const Optional<Vec2>& pivot = NullOpt, const Colour& colour = colours::White) const;
		void DrawTexturedScreenQuad(const Texture& texture, const Array<IVec2, 4u>& points, const FlipType flip = FlipType::None, const Colour& colour = colours::White) const;

		void BeginFrame();
		void EndFrame(const Camera2D& camera);

		void DrawWorldRect(const Camera2D& camera, const Colour& colour, const Vec2& position, const Vec2& size, const f32 rotation = 0.0f, const Optional<Vec2>& pivot = NullOpt);
		void DrawWorldRect(const Camera2D& camera, const Texture& texture, const Vec2& position, const Vec2& size, const Colour& colourMod = colours::White, const f32 rotation = 0.0f, const Optional<Vec2>& pivot = NullOpt);

		// Note: These functions only work properly when the points are centred around (0.0, 0.0).
		// Point [0] should be lower left; point [1] should be upper left; point [2] should be upper right; point [3] should be lower right.
		void DrawWorldQuad(const Camera2D& camera, const Array<Vec2, 4u>& points, const Colour& colour, const Vec2& position, const Vec2& scale, const f32 rotation = 0.0f, const Optional<Vec2>& pivot = NullOpt);
		void DrawWorldQuad(const Camera2D& camera, const Array<Vec2, 4u>& points, const Texture& texture, const Vec2& position, const Vec2& scale, const Colour& colourMod = colours::White, const f32 rotation = 0.0f, const Optional<Vec2>& pivot = NullOpt);

		// TODO: Add destroy functions to destroy method.

		void SetAntiAliasing(const bool enableAntiAliasing) const;

		[[nodiscard]] PixelReadData ReadPixels() const;

		inline const UVec2& GetVirtualSize() const noexcept { return m_virtualSize; }
		inline const Vec2& GetVirtualScale() const noexcept { return m_virtualScale; }
		inline f32 GetVirtualAspectRatio() const noexcept { return m_virtualAspectRatio; }

		inline const Mat4& GetScreenProjectionMatrix() const noexcept { return m_screenProjectionMatrix; }

		[[nodiscard]] Pair<UVec2, UVec2> GetViewportRect() const;
		inline const Window& GetWindow() const noexcept { return *m_window; }

		inline bool IsValid() const noexcept { return m_isValid; }

	private:
		void InitialiseVertexObjects();
		void InitialiseShaders();

		void BeginBatch();
		void EndBatch();
		void FlushAndDraw(const Camera2D& camera);

		[[nodiscard]] Mat4 CreateWorldModelMatrix(const Vec2& position, const Vec2& scale, const f32 rotation, const Optional<Vec2>& pivot) const;
		[[nodiscard]] Mat4 CreateScreenModelMatrix(const Vec2& position, const Vec2& size, const FlipType flip, const f32 rotation, const Optional<IVec2>& pivot) const;

		[[nodiscard]] Vec2 GetScaleFromFlipType(const FlipType flipType) const noexcept;

		void UpdateScreenProjectionMatrix();
	};
}

#endif