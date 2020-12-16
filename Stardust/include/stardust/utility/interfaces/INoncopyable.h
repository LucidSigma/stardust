#pragma once
#ifndef I_NONCOPYABLE_H
#define I_NONCOPYABLE_H

namespace stardust
{
	class INoncopyable
	{
	protected:
		INoncopyable() = default;

		INoncopyable(const INoncopyable&) = delete;
		INoncopyable& operator =(const INoncopyable&) = delete;

		~INoncopyable() noexcept = default;
	};
}

#endif