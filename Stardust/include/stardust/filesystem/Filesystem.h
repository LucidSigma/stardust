#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/enums/Status.h"

namespace stardust
{
	namespace filesystem
	{
		[[nodiscard]] extern bool DoesFileExist(const StringView& filepath);
		[[nodiscard]] extern bool IsDirectory(const StringView& filepath);

		[[nodiscard]] extern String GetParentFilepath(const StringView& filepath);
		[[nodiscard]] extern String GetFilenameFromDirectory(const StringView& filepath);
		[[nodiscard]] extern String GetFileStem(const StringView& filename);
		[[nodiscard]] extern String GetFileExtension(const StringView& filename);

		[[nodiscard]] extern Status CreateDirectory(const StringView& path);

		[[nodiscard]] extern String ReadFile(const StringView& filepath);
		[[nodiscard]] extern Vector<ubyte> ReadFileBytes(const StringView& filepath);
		[[nodiscard]] extern Status WriteToFile(const StringView& filepath, const Vector<ubyte>& data);
		[[nodiscard]] extern Status WriteToFile(const StringView& filepath, const String& data);
		[[nodiscard]] extern Status AppendToFile(const StringView& filepath, const Vector<ubyte>& data);
		[[nodiscard]] extern Status AppendToFile(const StringView& filepath, const String& data);
	}

	namespace fs = filesystem;
}

#endif