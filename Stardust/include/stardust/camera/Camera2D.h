#pragma once
#ifndef STARDUST_CAMERA_2D_H
#define STARDUST_CAMERA_2D_H

#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class Camera2D
    {
    private:
        static constexpr f32 s_DefaultZ = -10.0f;

        Vector3 m_position{ 0.0f, 0.0f, s_DefaultZ };

        f32 m_rotation = 0.0f;
        f32 m_zoom = 1.0f;

        f32 m_halfSize = 0.0f;
        f32 m_aspectRatio = 0.0f;
        f32 m_pixelsPerUnit = 0.0f;

        UVector2 m_virtualScreenSize = UVector2Zero;

        Matrix4 m_projectionMatrix = Matrix4Identity;
        Matrix4 m_screenProjectionMatrix = Matrix4Identity;

    public:
        Camera2D() = default;
        Camera2D(const f32 halfSize, const UVector2 screenSize);

        auto Initialise(const f32 halfSize, const UVector2 screenSize) -> void;

        auto ResetVirtualScreenSize(const UVector2 screenSize) -> void;
        auto ResetTransform() -> void;

        [[nodiscard]] inline auto GetPosition() const noexcept -> Vector2 { return Vector2{ m_position.x, m_position.y }; }
        [[nodiscard]] inline auto GetPosition3D() const noexcept -> Vector3 { return m_position; }
        inline auto SetPosition(const Vector2 position) noexcept -> void { m_position = Vector3{ position, m_position.z }; }
        inline auto SetPosition(const Vector3 position) noexcept -> void { m_position = position; }
        inline auto SetPosition(const f32 x, const f32 y) noexcept -> void { m_position = Vector3{ x, y, m_position.z }; }
        inline auto SetPosition(const f32 x, const f32 y, const f32 z) noexcept -> void { m_position = Vector3{ x, y, z }; }
        inline auto AdjustPosition(const Vector2 offset) noexcept -> void { m_position += Vector3{ offset, 0.0f }; }
        inline auto AdjustPosition(const Vector3 offset) noexcept -> void { m_position += offset; }
        inline auto AdjustPosition(const f32 xOffset, const f32 yOffset) noexcept -> void { m_position += Vector3{ xOffset, yOffset, 0.0f }; }
        inline auto AdjustPosition(const f32 xOffset, const f32 yOffset, const f32 zOffset) noexcept -> void { m_position += Vector3{ xOffset, yOffset, zOffset }; }

        [[nodiscard]] inline auto GetRotation() const noexcept -> f32 { return m_rotation; }
        inline auto SetRotation(const f32 rotation) noexcept -> void { m_rotation = rotation; }
        inline auto AdjustRotation(const f32 offset) noexcept -> void { m_rotation += offset; }

        [[nodiscard]] inline auto GetZoom() const noexcept -> f32 { return m_zoom; }
        inline auto SetZoom(const f32 zoom) noexcept -> void { m_zoom = zoom; }
        inline auto AdjustZoom(const f32 offset) noexcept -> void { m_zoom += offset; }

        [[nodiscard]] inline auto GetHalfSize() const noexcept -> f32 { return m_halfSize; }
        auto SetHalfSize(const f32 halfSize) noexcept -> void;

        [[nodiscard]] inline auto GetAspectRatio() const noexcept -> f32 { return m_aspectRatio; }
        [[nodiscard]] inline auto GetPixelsPerUnit() const noexcept -> f32 { return m_pixelsPerUnit; }
        [[nodiscard]] inline auto GetVirtualScreenSize() const noexcept -> const UVector2 { return m_virtualScreenSize; }

        [[nodiscard]] auto GetViewMatrix() const noexcept -> Matrix4;
        [[nodiscard]] inline auto GetProjectionMatrix() const noexcept -> const Matrix4& { return m_projectionMatrix; }
        [[nodiscard]] inline auto GetScreenProjectionMatrix() const noexcept -> const Matrix4& { return m_screenProjectionMatrix; }

        [[nodiscard]] auto WorldSpaceToScreenSpace(Vector2 position) const noexcept -> IVector2;
        [[nodiscard]] auto ScreenSpaceToWorldSpace(const IVector2 position) const noexcept -> Vector2;

        [[nodiscard]] auto WorldUnitsToScreenUnits(const f32 worldUnits) const noexcept -> i32;
        [[nodiscard]] auto WorldUnitsToScreenUnits(const Vector2 worldUnits) const noexcept -> IVector2;
        [[nodiscard]] auto ScreenUnitsToWorldUnits(const i32 screenUnits) const noexcept -> f32;
        [[nodiscard]] auto ScreenUnitsToWorldUnits(const IVector2 screenUnits) const noexcept -> Vector2;

    private:
        auto UpdateProjectionMatrices() -> void;
    };
}

#endif
