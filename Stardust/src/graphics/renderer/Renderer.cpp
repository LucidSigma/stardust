#include "stardust/graphics/renderer/Renderer.h"

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

		m_batchShader.Use();
		m_batchShader.SetTextureUniform("u_Textures[0]", 0);
		m_batchShader.SetTextureUniform("u_Textures[1]", 1);
		m_batchShader.Disuse();

		m_isValid = true;
	}

	void Renderer::Destroy() noexcept
	{
		m_shaderPrograms.clear();

		m_quadVertexLayout.Destroy();
		m_quadVBO.Destroy();
		m_quadIBO.Destroy();
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

	void Renderer::DrawWorldRect(const Camera2D& camera, const Vec2& position, const Vec2& size, const Colour& colour, const f32 rotation, const Optional<Vec2>& pivot) const
	{
		const Mat4 modelMatrix = CreateWorldModelMatrix(position, size, rotation, pivot);

		m_shaderPrograms.at(ShaderName::Quad).Use();
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_MVP", camera.GetProjectionMatrix() * camera.GetViewMatrix() * modelMatrix);
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_Colour", ColourToVec4(colour));

		m_quadVertexLayout.Bind();
		m_quadVertexLayout.DrawIndexed(m_quadIBO);
		m_quadVertexLayout.Unbind();

		m_shaderPrograms.at(ShaderName::Quad).Disuse();
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

	void Renderer::DrawWorldQuad(const Camera2D& camera, const Array<Vec2, 4u>& points, const Colour& colour, const Vec2& translation, const f32 rotation, const Optional<Vec2>& pivot) const
	{
		const Vector<f32> quadVertices{
			points[0].x, points[0].y, 0.0f, 0.0f,
			points[1].x, points[1].y, 0.0f, 0.0f,
			points[2].x, points[2].y, 0.0f, 0.0f,
			points[3].x, points[3].y, 0.0f, 0.0f,
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

		const Mat4 modelMatrix = CreateWorldModelMatrix(translation, Vec2{ 1.0f, 1.0f }, rotation, pivot);

		m_shaderPrograms.at(ShaderName::Quad).Use();
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_MVP", camera.GetProjectionMatrix() * camera.GetViewMatrix() * modelMatrix);
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_Colour", ColourToVec4(colour));

		quadVertexLayout.Bind();
		quadVertexLayout.DrawIndexed(m_quadIBO);
		quadVertexLayout.Unbind();

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

	void Renderer::DrawTexturedWorldRect(const Camera2D& camera, const Texture& texture, const Vec2& position, const Vec2& scale, const Colour& colour, const f32 rotation, const Optional<Vec2>& pivot) const
	{
		const Vec2 pixelScale = Vec2(texture.GetSize()) / camera.GetPixelsPerUnit();
		// TODO: Subtextures (texture coordinates/snippets) pixelScale.

		const Mat4 modelMatrix = CreateWorldModelMatrix(position, pixelScale * scale, rotation, pivot);

		texture.Bind();
		// TODO: Subtextures (texture coordinates/snippets) vertex buffer.

		m_shaderPrograms.at(ShaderName::TexturedQuad).Use();
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetUniform("u_MVP", camera.GetProjectionMatrix() * camera.GetViewMatrix() * modelMatrix);
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetUniform("u_ColourMod", ColourToVec4(colour));
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetTextureUniform("u_TextureSampler", 0);

		m_quadVertexLayout.Bind();
		m_quadVertexLayout.DrawIndexed(m_quadIBO);
		m_quadVertexLayout.Unbind();

		m_shaderPrograms.at(ShaderName::TexturedQuad).Disuse();
		texture.Unbind();
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

	void Renderer::DrawTexturedWorldQuad(const Camera2D& camera, const Texture& texture, const Array<Vec2, 4u>& points, const Vec2& translation, const f32 rotation, const Optional<Vec2>& pivot, const Colour& colour) const
	{
		const Vector<f32> quadVertices{
			points[0].x, points[0].y, 0.0f, 0.0f,
			points[1].x, points[1].y, 0.0f, 1.0f,
			points[2].x, points[2].y, 1.0f, 1.0f,
			points[3].x, points[3].y, 1.0f, 0.0f,
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

		const Mat4 modelMatrix = CreateWorldModelMatrix(translation, Vec2{ 1.0f, 1.0f }, rotation, pivot);

		texture.Bind();

		m_shaderPrograms.at(ShaderName::TexturedQuad).Use();
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetUniform("u_MVP", camera.GetProjectionMatrix() * camera.GetViewMatrix() * modelMatrix);
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetUniform("u_ColourMod", ColourToVec4(colour));
		m_shaderPrograms.at(ShaderName::TexturedQuad).SetTextureUniform("u_TextureSampler", 0);

		quadVertexLayout.Bind();
		quadVertexLayout.DrawIndexed(m_quadIBO);
		quadVertexLayout.Unbind();

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
		m_quadsDrawnThisFrame = 0u;
	}

	[[nodiscard]] Renderer::BatchVertex* Renderer::GenerateQuad(BatchVertex* target, const Vec2& position, const Vec4& colour, const float textureIndex)
	{
		target->position = Vec2{ -0.5f, -0.5f } + position;
		target->colour = colour;
		target->textureCoordinates = Vec2{ 0.0f, 0.0f };
		target->textureIndex = textureIndex;
		++target;

		target->position = Vec2{ -0.5f, 0.5f } + position;
		target->colour = colour;
		target->textureCoordinates = Vec2{ 0.0f, 1.0f };
		target->textureIndex = textureIndex;
		++target;
			
		target->position = Vec2{ 0.5f, 0.5f } + position;
		target->colour = colour;
		target->textureCoordinates = Vec2{ 1.0f, 1.0f };
		target->textureIndex = textureIndex;
		++target;

		target->position = Vec2{ 0.5f, -0.5f } + position;
		target->colour = colour;
		target->textureCoordinates = Vec2{ 1.0f, 0.0f };
		target->textureIndex = textureIndex;
		++target;

		++m_quadsDrawnThisFrame;

		return target;
	}

	void Renderer::BatchWorldRect()
	{
		Vector<BatchVertex> vertices(1'000u);
		BatchVertex* buffer = vertices.data();

		for (i32 x = -3; x <= 3; ++x)
		{
			for (i32 y = -3; y <= 3; ++y)
			{
				buffer = GenerateQuad(buffer, Vec2{ x, y }, Vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, static_cast<float>((x + y) % 2u));
			}
		}

		//const auto leftVertices = GenerateQuad({ -1.0f, 0.0f + offset }, { 1.0f, 0.93f, 0.24f, 1.0f }, 0.0f);
		//const auto rightVertices = GenerateQuad({ 1.0f, 0.0f + offset }, { 0.18f, 0.6f, 0.96f, 1.0f }, 1.0f);

		m_batchVertexBuffer.SetSubData(vertices);

		//m_batchVertexBuffer.SetSubData(leftVertices);
		//m_batchVertexBuffer.SetSubData(rightVertices, leftVertices.size() * sizeof(leftVertices.front()));
	}

	void Renderer::SubmitWorldBatch(const Camera2D& camera, const Texture& left, const Texture& right /* TEMPORARY */) const
	{
		left.Bind(0);
		right.Bind(1);

		m_batchShader.Use();
		m_batchShader.SetUniform("u_ViewProjection", camera.GetProjectionMatrix() * camera.GetViewMatrix());

		m_batchVertexLayout.Bind();
		m_batchVertexLayout.DrawIndexed(m_batchIndexBuffer, m_quadsDrawnThisFrame * 6u);
		m_batchVertexLayout.Unbind();

		m_batchShader.Disuse();

		left.Unbind();
		right.Unbind();
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

	[[nodiscard]] Mat4 Renderer::CreateWorldModelMatrix(const Vec2& position, const Vec2& scale, const f32 rotation, const Optional<Vec2>& pivot) const
	{
		Mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, Vec3(position, 0.0f));

		if (pivot.has_value())
		{
			modelMatrix = glm::translate(modelMatrix, Vec3{ pivot.value(), 0.0f });
		}

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), Vec3{ 0.0f, 0.0f, 1.0f });

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