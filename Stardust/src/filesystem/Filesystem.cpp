#include "stardust/filesystem/Filesystem.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace stardust
{
	namespace filesystem
	{
		[[nodiscard]] Vector<String> GetAllFilesInDirectory(const StringView& directory)
		{
			Vector<String> files{ };

			for (const auto& file : std::filesystem::directory_iterator(directory))
			{
				if (!file.is_directory())
				{
					files.push_back(file.path().string());
				}
			}

			return files;
		}

		[[nodiscard]] Vector<String> GetAllFilesInDirectoryRecursive(const StringView& directory)
		{
			Vector<String> files{ };

			for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
			{
				if (!file.is_directory())
				{
					files.push_back(file.path().string());
				}
			}

			return files;
		}

		[[nodiscard]] Vector<String> GetAllFileNamesInDirectory(const StringView& directory)
		{
			Vector<String> files{ };

			for (const auto& file : std::filesystem::directory_iterator(directory))
			{
				if (!file.is_directory())
				{
					files.push_back(file.path().filename().string());
				}
			}

			return files;
		}

		[[nodiscard]] Vector<String> GetAllFileNamesInDirectoryRecursive(const StringView& directory)
		{
			Vector<String> files{ };

			for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
			{
				if (!file.is_directory())
				{
					files.push_back(file.path().filename().string());
				}
			}

			return files;
		}

		[[nodiscard]] bool DoesFileExist(const StringView& filepath)
		{
			return std::filesystem::exists(filepath);
		}

		[[nodiscard]] bool IsDirectory(const StringView& filepath)
		{
			return std::filesystem::is_directory(filepath);
		}

		[[nodiscard]] String GetParentFilepath(const StringView& filepath)
		{
			return std::filesystem::path(filepath).parent_path().string();
		}

		[[nodiscard]] String GetFilenameFromDirectory(const StringView& filepath)
		{
			return std::filesystem::path(filepath).filename().string();
		}

		[[nodiscard]] String GetFileStem(const StringView& filename)
		{
			return std::filesystem::path(filename).stem().string();
		}

		[[nodiscard]] String GetFileExtension(const StringView& filename)
		{
			return std::filesystem::path(filename).extension().string();
		}

		[[nodiscard]] Status CreateDirectory(const StringView& path)
		{
			return std::filesystem::create_directory(path)
				? Status::Success
				: Status::Fail;
		}

		[[nodiscard]] String ReadFile(const StringView& filepath)
		{
			std::ifstream file(filepath);

			if (!file.is_open())
			{
				return "";
			}

			std::ostringstream fileDataStream;
			fileDataStream << file.rdbuf();
			file.close();

			return fileDataStream.str();
		}

		[[nodiscard]] Vector<String> ReadFileLines(const StringView& filepath)
		{
			std::ifstream file(filepath);

			if (!file.is_open())
			{
				return { };
			}

			Vector<String> fileLines{ };
			String currentLine;

			while (std::getline(file, currentLine))
			{
				fileLines.push_back(currentLine);
			}

			return fileLines;
		}

		[[nodiscard]] Vector<ubyte> ReadFileBytes(const StringView& filepath)
		{
			std::basic_ifstream<ubyte> file(filepath, std::ios_base::in | std::ios_base::binary);

			if (!file.is_open())
			{
				return { };
			}

			return Vector<ubyte>(
				std::istreambuf_iterator<ubyte>(file),
				std::istreambuf_iterator<ubyte>()
			);
		}

		[[nodiscard]] Status WriteToFile(const StringView& filepath, const Vector<ubyte>& data)
		{
			std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

			if (!outputFile.is_open())
			{
				return Status::Fail;
			}

			outputFile << data.data();
			outputFile.close();

			return Status::Success;
		}

		[[nodiscard]] Status WriteToFile(const StringView& filepath, const String& data)
		{
			std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::trunc);

			if (!outputFile.is_open())
			{
				return Status::Fail;
			}

			outputFile << data;
			outputFile.close();

			return Status::Success;
		}

		[[nodiscard]] Status WriteToFile(const StringView& filepath, const nlohmann::json& data)
		{
			std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::trunc);

			if (!outputFile.is_open())
			{
				return Status::Fail;
			}

			outputFile << data;
			outputFile.close();

			return Status::Success;
		}

		[[nodiscard]] Status AppendToFile(const StringView& filepath, const Vector<ubyte>& data)
		{
			std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::binary | std::ios_base::app);

			if (!outputFile.is_open())
			{
				return Status::Fail;
			}

			outputFile << data.data();
			outputFile.close();

			return Status::Success;
		}

		[[nodiscard]] Status AppendToFile(const StringView& filepath, const String& data)
		{
			std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::app);

			if (!outputFile.is_open())
			{
				return Status::Fail;
			}

			outputFile << data;
			outputFile.close();

			return Status::Success;
		}

		[[nodiscard]] Status AppendToFile(const StringView& filepath, const nlohmann::json& data)
		{
			std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::app);

			if (!outputFile.is_open())
			{
				return Status::Fail;
			}

			outputFile << data;
			outputFile.close();

			return Status::Success;
		}
	}
}