#pragma once
#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <array>
#include <cstddef>
#include <deque>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <string_view>
#include <vector>

#include <concurrentqueue/concurrentqueue.h>

namespace stardust
{
	using String = std::string;
	using StringView = std::string_view;

	using UTF8String = std::u8string;
	using UTF8StringView = std::u8string_view;

	using UTF16String = std::u16string;
	using UTF16StringView = std::u16string_view;

	using UTF32String = std::u32string;
	using UTF32StringView = std::u32string_view;

	template <typename T, std::size_t N = 1u>
	using Array = std::array<T, N>;

	template <typename T>
	using Vector = std::vector<T>;

	template <typename T>
	using Deque = std::deque<T>;

	template <typename T>
	using LinkedList = std::list<T>;

	template <typename K, typename V>
	using HashMap = std::unordered_map<K, V>;

	template <typename T>
	using HashSet = std::unordered_set<T>;

	template <typename K, typename V>
	using Map = std::map<K, V>;

	template <typename K, typename V>
	using MultiMap = std::multimap<K, V>;

	template <typename T>
	using Set = std::set<T>;

	template <typename T>
	using MultiSet = std::multiset<T>;

	template <typename T>
	using Queue = std::queue<T>;

	template <typename T>
	using PriorityQueue = std::priority_queue<T>;

	template <typename T>
	using Stack = std::stack<T>;

	template <typename T>
	using ConcurrentQueue = moodycamel::ConcurrentQueue<T>;

	[[nodiscard]] extern Vector<String> SplitString(const String& string, const char delimiter = ' ');
	[[nodiscard]] extern String MakeUpper(String string);
	[[nodiscard]] extern String MakeLower(String string);
}

#endif