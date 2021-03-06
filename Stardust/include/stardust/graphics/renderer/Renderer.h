#pragma once
#ifndef STARDUST_RENDERER_H
#define STARDUST_RENDERER_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <glad/glad.h>

#include "stardust/camera/Camera2D.h"
#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/objects/IndexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/graphics/renderer/FlipType.h"
#include "stardust/graphics/renderer/Quad.h"
#include "stardust/graphics/shaders/ShaderProgram.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/math/Math.h"
#include "stardust/scene/components/ScreenTransformComponent.h"
#include "stardust/scene/components/ShearTransformComponent.h"
#include "stardust/scene/components/SpriteComponent.h"
#include "stardust/scene/components/TransformComponent.h"
#include "stardust/window/Window.h"

namespace stardust
{
    class Renderer
        : private INoncopyable, private INonmovable
    {
    public:
        enum class PolygonMode
            : GLenum
        {
            Filled = GL_FILL,
            Outline = GL_LINE,
        };

        struct CreateInfo
        {
            ObserverPtr<Window> window = nullptr;
            Optional<UVec2> virtualSize = NullOpt;
        };

    private:
        struct BatchVertex
        {
            Vec2 position;
            Vec4 colour;
            Vec2 textureCoordinates;
            f32 textureIndex;
        };

        static constexpr usize s_MaxQuadsPerBatch = 4'000u;
        static constexpr usize s_MaxVerticesPerBatch = s_MaxQuadsPerBatch * 4u;
        static constexpr usize s_MaxIndicesPerBatch = s_MaxQuadsPerBatch * 6u;

        static constexpr u32 s_BlankTextureSlot = 0u;

        ObserverPtr<Window> m_window = nullptr;

        UVec2 m_virtualSize = UVec2Zero;
        Vec2 m_virtualScale = Vec2One;
        f32 m_virtualAspectRatio = 0.0f;

        VertexLayout m_worldVertexLayout;
        VertexBuffer m_worldVertexBuffer;
        IndexBuffer m_worldIndexBuffer;

        VertexLayout m_screenVertexLayout;
        VertexBuffer m_screenVertexBuffer;
        IndexBuffer m_screenIndexBuffer;

        ShaderProgram m_batchShader;
        Texture m_blankTexture;

        Vector<BatchVertex> m_worldQuadBuffer{ };
        BatchVertex* m_worldQuadBufferPtr = nullptr;

        Vector<BatchVertex> m_screenQuadBuffer{ };
        BatchVertex* m_screenQuadBufferPtr = nullptr;

        u32 m_worldIndexCount = 0u;
        u32 m_screenIndexCount = 0u;

        usize m_maxTextureUnits = 32u;

        Vector<ObserverPtr<const Texture>> m_worldTextureSlots{ nullptr };
        usize m_worldTextureSlotIndex = 1u;

        Vector<ObserverPtr<const Texture>> m_screenTextureSlots{ nullptr };
        usize m_screenTextureSlotIndex = 1u;

        Mat4 m_screenProjectionMatrix{ 1.0f };

        bool m_isValid = false;

    public:
        Renderer() = default;
        explicit Renderer(const CreateInfo& createInfo);
        ~Renderer() noexcept;

        void Initialise(const CreateInfo& createInfo);
        void Destroy() noexcept;

        void ProcessResize();
        void SetVirtualSize(const UVec2& virtualSize);

        void SetPolygonMode(const PolygonMode polygonMode) const;
        void SetClearColour(const Colour& colour) const;
        void Clear() const;

        void BeginFrame();
        void EndFrame(const Camera2D& camera);

        void NewWorldBatch(const Camera2D& camera);
        void NewScreenBatch();

        void DrawWorldRect(const components::Transform& transform, const Colour& colour, const Camera2D& camera);
        void DrawWorldRect(const components::Transform& transform, const components::ShearTransform& shear, const Colour& colour, const Camera2D& camera);
        void DrawWorldRect(const components::Transform& transform, const components::Sprite& sprite, const Camera2D& camera);
        void DrawWorldRect(const components::Transform& transform, const components::ShearTransform& shear, const components::Sprite& sprite, const Camera2D& camera);

        void DrawWorldQuad(const Quad& quad, const components::Transform& transform, const Colour& colour, const Camera2D& camera);
        void DrawWorldQuad(const Quad& quad, const components::Transform& transform, const components::ShearTransform& shear, const Colour& colour, const Camera2D& camera);
        void DrawWorldQuad(const Quad& quad, const components::Transform& transform, const components::Sprite& sprite, const Camera2D& camera);
        void DrawWorldQuad(const Quad& quad, const components::Transform& transform, const components::ShearTransform& shear, const components::Sprite& sprite, const Camera2D& camera);

        void DrawWorldLine(const Vec2& pointA, const Vec2& pointB, const components::Transform& transform, const Colour& colour, const Camera2D& camera);

        void DrawScreenRect(const components::ScreenTransform& transform, const Colour& colour);
        void DrawScreenRect(const components::ScreenTransform& transform, const components::ShearTransform& shear, const Colour& colour);
        void DrawScreenRect(const components::ScreenTransform& transform, const components::Sprite& sprite);
        void DrawScreenRect(const components::ScreenTransform& transform, const components::ShearTransform& shear, const components::Sprite& sprite);
        
        void EnableScissorRect(const bool enable) const;
        void SetScissorRect(const IVec2& topLeft, const UVec2& size) const;
        void ResetScissorRect() const;

        void SetAntiAliasing(const bool enableAntiAliasing) const;

        [[nodiscard]] inline const UVec2& GetVirtualSize() const noexcept { return m_virtualSize; }
        [[nodiscard]] inline const Vec2& GetVirtualScale() const noexcept { return m_virtualScale; }
        [[nodiscard]] inline f32 GetVirtualAspectRatio() const noexcept { return m_virtualAspectRatio; }

        [[nodiscard]] inline const Mat4& GetScreenProjectionMatrix() const noexcept { return m_screenProjectionMatrix; }

        [[nodiscard]] Pair<UVec2, UVec2> GetViewportRect() const;
        [[nodiscard]] inline const Window& GetWindow() const noexcept { return *m_window; }

        [[nodiscard]] inline bool IsValid() const noexcept { return m_isValid; }

    private:
        void InitialiseVertexObjects();
        void InitialiseShaders();
        void InitialiseTextureIndices();

        void BeginWorldBatch();
        void EndWorldBatch();
        void FlushWorldBatch(const Camera2D& camera);

        void BeginScreenBatch();
        void EndScreenBatch();
        void FlushScreenBatch();

        [[nodiscard]] Mat4 CreateWorldModelMatrix(const Vec2& position, const Vec2& scale, const f32 rotation, const Optional<Vec2>& pivot, const Optional<Vec2>& shear = NullOpt) const;
        [[nodiscard]] Mat4 CreateScreenModelMatrix(const Vec2& position, const Vec2& size, const FlipType flip, const f32 rotation, const Optional<IVec2>& pivot, const Optional<Vec2>& shear = NullOpt) const;

        void GenerateRect(const Mat4& modelMatrix, const Colour& colour, const TextureCoordinatePair& textureCoordinates, const f32 textureIndex, BatchVertex*& bufferPtr, u32& indexCount);
        void GenerateQuad(const Quad& quad, const Mat4& modelMatrix, const Colour& colour, const TextureCoordinatePair& textureCoordinates, const f32 textureIndex, BatchVertex*& bufferPtr, u32& indexCount);

        void UpdateScreenProjectionMatrix();
    };
}

#endif