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
		m_quadBuffer.resize(s_VerticesPerBatch);
		m_quadBufferPtr = m_quadBuffer.data();

		m_batchVertexBuffer.Initialise(s_VerticesPerBatch * sizeof(BatchVertex), BufferUsage::Dynamic);

		m_batchVertexLayout.AddAttribute({
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
		.AddVertexBuffer(m_batchVertexBuffer)
		.Initialise();

		Vector<u32> indices(s_IndicesPerBatch);
		u32 offset = 0u;

		for (usize i = 0u; i < s_IndicesPerBatch; i += 6u)
		{
			indices[i + 0u] = 0u + offset;
			indices[i + 1u] = 1u + offset;
			indices[i + 2u] = 2u + offset;

			indices[i + 3u] = 2u + offset;
			indices[i + 4u] = 3u + offset;
			indices[i + 5u] = 0u + offset;

			offset += 4u;
		}

		m_batchIndexBuffer.Initialise(indices);

		if (!m_batchVertexBuffer.IsValid() || !m_batchVertexLayout.IsValid() || !m_batchIndexBuffer.IsValid())
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

		m_textureSlots[s_BlankTextureSlot] = &m_blankTexture;

		for (usize i = 1u; i < s_MaxTextures; ++i)
		{
			m_textureSlots[i] = 0u;
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

	void Renderer::DrawScreenRect(const IVec2& position, const UVec2& size, const Colour& colour, const f32 rotation, const Optional<IVec2>& pivot) const
	{
		const Mat4 modelMatrix = CreateScreenModelMatrix(Vec2(position), size, FlipType::None, rotation, pivot);

		m_shaderPrograms.at(ShaderName::Quad).Use();
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_MVP", m_screenProjectionMatrix * modelMatrix);
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_Colour", ColourToVec4(colour));

		m_quadVertexLayout.Bind();
		m_quadVertexLayout.DrawIndexed(m_quadIBO);
		m_quadVertexLayout.Unbind();

		m_shaderPrograms.at(ShaderName::Quad).Disuse();
	}

	void Renderer::DrawScreenQuad(const Array<IVec2, 4u>& points, const Colour& colour) const
	{
		const Vector<f32> quadVertices{
			static_cast<f32>(points[0].x), static_cast<f32>(m_virtualSize.y - points[0].y), 0.0f, 0.0f,
			static_cast<f32>(points[1].x), static_cast<f32>(m_virtualSize.y - points[1].y), 0.0f, 0.0f,
			static_cast<f32>(points[2].x), static_cast<f32>(m_virtualSize.y - points[2].y), 0.0f, 0.0f,
			static_cast<f32>(points[3].x), static_cast<f32>(m_virtualSize.y - points[3].y), 0.0f, 0.0f,
		};

		VertexBuffer quadBuffer(quadVertices);
		VertexLayout quadVertexLayout;

		quadVertexLayout
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
			.AddVertexBuffer(quadBuffer)
			.Initialise();

		m_shaderPrograms.at(ShaderName::Quad).Use();
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_MVP", m_screenProjectionMatrix);
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_Colour", ColourToVec4(colour));

		quadVertexLayout.Bind();
		quadVertexLayout.DrawIndexed(m_quadIBO);
		quadVertexLayout.Unbind();

		m_shaderPrograms.at(ShaderName::Quad).Disuse();
	}

	void Renderer::DrawTexturedScreenRect(const Texture& texture, const IVec2& position, const Vec2& scale, const FlipType flip, const Colour& colour, const f32 rotation, const Optional<IVec2>& pivot) const
	{
		const Mat4 modelMatrix = CreateScreenModelMatrix(Vec2(position), scale * Vec2(texture.GetSize()), flip, rotation, pivot);

		texture.Bind();

		m_shaderPrograms.at(ShaderName::TexturedQuad).Use();
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetUniform("u_MVP", m_screenProjectionMatrix * modelMatrix);
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetUniform("u_ColourMod", ColourToVec4(colour));
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetTextureUniform("u_TextureSampler", 0);

		m_quadVertexLayout.Bind();
		m_quadVertexLayout.DrawIndexed(m_quadIBO);
		m_quadVertexLayout.Unbind();

		m_shaderPrograms.at(ShaderName::TexturedQuad).Disuse();
		texture.Unbind();
	}

	void Renderer::DrawTexturedScreenQuad(const Texture& texture, const Array<IVec2, 4u>& points, const FlipType flip, const Colour& colour) const
	{
		const Vector<f32> quadVertices{
			static_cast<f32>(points[0].x), static_cast<f32>(m_virtualSize.y - points[0].y), 0.0f, 0.0f,
			static_cast<f32>(points[1].x), static_cast<f32>(m_virtualSize.y - points[1].y), 0.0f, 1.0f,
			static_cast<f32>(points[2].x), static_cast<f32>(m_virtualSize.y - points[2].y), 1.0f, 1.0f,
			static_cast<f32>(points[3].x), static_cast<f32>(m_virtualSize.y - points[3].y), 1.0f, 0.0f,
		};

		VertexBuffer quadBuffer(quadVertices);
		VertexLayout quadVertexLayout;

		quadVertexLayout
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
			.AddVertexBuffer(quadBuffer)
			.Initialise();

		texture.Bind();

		m_shaderPrograms.at(ShaderName::TexturedQuad).Use();
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetUniform("u_MVP", m_screenProjectionMatrix);
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetUniform("u_ColourMod", ColourToVec4(colour));
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetTextureUniform("u_TextureSampler", 0);

		quadVertexLayout.Bind();
		quadVertexLayout.DrawIndexed(m_quadIBO);
		quadVertexLayout.Unbind();

		m_shaderPrograms.at(ShaderName::TexturedQuad).Disuse();
		texture.Unbind();
	}

	void Renderer::BeginFrame()
	{
		BeginBatch();
	}

	void Renderer::EndFrame(const Camera2D& camera)
	{
		EndBatch();
		FlushAndDraw(camera);
	}

	void Renderer::DrawWorldRect(const components::Transform& transform, const Colour& colour, const Camera2D& camera)
	{
		if (m_indexCount >= s_IndicesPerBatch) [[unlikely]]
		{
			EndBatch();
			FlushAndDraw(camera);
			BeginBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot);
		const Vec4 colourVector = ColourToVec4(colour);
		const f32 textureIndex = static_cast<f32>(s_BlankTextureSlot);

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ -0.5f, -0.5f, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ 0.0f, 0.0f };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ -0.5f, 0.5f, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ 0.0f, 1.0f };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ 0.5f, 0.5f, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ 1.0f, 1.0f };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ 0.5f, -0.5f, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ 1.0f, 0.0f };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_indexCount += 6u;
	}

	void Renderer::DrawWorldRect(const components::Transform& transform, const components::SpriteRender& sprite, const Camera2D& camera)
	{
		if (m_indexCount >= s_IndicesPerBatch || m_textureSlotIndex > s_MaxTextures - 1u) [[unlikely]]
		{
			EndBatch();
			FlushAndDraw(camera);
			BeginBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot);
		const Vec4 colourModVector = ColourToVec4(sprite.colourMod);
		f32 textureIndex = 0.0f;

		for (usize i = 0u; i < m_textureSlotIndex; ++i)
		{
			if (m_textureSlots[i] == sprite.texture)
			{
				textureIndex = static_cast<f32>(i);
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<f32>(m_textureSlotIndex);
			m_textureSlots[m_textureSlotIndex] = sprite.texture;

			++m_textureSlotIndex;
		}

		const Vec2 bottomLeftTextureCoordinate = sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().first : Vec2{ 0.0f, 0.0f };
		const Vec2 topRightTextureCoordinate = sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().second : Vec2{ 1.0f, 1.0f };

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ -0.5f, -0.5f, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourModVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ bottomLeftTextureCoordinate.x, bottomLeftTextureCoordinate.y };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ -0.5f, 0.5f, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourModVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ bottomLeftTextureCoordinate.x, topRightTextureCoordinate.y };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ 0.5f, 0.5f, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourModVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ topRightTextureCoordinate.x, topRightTextureCoordinate.y };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ 0.5f, -0.5f, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourModVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ topRightTextureCoordinate.x, bottomLeftTextureCoordinate.y };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_indexCount += 6u;
	}

	void Renderer::DrawWorldQuad(const Array<Vec2, 4u>& points, const components::Transform& transform, const Colour& colour, const Camera2D& camera)
	{
		if (m_indexCount >= s_IndicesPerBatch) [[unlikely]]
		{
			EndBatch();
			FlushAndDraw(camera);
			BeginBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot);
		const Vec4 colourVector = ColourToVec4(colour);
		const f32 textureIndex = static_cast<f32>(s_BlankTextureSlot);

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ points[0].x, points[0].y, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ 0.0f, 0.0f };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ points[1].x, points[1].y, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ 0.0f, 1.0f };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ points[2].x, points[2].y, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ 1.0f, 1.0f };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ points[3].x, points[3].y, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ 1.0f, 0.0f };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_indexCount += 6u;
	}

	void Renderer::DrawWorldQuad(const Array<Vec2, 4u>& points, const components::Transform& transform, const components::SpriteRender& sprite, const Camera2D& camera)
	{
		if (m_indexCount >= s_IndicesPerBatch || m_textureSlotIndex > s_MaxTextures - 1u) [[unlikely]]
		{
			EndBatch();
		FlushAndDraw(camera);
		BeginBatch();
		}

		const Mat4 modelMatrix = CreateWorldModelMatrix(transform.position, transform.scale, transform.rotation, transform.pivot);
		const Vec4 colourModVector = ColourToVec4(sprite.colourMod);
		f32 textureIndex = 0.0f;

		for (usize i = 0u; i < m_textureSlotIndex; ++i)
		{
			if (m_textureSlots[i] == sprite.texture)
			{
				textureIndex = static_cast<f32>(i);
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<f32>(m_textureSlotIndex);
			m_textureSlots[m_textureSlotIndex] = sprite.texture;

			++m_textureSlotIndex;
		}

		const Vec2 bottomLeftTextureCoordinate = sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().first : Vec2{ 0.0f, 0.0f };
		const Vec2 topRightTextureCoordinate = sprite.subTextureArea.has_value() ? sprite.subTextureArea.value().second : Vec2{ 1.0f, 1.0f };

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ points[0].x, points[0].y, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourModVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ bottomLeftTextureCoordinate.x, bottomLeftTextureCoordinate.y };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ points[1].x, points[1].y, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourModVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ bottomLeftTextureCoordinate.x, topRightTextureCoordinate.y };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ points[2].x, points[2].y, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourModVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ topRightTextureCoordinate.x, topRightTextureCoordinate.y };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_quadBufferPtr->position = Vec2(modelMatrix * Vec4{ points[3].x, points[3].y, 0.0f, 1.0f });
		m_quadBufferPtr->colour = colourModVector;
		m_quadBufferPtr->textureCoordinates = Vec2{ topRightTextureCoordinate.x, bottomLeftTextureCoordinate.y };
		m_quadBufferPtr->textureIndex = textureIndex;
		++m_quadBufferPtr;

		m_indexCount += 6u;
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

	void Renderer::BeginBatch()
	{
		m_quadBufferPtr = m_quadBuffer.data();
	}

	void Renderer::EndBatch()
	{
		const isize batchSize = m_quadBufferPtr - m_quadBuffer.data();

		m_batchVertexBuffer.SetSubData(m_quadBuffer, static_cast<usize>(batchSize));
	}

	void Renderer::FlushAndDraw(const Camera2D& camera)
	{
		for (usize i = 0u; i < m_textureSlotIndex; ++i)
		{
			if (m_textureSlots[i] != nullptr)
			{
				m_textureSlots[i]->Bind(static_cast<i32>(i));
			}
		}

		m_batchShader.Use();
		m_batchShader.SetUniform("u_ViewProjection", camera.GetProjectionMatrix() * camera.GetViewMatrix());

		m_batchVertexLayout.Bind();
		m_batchVertexLayout.DrawIndexed(m_batchIndexBuffer, m_indexCount);
		m_batchVertexLayout.Unbind();

		m_batchShader.Disuse();

		for (usize i = 0u; i < m_textureSlotIndex; ++i)
		{
			if (m_textureSlots[i] != nullptr)
			{
				m_textureSlots[i]->Unbind();
			}
		}

		m_indexCount = 0u;
		m_textureSlotIndex = 1u;
	}

	[[nodiscard]] Mat4 Renderer::CreateWorldModelMatrix(const Vec2& position, const Vec2& scale, const f32 rotation, const Optional<Vec2>& pivot) const
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

		modelMatrix = glm::scale(modelMatrix, Vec3{ scale, 1.0f });

		return modelMatrix;
	}

	[[nodiscard]] Mat4 Renderer::CreateScreenModelMatrix(const Vec2& position, const Vec2& size, const FlipType flip, const f32 rotation, const Optional<IVec2>& pivot) const
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

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), Vec3{ 0.0f, 0.0f, 1.0f });

		if (pivot.has_value())
		{
			const IVec2& pivotValue = pivot.value();
			modelMatrix = glm::translate(modelMatrix, Vec3{ -pivotValue.x, pivotValue.y, 0.0f });
		}

		const Vec2 flipScale = GetScaleFromFlipType(flip);
		modelMatrix = glm::scale(modelMatrix, Vec3{ static_cast<f32>(size.x) * flipScale.x, static_cast<f32>(size.y) * flipScale.y, 1.0f });

		return modelMatrix;
	}

	[[nodiscard]] Vec2 Renderer::GetScaleFromFlipType(const FlipType flipType) const noexcept
	{
		Vec2 flipScale{ 1.0f, 1.0f };

		switch (flipType)
		{
		case FlipType::Horizontal:
			flipScale.x *= -1.0f;

			break;

		case FlipType::Vertical:
			flipScale.y *= -1.0f;

			break;

		case FlipType::Both:
			flipScale *= -1.0f;

			break;

		case FlipType::None:
		default:
			break;
		}

		return flipScale;
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