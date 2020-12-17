#include "stardust/vfs/VFS.h"

#include <filesystem>

#include <physfs/physfs.h>

namespace stardust
{
	namespace vfs
	{
		[[nodiscard]] bool Initialise(const char* argv0)
		{
			return PHYSFS_init(argv0) != 0;
		}

		void Quit()
		{
			PHYSFS_deinit();
		}

		void AddToSearchPath(const String& directory)
		{
			PHYSFS_mount(directory.c_str(), nullptr, 1);
		}

		void AddToSearchPath(const Vector<String>& directories)
		{
			for (const auto& directory : directories)
			{
				AddToSearchPath(directory);
			}
		}

		void SetWriteDirectory(const String& writeDirectory)
		{
			PHYSFS_setWriteDir(writeDirectory.c_str());
		}

		[[nodiscard]] String GetDirectorySeparator()
		{
			return String(PHYSFS_getDirSeparator());
		}

		[[nodiscard]] Vector<String> GetAllFilesInDirectory(const String& directory)
		{
			Vector<String> files{ };
			char** enumeratedFiles = PHYSFS_enumerateFiles(directory.c_str());

			for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
			{
				const String fullFilepath = String(directory) + "/" + *filename;

				if (!IsDirectory(fullFilepath))
				{
					files.push_back(fullFilepath);
				}
			}

			PHYSFS_freeList(enumeratedFiles);
			enumeratedFiles = nullptr;

			return files;
		}

		[[nodiscard]] Vector<String> GetAllFilesInDirectoryRecursive(const String& directory)
		{
			Vector<String> files{ };
			char** enumeratedFiles = PHYSFS_enumerateFiles(directory.c_str());

			for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
			{
				const String fullFilepath = String(directory) + "/" + *filename;

				if (!IsDirectory(fullFilepath))
				{
					files.push_back(fullFilepath);
				}
				else
				{
					const auto filesInDirectory = GetAllFilesInDirectoryRecursive(fullFilepath);
					files.insert(eastl::cend(files), eastl::cbegin(filesInDirectory), eastl::cend(filesInDirectory));
				}
			}

			PHYSFS_freeList(enumeratedFiles);
			enumeratedFiles = nullptr;

			return files;
		}

		[[nodiscard]] Vector<String> GetAllFileNamesInDirectory(const String& directory)
		{
			Vector<String> files{ };
			char** enumeratedFiles = PHYSFS_enumerateFiles(directory.c_str());

			for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
			{
				if (!IsDirectory(String(directory) + "/" + *filename))
				{
					files.push_back(String(*filename));
				}
			}

			PHYSFS_freeList(enumeratedFiles);
			enumeratedFiles = nullptr;

			return files;
		}

		[[nodiscard]] Vector<String> GetAllFileNamesInDirectoryRecursive(const String& directory)
		{
			Vector<String> files{ };
			char** enumeratedFiles = PHYSFS_enumerateFiles(directory.c_str());

			for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
			{
				const String fullFilepath = String(directory) + "/" + *filename;

				if (!IsDirectory(fullFilepath))
				{
					files.push_back(String(*filename));
				}
				else
				{
					const auto filesInDirectory = GetAllFileNamesInDirectoryRecursive(fullFilepath);
					files.insert(eastl::cend(files), eastl::cbegin(filesInDirectory), eastl::cend(filesInDirectory));
				}
			}

			PHYSFS_freeList(enumeratedFiles);
			enumeratedFiles = nullptr;

			return files;
		}

		[[nodiscard]] bool DoesFileExist(const String& filepath)
		{
			return PHYSFS_exists(filepath.c_str()) != 0;
		}

		[[nodiscard]] bool IsDirectory(const String& filepath)
		{
			return PHYSFS_isDirectory(filepath.c_str()) != 0;
		}

		[[nodiscard]] String GetParentFilepath(const String& filepath)
		{
			return std::filesystem::path(filepath.cpp_str()).parent_path().string();
		}

		[[nodiscard]] String GetFilenameFromDirectory(const String& filepath)
		{
			return std::filesystem::path(filepath.cpp_str()).filename().string();
		}

		[[nodiscard]] String GetFileStem(const String& filename)
		{
			return std::filesystem::path(filename.cpp_str()).stem().string();
		}

		[[nodiscard]] String GetFileExtension(const String& filename)
		{
			return std::filesystem::path(filename.cpp_str()).extension().string();
		}

		[[nodiscard]] Vector<ubyte> ReadFileData(const String& filepath)
		{
			PHYSFS_File* file = PHYSFS_openRead(filepath.c_str());

			if (file == nullptr)
			{
				return Vector<ubyte>{ };
			}

			const PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
			Vector<ubyte> fileData(fileSize);

			if (PHYSFS_readBytes(file, fileData.data(), fileSize) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Vector<ubyte>{ };
			}

			PHYSFS_close(file);
			file = nullptr;

			return fileData;
		}

		[[nodiscard]] Status WriteToFile(const String& filepath, const Vector<ubyte>& data)
		{
			PHYSFS_File* file = PHYSFS_openWrite(filepath.c_str());

			if (file == nullptr)
			{
				return Status::Fail;
			}

			if (PHYSFS_writeBytes(file, data.data(), data.size()) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Status::Fail;
			}

			PHYSFS_close(file);
			file = nullptr;

			return Status::Success;
		}

		[[nodiscard]] Status WriteToFile(const String& filepath, const String& data)
		{
			PHYSFS_File* file = PHYSFS_openWrite(filepath.c_str());

			if (file == nullptr)
			{
				return Status::Fail;
			}

			if (PHYSFS_writeBytes(file, data.data(), data.length()) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Status::Fail;
			}

			PHYSFS_close(file);
			file = nullptr;

			return Status::Success;
		}

		[[nodiscard]] Status AppendToFile(const String& filepath, const Vector<ubyte>& data)
		{
			PHYSFS_File* file = PHYSFS_openAppend(filepath.c_str());

			if (file == nullptr)
			{
				return Status::Fail;
			}

			if (PHYSFS_writeBytes(file, data.data(), data.size()) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Status::Fail;
			}

			PHYSFS_close(file);
			file = nullptr;

			return Status::Success;
		}

		[[nodiscard]] Status AppendToFile(const String& filepath, const String& data)
		{
			PHYSFS_File* file = PHYSFS_openAppend(filepath.c_str());

			if (file == nullptr)
			{
				return Status::Fail;
			}

			if (PHYSFS_writeBytes(file, data.data(), data.length()) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Status::Fail;
			}

			PHYSFS_close(file);
			file = nullptr;

			return Status::Success;
		}
	}
}