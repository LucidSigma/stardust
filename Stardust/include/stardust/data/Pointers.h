#pragma once
#ifndef STARDUST_POINTERS_H
#define STARDUST_POINTERS_H

#include <functional>
#include <memory>

namespace stardust
{
    template <typename T, typename Deleter = std::default_delete<T>>
    using UniquePtr = std::unique_ptr<T, Deleter>;

    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T>
    using WeakPtr = std::weak_ptr<T>;

    template <typename T>
    using ObserverPtr = T*;

    template <typename T>
    using ReferenceWrapper = std::reference_wrapper<T>;
}

#endif