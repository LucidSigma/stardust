#pragma once
#ifndef STARDUST_SORTING_LAYER_H
#define STARDUST_SORTING_LAYER_H

#include <compare>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	class SortingLayer
	{
	private:
		inline static u32 s_idCounter = 0u;

		u32 m_id;
		f32 m_z = 0.0f;

		String m_name;

	public:
		explicit SortingLayer(const StringView& name, const f32 z = 0.0f);
		~SortingLayer() noexcept = default;

		bool operator ==(const SortingLayer&) const noexcept = default;
		bool operator !=(const SortingLayer&) const noexcept = default;

		inline bool operator <(const SortingLayer& other) const noexcept { return m_z < other.m_z; }
		inline bool operator <=(const SortingLayer& other) const noexcept { return m_z <= other.m_z; }
		inline bool operator >(const SortingLayer& other) const noexcept { return m_z > other.m_z; }
		inline bool operator >=(const SortingLayer& other) const noexcept { return m_z >= other.m_z; }

		inline std::partial_ordering operator <=>(const SortingLayer& other) const noexcept { return m_z <=> other.m_z; }

		inline u32 GetID() const noexcept { return m_id; }
		inline f32 GetZ() const noexcept { return m_z; }

		inline const String& GetName() const noexcept { return m_name; }
	};
}

#endif