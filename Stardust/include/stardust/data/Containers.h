#pragma once
#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <cstddef>

#include <EASTL/array.h>
#include <EASTL/deque.h>
#include <EASTL/hash_map.h>
#include <EASTL/hash_set.h>
#include <EASTL/list.h>
#include <EASTL/map.h>
#include <EASTL/priority_queue.h>
#include <EASTL/queue.h>
#include <EASTL/set.h>
#include <EASTL/stack.h>
#include <EASTL/vector.h>
#include <tinyutf8/tinyutf8.h>

#include <EASTL/string.h>

namespace stardust
{
	using String = tiny_utf8::basic_string<char32_t, char>;

	template <typename T>
	using Vector = eastl::vector<T>;

	template <typename T>
	using LinkedList = eastl::list<T>;

	template <typename K, typename V>
	using HashMap = eastl::hash_map<K, V>;

	template <typename T>
	using HashSet = eastl::hash_set<T>;

	template <typename T, std::size_t N = 1u>
	using Array = eastl::array<T, N>;

	template <typename T>
	using Deque = eastl::deque<T>;

	template <typename T>
	using PriorityQueue = eastl::priority_queue<T>;

	template <typename T>
	using Queue = eastl::queue<T>;

	template <typename T>
	using Stack = eastl::stack<T>;

	template <typename K, typename V>
	using Map = eastl::map<K, V>;

	template <typename T>
	using Set = eastl::set<T>;
}

#endif