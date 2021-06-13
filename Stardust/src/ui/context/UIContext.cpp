#include "stardust/ui/context/UIContext.h"

#include <utility>

#include "stardust/data/Types.h"

namespace stardust
{
    namespace ui
    {
        Context::Context(const StringView& name, const UVec2& dimensions)
        {
            Initialise(name, dimensions);
        }

        Context::Context(Context&& other) noexcept
        {
            Destroy();

            m_handle = std::exchange(other.m_handle, nullptr);
        }

        Context& Context::operator =(Context&& other) noexcept
        {
            Destroy();
            m_handle = std::exchange(other.m_handle, nullptr);

            return *this;
        }

        Context::~Context() noexcept
        {
            Destroy();
        }

        void Context::Initialise(const StringView& name, const UVec2& dimensions)
        {
            m_handle = Rml::CreateContext(name.data(), Rml::Vector2i{ static_cast<i32>(dimensions.x), static_cast<i32>(dimensions.y) });
        }

        void Context::Destroy() noexcept
        {
            if (m_handle != nullptr)
            {
                Rml::RemoveContext(m_handle->GetName());
                m_handle = nullptr;
            }
        }
        
        [[nodiscard]] Rml::ElementDocument* Context::LoadRMLDocument(const StringView& filepath) const
        {
            return m_handle->LoadDocument(filepath.data());
        }
    }
}