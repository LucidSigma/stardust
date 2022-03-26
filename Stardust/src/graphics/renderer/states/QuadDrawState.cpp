#include "stardust/graphics/renderer/states/QuadDrawState.h"

#include <utility>

#include "stardust/graphics/renderer/objects/BufferUsage.h"
#include "stardust/graphics/renderer/objects/VertexAttribute.h"
#include "stardust/graphics/renderer/objects/VertexLayoutBuilder.h"
#include "stardust/graphics/renderer/objects/Vertices.h"
#include "stardust/math/Math.h"

namespace stardust
{
    namespace graphics
    {
        QuadDrawState::QuadDrawState(const Texture& defaultTexture)
        {
            Initialise(defaultTexture);
        }

        QuadDrawState::QuadDrawState(QuadDrawState&& other) noexcept
        {
            Destroy();

            m_vertexLayout = std::move(other.m_vertexLayout);
            m_vertexBuffer = std::move(other.m_vertexBuffer);
            m_indexBuffer = std::move(other.m_indexBuffer);

            m_defaultTexture = std::exchange(other.m_defaultTexture, nullptr);
        }

        auto QuadDrawState::operator =(QuadDrawState&& other) noexcept -> QuadDrawState&
        {
            Destroy();

            m_vertexLayout = std::move(other.m_vertexLayout);
            m_vertexBuffer = std::move(other.m_vertexBuffer);
            m_indexBuffer = std::move(other.m_indexBuffer);

            m_defaultTexture = std::exchange(other.m_defaultTexture, nullptr);

            return *this;
        }

        QuadDrawState::~QuadDrawState() noexcept
        {
            Destroy();
        }

