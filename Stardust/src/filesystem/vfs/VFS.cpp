#include "stardust/filesystem/vfs/VFS.h"

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

        void AddToSearchPath(const StringView& directory)
        {
            PHYSFS_mount(directory.data(), nullptr, 1);
        }

        void AddToSearchPath(const Vector<StringView>& directories)
        {
            for (const auto& directory : directories)
            {
                AddToSearchPath(directory);
            }
        }

        void SetWriteDirectory(const StringView& writeDirectory)
        {
            PHYSFS_setWriteDir(writeDirectory.data());
        }

        [[nodiscard]] String GetDirectorySeparator()
        {
            return String(PHYSFS_getDirSeparator());
        }

        [[nodiscard]] Vector<String> GetAllFilesInDirectory(const StringView& directory)
        {
            Vector<String> files{ };
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

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

        [[nodiscard]] Vector<String> GetAllFilesInDirectoryRecursive(const StringView& directory)
        {
            Vector<String> files{ };
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

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
                    files.insert(std::cend(files), std::cbegin(filesInDirectory), std::cend(filesInDirectory));
                }
            }

            PHYSFS_freeList(enumeratedFiles);
            enumeratedFiles = nullptr;

            return files;
        }

        [[nodiscard]] Vector<String> GetAllFileNamesInDirectory(const StringView& directory)
        {
            Vector<String> files{ };
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

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

        [[nodiscard]] Vector<String> GetAllFileNamesInDirectoryRecursive(const StringView& directory)
        {
            Vector<String> files{ };
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

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
                    files.insert(std::cend(files), std::cbegin(filesInDirectory), std::cend(filesInDirectory));
                }
            }

            PHYSFS_freeList(enumeratedFiles);
            enumeratedFiles = nullptr;

            return files;
        }

        [[nodiscard]] bool DoesFileExist(const StringView& filepath)
        {
            return PHYSFS_exists(filepath.data()) != 0;
        }

        [[nodiscard]] bool IsDirectory(const StringView& filepath)
        {
            return PHYSFS_isDirectory(filepath.data()) != 0;
        }

        [[nodiscard]] Vector<ubyte> ReadFileData(const StringView& filepath)
        {
            PHYSFS_File* file = PHYSFS_openRead(filepath.data());

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

        [[nodiscard]] Status WriteToFile(const StringView& filepath, const Vector<ubyte>& data)
        {
            PHYSFS_File* file = PHYSFS_openWrite(filepath.data());

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

        [[nodiscard]] Status WriteToFile(const StringView& filepath, const String& data)
        {
            PHYSFS_File* file = PHYSFS_openWrite(filepath.data());

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

        [[nodiscard]] Status AppendToFile(const StringView& filepath, const Vector<ubyte>& data)
        {
            PHYSFS_File* file = PHYSFS_openAppend(filepath.data());

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

        [[nodiscard]] Status AppendToFile(const StringView& filepath, const String& data)
        {
            PHYSFS_File* file = PHYSFS_openAppend(filepath.data());

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