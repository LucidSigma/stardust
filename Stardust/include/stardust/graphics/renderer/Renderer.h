#pragma once
#ifndef STARDUST_RENDERER_H
#define STARDUST_RENDERER_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include "stardust/camera/Camera2D.h"
#include "stardust/ecs/components/ScreenTransformComponent.h"
#include "stardust/ecs/components/SpriteComponent.h"
#include "stardust/ecs/components/TransformComponent.h"
#include "stardust/geometry/Shapes.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/framebuffer/Framebuffer.h"
#include "stardust/graphics/pipeline/Pipeline.h"
#include "stardust/graphics/renderer/states/LineBatchState.h"
#include "stardust/graphics/renderer/states/LineDrawState.h"
#include "stardust/graphics/renderer/states/QuadBatchState.h"
#include "stardust/graphics/renderer/states/QuadDrawState.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Blending.h"
#include "stardust/graphics/RenderArea.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
    namespace graphics
    {
        class Renderer final
            : private INoncopyable, private INonmovable
        {
        public:
            struct CreateInfo final
            {
                ObserverPointer<const Window> window;
                ObserverPointer<const Camera2D> camera;

                String shadersDirectoryPath;

                usize maxShapesPerBatch;
                usize maxTextureSlotsPerBatch;
            };

        private:
            ObserverPointer<const Window> m_window = nullptr;
            ObserverPointer<const Camera2D> m_camera = nullptr;

            ObserverPointer<const Framebuffer> m_renderTarget = nullptr;

            RenderArea m_viewport{ };
            RenderArea m_scissorArea{ };

            Texture m_blankTexture;

            Pipeline m_linePipeline;
            Pipeline m_quadPipeline;
            Pipeline m_lineBatchPipeline;
            Pipeline m_quadBatchPipeline;

            LineDrawState m_lineDrawState;
            QuadDrawState m_quadDrawState;

            LineBatchState m_lineBatchState;
            QuadBatchState m_quadBatchState;

            ObserverPointer<const Pipeline> m_mostRecentlyUsedPipeline = nullptr;

        public:
            Renderer() = default;
            explicit Renderer(const CreateInfo& createInfo);
            ~Renderer() noexcept;

            auto Initialise(const CreateInfo& createInfo) -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] auto IsValid() const noexcept -> bool;

            auto ProcessResize() -> void;

            [[nodiscard]] auto GetClearColour() const -> Colour;
            auto SetClearColour(const Colour& clearColour) const -> void;

            auto EnableDepthTest(const bool enableDepthTest) const -> void;
            auto SetDepthFunction(const DepthFunction depthFunction) const -> void;

            auto ClearColour() const -> void;
            auto ClearDepthBuffer() const -> void;
            auto ClearStencilBuffer() const -> void;

            [[nodiscard]] inline auto GetViewport() const noexcept -> const RenderArea& { return m_viewport; }
            auto SetViewport(const RenderArea& viewport) -> void;

            auto EnableScissorTest(const bool enableScissorTest) const -> void;
            [[nodiscard]] inline auto GetScissorArea() const noexcept -> const RenderArea& { return m_scissorArea; }
            auto SetScissorArea(const RenderArea& scissorArea) -> void;

            auto SetBlendMode(const BlendMode& blendMode) -> void;

            auto SetRenderTarget(const DefaultFramebufferType) -> void;
            auto SetRenderTarget(const Framebuffer& framebuffer) -> void;
            [[nodiscard]] inline auto GetRenderTarget() const noexcept -> ObserverPointer<const Framebuffer> { return m_renderTarget; }
            [[nodiscard]] auto GetRenderTargetSize() const noexcept -> UVector2;
            [[nodiscard]] inline auto IsRenderingToWindow() const noexcept -> bool { return m_renderTarget == nullptr; }

            auto DrawPoint(const components::Transform& transform, const Colour& colour, const bool useInbuiltPipeline = true) -> void;
            auto DrawScreenPoint(const components::ScreenTransform& transform, const Colour& colour, const bool useInbuiltPipeline = true) -> void;
            auto DrawLine(const geometry::Line& line, const components::Transform& transform, const Colour& colour, const bool useInbuiltPipeline = true) -> void;
            auto DrawScreenLine(const geometry::ScreenLine& line, const components::ScreenTransform& transform, const Colour& colour, const bool useInbuiltPipeline = true) -> void;
            auto DrawRectangleOutline(const components::Transform& transform, const Colour& colour, const bool useInbuiltPipeline = true) -> void;
            auto DrawScreenRectangleOutline(const UVector2 size, const components::ScreenTransform& transform, const Colour& colour, const bool useInbuiltPipeline = true) -> void;

            auto DrawRectangle(const components::Transform& transform, const components::Sprite& sprite, const bool useInbuiltPipeline = true) -> void;
            auto DrawScreenRectangle(const UVector2 size, const components::ScreenTransform& transform, const components::Sprite& sprite, const bool useInbuiltPipeline = true) -> void;
            auto DrawQuad(const geometry::Quad& quad, const components::Transform& transform, const components::Sprite& sprite, const bool useInbuiltPipeline = true) -> void;
            auto DrawScreenQuad(const geometry::ScreenQuad& quad, const components::ScreenTransform& transform, const components::Sprite& sprite, const bool useInbuiltPipeline = true) -> void;

            auto StartLineBatch() -> void;
            auto BatchPoint(const components::Transform& transform, const Colour& colour) -> void;
            auto BatchScreenPoint(const components::ScreenTransform& transform, const Colour& colour) -> void;
            auto BatchLine(const geometry::Line& line, const components::Transform& transform, const Colour& colour) -> void;
            auto BatchScreenLine(const geometry::ScreenLine& line, const components::ScreenTransform& transform, const Colour& colour) -> void;
            auto BatchRectangleOutline(const components::Transform& transform, const Colour& colour) -> void;
            auto BatchScreenRectangleOutline(const UVector2 size, const components::ScreenTransform& transform, const Colour& colour) -> void;
            auto FlushLineBatch(const bool useInbuiltPipeline = true) -> void;
            auto RestartLineBatch(const bool useInbuiltPipeline = true) -> void;

            auto StartQuadBatch() -> void;
            auto BatchRectangle(const components::Transform& transform, const components::Sprite& sprite) -> void;
            auto BatchScreenRectangle(const UVector2 size, const components::ScreenTransform& transform, const components::Sprite& sprite) -> void;
            auto BatchQuad(const geometry::Quad& quad, const components::Transform& transform, const components::Sprite& sprite) -> void;
            auto BatchScreenQuad(const geometry::ScreenQuad& quad, const components::ScreenTransform& transform, const components::Sprite& sprite) -> void;
            auto FlushQuadBatch(const bool useInbuiltPipeline = true) -> void;
            auto RestartQuadBatch(const bool useInbuiltPipeline = true) -> void;

            auto EnableStencilTest(const bool enableStencilTest) const -> void;
            auto SetStencilParameters(const StencilParameters& stencilParameters) const -> void;
            auto SetStencilOperations(const StencilOperations& stencilOperations) const -> void;

            [[nodiscard]] auto ReadPixels() const -> PixelData;
            [[nodiscard]] auto ReadPixels(const RenderZone renderZone) const -> PixelData;
            [[nodiscard]] auto ReadPixels(const RenderArea& readArea) const -> PixelData;

            [[nodiscard]] inline auto GetBlankTexture() const noexcept -> const Texture& { return m_blankTexture; }

            [[nodiscard]] inline auto GetWindow() const noexcept -> ObserverPointer<const Window> { return m_window; }
            [[nodiscard]] inline auto GetCamera() const noexcept -> ObserverPointer<const Camera2D> { return m_camera; }

        private:
            auto InitialiseBlankTexture() -> void;
            auto InitialisePipelines(const CreateInfo& createInfo) -> void;
            [[nodiscard]] auto InitialisePipeline(Pipeline& pipeline, const StringView vertexShaderFilepath, const StringView fragmentShaderFilepath) -> Status;
            auto InitialiseDrawingStates(const CreateInfo& createInfo) -> void;

            auto UpdateActivePipeline(const Pipeline& pipelineToUse) -> void;

            [[nodiscard]] auto GetModelMatrixFromTransform(const components::Transform& transform) const -> Matrix4;
            [[nodiscard]] auto GetModelMatrixFromScreenTransform(const components::ScreenTransform& transform, const geometry::ScreenLine& line) const -> Matrix4;
            [[nodiscard]] auto GetModelMatrixFromScreenTransform(const components::ScreenTransform& transform, const UVector2 rectangleSize) const -> Matrix4;
            [[nodiscard]] auto GetModelMatrixFromScreenTransform(const components::ScreenTransform& transform, const geometry::ScreenQuad& quad) const -> Matrix4;
            [[nodiscard]] auto GetModelMatrixFromScreenTransform(const components::ScreenTransform& transform, const IVector2 translation, const IVector2 pivot) const -> Matrix4;
        };
    }
}

#endif
