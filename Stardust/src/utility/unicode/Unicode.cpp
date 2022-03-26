#include "stardust/utility/unicode/Unicode.h"

#include <iterator>

#include <utfcpp/utf8.h>

namespace stardust
{
    namespace unicode
    {
        [[nodiscard]] auto CodepointToUTF8(const u32 codepoint) -> Array<char, 4u>
        {
            Array<char, 4u> utf8Buffer{ '\0', '\0', '\0', '\0' };

            if (codepoint <= 0x7Fu)
            {
                utf8Buffer[0u] = static_cast<char>(codepoint);
            }

            if (codepoint <= 0x7FFu)
            {
                utf8Buffer[0u] = static_cast<char>(0xC0u | (codepoint >> 6u));
                utf8Buffer[1u] = static_cast<char>(0x80u | (codepoint & 0x3Fu));
            }

            if (codepoint <= 0xFFFFu)
            {
                utf8Buffer[0u] = static_cast<char>(0xE0u | (codepoint >> 12u));
                utf8Buffer[1u] = static_cast<char>(0x80u | ((codepoint >> 6u) & 0x3Fu));
                utf8Buffer[2u] = static_cast<char>(0x80u | (codepoint & 0x3Fu));
            }

            if (codepoint <= 0x10FFFFu)
            {
                utf8Buffer[0u] = static_cast<char>(0xF0u | (codepoint >> 18u));
                utf8Buffer[1u] = static_cast<char>(0x80u | ((codepoint >> 12u) & 0x3Fu));
                utf8Buffer[2u] = static_cast<char>(0x80u | ((codepoint >> 6u) & 0x3Fu));
                utf8Buffer[3u] = static_cast<char>(0x80u | (codepoint & 0x3Fu));
            }

            return utf8Buffer;
        }

        [[nodiscard]] auto UTF8ToUTF16(const String& utf8String) -> UTF16String
        {
            UTF16String utf16String{ };
            auto utf16StringBackInserter = std::back_inserter(utf16String);

            utf8::utf8to16(std::cbegin(utf8String), std::cend(utf8String), utf16StringBackInserter);

            return utf16String;
        }

        [[nodiscard]] auto UTF8ToUTF32(const String& utf8String) -> UTF32String
        {
            UTF32String utf32String{ };
            auto utf32StringBackInserter = std::back_inserter(utf32String);

            utf8::utf8to32(std::cbegin(utf8String), std::cend(utf8String), utf32StringBackInserter);

            return utf32String;
        }

        [[nodiscard]] auto UTF16ToUTF8(const UTF16String& utf16String) -> UTF8String
        {
            UTF8String utf8String{ };
            auto utf8StringBackInserter = std::back_inserter(utf8String);

            utf8::utf16to8(std::cbegin(utf16String), std::cend(utf16String), utf8StringBackInserter);

            return utf8String;
        }

        [[nodiscard]] auto UTF16ToStandardUTF8(const UTF16String& utf16String) -> String
        {
            String utf8String{ };
            auto utf8StringBackInserter = std::back_inserter(utf8String);

            utf8::utf16to8(std::cbegin(utf16String), std::cend(utf16String), utf8StringBackInserter);

            return utf8String;
        }

        [[nodiscard]] auto UTF32ToUTF8(const UTF32String& utf32String) -> UTF8String
        {
            UTF8String utf8String{ };
            auto utf8StringBackInserter = std::back_inserter(utf8String);

            utf8::utf32to8(std::cbegin(utf32String), std::cend(utf32String), utf8StringBackInserter);

            return utf8String;
        }

        [[nodiscard]] auto UTF32ToStandardUTF8(const UTF32String& utf32String) -> String
        {
            String utf8String{ };
            auto utf8StringBackInserter = std::back_inserter(utf8String);

            utf8::utf32to8(std::cbegin(utf32String), std::cend(utf32String), utf8StringBackInserter);

            return utf8String;
        }

        [[nodiscard]] auto UTF16ToUTF32(const UTF16String& utf16String) -> UTF32String
        {
            return UTF8ToUTF32(UTF16ToStandardUTF8(utf16String));
        }

        [[nodiscard]] auto UTF32ToUTF16(const UTF32String& utf32String) -> UTF16String
        {
            return UTF8ToUTF16(UTF32ToStandardUTF8(utf32String));
        }

        [[nodiscard]] auto UTF8ToStandardString(const UTF8String& utf8String) -> String
        {
            return String(std::cbegin(utf8String), std::cend(utf8String));
        }
    }
}
