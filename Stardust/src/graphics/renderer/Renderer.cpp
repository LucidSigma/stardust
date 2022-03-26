#include "stardust/graphics/renderer/Renderer.h"

#include <utility>
#include <variant>

#include <ANGLE/GLES3/gl3.h>
#include <SDL2/SDL.h>

#include "stardust/graphics/colour/Colours.h"
#include "stardust/graphics/framebuffer/RenderBuffer.h"
#include "stardust/graphics/pipeline/Shader.h"
#include "stardust/graphics/Graphics.h"
#include "stardust/math/Math.h"
#include "stardust/utility/Utility.h"

namespace stardust
{
    namespace graphics
    {
        namespace
        {
            constexpr const char* ViewProjectionUniformName = "u_ViewProjection";
            constexpr const char* ScreenProjectionUniformName = "u_ScreenProjection";
        }

        Renderer::Renderer(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        Renderer::~Renderer() noexcept
        {
            Destroy();
        }

        auto Renderer::Initialise(const CreateInfo& createInfo) -> void
        {
            m_window = createInfo.window;
            m_camera = createInfo.camera;

            EnableScissorTest(true);
            SetViewport(GetRenderAreaFromZone(*m_window, RenderZone::Whole));
            SetScissorArea(GetRenderAreaFromZone(*m_window, RenderZone::Whole));

            SetClearColour(colours::Black);

            EnableDepthTest(true);
            SetDepthFunction(DepthFunction::LessThanOrEqual);

            glEnable(GL_BLEND);
            SetBlendMode(blend_modes::Alpha);

            InitialiseBlankTexture();
            InitialisePipelines(createInfo);
            InitialiseDrawingStates(createInfo);
        }

        auto Renderer::Destroy() noexcept -> void
        {
            m_blankTexture.Destroy();

            m_lineDrawState.Destroy();
            m_quadDrawState.Destroy();
            m_lineBatchState.Destroy();
            m_quadBatchState.Destroy();

            m_linePipeline.Destroy();
            m_quadPipeline.Destroy();
            m_lineBatchPipeline.Destroy();
            m_quadBatchPipeline.Destroy();

            m_window = nullptr;
            m_camera = nullptr;
        }

        [[nodiscard]] auto Renderer::IsValid() const noexcept -> bool
        {
            const bool areDrawingStatesValid = m_lineDrawState.IsValid() && m_quadDrawState.IsValid();
            const bool areBatchStatesValid = m_lineBatchState.IsValid() && m_quadBatchState.IsValid();
            const bool arePipelinesValid = m_linePipeline.IsValid() && m_quadPipeline.IsValid() && m_lineBatchPipeline.IsValid() && m_quadBatchPipeline.IsValid();
            const bool isBlankTextureValid = m_blankTexture.IsValid();

            return m_window != nullptr && m_camera != nullptr && areDrawingStatesValid && areBatchStatesValid && arePipelinesValid && isBlankTextureValid;
        }

        auto Renderer::ProcessResize() -> void
        {
            SetViewport(GetRenderAreaFromZone(*m_window, RenderZone::Whole));
            SetScissorArea(GetRenderAreaFromZone(*m_window, RenderZone::Whole));
        }

        [[nodiscard]] auto Renderer::GetClearColour() const -> Colour
        {
            Array<GLfloat, 4u> clearColourElements{ };
            glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColourElements.data());

            return Colour(clearColourElements[0], clearColourElements[1], clearColourElements[2], clearColourElements[3]);
        }

        auto Renderer::SetClearColour(const Colour& clearColour) const -> void
        {
            glClearColor(
                static_cast<f32>(clearColour.red) / 255.0f,
                static_cast<f32>(clearColour.green) / 255.0f,
                static_cast<f32>(clearColour.blue) / 255.0f,
                static_cast<f32>(clearColour.alpha) / 255.0f
            );
        }

        auto Renderer::EnableDepthTest(const bool enableDepthTest) const -> void
        {
            if (enableDepthTest)
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
        }

        auto Renderer::SetDepthFunction(const DepthFunction depthFunction) const -> void
        {
            glDepthFunc(static_cast<GLenum>(depthFunction));
        }

