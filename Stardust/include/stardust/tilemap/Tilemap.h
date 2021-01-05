#pragma once
#ifndef STARDUST_TILEMAP_H
#define STARDUST_TILEMAP_H

#include "stardust/data/Containers.h"

namespace stardust
{
	class Tilemap
	{
	private:


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