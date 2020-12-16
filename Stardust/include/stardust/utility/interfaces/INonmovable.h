#pragma once
#ifndef I_NONMOVABLE_H
#define I_NONMOVABLE_H

namespace stardust
{
	class INonmovable
	{
	protected:
		INonmovable() = default;

		INonmovable(INonmovable&&) noexcept = delete;
		INonmovable& operator =(INonmovable&&) noexcept = delete;

		~INonmovable() noexcept = default;
	};
}

#endif