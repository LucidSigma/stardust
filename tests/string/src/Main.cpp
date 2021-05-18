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
}