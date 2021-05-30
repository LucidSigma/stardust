#pragma once
#ifndef STARDUST_CONTAINERS_H
#define STARDUST_CONTAINERS_H

#include <array>
#include <cstddef>
#include <deque>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <tuple>
#include <string>
#include <string_view>
#include <vector>

#include <concurrentqueue/concurrentqueue.h>
#include <concurrentqueue/blockingconcurrentqueue.h>
#include <fifo_map/fifo_map.hpp>

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

    template <typename K, typename V, typename Comp = std::less<K>>
    using Map = std::map<K, V, Comp>;

    template <typename K, typename V, typename Comp = std::less<K>>
    using MultiMap = std::multimap<K, V, Comp>;

    template <typename T, typename Comp = std::less<T>>
    using Set = std::set<T, Comp>;

    template <typename T, typename Comp = std::less<T>>
    using MultiSet = std::multiset<T, Comp>;

    template <typename K, typename V, typename Comp = nlohmann::fifo_map_compare<K>>
    using FifoMap = nlohmann::fifo_map<K, V, Comp>;

    template <typename T>
    using Queue = std::queue<T>;

    template <typename T, typename Comp = std::less<T>>
    using PriorityQueue = std::priority_queue<T, Vector<T>, Comp>;

    template <typename T>
    using Stack = std::stack<T>;

    template <typename T>
    using ConcurrentQueue = moodycamel::ConcurrentQueue<T>;

    template <typename T>
    using BlockingConcurrentQueue = moodycamel::BlockingConcurrentQueue<T>;

    template <typename T, typename U>
    using Pair = std::pair<T, U>;

    template <typename... Types>
    using Tuple = std::tuple<Types...>;

    namespace string
    {
        [[nodiscard]] extern Vector<String> Split(const String& string, const char delimiter = ' ');

        [[nodiscard]] extern String MakeUpper(String string);
        [[nodiscard]] extern String MakeLower(String string);

        [[nodiscard]] extern String RemoveFirstCharacter(const String& string);
        [[nodiscard]] extern String RemoveLastCharacter(const String& string);
        [[nodiscard]] extern String RemoveFirstAndLastCharacters(const String& string);

        [[nodiscard]] extern String LeftTrim(String string);
        [[nodiscard]] extern String RightTrim(String string);
        [[nodiscard]] extern String Trim(String string);
    }
}

#endif