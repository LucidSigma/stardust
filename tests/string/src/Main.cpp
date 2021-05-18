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
        sd::Vector<sd::String> values = sd::string::SplitString("These are some words.");
        REQUIRE(values == sd::Vector<sd::String>{ "These", "are", "some", "words." });

        values = sd::string::SplitString("A|B|C|D", '|');
        REQUIRE(values == sd::Vector<sd::String>{ "A", "B", "C", "D" });

        values = sd::string::SplitString("");
        REQUIRE(values.empty());
    }
}