        auto Renderer::ClearColour() const -> void
        {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        auto Renderer::ClearDepthBuffer() const -> void
        {
            glClear(GL_DEPTH_BUFFER_BIT);
        }

        auto Renderer::ClearStencilBuffer() const -> void
        {
            glClear(GL_STENCIL_BUFFER_BIT);
        }

        auto Renderer::SetViewport(const RenderArea& viewport) -> void
        {
            m_viewport = viewport;

            glViewport(
                static_cast<GLint>(m_viewport.bottomLeft.x),
                static_cast<GLint>(m_viewport.bottomLeft.y),
                static_cast<GLsizei>(m_viewport.size.x),
                static_cast<GLsizei>(m_viewport.size.y)
            );
        }

        auto Renderer::EnableScissorTest(const bool enableScissorTest) const -> void
        {
            if (enableScissorTest)
            {
                glEnable(GL_SCISSOR_TEST);
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }
        }

        auto Renderer::SetScissorArea(const RenderArea& scissorArea) -> void
        {
            m_scissorArea = scissorArea;

            glScissor(
                static_cast<GLint>(m_scissorArea.bottomLeft.x),
                static_cast<GLint>(m_scissorArea.bottomLeft.y),
                static_cast<GLsizei>(m_scissorArea.size.x),
                static_cast<GLsizei>(m_scissorArea.size.y)
            );
        }

        auto Renderer::SetBlendMode(const BlendMode& blendMode) -> void
        {
            glBlendFuncSeparate(
                static_cast<GLenum>(blendMode.sourceColourFactor),
                static_cast<GLenum>(blendMode.destinationColourFactor),
                static_cast<GLenum>(blendMode.sourceAlphaFactor),
                static_cast<GLenum>(blendMode.destinationAlphaFactor)
            );

            glBlendEquationSeparate(
                static_cast<GLenum>(blendMode.colourOperation),
                static_cast<GLenum>(blendMode.alphaOperation)
            );

            if (blendMode.colourConstant.has_value())
            {
                glBlendColor(
                    static_cast<GLfloat>(blendMode.colourConstant.value().red) / 255.0f,
                    static_cast<GLfloat>(blendMode.colourConstant.value().green) / 255.0f,
                    static_cast<GLfloat>(blendMode.colourConstant.value().blue) / 255.0f,
                    static_cast<GLfloat>(blendMode.colourConstant.value().alpha) / 255.0f
                );
            }
            else
            {
                glBlendColor(
                    static_cast<GLfloat>(colours::White.red) / 255.0f,
                    static_cast<GLfloat>(colours::White.green) / 255.0f,
                    static_cast<GLfloat>(colours::White.blue) / 255.0f,
                    static_cast<GLfloat>(colours::White.alpha) / 255.0f
                );
            }
        }

        auto Renderer::SetRenderTarget(const DefaultFramebufferType) -> void
        {
            m_renderTarget = nullptr;
            Framebuffer::BindDefault();
        }

        auto Renderer::SetRenderTarget(const Framebuffer& framebuffer) -> void
        {
            m_renderTarget = &framebuffer;
            framebuffer.Bind();
        }

        [[nodiscard]] auto Renderer::GetRenderTargetSize() const noexcept -> UVector2
        {
            if (m_renderTarget == nullptr)
            {
                return m_window->GetSize();
            }
            else
            {
                return std::visit(
                    utility::VariantOverloader{
                        [](const ObserverPointer<const Texture> texture) { return texture->GetSize(); },
                        [](const ObserverPointer<const RenderBuffer> renderBuffer) { return renderBuffer->GetSize(); },
                    },
                    m_renderTarget->GetAttachment(Framebuffer::AttachmentType::Colour0)
                );
            }
        }

        auto Renderer::DrawPoint(const components::Transform& transform, const Colour& colour, const bool useInbuiltPipeline) -> void
        {
            DrawLine(
                geometry::Line{
                    .pointA = Vector2Zero,
                    .pointB = Vector2Down * 1.0f / m_camera->GetPixelsPerUnit(),
                },
                components::Transform{
                    .translation = transform.translation,
                    .scale = Vector2One,
                    .reflection = Reflection::None,
                    .rotation = transform.rotation,
                    .pivot = transform.pivot,
                    .shear = transform.shear,
                },
                colour,
                useInbuiltPipeline
            );
        }

        auto Renderer::DrawScreenPoint(const components::ScreenTransform& transform, const Colour& colour, const bool useInbuiltPipeline) -> void
        {
            DrawScreenLine(
                geometry::ScreenLine{
                    .pointA = IVector2Zero,
                    .pointB = IVector2Down,
                },
                components::ScreenTransform{
                    .translation = transform.translation,
                    .reflection = Reflection::None,
                    .rotation = transform.rotation,
                    .pivot = transform.pivot,
                    .shear = transform.shear,
                },
                colour,
                useInbuiltPipeline
            );
        }

        auto Renderer::DrawLine(const geometry::Line& line, const components::Transform& transform, const Colour& colour, const bool useInbuiltPipeline) -> void
        {
            if (useInbuiltPipeline)
            {
                UpdateActivePipeline(m_linePipeline);

                m_linePipeline.SetUniform<Matrix4>(ViewProjectionUniformName, m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix());
            }

            const Matrix4 modelMatrix = GetModelMatrixFromTransform(transform);
            m_lineDrawState.DrawLine(line, modelMatrix, colour);
        }

        auto Renderer::DrawScreenLine(const geometry::ScreenLine& line, const components::ScreenTransform& transform, const Colour& colour, const bool useInbuiltPipeline) -> void
        {
            if (useInbuiltPipeline)
            {
                UpdateActivePipeline(m_linePipeline);

                m_linePipeline.SetUniform<Matrix4>(ViewProjectionUniformName, m_camera->GetScreenProjectionMatrix());
            }

            const Matrix4 modelMatrix = GetModelMatrixFromScreenTransform(transform, line);
            m_lineDrawState.DrawScreenLine(line, modelMatrix, colour);
        }

        auto Renderer::DrawRectangleOutline(const components::Transform& transform, const Colour& colour, const bool useInbuiltPipeline) -> void
        {
            DrawLine(
                geometry::Line{
                    .pointA = Vector2{ 0.5f, 0.5f },
                    .pointB = Vector2{ -0.5f, 0.5f },
                },
                transform,
                colour,
                useInbuiltPipeline
            );

            DrawLine(
                geometry::Line{
                    .pointA = Vector2{ -0.5f, 0.5f },
                    .pointB = Vector2{ -0.5f, -0.5f },
                },
                transform,
                colour,
                useInbuiltPipeline
            );

            DrawLine(
                geometry::Line{
                    .pointA = Vector2{ -0.5f, -0.5f },
                    .pointB = Vector2{ 0.5f, -0.5f },
                },
                transform,
                colour,
                useInbuiltPipeline
            );

            DrawLine(
                geometry::Line{
                    .pointA = Vector2{ 0.5f, -0.5f },
                    .pointB = Vector2{ 0.5f, 0.5f },
                },
                transform,
                colour,
                useInbuiltPipeline
            );
        }

        auto Renderer::DrawScreenRectangleOutline(const UVector2 size, const components::ScreenTransform& transform, const Colour& colour, const bool useInbuiltPipeline) -> void
        {
            const IVector2 topLeftPointOffset = IVector2Zero;
            const IVector2 topRightPointOffset{ static_cast<i32>(size.x), 0 };
            const IVector2 bottomLeftPointOffset{ 0, static_cast<i32>(size.y) };
            const IVector2 bottomRightPointOffset(size);

            DrawScreenLine(
                geometry::ScreenLine{
                    .pointA = topRightPointOffset,
                    .pointB = topLeftPointOffset,
                },
                transform,
                colour,
                useInbuiltPipeline
            );

            DrawScreenLine(
                geometry::ScreenLine{
                    .pointA = topLeftPointOffset,
                    .pointB = bottomLeftPointOffset,
                },
                transform,
                colour,
                useInbuiltPipeline
            );

            DrawScreenLine(
                geometry::ScreenLine{
                    .pointA = bottomLeftPointOffset,
                    .pointB = bottomRightPointOffset,
                },
                transform,
                colour,
                useInbuiltPipeline
            );

            DrawScreenLine(
                geometry::ScreenLine{
                    .pointA = bottomRightPointOffset,
                    .pointB = topRightPointOffset,
                },
                transform,
                colour,
                useInbuiltPipeline
            );
        }

        auto Renderer::DrawRectangle(const components::Transform& transform, const components::Sprite& sprite, const bool useInbuiltPipeline) -> void
        {
            if (useInbuiltPipeline)
            {
                UpdateActivePipeline(m_quadPipeline);

                m_quadPipeline.SetUniform<Matrix4>(ViewProjectionUniformName, m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix());
            }

            const Matrix4 modelMatrix = GetModelMatrixFromTransform(transform);
            m_quadDrawState.DrawRectangle(modelMatrix, sprite);
        }

        auto Renderer::DrawScreenRectangle(const UVector2 size, const components::ScreenTransform& transform, const components::Sprite& sprite, const bool useInbuiltPipeline) -> void
        {
            if (useInbuiltPipeline)
            {
                UpdateActivePipeline(m_quadPipeline);

                m_quadPipeline.SetUniform<Matrix4>(ViewProjectionUniformName, m_camera->GetScreenProjectionMatrix());
            }

            const Matrix4 modelMatrix = GetModelMatrixFromScreenTransform(transform, size);
            m_quadDrawState.DrawScreenRectangle(size, modelMatrix, sprite);
        }

        auto Renderer::DrawQuad(const geometry::Quad& quad, const components::Transform& transform, const components::Sprite& sprite, const bool useInbuiltPipeline) -> void
        {
            if (useInbuiltPipeline)
            {
                UpdateActivePipeline(m_quadPipeline);

                m_quadPipeline.SetUniform<Matrix4>(ViewProjectionUniformName, m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix());
            }

            const Matrix4 modelMatrix = GetModelMatrixFromTransform(transform);
            m_quadDrawState.DrawQuad(quad, modelMatrix, sprite);
        }

        auto Renderer::DrawScreenQuad(const geometry::ScreenQuad& quad, const components::ScreenTransform& transform, const components::Sprite& sprite, const bool useInbuiltPipeline) -> void
        {
            UpdateActivePipeline(m_quadPipeline);

            m_quadPipeline.SetUniform<Matrix4>(ViewProjectionUniformName, m_camera->GetScreenProjectionMatrix());

            const Matrix4 modelMatrix = GetModelMatrixFromScreenTransform(transform, quad);
            m_quadDrawState.DrawScreenQuad(quad, modelMatrix, sprite);
        }

        auto Renderer::StartLineBatch() -> void
        {
            m_lineBatchState.Begin();
        }

        auto Renderer::BatchPoint(const components::Transform& transform, const Colour& colour) -> void
        {
            BatchLine(
                geometry::Line{
                    .pointA = Vector2Zero,
                    .pointB = Vector2Down * 1.0f / m_camera->GetPixelsPerUnit(),
                },
                components::Transform{
                    .translation = transform.translation,
                    .scale = Vector2One,
                    .reflection = Reflection::None,
                    .rotation = transform.rotation,
                    .pivot = transform.pivot,
                    .shear = transform.shear,
                },
                colour
            );
        }

        auto Renderer::BatchScreenPoint(const components::ScreenTransform& transform, const Colour& colour) -> void
        {
            BatchScreenLine(
                geometry::ScreenLine{
                    .pointA = IVector2Zero,
                    .pointB = IVector2Down,
                },
                components::ScreenTransform{
                    .translation = transform.translation,
                    .reflection = Reflection::None,
                    .rotation = transform.rotation,
                    .pivot = transform.pivot,
                    .shear = transform.shear,
                },
                colour
            );
        }

        auto Renderer::BatchLine(const geometry::Line& line, const components::Transform& transform, const Colour& colour) -> void
        {
            const Matrix4 modelMatrix = GetModelMatrixFromTransform(transform);

            m_lineBatchState.BatchLine(line, modelMatrix, colour);
        }

        auto Renderer::BatchScreenLine(const geometry::ScreenLine& line, const components::ScreenTransform& transform, const Colour& colour) -> void
        {
            const Matrix4 modelMatrix = GetModelMatrixFromScreenTransform(transform, line);

            m_lineBatchState.BatchScreenLine(line, modelMatrix, colour);
        }

        auto Renderer::BatchRectangleOutline(const components::Transform& transform, const Colour& colour) -> void
        {
            BatchLine(
                geometry::Line{
                    .pointA = Vector2{ 0.5f, 0.5f },
                    .pointB = Vector2{ -0.5f, 0.5f },
                },
                transform,
                colour
            );

            BatchLine(
                geometry::Line{
                    .pointA = Vector2{ -0.5f, 0.5f },
                    .pointB = Vector2{ -0.5f, -0.5f },
                },
                transform,
                colour
            );

            BatchLine(
                geometry::Line{
                    .pointA = Vector2{ -0.5f, -0.5f },
                    .pointB = Vector2{ 0.5f, -0.5f },
                },
                transform,
                colour
            );

            BatchLine(
                geometry::Line{
                    .pointA = Vector2{ 0.5f, -0.5f },
                    .pointB = Vector2{ 0.5f, 0.5f },
                },
                transform,
                colour
            );
        }

        auto Renderer::BatchScreenRectangleOutline(const UVector2 size, const components::ScreenTransform& transform, const Colour& colour) -> void
        {
            const IVector2 topLeftPointOffset = IVector2Zero;
            const IVector2 topRightPointOffset{ static_cast<i32>(size.x), 0 };
            const IVector2 bottomLeftPointOffset{ 0, static_cast<i32>(size.y) };
            const IVector2 bottomRightPointOffset(size);

            BatchScreenLine(
                geometry::ScreenLine{
                    .pointA = topRightPointOffset,
                    .pointB = topLeftPointOffset,
                },
                transform,
                colour
            );

            BatchScreenLine(
                geometry::ScreenLine{
                    .pointA = topLeftPointOffset,
                    .pointB = bottomLeftPointOffset,
                },
                transform,
                colour
            );

            BatchScreenLine(
                geometry::ScreenLine{
                    .pointA = bottomLeftPointOffset,
                    .pointB = bottomRightPointOffset,
                },
                transform,
                colour
            );

            BatchScreenLine(
                geometry::ScreenLine{
                    .pointA = bottomRightPointOffset,
                    .pointB = topRightPointOffset,
                },
                transform,
                colour
            );
        }

        auto Renderer::FlushLineBatch(const bool useInbuiltPipeline) -> void
        {
            if (useInbuiltPipeline)
            {
                UpdateActivePipeline(m_lineBatchPipeline);

                m_lineBatchPipeline.SetUniform<Matrix4>(ViewProjectionUniformName, m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix());
                m_lineBatchPipeline.SetUniform<Matrix4>(ScreenProjectionUniformName, m_camera->GetScreenProjectionMatrix());
            }

            m_lineBatchState.Flush();
        }

        auto Renderer::RestartLineBatch(const bool useInbuiltPipeline) -> void
        {
            FlushLineBatch(useInbuiltPipeline);
            StartLineBatch();
        }

        auto Renderer::StartQuadBatch() -> void
        {
            m_quadBatchState.Begin();
        }

        auto Renderer::BatchRectangle(const components::Transform& transform, const components::Sprite& sprite) -> void
        {
            const Matrix4 modelMatrix = GetModelMatrixFromTransform(transform);

            m_quadBatchState.BatchRectangle(modelMatrix, sprite);
        }

        auto Renderer::BatchScreenRectangle(const UVector2 size, const components::ScreenTransform& transform, const components::Sprite& sprite) -> void
        {
            const Matrix4 modelMatrix = GetModelMatrixFromScreenTransform(transform, size);

            m_quadBatchState.BatchScreenRectangle(size, modelMatrix, sprite);
        }

        auto Renderer::BatchQuad(const geometry::Quad& quad, const components::Transform& transform, const components::Sprite& sprite) -> void
        {
            const Matrix4 modelMatrix = GetModelMatrixFromTransform(transform);

            m_quadBatchState.BatchQuad(quad, modelMatrix, sprite);
        }

        auto Renderer::BatchScreenQuad(const geometry::ScreenQuad& quad, const components::ScreenTransform& transform, const components::Sprite& sprite) -> void
        {
            const Matrix4 modelMatrix = GetModelMatrixFromScreenTransform(transform, quad);

            m_quadBatchState.BatchScreenQuad(quad, modelMatrix, sprite);
        }

        auto Renderer::FlushQuadBatch(const bool useInbuiltPipeline) -> void
        {
            if (useInbuiltPipeline)
            {
                UpdateActivePipeline(m_quadBatchPipeline);

                m_quadBatchPipeline.SetUniform<Matrix4>(ViewProjectionUniformName, m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix());
                m_quadBatchPipeline.SetUniform<Matrix4>(ScreenProjectionUniformName, m_camera->GetScreenProjectionMatrix());
            }

            m_quadBatchState.Flush();
        }

        auto Renderer::RestartQuadBatch(const bool useInbuiltPipeline) -> void
        {
            FlushQuadBatch(useInbuiltPipeline);
            StartQuadBatch();
        }

        auto Renderer::EnableStencilTest(const bool enableStencilTest) const -> void
        {
            if (enableStencilTest)
            {
                glEnable(GL_STENCIL_TEST);
            }
            else
            {
                glDisable(GL_STENCIL_TEST);
            }
        }

        auto Renderer::SetStencilParameters(const StencilParameters& stencilParameters) const -> void
        {
            glStencilFunc(
                static_cast<GLenum>(stencilParameters.function),
                static_cast<i32>(stencilParameters.reference),
                static_cast<u32>(stencilParameters.mask)
            );
        }

        auto Renderer::SetStencilOperations(const StencilOperations& stencilOperations) const -> void
        {
            glStencilOp(
                static_cast<GLenum>(stencilOperations.onStencilFail),
                static_cast<GLenum>(stencilOperations.onStencilPassAndDepthFail),
                static_cast<GLenum>(stencilOperations.onStencilPassAndDepthPass)
            );
        }

        [[nodiscard]] auto Renderer::ReadPixels() const -> PixelData
        {
            if (m_renderTarget == nullptr)
            {
                return ReadPixels(GetRenderAreaFromZone(*m_window, RenderZone::Whole));
            }
            else
            {
                return std::visit(
                    utility::VariantOverloader{
                        [this](const ObserverPointer<const Texture> texture) -> PixelData
                        {
                            return ReadPixels(GetRenderAreaFromZone(*texture, RenderZone::Whole));
                        },
                        [this](const ObserverPointer<const RenderBuffer> renderBuffer) -> PixelData
                        {
                            return ReadPixels(GetRenderAreaFromZone(*renderBuffer, RenderZone::Whole));
                        },
                    },
                    m_renderTarget->GetAttachment(Framebuffer::AttachmentType::Colour0)
                );
            }
        }

        [[nodiscard]] auto Renderer::ReadPixels(const RenderZone renderZone) const -> PixelData
        {
            return ReadPixels(GetRenderAreaFromZone(*m_window, renderZone));

            if (m_renderTarget == nullptr)
            {
                return ReadPixels(GetRenderAreaFromZone(*m_window, renderZone));
            }
            else
            {
                return std::visit(
                    utility::VariantOverloader{
                        [this, &renderZone](const ObserverPointer<const Texture> texture) -> PixelData
                        {
                            return ReadPixels(GetRenderAreaFromZone(*texture, renderZone));
                        },
                        [this, &renderZone](const ObserverPointer<const RenderBuffer> renderBuffer) -> PixelData
                        {
                            return ReadPixels(GetRenderAreaFromZone(*renderBuffer, renderZone));
                        },
                    },
                    m_renderTarget->GetAttachment(Framebuffer::AttachmentType::Colour0)
                );
            }
        }

        [[nodiscard]] auto Renderer::ReadPixels(const RenderArea& readArea) const -> PixelData
        {
            constexpr usize PixelChannelCount = 4u;
            List<ubyte> imageData(PixelChannelCount * static_cast<usize>(readArea.size.x) * static_cast<usize>(readArea.size.y));

            glReadPixels(
                static_cast<GLint>(readArea.bottomLeft.x),
                static_cast<GLint>(readArea.bottomLeft.y),
                static_cast<GLsizei>(readArea.size.x),
                static_cast<GLsizei>(readArea.size.y),
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                imageData.data()
            );

            return PixelData{
                .pixels = std::move(imageData),
                .extent = readArea.size,
                .channelCount = static_cast<u32>(PixelChannelCount),
            };
        }

        auto Renderer::InitialiseBlankTexture() -> void
        {
            m_blankTexture.Initialise({ 0xFFu, 0xFFu, 0xFFu, 0xFFu }, UVector2One, 4u);
        }

        auto Renderer::InitialisePipelines(const CreateInfo& createInfo) -> void
        {
            if (InitialisePipeline(m_linePipeline, createInfo.shadersDirectoryPath + "/line.vert", createInfo.shadersDirectoryPath + "/line.frag") != Status::Success)
            {
                return;
            }

            if (InitialisePipeline(m_quadPipeline, createInfo.shadersDirectoryPath + "/quad.vert", createInfo.shadersDirectoryPath + "/quad.frag") != Status::Success)
            {
                return;
            }

            if (InitialisePipeline(m_lineBatchPipeline, createInfo.shadersDirectoryPath + "/line_batch.vert", createInfo.shadersDirectoryPath + "/line_batch.frag") != Status::Success)
            {
                return;
            }

            if (InitialisePipeline(m_quadBatchPipeline, createInfo.shadersDirectoryPath + "/quad_batch.vert", createInfo.shadersDirectoryPath + "/quad_batch.frag") != Status::Success)
            {
                return;
            }
        }

        [[nodiscard]] auto Renderer::InitialisePipeline(Pipeline& pipeline, const StringView vertexShaderFilepath, const StringView fragmentShaderFilepath) -> Status
        {
            const Shader vertexShader(Shader::Type::Vertex, vertexShaderFilepath);
            const Shader fragmentShader(Shader::Type::Fragment, fragmentShaderFilepath);

            if (!vertexShader.IsValid() || !fragmentShader.IsValid())
            {
                return Status::Fail;
            }

            pipeline.Initialise(vertexShader, fragmentShader);

            return pipeline.IsValid()
                ? Status::Success
                : Status::Fail;
        }

        auto Renderer::InitialiseDrawingStates(const CreateInfo& createInfo) -> void
        {
            if (!m_blankTexture.IsValid())
            {
                return;
            }

            m_lineDrawState.Initialise();
            m_quadDrawState.Initialise(m_blankTexture);

            m_lineBatchState.Initialise(createInfo.maxShapesPerBatch);
            m_quadBatchState.Initialise(QuadBatchState::CreateInfo{
                .maxShapesPerBatch = createInfo.maxShapesPerBatch,
                .batchPipeline = &m_quadBatchPipeline,
                .textureArrayUniformName = "u_Textures",
                .defaultTexture = &m_blankTexture,
                .maxTextureSlots = createInfo.maxTextureSlotsPerBatch,
            });
        }

        auto Renderer::UpdateActivePipeline(const Pipeline& pipelineToUse) -> void
        {
            if (m_mostRecentlyUsedPipeline != &pipelineToUse)
            {
                pipelineToUse.Use();
                m_mostRecentlyUsedPipeline = &pipelineToUse;
            }
        }

        [[nodiscard]] auto Renderer::GetModelMatrixFromTransform(const components::Transform& transform) const -> Matrix4
        {
            Matrix4 modelMatrix = Matrix4Identity;
            modelMatrix = glm::translate(modelMatrix, Vector3{ transform.translation, 0.0f });

            if (transform.pivot.has_value())
            {
                modelMatrix = glm::translate(modelMatrix, Vector3{ transform.pivot.value(), 0.0f });
            }

            modelMatrix = glm::rotate(modelMatrix, -glm::radians(transform.rotation), Vector3Forward);
            
            if (transform.pivot.has_value())
            {
                modelMatrix = glm::translate(modelMatrix, Vector3{ -transform.pivot.value(), 0.0f });
            }

            if (transform.shear.has_value())
            {
                modelMatrix = glm::shearY3D(modelMatrix, glm::tan(glm::radians(transform.shear.value().x)), 0.0f);
                modelMatrix = glm::shearX3D(modelMatrix, glm::tan(glm::radians(transform.shear.value().y)), 0.0f);
            }

            glm::vec2 scale = transform.scale;

            if (transform.reflection == Reflection::Horizontal || transform.reflection == Reflection::Both)
            {
                scale.x *= -1.0f;
            }

            if (transform.reflection == Reflection::Vertical || transform.reflection == Reflection::Both)
            {
                scale.y *= -1.0f;
            }

            modelMatrix = glm::scale(modelMatrix, Vector3{ scale, 1.0f });

            return modelMatrix;
        }

        [[nodiscard]] auto Renderer::GetModelMatrixFromScreenTransform(const components::ScreenTransform& transform, const geometry::ScreenLine& line) const -> Matrix4
        {
            IVector2 translation = transform.translation;
            IVector2 pivot = transform.pivot.value_or(IVector2Zero) + line.pointA;

            if (transform.reflection != Reflection::None)
            {
                const IVector2 lineDimensions = glm::abs(line.pointB - line.pointA);

                if (transform.reflection == Reflection::Horizontal || transform.reflection == Reflection::Both)
                {
                    const i32 xOffset = lineDimensions.x;

                    translation.x += xOffset;
                    pivot.x -= xOffset;
                }

                if (transform.reflection == Reflection::Vertical || transform.reflection == Reflection::Both)
                {
                    const i32 yOffset = lineDimensions.y;

                    translation.y += yOffset;
                    pivot.y -= yOffset;
                }
            }

            return GetModelMatrixFromScreenTransform(transform, translation, pivot);
        }
        
        [[nodiscard]] auto Renderer::GetModelMatrixFromScreenTransform(const components::ScreenTransform& transform, const UVector2 rectangleSize) const -> Matrix4
        {
            IVector2 translation = transform.translation;
            IVector2 pivot = transform.pivot.value_or(IVector2Zero) + (IVector2(rectangleSize) / 2);
            
            if (transform.reflection == Reflection::Horizontal || transform.reflection == Reflection::Both)
            {
                translation.x += rectangleSize.x;
                pivot.x -= rectangleSize.x;
            }
            
            if (transform.reflection == Reflection::Vertical || transform.reflection == Reflection::Both)
            {
                translation.y += rectangleSize.y;
                pivot.y -= rectangleSize.y;
            }

            return GetModelMatrixFromScreenTransform(transform, translation, pivot);
        }

        [[nodiscard]] auto Renderer::GetModelMatrixFromScreenTransform(const components::ScreenTransform& transform, const geometry::ScreenQuad& quad) const -> Matrix4
        {
            IVector2 translation = transform.translation;
            IVector2 pivot = transform.pivot.value_or(IVector2Zero) + quad.upperLeft;

            if (transform.reflection != Reflection::None)
            {
                const i32 leftmostPoint = glm::min(quad.lowerLeft.x, quad.upperLeft.x);
                const i32 rightmostPoint = glm::max(quad.upperRight.x, quad.lowerRight.x);
                const i32 topmostPoint = glm::min(quad.upperLeft.y, quad.upperRight.y);
                const i32 bottommostPoint = glm::max(quad.lowerLeft.y, quad.lowerRight.y);

                const IVector2 quadDimensions{ glm::abs(rightmostPoint - leftmostPoint), glm::abs(bottommostPoint - topmostPoint) };

                if (transform.reflection == Reflection::Horizontal || transform.reflection == Reflection::Both)
                {
                    const i32 xOffset = quadDimensions.x;

                    translation.x += xOffset;
                    pivot.x -= xOffset;
                }

                if (transform.reflection == Reflection::Vertical || transform.reflection == Reflection::Both)
                {
                    const i32 yOffset = quadDimensions.y;

                    translation.y += yOffset;
                    pivot.y -= yOffset;
                }
            }

            return GetModelMatrixFromScreenTransform(transform, translation, pivot);
        }
        
        [[nodiscard]] auto Renderer::GetModelMatrixFromScreenTransform(const components::ScreenTransform& transform, const IVector2 translation, const IVector2 pivot) const -> Matrix4
        {
            Matrix4 modelMatrix = Matrix4Identity;

            modelMatrix = glm::translate(
                modelMatrix,
                Vector3{
                    translation.x,
                    translation.y,
                    0.0f,
                }
            );

            modelMatrix = glm::translate(
                modelMatrix,
                Vector3{
                    pivot,
                    0.0f,
                }
            );

            modelMatrix = glm::rotate(modelMatrix, -glm::radians(transform.rotation), Vector3Forward);

            modelMatrix = glm::translate(
                modelMatrix,
                Vector3{
                    -pivot,
                    0.0f,
                }
            );

            if (transform.shear.has_value())
            {
                modelMatrix = glm::shearY3D(modelMatrix, glm::tan(glm::radians(transform.shear.value().x)), 0.0f);
                modelMatrix = glm::shearX3D(modelMatrix, glm::tan(glm::radians(transform.shear.value().y)), 0.0f);
            }

            Vector2 scale = Vector2One;

            if (transform.reflection == Reflection::Horizontal || transform.reflection == Reflection::Both)
            {
                scale.x *= -1.0f;
            }

            if (transform.reflection == Reflection::Vertical || transform.reflection == Reflection::Both)
            {
                scale.y *= -1.0f;
            }

            modelMatrix = glm::scale(modelMatrix, Vector3{ scale, 1.0f });

            return modelMatrix;
        }
    }
}
