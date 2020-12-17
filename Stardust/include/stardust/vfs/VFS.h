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

		extern void AddToSearchPath(const String& directory);
		extern void AddToSearchPath(const Vector<String>& directories);

		extern void SetWriteDirectory(const String& writeDirectory);

		[[nodiscard]] extern String GetDirectorySeparator();

		[[nodiscard]] extern Vector<String> GetAllFilesInDirectory(const String& directory);
		[[nodiscard]] extern Vector<String> GetAllFilesInDirectoryRecursive(const String& directory);
		[[nodiscard]] extern Vector<String> GetAllFileNamesInDirectory(const String& directory);
		[[nodiscard]] extern Vector<String> GetAllFileNamesInDirectoryRecursive(const String& directory);
		[[nodiscard]] extern bool DoesFileExist(const String& filepath);

		[[nodiscard]] extern bool IsDirectory(const String& filepath);
		[[nodiscard]] extern String GetParentFilepath(const String& filepath);
		[[nodiscard]] extern String GetFilenameFromDirectory(const String& filepath);
		[[nodiscard]] extern String GetFileStem(const String& filename);
		[[nodiscard]] extern String GetFileExtension(const String& filename);

		[[nodiscard]] extern Vector<ubyte> ReadFileData(const String& filepath);
		[[nodiscard]] extern Status WriteToFile(const String& filepath, const Vector<ubyte>& data);
		[[nodiscard]] extern Status WriteToFile(const String& filepath, const String& data);
		[[nodiscard]] extern Status AppendToFile(const String& filepath, const Vector<ubyte>& data);
		[[nodiscard]] extern Status AppendToFile(const String& filepath, const String& data);
	}
}

#endif