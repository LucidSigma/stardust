#pragma once
#ifndef STARDUST_CONTAINERS_H
#define STARDUST_CONTAINERS_H

#include <any>
#include <array>
#include <deque>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <span>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>
#include <pugixml/pugixml.hpp>
#include <tl/generator.hpp>
#include <toml++/toml.h>

#include "stardust/types/primitives.h"

namespace stardust
{
    template <typename T>
    using Optional = std::optional<T>;

    constexpr std::nullopt_t None = std::nullopt;

    template <typename... Types>
    using Variant = std::variant<Types...>;

    using Any = std::any;

    using String = std::string;
    using StringView = std::string_view;

    using UTF8String = std::u8string;
    using UTF8StringView = std::u8string_view;
    
    using UTF16String = std::u16string;
    using UTF16StringView = std::u16string_view;
    
    using UTF32String = std::u32string;
    using UTF32StringView = std::u32string_view;

    template <typename T, usize Extent = std::dynamic_extent>
    using Slice = std::span<T, Extent>;

    template <typename T, usize Size>
    using Array = std::array<T, Size>;

    template <typename T>
    using List = std::vector<T>;

    template <typename T>
    using Deque = std::deque<T>;

    template <typename T>
    using LinkedList = std::list<T>;

    template <typename K, typename V>
    using HashMap = std::unordered_map<K, V>;

    template <typename K>
    using HashSet = std::unordered_set<K>;

    template <typename K, typename V, typename Comp = std::less<K>>
    using Map = std::map<K, V, Comp>;

    template <typename K, typename V, typename Comp = std::less<K>>
    using MultiMap = std::multimap<K, V, Comp>;

    template <typename K, typename Comp = std::less<K>>
    using Set = std::set<K, Comp>;

    template <typename K, typename Comp = std::less<K>>
    using MultiSet = std::multiset<K, Comp>;

    template <typename T>
    using Queue = std::queue<T>;

    template <typename T, typename Comp = std::less<T>>
    using PriorityQueue = std::priority_queue<T, List<T>, Comp>;

    template <typename T>
    using Stack = std::stack<T>;

    template <typename T, typename U>
    using Pair = std::pair<T, U>;

    template <typename... Types>
    using Tuple = std::tuple<Types...>;

    using JSON = nlohmann::json;
    using TOML = toml::table;
    using XML = pugi::xml_document;

    template <typename T>
    using Generator = tl::generator<T>;
}

#endif
