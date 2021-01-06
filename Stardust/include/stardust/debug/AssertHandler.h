#pragma once
#ifndef STARDUST_ASSERT_HANDLER_H
#define STARDUST_ASSERT_HANDLER_H

#include <debug_assert/debug_assert.hpp>

namespace stardust
{
	struct AssertHandler
		: public debug_assert::default_handler, public debug_assert::set_level<-1>
	{ };
}

#endif