#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Strings can be manipulated in different ways", "[string]")
{
    SECTION("Can convert a string to lowercase")
    {
        REQUIRE(sd::string::MakeLower("Hello.") == "hello.");
        REQUIRE(sd::string::MakeLower("test") == "test");
        REQUIRE(sd::string::MakeLower("aBCdeFgHiJKlmNopQRstuVwXyz ") == "abcdefghijklmnopqrstuvwxyz ");
        REQUIRE(sd::string::MakeLower("") == "");
    }

    SECTION("Can convert a string to uppercase")
    {
        REQUIRE(sd::string::MakeUpper("Hello.") == "HELLO.");
        REQUIRE(sd::string::MakeUpper("TEST") == "TEST");
        REQUIRE(sd::string::MakeUpper("aBCdeFgHiJKlmNopQRstuVwXyz ") == "ABCDEFGHIJKLMNOPQRSTUVWXYZ ");
        REQUIRE(sd::string::MakeUpper("") == "");
    }

    SECTION("Can split a string on a delimiter")
    {
        sd::Vector<sd::String> values = sd::string::Split("These are some words.");
        REQUIRE(values == sd::Vector<sd::String>{ "These", "are", "some", "words." });

        values = sd::string::Split("A|B|C|D", '|');
        REQUIRE(values == sd::Vector<sd::String>{ "A", "B", "C", "D" });

        values = sd::string::Split("");
        REQUIRE(values.empty());
    }

    SECTION("Can remove first and last characters from a string")
    {
        REQUIRE(sd::string::RemoveFirstCharacter("test") == "est");
        REQUIRE(sd::string::RemoveFirstCharacter("abcde") == "bcde");
        REQUIRE(sd::string::RemoveFirstCharacter("a") == "");
        REQUIRE(sd::string::RemoveFirstCharacter("") == "");

        REQUIRE(sd::string::RemoveLastCharacter("test") == "tes");
        REQUIRE(sd::string::RemoveLastCharacter("abcde") == "abcd");
        REQUIRE(sd::string::RemoveLastCharacter("a") == "");
        REQUIRE(sd::string::RemoveLastCharacter("") == "");

        REQUIRE(sd::string::RemoveFirstAndLastCharacters("test") == "es");
        REQUIRE(sd::string::RemoveFirstAndLastCharacters("abcde") == "bcd");
        REQUIRE(sd::string::RemoveFirstAndLastCharacters("ab") == "");
        REQUIRE(sd::string::RemoveFirstAndLastCharacters("a") == "");
        REQUIRE(sd::string::RemoveFirstAndLastCharacters("") == "");
    }

    SECTION("Can trim whitespace from a string")
    {
        REQUIRE(sd::string::LeftTrim("    hello    ") == "hello    ");
        REQUIRE(sd::string::LeftTrim("  \n\n  test") == "test");
        REQUIRE(sd::string::LeftTrim("\t\t") == "");

        REQUIRE(sd::string::RightTrim("    hello    ") == "    hello");
        REQUIRE(sd::string::RightTrim("test  \n\n  ") == "test");
        REQUIRE(sd::string::RightTrim("\t\t") == "");

        REQUIRE(sd::string::Trim("    hello    ") == "hello");
        REQUIRE(sd::string::Trim("  \n\n  test  \n\n  ") == "test");
        REQUIRE(sd::string::Trim("\t\t") == "");
    }
}

TEST_CASE("Can convert version information to and from strings", "[version]")
{
    SECTION("Can convert strings into version information")
    {
        const sd::VersionInfo versionA = sd::CreateVersionFromString("1.2.3");
        REQUIRE(versionA == sd::CreateVersionInfo(1u, 2u, 3u));

        const sd::VersionInfo versionB = sd::CreateVersionFromString("2.0");
        REQUIRE(versionB == sd::CreateVersionInfo(2u, 0u, 0u));

        const sd::VersionInfo versionC = sd::CreateVersionFromString("1");
        REQUIRE(versionC == sd::CreateVersionInfo(1u, 0u, 0u));
    }

    SECTION("Can convert version information into strings")
    {
        REQUIRE(sd::CreateVersionInfo(4u, 6u, 8u).ToString() == sd::String("4.6.8"));
        REQUIRE(sd::CreateVersionInfo(1u, 10u).ToString() == sd::String("1.10.0"));
        REQUIRE(sd::CreateVersionInfo(5u).ToString() == sd::String("5.0.0"));
        REQUIRE(sd::CreateVersionInfo(7u).ToString(false) == sd::String("7.0"));
    }
}