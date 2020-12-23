#include "stardust/graphics/renderer/Renderer.h"

#include <utility>

#include <glad/glad.h>

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

		// Put in-built shaders and objects here and test for their validity.

		SetClearColour(colours::Black);
		m_isValid = true;
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