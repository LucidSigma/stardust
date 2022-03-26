#pragma once
#ifndef STARDUST_QUAD_DRAW_STATE_H
#define STARDUST_QUAD_DRAW_STATE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/ecs/components/SpriteComponent.h"
#include "stardust/geometry/Shapes.h"
#include "stardust/graphics/renderer/objects/IndexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace graphics
    {
        class QuadDrawState final
            : private INoncopyable
        {
        private:
            VertexLayout m_vertexLayout;
            VertexBuffer m_vertexBuffer;
            IndexBuffer m_indexBuffer;

            ObserverPointer<const Texture> m_defaultTexture = nullptr;

        public:
            QuadDrawState() = default;
            explicit QuadDrawState(const Texture& defaultTexture);

            QuadDrawState(QuadDrawState&& other) noexcept;
            auto operator =(QuadDrawState&& other) noexcept -> QuadDrawState&;

            ~QuadDrawState() noexcept;

            auto Initialise(const Texture& defaultTexture) -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] auto IsValid() const noexcept -> bool;

            auto DrawRectangle(const Matrix4& modelMatrix, const components::Sprite& sprite) -> void;
            auto DrawScreenRectangle(const UVector2 size, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void;
            auto DrawQuad(const geometry::Quad& quad, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void;
            auto DrawScreenQuad(const geometry::ScreenQuad& quad, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void;
        };
    }
}

#endif
