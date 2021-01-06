#pragma once
#ifndef STARDUST_TILEMAP_H
#define STARDUST_TILEMAP_H

#include <compare>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
	class Tilemap
	{
	public:
		class Layer
		{
		public:
			struct CreateInfo
			{
				u32 id;
				StringView name;

				UVec2 size;
				IVec2 offset;

				f32 opacity = 1.0f;

				Vector<u32> tiles;
			};

		private:
			u32 m_id = 0u;
			String m_name;

			UVec2 m_size{ 0u, 0u };
			IVec2 m_offset{ 0, 0 };

			f32 m_opacity = 0.0f;

			Vector<u32> m_tileData{ };

		public:
			Layer() = default;
			Layer(const CreateInfo& createInfo);
			~Layer() noexcept = default;

			void Initialise(const CreateInfo& createInfo);

			inline u32 GetID() const noexcept { return m_id; }
			inline const String& GetName() const noexcept { return m_name; }

			inline const UVec2& GetSize() const noexcept { return m_size; }
			inline const IVec2& GetOffset() const noexcept { return m_offset; }
			inline void SetOffset(const IVec2& offset) noexcept { m_offset = offset; }

			inline f32 GetOpacity() const noexcept { return m_opacity; }
			void SetOpacity(const f32 opacity) noexcept;

			[[nodiscard]] u32 GetTile(const u32 x, const u32 y) const;
			void SetTile(const u32 x, const u32 y, const u32 tile);

			bool operator ==(const Layer&) const noexcept = default;
			bool operator !=(const Layer&) const noexcept = default;

			inline bool operator <(const Layer& other) const noexcept { return m_id < other.m_id; }
			inline bool operator <=(const Layer& other) const noexcept { return m_id <= other.m_id; }
			inline bool operator >(const Layer& other) const noexcept { return m_id > other.m_id; }
			inline bool operator >=(const Layer& other) const noexcept { return m_id >= other.m_id; }

			inline std::strong_ordering operator <=>(const Layer& other) const noexcept { return m_id <=> other.m_id; }
		};

	private:
		UVec2 m_size{ 0u, 0u };

		bool m_isValid = false;

	public:
		Tilemap() = default;
		Tilemap(const StringView& filepath);
		~Tilemap() noexcept = default;

		void Initialise(const StringView& filepath);

		inline bool IsValid() const noexcept { return m_isValid; }
	};
}

#endif