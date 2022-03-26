#pragma once
#ifndef STARDUST_I_NONCOPYABLE_H
#define STARDUST_I_NONCOPYABLE_H

namespace stardust
{
    class INoncopyable
    {
    protected:
        INoncopyable() = default;

        INoncopyable(const INoncopyable&) = delete;
        auto operator =(const INoncopyable&) -> INoncopyable& = delete;

        INoncopyable(INoncopyable&&) noexcept = default;
        auto operator =(INoncopyable&&) noexcept -> INoncopyable& = default;

        ~INoncopyable() noexcept = default;
    };
}

#endif
