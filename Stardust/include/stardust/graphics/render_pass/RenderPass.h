#pragma once
#ifndef STARDUST_RENDER_PASS_H
#define STARDUST_RENDER_PASS_H

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace graphics
    {
        class RenderPass final
        {
        private:
            ObserverPointer<Renderer> m_renderer = nullptr;

            List<UniquePointer<Subpass>> m_subpasses{ };

        public:
            RenderPass() = default;
            RenderPass(Renderer& renderer);
            ~RenderPass() noexcept = default;

            auto Initialise(Renderer& renderer) -> void;

            template <std::derived_from<Subpass> S, typename... Args>
            auto AddSubpass(Args&&... args) -> void
                requires std::is_constructible_v<S, Args...>
            {
                m_subpasses.emplace_back(std::make_unique<S>(std::forward<Args>(args)...));
            }

            template <std::derived_from<Subpass> S>
            auto AddSubpass(const S& subpass) -> void
            {
                m_subpasses.emplace_back(std::make_unique<S>(subpass));
            }

            auto Begin() const -> void;
        };
    }
}

#endif
