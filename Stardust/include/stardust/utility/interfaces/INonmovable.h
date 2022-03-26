#pragma once
#ifndef STARDUST_I_NONMOVABLE_H
#define STARDUST_I_NONMOVABLE_H

namespace stardust
{
    class INonmovable
    {
    protected:
        INonmovable() = default;

        INonmovable(const INonmovable&) = default;
        auto operator =(const INonmovable&) -> INonmovable& = default;

        INonmovable(INonmovable&&) noexcept = delete;
        auto operator =(INonmovable&&) noexcept -> INonmovable& = delete;

        ~INonmovable() noexcept = default;
    };
}

#endif
