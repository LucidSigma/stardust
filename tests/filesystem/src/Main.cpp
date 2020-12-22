#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("VFS can open and read from zip files", "[vfs]")
{
	REQUIRE(sd::vfs::Initialise(nullptr));
	sd::vfs::AddToSearchPath("vfs_resources.zip");
	
	SECTION("Can read files from a zip archive")
	{
		REQUIRE(sd::vfs::GetAllFilesInDirectory("resources").size() == 2u);
		REQUIRE(sd::vfs::GetAllFileNamesInDirectory("resources").size() == 2u);
	}
	
	SECTION("Can recursively read files from a zip archive")
	{
		REQUIRE(sd::vfs::GetAllFilesInDirectoryRecursive("resources").size() == 4u);
		REQUIRE(sd::vfs::GetAllFileNamesInDirectoryRecursive("resources").size() == 4);
	}
	
	SECTION("Can determine if files/directories exist")
	{
		REQUIRE(sd::vfs::DoesFileExist("resources/1.txt"));
		REQUIRE(!sd::vfs::DoesFileExist("resources/3.txt"));
	
		REQUIRE(sd::vfs::IsDirectory("resources/a"));
		REQUIRE(!sd::vfs::IsDirectory("resources/c"));
		REQUIRE(!sd::vfs::IsDirectory("resources/1.txt"));
	}
	
	SECTION("Can query for data in a filename/filepath")
	{
		REQUIRE(sd::vfs::GetParentFilepath("resources/a/3.txt") == "resources/a");
		REQUIRE(sd::vfs::GetParentFilepath("resources/a/c") == "resources/a");
		REQUIRE(sd::vfs::GetParentFilepath("resources/a/b/4.txt") == "resources/a/b");
	
		REQUIRE(sd::vfs::GetFilenameFromDirectory("resources/a/3.txt") == "3.txt");
		REQUIRE(sd::vfs::GetFilenameFromDirectory("resources/a/b/4.txt") == "4.txt");
	
		REQUIRE(sd::vfs::GetFileStem("resources/a") == "a");
		REQUIRE(sd::vfs::GetFileStem("resources/a/b/4.txt") == "4");
	
		REQUIRE(sd::vfs::GetFileExtension("resources/1.txt") == ".txt");
	}

	sd::vfs::Quit();
}