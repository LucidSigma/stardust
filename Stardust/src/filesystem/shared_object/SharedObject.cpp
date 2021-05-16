#include "stardust/filesystem/shared_object/SharedObject.h"

#include <utility>

namespace stardust
{
    SharedObject::SharedObject(const StringView& filepath)
    {
        Load(filepath);
    }

    SharedObject::SharedObject(SharedObject&& other) noexcept
    {
        Unload();

        m_handle = std::exchange(other.m_handle, nullptr);
    }

    SharedObject& SharedObject::operator =(SharedObject&& other) noexcept
    {
        Unload();
        m_handle = std::exchange(other.m_handle, nullptr);

        return *this;
    }

    SharedObject::~SharedObject() noexcept
    {
        Unload();
    }

    void SharedObject::Load(const StringView& filepath)
    {
        m_handle = SDL_LoadObject(filepath.data());
    }

    void SharedObject::Unload() noexcept
    {
        if (m_handle != nullptr)
        {
            SDL_UnloadObject(m_handle);
            m_handle = nullptr;
        }
    }

    [[nodiscard]] bool SharedObject::HasFunction(const StringView& name) const
    {
        const void* const function = SDL_LoadFunction(m_handle, name.data());

        return function != nullptr;
    }
}