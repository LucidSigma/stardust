#include "stardust/camera/Camera2D.h"

#include "stardust/math/Math.h"

namespace stardust
{
	Camera2D::Camera2D(const f32 halfSize/*, const Renderer& renderer*/)
		//: m_renderer(renderer)
	{
		Initialise(halfSize);
	}

	void Camera2D::Refresh()
	{
		// m_aspectRatio = static_cast<f32>(m_renderer->GetVirtualSize().x) / static_cast<f32>(m_renderer->GetVirtualSize().y);
		// m_pixelsPerUnit = static_cast<f32>(m_renderer->GetVirtualSize().x) / (m_halfSize * 2.0f);

		UpdateProjectionMatrix();
	}

	void Camera2D::SetHalfSize(const f32 halfSize) noexcept
	{
		m_halfSize = halfSize;
		// m_pixelsPerUnit = static_cast<f32>(m_renderer->GetVirtualSize().x) / (m_halfSize * 2.0f);

		UpdateProjectionMatrix();
	}

	[[nodiscard]] UVec2 Camera2D::WorldSpaceToScreenSpace(Vec2 position) const noexcept
	{
		position.x += m_halfSize;
		position.y -= m_halfSize / m_aspectRatio;

		position *= m_pixelsPerUnit;

		return UVec2{ position.x, -position.y };
	}

	[[nodiscard]] Vec2 Camera2D::ScreenSpaceToWorldSpace(const UVec2& position) const noexcept
	{
		Vec2 worldSpace = position;
		worldSpace.y *= -1.0f;

		worldSpace /= m_pixelsPerUnit;

		worldSpace.x -= m_halfSize;
		worldSpace.y += m_halfSize / m_aspectRatio;

		return worldSpace;
	}

	void Camera2D::Initialise(const f32 halfSize)
	{
		//m_aspectRatio = static_cast<f32>(m_renderer->GetVirtualSize().x) / static_cast<f32>(m_renderer->GetVirtualSize().y);
		SetHalfSize(halfSize);
	}

	void Camera2D::UpdateProjectionMatrix()
	{
		m_projectionMatrix = glm::ortho(
			-m_halfSize,
			m_halfSize,
			-m_halfSize / m_aspectRatio,
			m_halfSize / m_aspectRatio
		);
	}
}