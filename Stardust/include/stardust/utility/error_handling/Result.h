#pragma once
#ifndef STARDUST_RESULT_H
#define STARDUST_RESULT_H

#include <stx/result.h>

namespace stardust
{
    template <typename T, typename E>
    using Result = stx::Result<T, E>;

    template <typename T>
    using Ok = stx::Ok<T>;

    template <typename E>
    using Error = stx::Err<E>;
}

#endif