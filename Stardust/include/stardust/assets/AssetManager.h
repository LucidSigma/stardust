#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <type_traits>
#include <utility>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"

namespace stardust
{
	template <typename T>
	class AssetManager
	{
	private:
		HashMap<String, UniquePtr<T>> m_assets;

	public:
		AssetManager() = default;
		~AssetManager() noexcept = default;

		template <typename... Args, typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
		void Add(const String& name, Args&&... args)
		{
			m_assets[name] = std::make_unique<T>(std::forward<Args>(args)...);
		}

		inline void Remove(const String& name) { m_assets.erase(name); }
		inline void Clear() { m_assets.clear(); }

		inline const T& Get(const String& name) const { return *m_assets.at(name); }
		inline const T& operator [](const String& name) const { return Get(name); }

		inline T& Get(const String& name) { return *m_assets.at(name); }
		inline T& operator [](const String& name) { return Get(name); }

		inline decltype(m_assets)& GetAssets() { return m_assets; }
		inline const decltype(m_assets)& GetAssets() const { return m_assets; }
	};
}

#endif