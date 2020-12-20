#pragma once
#ifndef VFS_H
#define VFS_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/enums/Status.h"

namespace stardust
{
	namespace vfs
	{
		[[nodiscard]] extern bool Initialise(const char* argv0);
		extern void Quit();

		extern void AddToSearchPath(const StringView& directory);
		extern void AddToSearchPath(const Vector<StringView>& directories);

		extern void SetWriteDirectory(const StringView& writeDirectory);

		[[nodiscard]] extern String GetDirectorySeparator();

		[[nodiscard]] extern Vector<String> GetAllFilesInDirectory(const StringView& directory);
		[[nodiscard]] extern Vector<String> GetAllFilesInDirectoryRecursive(const StringView& directory);
		[[nodiscard]] extern Vector<String> GetAllFileNamesInDirectory(const StringView& directory);
		[[nodiscard]] extern Vector<String> GetAllFileNamesInDirectoryRecursive(const StringView& directory);
		[[nodiscard]] extern bool DoesFileExist(const StringView& filepath);

		[[nodiscard]] extern bool IsDirectory(const StringView& filepath);
		[[nodiscard]] extern String GetParentFilepath(const StringView& filepath);
		[[nodiscard]] extern String GetFilenameFromDirectory(const StringView& filepath);
		[[nodiscard]] extern String GetFileStem(const StringView& filename);
		[[nodiscard]] extern String GetFileExtension(const StringView& filename);

		[[nodiscard]] extern Vector<ubyte> ReadFileData(const StringView& filepath);
		[[nodiscard]] extern Status WriteToFile(const StringView& filepath, const Vector<ubyte>& data);
		[[nodiscard]] extern Status WriteToFile(const StringView& filepath, const String& data);
		[[nodiscard]] extern Status AppendToFile(const StringView& filepath, const Vector<ubyte>& data);
		[[nodiscard]] extern Status AppendToFile(const StringView& filepath, const String& data);

		[[nodiscard]] extern String UTF16ToUTF8(const UTF16String& utf16String);
		[[nodiscard]] extern UTF16String UTF8ToUTF16(const String& utf8String);
	}
}

#endif