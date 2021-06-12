#include "stardust/ui/context/UIContext.h"

#include "stardust/data/Types.h"

namespace stardust
{
    namespace ui
    {
        Context::Context(const StringView& name, const UVec2& windowSize)
        {
            Initialise(name, windowSize);
        }

        Context::~Context() noexcept
        {
            Destroy();
        }

        void Context::Initialise(const StringView& name, const UVec2& windowSize)
        {
            m_handle = Rml::CreateContext(name.data(), Rml::Vector2i{ static_cast<i32>(windowSize.x), static_cast<i32>(windowSize.y) });
        }

        void Context::Destroy() noexcept
        {
            if (m_handle != nullptr)
            {
                Rml::RemoveContext(m_handle->GetName());
                m_handle = nullptr;
            }
        }
    }
}