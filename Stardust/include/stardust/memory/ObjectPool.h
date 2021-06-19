#pragma once
#ifndef STARDUST_OBJECT_POOL_H
#define STARDUST_OBJECT_POOL_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <utility>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    template <typename T>
    class ObjectPool
        : private INoncopyable
    {
    private:
        Queue<UniquePtr<T>> m_availableObjects{ };

    public:
        ObjectPool() = default;

        ObjectPool(ObjectPool&&) noexcept = default;
        ObjectPool& operator =(ObjectPool&&) noexcept = default;

        virtual ~ObjectPool() noexcept = default;

        [[nodiscard]] SharedPtr<T> AcquireObject()
        {
            if (m_availableObjects.empty())
            {
                m_availableObjects.emplace(std::make_unique<T>());
            }

            UniquePtr<T> newObject = std::move(m_availableObjects.front());
            m_availableObjects.pop();

            SharedPtr<T> sharedObject(newObject.release(), [this](T* object) { m_availableObjects.emplace(object); });

            return sharedObject;
        }
    };
}

#endif