#pragma once
#ifndef STARDUST_POINTERS_H
#define STARDUST_POINTERS_H

#include <functional>
#include <memory>

namespace stardust
{
    template <typename T, typename Deleter = std::default_delete<T>>
    using UniquePointer = std::unique_ptr<T, Deleter>;

    template <typename T>
    using SharedPointer = std::shared_ptr<T>;

    template <typename T>
    using WeakPointer = std::weak_ptr<T>;

    template <typename T>
    using ObserverPointer = T*;

    template <typename T>
    using ReferenceWrapper = std::reference_wrapper<T>;
}

#endif
