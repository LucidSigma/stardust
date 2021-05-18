#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Query and iterate files in directories", "[filesystem]")
{
    SECTION("Can read files from a directory")
    {
        REQUIRE(sd::fs::GetAllFilesInDirectory("fs_resources").size() == 4u);
        REQUIRE(sd::fs::GetAllFileNamesInDirectory("fs_resources").size() == 4u);
    }
    
    SECTION("Can recursively read files from a directory")
    {
        REQUIRE(sd::fs::GetAllFilesInDirectoryRecursive("fs_resources").size() == 6u);
        REQUIRE(sd::fs::GetAllFileNamesInDirectoryRecursive("fs_resources").size() == 6u);
    }
    
    SECTION("Can query for data in a filename/filepath")
    {
        REQUIRE(sd::fs::GetParentFilepath("fs_resources/a/3.txt") == "fs_resources/a");
        REQUIRE(sd::fs::GetParentFilepath("fs_resources/a/c") == "fs_resources/a");
        REQUIRE(sd::fs::GetParentFilepath("fs_resources/a/b/4.txt") == "fs_resources/a/b");
    
        REQUIRE(sd::fs::GetFilenameFromDirectory("fs_resources/a/3.txt") == "3.txt");
        REQUIRE(sd::fs::GetFilenameFromDirectory("fs_resources/a/b/4.txt") == "4.txt");
    
        REQUIRE(sd::fs::GetFileStem("fs_resources/a") == "a");
        REQUIRE(sd::fs::GetFileStem("fs_resources/a/b/4.txt") == "4");
    
        REQUIRE(sd::fs::GetFileExtension("fs_resources/1.txt") == ".txt");
    }

    SECTION("Can convert JSON to and from MessagePack")
    {
        const nlohmann::json json = nlohmann::json::parse(sd::fs::ReadFile("fs_resources/test.json"));
        REQUIRE(sd::fs::SaveToMessagePack("fs_resources/test.msgpack", json) == sd::Status::Success);

        const nlohmann::json fromMessagePack = sd::fs::ReadMessagePack("fs_resources/test.msgpack");
        REQUIRE(!fromMessagePack.is_discarded());

        REQUIRE(fromMessagePack["a"] == 1);
        REQUIRE(fromMessagePack["b"] == "abc");
        REQUIRE(fromMessagePack["c"]);
    }
}

TEST_CASE("VFS can open and read from zip files", "[vfs]")
{
    REQUIRE(sd::vfs::Initialise(nullptr));
    sd::vfs::AddToSearchPath("fs_resources.zip");
    
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

    sd::vfs::Quit();
}