        auto QuadDrawState::Initialise(const Texture& defaultTexture) -> void
        {
            m_defaultTexture = &defaultTexture;
            m_vertexBuffer.Initialise(4u * sizeof(QuadVertex), BufferUsage::Dynamic);

            m_indexBuffer.Initialise({
                0u, 1u, 3u,
                1u, 2u, 3u,
            });

            m_vertexLayout = VertexLayoutBuilder()
                .AddAttribute(VertexAttribute{
                    .elementCount = 2u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddAttribute(VertexAttribute{
                    .elementCount = 4u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddAttribute(VertexAttribute{
                    .elementCount = 2u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddVertexBuffer(m_vertexBuffer)
                .Build();
        }

        auto QuadDrawState::Destroy() noexcept -> void
        {
            if (m_vertexLayout.IsValid())
            {
                m_vertexLayout.Destroy();
            }

            if (m_indexBuffer.IsValid())
            {
                m_indexBuffer.Destroy();
            }

            if (m_vertexBuffer.IsValid())
            {
                m_vertexBuffer.Destroy();
            }

            m_defaultTexture = nullptr;
        }

        [[nodiscard]] auto QuadDrawState::IsValid() const noexcept -> bool
        {
            return m_vertexLayout.IsValid() && m_vertexBuffer.IsValid() && m_indexBuffer.IsValid() && m_defaultTexture->IsValid();
        }

        auto QuadDrawState::DrawRectangle(const Matrix4& modelMatrix, const components::Sprite& sprite) -> void
        {
            m_vertexBuffer.SetSubData<QuadVertex>(
                {
                    QuadVertex{
                        .position = Vector2(modelMatrix * Vector4{ 0.5f, 0.5f, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? sprite.subTextureArea.value().upperRight
                            : Vector2One,
                    },
                    QuadVertex{
                        .position = Vector2(modelMatrix * Vector4{ 0.5f, -0.5f, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? Vector2{ sprite.subTextureArea.value().upperRight.x, sprite.subTextureArea.value().lowerLeft.y }
                            : Vector2{ 1.0f, 0.0f },
                    },
                    QuadVertex{
                        .position = Vector2(modelMatrix * Vector4{ -0.5f, -0.5f, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? sprite.subTextureArea.value().lowerLeft
                            : Vector2Zero,
                    },
                    QuadVertex{
                        .position = Vector2(modelMatrix * Vector4{ -0.5f, 0.5f, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? Vector2{ sprite.subTextureArea.value().lowerLeft.x, sprite.subTextureArea.value().upperRight.y }
                            : Vector2{ 0.0f, 1.0f },
                    },
                },
                4u
            );

            const ObserverPointer<const Texture> texture = sprite.texture != nullptr ? sprite.texture : m_defaultTexture;
            texture->Bind();

            m_vertexLayout.Bind();
            m_vertexLayout.DrawIndexed(m_indexBuffer);
            m_vertexLayout.Unbind();

            texture->Unbind();
        }

        auto QuadDrawState::DrawScreenRectangle(const UVector2 size, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void
        {
            m_vertexBuffer.SetSubData<QuadVertex>(
                {
                    QuadVertex{
                        .position = Vector2(modelMatrix * Vector4{ size.x, 0.0f, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? sprite.subTextureArea.value().upperRight
                            : Vector2One,
                    },
                    QuadVertex{
                        .position = Vector2(modelMatrix * Vector4{ size, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? Vector2{ sprite.subTextureArea.value().upperRight.x, sprite.subTextureArea.value().lowerLeft.y }
                            : Vector2{ 1.0f, 0.0f },
                    },
                    QuadVertex{
                        .position = Vector2(modelMatrix * Vector4{ 0.0f, size.y, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? sprite.subTextureArea.value().lowerLeft
                            : Vector2Zero,
                    },
                    QuadVertex{
                        .position = Vector2(modelMatrix * Vector4{ 0.0f, 0.0f, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? Vector2{ sprite.subTextureArea.value().lowerLeft.x, sprite.subTextureArea.value().upperRight.y }
                            : Vector2{ 0.0f, 1.0f },
                    },
                },
                4u
            );

            const ObserverPointer<const Texture> texture = sprite.texture != nullptr ? sprite.texture : m_defaultTexture;
            texture->Bind();

            m_vertexLayout.Bind();
            m_vertexLayout.DrawIndexed(m_indexBuffer);
            m_vertexLayout.Unbind();

            texture->Unbind();
        }

        auto QuadDrawState::DrawQuad(const geometry::Quad& quad, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void
        {
            m_vertexBuffer.SetSubData<QuadVertex>(
                {
                    QuadVertex{
                        .position = modelMatrix * Vector4{ quad.upperRight, 0.0f, 1.0f },
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? sprite.subTextureArea.value().upperRight
                            : Vector2{ 1.0f, 1.0f },
                    },
                    QuadVertex{
                        .position = modelMatrix * Vector4{ quad.lowerRight, 0.0f, 1.0f },
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? Vector2{ sprite.subTextureArea.value().upperRight.x, sprite.subTextureArea.value().lowerLeft.y }
                            : Vector2{ 1.0f, 0.0f },
                    },
                    QuadVertex{
                        .position = modelMatrix * Vector4{ quad.lowerLeft, 0.0f, 1.0f },
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? sprite.subTextureArea.value().lowerLeft
                            : Vector2{ 0.0f, 0.0f },
                    },
                    QuadVertex{
                        .position = modelMatrix * Vector4{ quad.upperLeft, 0.0f, 1.0f },
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? Vector2{ sprite.subTextureArea.value().lowerLeft.x, sprite.subTextureArea.value().upperRight.y }
                            : Vector2{ 0.0f, 1.0f },
                    },
                },
                4u
            );

            const ObserverPointer<const Texture> texture = sprite.texture != nullptr ? sprite.texture : m_defaultTexture;
            texture->Bind();

            m_vertexLayout.Bind();
            m_vertexLayout.DrawIndexed(m_indexBuffer);
            m_vertexLayout.Unbind();

            texture->Unbind();
        }

        auto QuadDrawState::DrawScreenQuad(const geometry::ScreenQuad& quad, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void
        {
            m_vertexBuffer.SetSubData<QuadVertex>(
                {
                    QuadVertex{
                        .position = modelMatrix * Vector4{ quad.upperRight, 0.0f, 1.0f },
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? sprite.subTextureArea.value().upperRight
                            : Vector2{ 1.0f, 1.0f },
                    },
                    QuadVertex{
                        .position = modelMatrix * Vector4{ quad.lowerRight, 0.0f, 1.0f },
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? Vector2{ sprite.subTextureArea.value().upperRight.x, sprite.subTextureArea.value().lowerLeft.y }
                            : Vector2{ 1.0f, 0.0f },
                    },
                    QuadVertex{
                        .position = modelMatrix * Vector4{ quad.lowerLeft, 0.0f, 1.0f },
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? sprite.subTextureArea.value().lowerLeft
                            : Vector2{ 0.0f, 0.0f },
                    },
                    QuadVertex{
                        .position = modelMatrix * Vector4{ quad.upperLeft, 0.0f, 1.0f },
                        .colour = static_cast<Vector4>(sprite.colourMod),
                        .textureCoordinates = sprite.subTextureArea.has_value()
                            ? Vector2{ sprite.subTextureArea.value().lowerLeft.x, sprite.subTextureArea.value().upperRight.y }
                            : Vector2{ 0.0f, 1.0f },
                    },
                },
                4u
            );

            const ObserverPointer<const Texture> texture = sprite.texture != nullptr ? sprite.texture : m_defaultTexture;
            texture->Bind();

            m_vertexLayout.Bind();
            m_vertexLayout.DrawIndexed(m_indexBuffer);
            m_vertexLayout.Unbind();

            texture->Unbind();
        }
    }
}
