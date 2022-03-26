#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <stardust/Stardust.h>

TEST_CASE("Strings can be queried and manipulated in different ways", "[string]")
{
    SECTION("Can convert a string to lowercase")
    {
        REQUIRE(sd::string::ToLowercase("HELLO.") == "hello.");
        REQUIRE(sd::string::ToLowercase(sd::unicode::UTF8ToStandardString(u8"HELLÖ.")) == sd::unicode::UTF8ToStandardString(u8"hellö."));
        REQUIRE(sd::string::ToLowercase("test") == "test");
        REQUIRE(sd::string::ToLowercase("aBCdeFgHiJKlmNopQRstuVwXyz ") == "abcdefghijklmnopqrstuvwxyz ");
        REQUIRE(sd::string::ToLowercase("") == "");

        REQUIRE(sd::string::ToLowercase("I", "en") == "i");
        REQUIRE(sd::string::ToLowercase("I", "tr") == sd::unicode::UTF8ToStandardString(u8"ı"));
    }

    SECTION("Can convert a string to uppercase")
    {
        REQUIRE(sd::string::ToUppercase("hello.") == "HELLO.");
        REQUIRE(sd::string::ToUppercase(sd::unicode::UTF8ToStandardString(u8"hellö.")) == sd::unicode::UTF8ToStandardString(u8"HELLÖ."));
        REQUIRE(sd::string::ToUppercase("TEST") == "TEST");
        REQUIRE(sd::string::ToUppercase("aBCdeFgHiJKlmNopQRstuVwXyz ") == "ABCDEFGHIJKLMNOPQRSTUVWXYZ ");
        REQUIRE(sd::string::ToUppercase("") == "");

        REQUIRE(sd::string::ToUppercase(sd::unicode::UTF8ToStandardString(u8"ß")) == "SS");
    }

    SECTION("Can check if a string is uppercase/lowercase")
    {
        REQUIRE(sd::string::IsUppercase("HELLO."));
        REQUIRE(sd::string::IsUppercase("TEST"));
        REQUIRE(!sd::string::IsUppercase(""));
        REQUIRE(!sd::string::IsUppercase("hello."));
        REQUIRE(!sd::string::IsUppercase("test"));

        REQUIRE(sd::string::IsLowercase("hello."));
        REQUIRE(sd::string::IsLowercase("test"));
        REQUIRE(!sd::string::IsLowercase(""));
        REQUIRE(!sd::string::IsLowercase("HELLO."));
        REQUIRE(!sd::string::IsLowercase("TEST"));
    }

    SECTION("Can split a string on a delimiter")
    {
        sd::List<sd::String> values = sd::string::Split("These are some words.");
        REQUIRE(values == sd::List<sd::String>{ "These", "are", "some", "words." });

        values = sd::string::Split("A|B|C|D", '|');
        REQUIRE(values == sd::List<sd::String>{ "A", "B", "C", "D" });

        values = sd::string::Split("");
        REQUIRE(values.empty());
    }

    SECTION("Can replace all instances of characters and substrings within a string")
    {
        REQUIRE(sd::string::ReplaceAll("abcabc", 'a', 'd') == "dbcdbc");
        REQUIRE(sd::string::ReplaceAll("test", 't', 'k') == "kesk");
        REQUIRE(sd::string::ReplaceAll("aaaa", 'b', 'c') == "aaaa");

        REQUIRE(sd::string::ReplaceAll("123 123 123", "123", "321") == "321 321 321");
        REQUIRE(sd::string::ReplaceAll("this is a test", "is", "at") == "that at a test");
        REQUIRE(sd::string::ReplaceAll("hello world", "world!", "everyone!") == "hello world");
    }
}
