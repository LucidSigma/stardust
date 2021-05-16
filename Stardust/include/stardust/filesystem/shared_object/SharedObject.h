#pragma once
#ifndef STARDUST_SHARED_OBJECT_H
#define STARDUST_SHARED_OBJECT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <functional>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"

namespace stardust
{
    class SharedObject
        : private INoncopyable
    {
    private:
        void* m_handle = nullptr;

    public:
        SharedObject() = default;
        SharedObject(const StringView& filepath);

        SharedObject(SharedObject&& other) noexcept;
        SharedObject& operator =(SharedObject&& other) noexcept;

        ~SharedObject() noexcept;

        void Load(const StringView& filepath);
        void Unload() noexcept;

        inline bool IsValid() const noexcept { return m_handle != nullptr; }

        [[nodiscard]] bool HasFunction(const StringView& name) const;

        template <typename T>
        [[nodiscard]] std::function<T> GetFunction(const StringView& name) const
        {
            const auto function = SDL_LoadFunction(m_handle, name.data());

            return std::function<T>(reinterpret_cast<T*>(function));
        }
    };
}

#endif