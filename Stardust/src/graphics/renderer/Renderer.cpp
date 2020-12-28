#include "stardust/graphics/renderer/Renderer.h"

#include <numeric>
#include <utility>

#include "stardust/graphics/renderer/objects/BufferUsage.h"
#include "stardust/graphics/shaders/Shader.h"
#include "stardust/math/Math.h"

namespace stardust
{
	Renderer::Renderer(const CreateInfo& createInfo)
	{
		Initialise(createInfo);
	}

	Renderer::~Renderer() noexcept
	{
		Destroy();
	}

	void Renderer::Initialise(const CreateInfo& createInfo)
	{
		m_window = createInfo.window;

		SetVirtualSize(createInfo.virtualSize.has_value() ? createInfo.virtualSize.value() : m_window->GetDrawableSize());
		UpdateScreenProjectionMatrix();
		ProcessResize();

		SetClearColour(colours::Black);
		SetPolygonMode(PolygonMode::Filled);
		
		InitialiseVertexObjects();

		if (!m_quadVertexLayout.IsValid() || !m_quadVBO.IsValid() || !m_quadIBO.IsValid())
		{
			return;
		}

		InitialiseShaders();

		for (const auto& [shaderName, shaderProgram] : m_shaderPrograms)
		{
			if (!shaderProgram.IsValid())
			{
				return;
			}
		}

		// BATCHING.
		m_worldQuadBuffer.resize(s_MaxVerticesPerBatch);
		m_worldQuadBufferPtr = m_worldQuadBuffer.data();

		m_screenQuadBuffer.resize(s_MaxVerticesPerBatch);
		m_screenQuadBufferPtr = m_screenQuadBuffer.data();

		m_worldVertexBuffer.Initialise(s_MaxVerticesPerBatch * sizeof(BatchVertex), BufferUsage::Dynamic);
		m_screenVertexBuffer.Initialise(s_MaxVerticesPerBatch * sizeof(BatchVertex), BufferUsage::Dynamic);

		m_worldVertexLayout.AddAttribute({
			// Position.
			.elementCount = 2u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddAttribute({
			// Colour.
			.elementCount = 4u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddAttribute({
			// Texture coordinates.
			.elementCount = 2u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddAttribute({
			// Texture unit index.
			.elementCount = 1u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddVertexBuffer(m_worldVertexBuffer)
		.Initialise();

		m_screenVertexLayout.AddAttribute({
			// Position.
			.elementCount = 2u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddAttribute({
			// Colour.
			.elementCount = 4u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddAttribute({
			// Texture coordinates.
			.elementCount = 2u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddAttribute({
			// Texture unit index.
			.elementCount = 1u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddVertexBuffer(m_screenVertexBuffer)
		.Initialise();

		Vector<u32> indices(s_MaxIndicesPerBatch);
		u32 offset = 0u;

		for (usize i = 0u; i < s_MaxIndicesPerBatch; i += 6u)
		{
			indices[i + 0u] = 0u + offset;
			indices[i + 1u] = 1u + offset;
			indices[i + 2u] = 2u + offset;

			indices[i + 3u] = 2u + offset;
			indices[i + 4u] = 3u + offset;
			indices[i + 5u] = 0u + offset;

			offset += 4u;
		}

		m_worldIndexBuffer.Initialise(indices);
		m_screenIndexBuffer.Initialise(indices);

		if (!m_worldVertexBuffer.IsValid() || !m_worldVertexLayout.IsValid() || !m_worldIndexBuffer.IsValid() ||
			!m_screenVertexBuffer.IsValid() || !m_screenVertexLayout.IsValid() || !m_screenIndexBuffer.IsValid())
		{
			return;
		}

		const Vector<ubyte> blankTextureData{
			0xFF, 0xFF, 0xFF, 0xFF,
		};

		m_blankTexture.Initialise(blankTextureData, UVec2{ 1u, 1u }, 4u);

		if (!m_blankTexture.IsValid())
		{
			return;
		}

		const Shader batchVertexShader(Shader::Type::Vertex, "assets/shaders/quad_batch.vert");
		const Shader batchFragmentShader(Shader::Type::Fragment, "assets/shaders/quad_batch.frag");

		m_batchShader.Initialise({
			&batchVertexShader,
			&batchFragmentShader,
		});

		if (!m_batchShader.IsValid())
		{
			return;
		}

		m_worldTextureSlots[s_BlankTextureSlot] = &m_blankTexture;
		m_screenTextureSlots[s_BlankTextureSlot] = &m_blankTexture;

		for (usize i = 1u; i < s_MaxTextures; ++i)
		{
			m_worldTextureSlots[i] = 0u;
			m_screenTextureSlots[i] = 0u;
		}

		Vector<i32> textureIndices(s_MaxTextures);
		std::iota(std::begin(textureIndices), std::end(textureIndices), 0);

		m_batchShader.Use();
		m_batchShader.SetUniformVector("u_Textures", textureIndices);
		m_batchShader.Disuse();

		m_isValid = true;
	}

	void Renderer::Destroy() noexcept
	{
		if (m_isValid)
		{
			m_shaderPrograms.clear();

			m_quadVertexLayout.Destroy();
			m_quadVBO.Destroy();
			m_quadIBO.Destroy();

			m_isValid = false;
		}
	}

	void Renderer::ProcessResize()
	{
		const UVec2 windowSize = m_window->GetDrawableSize();

		m_virtualScale = Vec2{
			static_cast<f32>(windowSize.x) / static_cast<f32>(m_virtualSize.x),
			static_cast<f32>(windowSize.y) / static_cast<f32>(m_virtualSize.y),
		};

		const auto [viewportTopLeft, viewportSize] = GetViewportRect();

		glViewport(viewportTopLeft.x, viewportTopLeft.y, viewportSize.x, viewportSize.y);
		glScissor(viewportTopLeft.x, viewportTopLeft.y, viewportSize.x, viewportSize.y);
	}

	void Renderer::SetVirtualSize(const UVec2& virtualSize)
	{
		m_virtualSize = virtualSize;
		m_virtualAspectRatio = static_cast<f32>(m_virtualSize.x) / static_cast<f32>(m_virtualSize.y);

		UpdateScreenProjectionMatrix();
		ProcessResize();
	}

	void Renderer::SetPolygonMode(const PolygonMode polygonMode) const
	{
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(polygonMode));
	}

	void Renderer::SetClearColour(const Colour& colour) const
	{
		glClearColor(
			static_cast<f32>(colour.r / 255.0f),
			static_cast<f32>(colour.g / 255.0f),
			static_cast<f32>(colour.b / 255.0f),
			static_cast<f32>(colour.a / 255.0f)
		);
	}

	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::BeginFrame()
	{
		BeginWorldBatch();
		BeginScreenBatch();
	}

	void Renderer::EndFrame(const Camera2D& camera)
	{
		EndWorldBatch();
		FlushWorldBatch(camera);

		EndScreenBatch();
		FlushScreenBatch();
	}

	void Renderer::DrawWorldRect(const components::Transform& transform, const Colour& colour, const Camera2D& camera)
	{
		if (m_worldIndexCount >= s_MaxIndicesPerBatch) [[unlikely]]
		{
			EndWorldBatch();
			FlushWorldBatch(camera);
			BeginWorldBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot);
		const f32 textureIndex = static_cast<f32>(s_BlankTextureSlot);

		GenerateRect(modelMatrix, colour, { Vec2{ 0.0f, 0.0f }, Vec2{ 1.0f, 1.0f } }, textureIndex, m_worldQuadBufferPtr, m_worldIndexCount);
	}

	void Renderer::DrawWorldRect(const components::Transform& transform, const components::ShearTransform& shear, const Colour& colour, const Camera2D& camera)
	{
		if (m_worldIndexCount >= s_MaxIndicesPerBatch) [[unlikely]]
		{
			EndWorldBatch();
			FlushWorldBatch(camera);
			BeginWorldBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot, Vec2{ shear.xShear, shear.yShear });
		const f32 textureIndex = static_cast<f32>(s_BlankTextureSlot);

		GenerateRect(modelMatrix, colour, { Vec2{ 0.0f, 0.0f }, Vec2{ 1.0f, 1.0f } }, textureIndex, m_worldQuadBufferPtr, m_worldIndexCount);
	}

	void Renderer::DrawWorldRect(const components::Transform& transform, const components::SpriteRender& sprite, const Camera2D& camera)
	{
		if (m_worldIndexCount >= s_MaxIndicesPerBatch || m_worldTextureSlotIndex > s_MaxTextures - 1u) [[unlikely]]
		{
			EndWorldBatch();
			FlushWorldBatch(camera);
			BeginWorldBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot);
		f32 textureIndex = 0.0f;

		for (usize i = 0u; i < m_worldTextureSlotIndex; ++i)
		{
			if (m_worldTextureSlots[i] == sprite.texture)
			{
				textureIndex = static_cast<f32>(i);
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<f32>(m_worldTextureSlotIndex);
			m_worldTextureSlots[m_worldTextureSlotIndex] = sprite.texture;

			++m_worldTextureSlotIndex;
		}

		const Pair<Vec2, Vec2> textureCoordinates{
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().first : Vec2{ 0.0f, 0.0f },
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().second : Vec2{ 1.0f, 1.0f },
		};

		GenerateRect(modelMatrix, sprite.colourMod, textureCoordinates, textureIndex, m_worldQuadBufferPtr, m_worldIndexCount);
	}

	void Renderer::DrawWorldRect(const components::Transform& transform, const components::ShearTransform& shear, const components::SpriteRender& sprite, const Camera2D& camera)
	{
		if (m_worldIndexCount >= s_MaxIndicesPerBatch || m_worldTextureSlotIndex > s_MaxTextures - 1u) [[unlikely]]
		{
			EndWorldBatch();
			FlushWorldBatch(camera);
			BeginWorldBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot, Vec2{ shear.xShear, shear.yShear });
		f32 textureIndex = 0.0f;

		for (usize i = 0u; i < m_worldTextureSlotIndex; ++i)
		{
			if (m_worldTextureSlots[i] == sprite.texture)
			{
				textureIndex = static_cast<f32>(i);
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<f32>(m_worldTextureSlotIndex);
			m_worldTextureSlots[m_worldTextureSlotIndex] = sprite.texture;

			++m_worldTextureSlotIndex;
		}

		const Pair<Vec2, Vec2> textureCoordinates{
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().first : Vec2{ 0.0f, 0.0f },
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().second : Vec2{ 1.0f, 1.0f },
		};

		GenerateRect(modelMatrix, sprite.colourMod, textureCoordinates, textureIndex, m_worldQuadBufferPtr, m_worldIndexCount);
	}

	void Renderer::DrawWorldQuad(const Array<Vec2, 4u>& points, const components::Transform& transform, const Colour& colour, const Camera2D& camera)
	{
		if (m_worldIndexCount >= s_MaxIndicesPerBatch) [[unlikely]]
		{
			EndWorldBatch();
			FlushWorldBatch(camera);
			BeginWorldBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot);
		const f32 textureIndex = static_cast<f32>(s_BlankTextureSlot);

		GenerateQuad(points, modelMatrix, colour, { Vec2{ 0.0f, 0.0f }, Vec2{ 1.0f, 1.0f } }, textureIndex, m_worldQuadBufferPtr, m_worldIndexCount);
	}

	void Renderer::DrawWorldQuad(const Array<Vec2, 4u>& points, const components::Transform& transform, const components::ShearTransform& shear, const Colour& colour, const Camera2D& camera)
	{
		if (m_worldIndexCount >= s_MaxIndicesPerBatch) [[unlikely]]
		{
			EndWorldBatch();
			FlushWorldBatch(camera);
			BeginWorldBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot, Vec2{ shear.xShear, shear.yShear });
		const f32 textureIndex = static_cast<f32>(s_BlankTextureSlot);

		GenerateQuad(points, modelMatrix, colour, { Vec2{ 0.0f, 0.0f }, Vec2{ 1.0f, 1.0f } }, textureIndex, m_worldQuadBufferPtr, m_worldIndexCount);
	}

	void Renderer::DrawWorldQuad(const Array<Vec2, 4u>& points, const components::Transform& transform, const components::SpriteRender& sprite, const Camera2D& camera)
	{
		if (m_worldIndexCount >= s_MaxIndicesPerBatch || m_worldTextureSlotIndex > s_MaxTextures - 1u) [[unlikely]]
		{
			EndWorldBatch();
			FlushWorldBatch(camera);
			BeginWorldBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot);
		f32 textureIndex = 0.0f;

		for (usize i = 0u; i < m_worldTextureSlotIndex; ++i)
		{
			if (m_worldTextureSlots[i] == sprite.texture)
			{
				textureIndex = static_cast<f32>(i);
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<f32>(m_worldTextureSlotIndex);
			m_worldTextureSlots[m_worldTextureSlotIndex] = sprite.texture;

			++m_worldTextureSlotIndex;
		}

		const Pair<Vec2, Vec2> textureCoordinates{
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().first : Vec2{ 0.0f, 0.0f },
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().second : Vec2{ 1.0f, 1.0f },
		};

		GenerateQuad(points, modelMatrix, sprite.colourMod, textureCoordinates, textureIndex, m_worldQuadBufferPtr, m_worldIndexCount);
	}

	void Renderer::DrawWorldQuad(const Array<Vec2, 4u>& points, const components::Transform& transform, const components::ShearTransform& shear, const components::SpriteRender& sprite, const Camera2D& camera)
	{
		if (m_worldIndexCount >= s_MaxIndicesPerBatch || m_worldTextureSlotIndex > s_MaxTextures - 1u) [[unlikely]]
		{
			EndWorldBatch();
			FlushWorldBatch(camera);
			BeginWorldBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot, Vec2{ shear.xShear, shear.yShear });
		f32 textureIndex = 0.0f;

		for (usize i = 0u; i < m_worldTextureSlotIndex; ++i)
		{
			if (m_worldTextureSlots[i] == sprite.texture)
			{
				textureIndex = static_cast<f32>(i);
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<f32>(m_worldTextureSlotIndex);
			m_worldTextureSlots[m_worldTextureSlotIndex] = sprite.texture;

			++m_worldTextureSlotIndex;
		}

		const Pair<Vec2, Vec2> textureCoordinates{
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().first : Vec2{ 0.0f, 0.0f },
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().second : Vec2{ 1.0f, 1.0f },
		};

		GenerateQuad(points, modelMatrix, sprite.colourMod, textureCoordinates, textureIndex, m_worldQuadBufferPtr, m_worldIndexCount);
	}

	void Renderer::DrawScreenRect(const components::ScreenTransform& transform, const Colour& colour)
	{
		if (m_screenIndexCount >= s_MaxIndicesPerBatch) [[unlikely]]
		{
			EndScreenBatch();
			FlushScreenBatch();
			BeginScreenBatch();
		}

		const Mat4 modelMatrix = CreateScreenModelMatrix(Vec2(transform.position), Vec2(transform.size), transform.flip, transform.rotation, transform.pivot);
		const f32 textureIndex = static_cast<f32>(s_BlankTextureSlot);

		GenerateRect(modelMatrix, colour, { Vec2{ 0.0f, 0.0f }, Vec2{ 1.0f, 1.0f } }, textureIndex, m_screenQuadBufferPtr, m_screenIndexCount);
	}

	void Renderer::DrawScreenRect(const components::ScreenTransform& transform, const components::ShearTransform& shear, const Colour& colour)
	{
		if (m_screenIndexCount >= s_MaxIndicesPerBatch) [[unlikely]]
		{
			EndScreenBatch();
			FlushScreenBatch();
			BeginScreenBatch();
		}

		const Mat4 modelMatrix = CreateScreenModelMatrix(Vec2(transform.position), Vec2(transform.size), transform.flip, transform.rotation, transform.pivot, Vec2{ shear.xShear, shear.yShear });
		const f32 textureIndex = static_cast<f32>(s_BlankTextureSlot);

		GenerateRect(modelMatrix, colour, { Vec2{ 0.0f, 0.0f }, Vec2{ 1.0f, 1.0f } }, textureIndex, m_screenQuadBufferPtr, m_screenIndexCount);
	}

	void Renderer::DrawScreenRect(const components::ScreenTransform& transform, const components::SpriteRender& sprite)
	{
		if (m_screenIndexCount >= s_MaxIndicesPerBatch || m_screenTextureSlotIndex > s_MaxTextures - 1u) [[unlikely]]
		{
			EndScreenBatch();
			FlushScreenBatch();
			BeginScreenBatch();
		}

		const Mat4 modelMatrix = CreateScreenModelMatrix(Vec2(transform.position), Vec2(transform.size), transform.flip, transform.rotation, transform.pivot);
		f32 textureIndex = 0.0f;

		for (usize i = 0u; i < m_screenTextureSlotIndex; ++i)
		{
			if (m_screenTextureSlots[i] == sprite.texture)
			{
				textureIndex = static_cast<f32>(i);
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<f32>(m_screenTextureSlotIndex);
			m_screenTextureSlots[m_screenTextureSlotIndex] = sprite.texture;

			++m_screenTextureSlotIndex;
		}

		const Pair<Vec2, Vec2> textureCoordinates{
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().first : Vec2{ 0.0f, 0.0f },
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().second : Vec2{ 1.0f, 1.0f },
		};

		GenerateRect(modelMatrix, sprite.colourMod, textureCoordinates, textureIndex, m_screenQuadBufferPtr, m_screenIndexCount);
	}

	void Renderer::DrawScreenRect(const components::ScreenTransform& transform, const components::ShearTransform& shear, const components::SpriteRender& sprite)
	{
		if (m_screenIndexCount >= s_MaxIndicesPerBatch || m_screenTextureSlotIndex > s_MaxTextures - 1u) [[unlikely]]
		{
			EndScreenBatch();
		FlushScreenBatch();
		BeginScreenBatch();
		}

		const Mat4 modelMatrix = CreateScreenModelMatrix(Vec2(transform.position), Vec2(transform.size), transform.flip, transform.rotation, transform.pivot, Vec2{ shear.xShear, shear.yShear });
		f32 textureIndex = 0.0f;

		for (usize i = 0u; i < m_screenTextureSlotIndex; ++i)
		{
			if (m_screenTextureSlots[i] == sprite.texture)
			{
				textureIndex = static_cast<f32>(i);
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<f32>(m_screenTextureSlotIndex);
			m_screenTextureSlots[m_screenTextureSlotIndex] = sprite.texture;

			++m_screenTextureSlotIndex;
		}

		const Pair<Vec2, Vec2> textureCoordinates{
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().first : Vec2{ 0.0f, 0.0f },
			sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().second : Vec2{ 1.0f, 1.0f },
		};

		GenerateRect(modelMatrix, sprite.colourMod, textureCoordinates, textureIndex, m_screenQuadBufferPtr, m_screenIndexCount);
	}

	void Renderer::SetAntiAliasing(const bool enableAntiAliasing) const
	{
		if (enableAntiAliasing)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}
	}

	[[nodiscard]] Renderer::PixelReadData Renderer::ReadPixels() const
	{
		const auto [viewportTopLeft, viewportSize] = GetViewportRect();

		constexpr usize PixelChannelCount = 4u;
		Vector<ubyte> imageData(PixelChannelCount * viewportSize.x * viewportSize.y);

		glReadPixels(
			viewportTopLeft.x, viewportTopLeft.y,
			viewportSize.x, viewportSize.y,
			GL_RGBA, GL_UNSIGNED_BYTE,
			imageData.data()
		);

		return PixelReadData{
			.pixels = std::move(imageData),
			.extent = UVec2{ viewportSize.x, viewportSize.y },
			.channelCount = PixelChannelCount,
		};
	}

	Pair<UVec2, UVec2> Renderer::GetViewportRect() const
	{
		const UVec2 windowSize = m_window->GetDrawableSize();

		u32 width = windowSize.x;
		u32 height = static_cast<u32>(width / m_virtualAspectRatio + 0.5f);

		if (height > static_cast<u32>(windowSize.y))
		{
			height = windowSize.y;
			width = static_cast<u32>(height * m_virtualAspectRatio + 0.5f);
		}

		const i32 viewportX = (windowSize.x / 2) - (width / 2);
		const i32 viewportY = (windowSize.y / 2) - (height / 2);

		return {
			Vec2{ viewportX, viewportY },
			Vec2{ width, height },
		};
	}

	void Renderer::InitialiseVertexObjects()
	{
		m_quadVBO.Initialise(s_quadVertices);
		m_quadIBO.Initialise(s_quadIndices);

		m_quadVertexLayout
			.AddAttribute({
				// Position.
				.elementCount = 2u,
				.dataType = GL_FLOAT,
				.isNormalised = true,
			})
			.AddAttribute({
				// Texture coordinate.
				.elementCount = 2u,
				.dataType = GL_FLOAT,
				.isNormalised = true,
			})
			.AddVertexBuffer(m_quadVBO)
			.Initialise();
	}

	void Renderer::InitialiseShaders()
	{
		const Shader quadVertexShader(Shader::Type::Vertex, "assets/shaders/quad.vert");
		const Shader quadFragmentShader(Shader::Type::Fragment, "assets/shaders/quad.frag");

		m_shaderPrograms.emplace(ShaderName::Quad, Vector<ObserverPtr<const Shader>>{
			&quadVertexShader,
			&quadFragmentShader,
		});

		const Shader texturedQuadVertexShader(Shader::Type::Vertex, "assets/shaders/textured_quad.vert");
		const Shader texturedQuadFragmentShader(Shader::Type::Fragment, "assets/shaders/textured_quad.frag");

		m_shaderPrograms.emplace(ShaderName::TexturedQuad, Vector<ObserverPtr<const Shader>>{
			&texturedQuadVertexShader,
			&texturedQuadFragmentShader,
		});
	}

	void Renderer::BeginWorldBatch()
	{
		m_worldQuadBufferPtr = m_worldQuadBuffer.data();
	}

	void Renderer::EndWorldBatch()
	{
		const isize batchSize = m_worldQuadBufferPtr - m_worldQuadBuffer.data();

		m_worldVertexBuffer.SetSubData(m_worldQuadBuffer, static_cast<usize>(batchSize));
	}

	void Renderer::FlushWorldBatch(const Camera2D& camera)
	{
		for (usize i = 0u; i < m_worldTextureSlotIndex; ++i)
		{
			if (m_worldTextureSlots[i] != nullptr) [[likely]]
			{
				m_worldTextureSlots[i]->Bind(static_cast<i32>(i));
			}
		}

		m_batchShader.Use();
		m_batchShader.SetUniform("u_ViewProjection", camera.GetProjectionMatrix() * camera.GetViewMatrix());

		m_worldVertexLayout.Bind();
		m_worldVertexLayout.DrawIndexed(m_worldIndexBuffer, m_worldIndexCount);
		m_worldVertexLayout.Unbind();

		m_batchShader.Disuse();

		for (usize i = 0u; i < m_worldTextureSlotIndex; ++i)
		{
			if (m_worldTextureSlots[i] != nullptr) [[likely]]
			{
				m_worldTextureSlots[i]->Unbind();
			}
		}

		m_worldIndexCount = 0u;
		m_worldTextureSlotIndex = 1u;
	}

	void Renderer::BeginScreenBatch()
	{
		m_screenQuadBufferPtr = m_screenQuadBuffer.data();
	}

	void Renderer::EndScreenBatch()
	{
		const isize batchSize = m_screenQuadBufferPtr - m_screenQuadBuffer.data();

		m_screenVertexBuffer.SetSubData(m_screenQuadBuffer, static_cast<usize>(batchSize));
	}

	void Renderer::FlushScreenBatch()
	{
		for (usize i = 0u; i < m_screenTextureSlotIndex; ++i)
		{
			if (m_screenTextureSlots[i] != nullptr) [[likely]]
			{
				m_screenTextureSlots[i]->Bind(static_cast<i32>(i));
			}
		}

		m_batchShader.Use();
		m_batchShader.SetUniform("u_ViewProjection", m_screenProjectionMatrix);

		m_screenVertexLayout.Bind();
		m_screenVertexLayout.DrawIndexed(m_screenIndexBuffer, m_screenIndexCount);
		m_screenVertexLayout.Unbind();

		m_batchShader.Disuse();

		for (usize i = 0u; i < m_screenTextureSlotIndex; ++i)
		{
			if (m_screenTextureSlots[i] != nullptr) [[likely]]
			{
				m_screenTextureSlots[i]->Unbind();
			}
		}

		m_screenIndexCount = 0u;
		m_screenTextureSlotIndex = 1u;
	}

	[[nodiscard]] Mat4 Renderer::CreateWorldModelMatrix(const Vec2& position, const Vec2& scale, const f32 rotation, const Optional<Vec2>& pivot, const Optional<Vec2>& shear) const
	{
		Mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, Vec3(position, 0.0f));

		if (pivot.has_value())
		{
			modelMatrix = glm::translate(modelMatrix, Vec3{ pivot.value(), 0.0f });
		}

		modelMatrix = glm::rotate(modelMatrix, -glm::radians(rotation), Vec3{ 0.0f, 0.0f, 1.0f });

		if (pivot.has_value())
		{
			modelMatrix = glm::translate(modelMatrix, Vec3{ -pivot.value(), 0.0f });
		}

		if (shear.has_value())
		{
			modelMatrix = glm::shearY3D(modelMatrix, glm::tan(glm::radians(shear.value().x)), 0.0f);
			modelMatrix = glm::shearX3D(modelMatrix, glm::tan(glm::radians(shear.value().y)), 0.0f);
		}

		modelMatrix = glm::scale(modelMatrix, Vec3{ scale, 1.0f });

		return modelMatrix;
	}

	[[nodiscard]] Mat4 Renderer::CreateScreenModelMatrix(const Vec2& position, const Vec2& size, const FlipType flip, const f32 rotation, const Optional<IVec2>& pivot, const Optional<Vec2>& shear) const
	{
		Mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, Vec3{
			position.x + static_cast<i32>(size.x) / 2,
			m_virtualSize.y - position.y - static_cast<i32>(size.y) / 2,
			0.0f,
		});

		if (pivot.has_value())
		{
			const IVec2& pivotValue = pivot.value();
			modelMatrix = glm::translate(modelMatrix, Vec3{ pivotValue.x, -pivotValue.y, 0.0f });
		}

		modelMatrix = glm::rotate(modelMatrix, -glm::radians(rotation), Vec3{ 0.0f, 0.0f, 1.0f });

		if (pivot.has_value())
		{
			const IVec2& pivotValue = pivot.value();
			modelMatrix = glm::translate(modelMatrix, Vec3{ -pivotValue.x, pivotValue.y, 0.0f });
		}

		if (shear.has_value())
		{
			modelMatrix = glm::shearY3D(modelMatrix, glm::tan(glm::radians(shear.value().x)), 0.0f);
			modelMatrix = glm::shearX3D(modelMatrix, glm::tan(glm::radians(shear.value().y)), 0.0f);
		}

		const Vec2 flipScale = GetScaleFromFlipType(flip);
		modelMatrix = glm::scale(modelMatrix, Vec3{ static_cast<f32>(size.x) * flipScale.x, static_cast<f32>(size.y) * flipScale.y, 1.0f });

		return modelMatrix;
	}

	void Renderer::GenerateRect(const Mat4& modelMatrix, const Colour& colour, const Pair<Vec2, Vec2>& textureCoordinates, const f32 textureIndex, BatchVertex*& bufferPtr, u32& indexCount)
	{
		const auto& [bottomLeftTextureCoordinate, topRightTextureCoordinate] = textureCoordinates;
		const Vec4 colourVector = ColourToVec4(colour);

		bufferPtr->position = Vec2(modelMatrix * Vec4{ -0.5f, -0.5f, 0.0f, 1.0f });
		bufferPtr->colour = colourVector;
		bufferPtr->textureCoordinates = Vec2{ bottomLeftTextureCoordinate.x, bottomLeftTextureCoordinate.y };
		bufferPtr->textureIndex = textureIndex;
		++bufferPtr;

		bufferPtr->position = Vec2(modelMatrix * Vec4{ -0.5f, 0.5f, 0.0f, 1.0f });
		bufferPtr->colour = colourVector;
		bufferPtr->textureCoordinates = Vec2{ bottomLeftTextureCoordinate.x, topRightTextureCoordinate.y };
		bufferPtr->textureIndex = textureIndex;
		++bufferPtr;

		bufferPtr->position = Vec2(modelMatrix * Vec4{ 0.5f, 0.5f, 0.0f, 1.0f });
		bufferPtr->colour = colourVector;
		bufferPtr->textureCoordinates = Vec2{ topRightTextureCoordinate.x, topRightTextureCoordinate.y };
		bufferPtr->textureIndex = textureIndex;
		++bufferPtr;

		bufferPtr->position = Vec2(modelMatrix * Vec4{ 0.5f, -0.5f, 0.0f, 1.0f });
		bufferPtr->colour = colourVector;
		bufferPtr->textureCoordinates = Vec2{ topRightTextureCoordinate.x, bottomLeftTextureCoordinate.y };
		bufferPtr->textureIndex = textureIndex;
		++bufferPtr;

		indexCount += 6u;
	}

	void Renderer::GenerateQuad(const Array<Vec2, 4u>& points, const Mat4& modelMatrix, const Colour& colour, const Pair<Vec2, Vec2>& textureCoordinates, const f32 textureIndex, BatchVertex*& bufferPtr, u32& indexCount)
	{
		const auto& [bottomLeftTextureCoordinate, topRightTextureCoordinate] = textureCoordinates;
		const Vec4 colourVector = ColourToVec4(colour);

		bufferPtr->position = Vec2(modelMatrix * Vec4{ points[0].x, points[0].y, 0.0f, 1.0f });
		bufferPtr->colour = colourVector;
		bufferPtr->textureCoordinates = Vec2{ bottomLeftTextureCoordinate.x, bottomLeftTextureCoordinate.y };
		bufferPtr->textureIndex = textureIndex;
		++bufferPtr;

		bufferPtr->position = Vec2(modelMatrix * Vec4{ points[1].x, points[1].y, 0.0f, 1.0f });
		bufferPtr->colour = colourVector;
		bufferPtr->textureCoordinates = Vec2{ bottomLeftTextureCoordinate.x, topRightTextureCoordinate.y };
		bufferPtr->textureIndex = textureIndex;
		++bufferPtr;

		bufferPtr->position = Vec2(modelMatrix * Vec4{ points[2].x, points[2].y, 0.0f, 1.0f });
		bufferPtr->colour = colourVector;
		bufferPtr->textureCoordinates = Vec2{ topRightTextureCoordinate.x, topRightTextureCoordinate.y };
		bufferPtr->textureIndex = textureIndex;
		++bufferPtr;

		bufferPtr->position = Vec2(modelMatrix * Vec4{ points[3].x, points[3].y, 0.0f, 1.0f });
		bufferPtr->colour = colourVector;
		bufferPtr->textureCoordinates = Vec2{ topRightTextureCoordinate.x, bottomLeftTextureCoordinate.y };
		bufferPtr->textureIndex = textureIndex;
		++bufferPtr;

		indexCount += 6u;
	}

	void Renderer::UpdateScreenProjectionMatrix()
	{
		m_screenProjectionMatrix = glm::ortho(
			0.0f,
			static_cast<f32>(m_virtualSize.x),
			0.0f,
			static_cast<f32>(m_virtualSize.y)
		);
	}
}