#pragma once
#ifndef POINTERS_H
#define POINTERS_H

#include <EASTL/functional.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/weak_ptr.h>

namespace stardust
{
	template <typename T, typename Deleter = eastl::default_delete<T>>
	using UniquePtr = eastl::unique_ptr<T, Deleter>;

	template <typename T>
	using SharedPtr = eastl::shared_ptr<T>;

	template <typename T>
	using WeakPtr = eastl::weak_ptr<T>;

	template <typename T>
	using ObserverPtr = T*;

	template <typename T>
	using ReferenceWrapper = eastl::reference_wrapper<T>;
}

#endif