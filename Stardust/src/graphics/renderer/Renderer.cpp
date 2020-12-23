#include "stardust/graphics/renderer/Renderer.h"

#include <utility>

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

		u32 width = windowSize.x;
		u32 height = static_cast<u32>(width / m_virtualAspectRatio + 0.5f);

		if (height > static_cast<u32>(windowSize.y))
		{
			height = windowSize.y;
			width = static_cast<u32>(height * m_virtualAspectRatio + 0.5f);
		}

		const i32 viewportX = (windowSize.x / 2) - (width / 2);
		const i32 viewportY = (windowSize.y / 2) - (height / 2);

		glViewport(viewportX, viewportY, width, height);
		glScissor(viewportX, viewportY, width, height);
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
		const Mat4 modelMatrix = CreateScreenModelMatrix(position, size, FlipType::None, rotation, pivot);

		m_shaderPrograms.at(ShaderName::Quad).Use();
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_MVP", m_screenProjectionMatrix * modelMatrix);
		m_shaderPrograms.at(ShaderName::Quad).SetUniform("u_Colour", ColourToVec4(colour));

		m_quadVertexLayout.Bind();
		m_quadVertexLayout.DrawIndexed(m_quadIBO);
		m_quadVertexLayout.Unbind();

		m_shaderPrograms.at(ShaderName::Quad).Disuse();
	}

	[[nodiscard]] Renderer::PixelReadData Renderer::ReadPixels() const
	{
		const UVec2 windowSize = m_window->GetDrawableSize();

		u32 pixelsWidth = windowSize.x;
		u32 pixelsHeight = static_cast<u32>(pixelsWidth / m_virtualAspectRatio + 0.5f);

		if (pixelsHeight > static_cast<u32>(windowSize.y))
		{
			pixelsHeight = windowSize.y;
			pixelsWidth = static_cast<u32>(pixelsHeight * m_virtualAspectRatio + 0.5f);
		}

		const i32 pixelReadX = (windowSize.x / 2) - (pixelsWidth / 2);
		const i32 pixelReadY = (windowSize.y / 2) - (pixelsHeight / 2);

		constexpr usize PixelChannelCount = 4u;
		Vector<ubyte> imageData(PixelChannelCount * pixelsWidth * pixelsHeight);

		glReadPixels(
			pixelReadX, pixelReadY,
			pixelsWidth, pixelsHeight,
			GL_RGBA, GL_UNSIGNED_BYTE,
			imageData.data()
		);

		return PixelReadData{
			.pixels = std::move(imageData),
			.extent = UVec2{ pixelsWidth, pixelsHeight },
			.channelCount = PixelChannelCount,
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

	Mat4 Renderer::CreateScreenModelMatrix(const Vec2& position, const Vec2& size, const FlipType flip, const f32 rotation, const Optional<IVec2>& pivot) const
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

	Vec2 Renderer::GetScaleFromFlipType(const FlipType flipType) const noexcept
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