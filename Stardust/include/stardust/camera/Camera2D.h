#pragma once
#ifndef STARDUST_CAMERA_2D_H
#define STARDUST_CAMERA_2D_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    class Camera2D
    {
    private:
        Vec3 m_position{ 0.0f, 0.0f, -10.0f };

        f32 m_rotation = 0.0f;
        f32 m_zoom = 1.0f;

        f32 m_halfSize = 0.0f;
        f32 m_aspectRatio = 0.0f;
        f32 m_pixelsPerUnit = 0.0f;

        Mat4 m_projectionMatrix{ 1.0f };

        ObserverPtr<const class Renderer> m_renderer = nullptr;

    public:
        Camera2D() = default;
        Camera2D(const f32 halfSize, const class Renderer& renderer);
        ~Camera2D() noexcept = default;

        void Initialise(const f32 halfSize, const class Renderer& renderer);

        void Refresh();
        void ResetTransform();

        [[nodiscard]] inline Vec3& GetPosition() noexcept { return m_position; }
        [[nodiscard]] inline const Vec3& GetPosition() const noexcept { return m_position; }
        inline void SetPosition(const Vec2& position) noexcept { m_position = Vec3{ position, m_position.z }; }
        inline void SetPosition(const Vec3& position) noexcept { m_position = position; }
        inline void SetPosition(const f32 x, const f32 y, const f32 z) noexcept { m_position = Vec3{ x, y, z }; }

        [[nodiscard]] inline f32 GetRotation() const noexcept { return m_rotation; }
        inline void SetRotation(const f32 rotation) noexcept { m_rotation = rotation; }

        [[nodiscard]] inline f32 GetZoom() const noexcept { return m_zoom; }
        inline void SetZoom(const f32 zoom) noexcept { m_zoom = zoom; }

        [[nodiscard]] inline f32 GetHalfSize() const noexcept { return m_halfSize; }
        void SetHalfSize(const f32 halfSize) noexcept;

        [[nodiscard]] inline f32 GetAspectRatio() const noexcept { return m_aspectRatio; }
        [[nodiscard]] inline f32 GetPixelsPerUnit() const noexcept { return m_pixelsPerUnit; }

        [[nodiscard]] Mat4 GetViewMatrix() const noexcept;
        [[nodiscard]] inline const Mat4& GetProjectionMatrix() const noexcept { return m_projectionMatrix; }
        [[nodiscard]] inline const class Renderer& GetRenderer() const noexcept { return *m_renderer; }

        [[nodiscard]] UVec2 WorldSpaceToScreenSpace(Vec2 position) const noexcept;
        [[nodiscard]] Vec2 ScreenSpaceToWorldSpace(const UVec2& position) const noexcept;

    private:
        void UpdateProjectionMatrix();
    };
}

#endif