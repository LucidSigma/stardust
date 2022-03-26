#include "stardust/camera/Camera2D.h"

namespace stardust
{
    Camera2D::Camera2D(const f32 halfSize, const UVector2 screenSize)
    {
        Initialise(halfSize, screenSize);
    }

    auto Camera2D::Initialise(const f32 halfSize, const UVector2 screenSize) -> void
    {
        m_virtualScreenSize = screenSize;
        m_aspectRatio = static_cast<f32>(m_virtualScreenSize.x) / static_cast<f32>(m_virtualScreenSize.y);

        SetHalfSize(halfSize);
    }

    auto Camera2D::ResetVirtualScreenSize(const UVector2 screenSize) -> void
    {
        m_virtualScreenSize = screenSize;
        m_aspectRatio = static_cast<f32>(m_virtualScreenSize.x) / static_cast<f32>(m_virtualScreenSize.y);
        m_pixelsPerUnit = static_cast<f32>(m_virtualScreenSize.x) / (m_halfSize * 2.0f);

        UpdateProjectionMatrices();
    }

    auto Camera2D::ResetTransform() -> void
    {
        m_position = Vector3{ 0.0f, 0.0f, s_DefaultZ };
        m_rotation = 0.0f;
        m_zoom = 1.0f;
    }

    auto Camera2D::SetHalfSize(const f32 halfSize) noexcept -> void
    {
        m_halfSize = halfSize;
        m_pixelsPerUnit = static_cast<f32>(m_virtualScreenSize.x) / (m_halfSize * 2.0f);

        UpdateProjectionMatrices();
    }

    [[nodiscard]] auto Camera2D::GetViewMatrix() const noexcept -> Matrix4
    {
        Matrix4 viewMatrix = Matrix4Identity;
        viewMatrix = glm::rotate(viewMatrix, -glm::radians(m_rotation), Vector3Forward);
        viewMatrix = glm::scale(viewMatrix, Vector3{ m_zoom, m_zoom, 1.0f });
        viewMatrix = glm::translate(viewMatrix, Vector3{ -m_position.x, -m_position.y, 0.0f });

        return viewMatrix;
    }

    [[nodiscard]] auto Camera2D::WorldSpaceToScreenSpace(Vector2 position) const noexcept -> IVector2
    {
        position.x += m_halfSize;
        position.y -= m_halfSize / m_aspectRatio;

        position *= m_pixelsPerUnit;

        return IVector2{ position.x, -position.y };
    }

    [[nodiscard]] auto Camera2D::ScreenSpaceToWorldSpace(const IVector2 position) const noexcept -> Vector2
    {
        Vector2 worldSpace = position;
        worldSpace.y *= -1.0f;

        worldSpace /= m_pixelsPerUnit;

        worldSpace.x -= m_halfSize;
        worldSpace.y += m_halfSize / m_aspectRatio;

        return worldSpace;
    }

    [[nodiscard]] auto Camera2D::WorldUnitsToScreenUnits(const f32 worldUnits) const noexcept -> i32
    {
        return static_cast<i32>(worldUnits * m_pixelsPerUnit);
    }

    [[nodiscard]] auto Camera2D::WorldUnitsToScreenUnits(const Vector2 worldUnits) const noexcept -> IVector2
    {
        return IVector2{
            static_cast<i32>(worldUnits.x * m_pixelsPerUnit),
            static_cast<i32>(worldUnits.y * m_pixelsPerUnit),
        };
    }

    [[nodiscard]] auto Camera2D::ScreenUnitsToWorldUnits(const i32 screenUnits) const noexcept -> f32
    {
        return static_cast<f32>(screenUnits) / m_pixelsPerUnit;
    }

    [[nodiscard]] auto Camera2D::ScreenUnitsToWorldUnits(const IVector2 screenUnits) const noexcept -> Vector2
    {
        return Vector2{
            static_cast<f32>(screenUnits.x) / m_pixelsPerUnit,
            static_cast<f32>(screenUnits.y) / m_pixelsPerUnit,
        };
    }

    auto Camera2D::UpdateProjectionMatrices() -> void
    {
        m_projectionMatrix = glm::ortho(
            -m_halfSize,
            m_halfSize,
            -m_halfSize / m_aspectRatio,
            m_halfSize / m_aspectRatio
        );

        m_screenProjectionMatrix = glm::ortho(
            0.0f,
            static_cast<f32>(m_virtualScreenSize.x),
            static_cast<f32>(m_virtualScreenSize.y),
            0.0f
        );
    }
}
