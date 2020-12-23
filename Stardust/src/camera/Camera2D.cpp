#include "stardust/camera/Camera2D.h"

#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/math/Math.h"

namespace stardust
{
	Camera2D::Camera2D(const f32 halfSize, const Renderer& renderer)
	{
		Initialise(halfSize, renderer);
	}

	void Camera2D::Initialise(const f32 halfSize, const Renderer& renderer)
	{
		m_renderer = &renderer;
		m_aspectRatio = static_cast<f32>(m_renderer->GetVirtualSize().x) / static_cast<f32>(m_renderer->GetVirtualSize().y);
		SetHalfSize(halfSize);
	}

	void Camera2D::Refresh()
	{
		m_aspectRatio = static_cast<f32>(m_renderer->GetVirtualSize().x) / static_cast<f32>(m_renderer->GetVirtualSize().y);
		m_pixelsPerUnit = static_cast<f32>(m_renderer->GetVirtualSize().x) / (m_halfSize * 2.0f);

		UpdateProjectionMatrix();
	}

	void Camera2D::ResetTransform()
	{
		m_position = Vec3{ 0.0f, 0.0f, 0.0f };
		m_rotation = 0.0f;
		m_zoom = 1.0f;
	}

	void Camera2D::SetHalfSize(const f32 halfSize) noexcept
	{
		m_halfSize = halfSize;
		m_pixelsPerUnit = static_cast<f32>(m_renderer->GetVirtualSize().x) / (m_halfSize * 2.0f);

		UpdateProjectionMatrix();
	}

	[[nodiscard]] Mat4 Camera2D::GetViewMatrix() const noexcept
	{
		Mat4 viewMatrix{ 1.0f };
		viewMatrix = glm::rotate(viewMatrix, glm::radians(m_rotation), Vec3{ 0.0f, 0.0f, 1.0f });
		viewMatrix = glm::scale(viewMatrix, Vec3{ m_zoom, m_zoom, 1.0f });
		viewMatrix = glm::translate(viewMatrix, Vec3{ -m_position.x, -m_position.y, 0.0f });

		return viewMatrix;
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