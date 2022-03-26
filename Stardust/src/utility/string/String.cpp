#include "stardust/utility/string/String.h"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <sstream>
#include <string>

#include <icu/unicode/uchar.h>
#include <icu/unicode/ustring.h>
#include <icu/unicode/utypes.h>
#include <utfcpp/utf8.h>

#include "stardust/utility/unicode/Unicode.h"

namespace stardust
{
    namespace string
    {
        [[nodiscard]] auto Split(const String& string, const char delimiter, const bool keepEmptyStrings) -> List<String>
        {
            String currentToken;
            std::istringstream reader(string);

            List<String> tokens{ };

            while (std::getline(reader, currentToken, delimiter))
            {
                if (!keepEmptyStrings && currentToken.empty())
                {
                    continue;
                }

                tokens.push_back(currentToken);
            }

            return tokens;
        }

        [[nodiscard]] auto Split(const String& string, const List<char>& delimiters, const bool keepEmptyStrings) -> List<String>
        {
            List<String> tokens{ };

            usize previousPosition = 0u;
            usize currentPosition = string.find_first_of(delimiters.data(), previousPosition);

            while (currentPosition != String::npos)
            {
                const bool hasNewToken = keepEmptyStrings
                    ? currentPosition >= previousPosition
                    : currentPosition > previousPosition;

                if (hasNewToken)
                {
                    tokens.push_back(string.substr(previousPosition, currentPosition - previousPosition));
                }

                previousPosition = currentPosition + 1u;
                currentPosition = string.find_first_of(delimiters.data(), previousPosition);
            }

            if (previousPosition < string.length())
            {
                tokens.push_back(string.substr(previousPosition, String::npos));
            }

            return tokens;
        }

        [[nodiscard]] auto Split(const String& string, const HashSet<u32>& utf8CodepointDelimiters, const bool keepEmptyStrings) -> List<String>
        {
            List<String> tokens{ };

            auto iterator = std::cbegin(string);

            String currentToken;
            bool isCurrentTokenInProgress = false;
            auto currentTokenBackInserter = std::back_inserter(currentToken);

            while (iterator != std::cend(string))
            {
                const u32 codepoint = utf8::next(iterator, std::cend(string));
                isCurrentTokenInProgress = true;

                if (utf8CodepointDelimiters.contains(codepoint))
                {
                    if (keepEmptyStrings || (!keepEmptyStrings && !currentToken.empty()))
                    {
                        tokens.push_back(currentToken);
                    }

                    currentToken.clear();
                    isCurrentTokenInProgress = false;
                }
                else
                {
                    currentTokenBackInserter = utf8::append(codepoint, currentTokenBackInserter);
                }
            }

            if (isCurrentTokenInProgress)
            {
                tokens.push_back(currentToken);
            }

            return tokens;
        }

        [[nodiscard]] auto SplitWhitespace(const String& string, const bool keepEmptyStrings) -> List<String>
        {
            List<String> tokens{ };

            auto iterator = std::cbegin(string);

            String currentToken;
            bool isCurrentTokenInProgress = false;
            auto currentTokenBackInserter = std::back_inserter(currentToken);

            while (iterator != std::cend(string))
            {
                const u32 codepoint = utf8::next(iterator, std::cend(string));
                isCurrentTokenInProgress = true;

                if (u_isWhitespace(codepoint) != 0)
                {
                    if (keepEmptyStrings || (!keepEmptyStrings && !currentToken.empty()))
                    {
                        tokens.push_back(currentToken);
                    }

                    currentToken.clear();
                    isCurrentTokenInProgress = false;
                }
                else
                {
                    currentTokenBackInserter = utf8::append(codepoint, currentTokenBackInserter);
                }
            }

            if (isCurrentTokenInProgress)
            {
                tokens.push_back(currentToken);
            }

            return tokens;
        }

        [[nodiscard]] auto SplitASCIIWhitespace(const String& string, const bool keepEmptyStrings) -> List<String>
        {
            List<String> tokens{ };

            auto iterator = std::cbegin(string);

            String currentToken;
            bool isCurrentTokenInProgress = false;
            auto currentTokenBackInserter = std::back_inserter(currentToken);

            while (iterator != std::cend(string))
            {
                const u32 codepoint = utf8::next(iterator, std::cend(string));
                isCurrentTokenInProgress = true;

                if (std::isspace(codepoint) != 0)
                {
                    if (keepEmptyStrings || (!keepEmptyStrings && !currentToken.empty()))
                    {
                        tokens.push_back(currentToken);
                    }

                    currentToken.clear();
                    isCurrentTokenInProgress = false;
                }
                else
                {
                    currentTokenBackInserter = utf8::append(codepoint, currentTokenBackInserter);
                }
            }

            if (isCurrentTokenInProgress)
            {
                tokens.push_back(currentToken);
            }

            return tokens;
        }

        [[nodiscard]] auto IsASCII(const String& string) -> bool
        {
            return std::ranges::all_of(
                string,
                [](const char glyph) -> bool
                {
                    return glyph >= '\0' && glyph <= static_cast<char>(127);
                }
            );
        }

        [[nodiscard]] auto ToUppercase(const String& string, const Optional<String>& locale) -> String
        {
            const UTF16String utf16String = unicode::UTF8ToUTF16(string);
            UTF16String uppercaseString(utf16String.length() * 2u + 1u, '\0');

            UErrorCode errorCode = U_ZERO_ERROR;

            const i32 uppercaseStringLength = u_strToUpper(
                uppercaseString.data(),
                static_cast<i32>(uppercaseString.size()),
                utf16String.data(),
                static_cast<i32>(utf16String.size()),
                locale.has_value() ? locale.value().c_str() : nullptr,
                &errorCode
            );

            if (U_SUCCESS(errorCode) == 0)
            {
                return String{ };
            }

            const String utf8String = unicode::UTF16ToStandardUTF8(
                UTF16String(
                    std::cbegin(uppercaseString),
                    std::cbegin(uppercaseString) + uppercaseStringLength
                )
            );

            return utf8String;
        }

        [[nodiscard]] auto ToSimpleUppercase(const String& string) -> String
        {
            String uppercaseString;
            auto uppercaseStringBackInserter = std::back_inserter(uppercaseString);

            auto iterator = std::cbegin(string);

            while (iterator != std::cend(string))
            {
                const u32 codepoint = utf8::next(iterator, std::cend(string));
                uppercaseStringBackInserter = utf8::append(u_toupper(codepoint), uppercaseStringBackInserter);
            }

            return uppercaseString;
        }

        [[nodiscard]] auto ToASCIIUppercase(String string) -> String
        {
            std::ranges::transform(
                std::cbegin(string),
                std::cend(string),
                std::begin(string),
                [](const char glyph) -> char
                {
                    return std::toupper(glyph);
                }
            );

            return string;
        }

        [[nodiscard]] auto ToLowercase(const String& string, const Optional<String>& locale) -> String
        {
            const UTF16String utf16String = unicode::UTF8ToUTF16(string);
            UTF16String lowercaseString(utf16String.length() * 2u + 1u, '\0');

            UErrorCode errorCode = U_ZERO_ERROR;

            const i32 lowercaseStringLength = u_strToLower(
                lowercaseString.data(),
                static_cast<i32>(lowercaseString.size()),
                utf16String.data(),
                static_cast<i32>(utf16String.size()),
                locale.has_value() ? locale.value().c_str() : nullptr,
                &errorCode
            );

            if (U_SUCCESS(errorCode) == 0)
            {
                return String{ };
            }

            const String utf8String = unicode::UTF16ToStandardUTF8(
                UTF16String(
                    std::cbegin(lowercaseString),
                    std::cbegin(lowercaseString) + lowercaseStringLength
                )
            );

            return utf8String;
        }

        [[nodiscard]] auto ToSimpleLowercase(const String& string) -> String
        {
            String lowercaseString;
            auto lowercaseStringBackInserter = std::back_inserter(lowercaseString);

            auto iterator = std::cbegin(string);

            while (iterator != std::cend(string))
            {
                const u32 codepoint = utf8::next(iterator, std::cend(string));
                lowercaseStringBackInserter = utf8::append(u_tolower(codepoint), lowercaseStringBackInserter);
            }

            return lowercaseString;
        }

        [[nodiscard]] auto ToASCIILowercase(String string) -> String
        {
            std::ranges::transform(
                std::cbegin(string),
                std::cend(string),
                std::begin(string),
                [](const char glyph) -> char
                {
                    return std::toupper(glyph);
                }
            );

            return string;
        }

        [[nodiscard]] auto IsUppercase(const String& string) -> bool
        {
            if (string.empty())
            {
                return false;
            }

            auto iterator = std::cbegin(string);

            while (iterator != std::cend(string))
            {
                const u32 codepoint = utf8::next(iterator, std::cend(string));

                if (u_isalpha(codepoint) != 0 && u_isupper(codepoint) == 0)
                {
                    return false;
                }
            }

            return true;
        }

        [[nodiscard]] auto IsASCIIUppercase(const String& string) -> bool
        {
            return std::ranges::all_of(
                string,
                [](const char glyph) -> bool
                {
                    return std::isupper(glyph);
                }
            );
        }

        [[nodiscard]] auto IsLowercase(const String& string) -> bool
        {
            if (string.empty())
            {
                return false;
            }

            auto iterator = std::cbegin(string);

            while (iterator != std::cend(string))
            {
                const u32 codepoint = utf8::next(iterator, std::cend(string));

                if (u_isalpha(codepoint) != 0 && u_islower(codepoint) == 0)
                {
                    return false;
                }
            }

            return true;
        }

        [[nodiscard]] auto IsASCIILowercase(const String& string) -> bool
        {
            return std::ranges::all_of(
                string,
                [](const char glyph) -> bool
                {
                    return std::islower(glyph);
                }
            );
        }
        
        [[nodiscard]] auto ReplaceAll(String inputString, const char searchCharacter, const char replacementCharacter) -> String
        {
            std::ranges::replace(inputString, searchCharacter, replacementCharacter);

            return inputString;
        }

        [[nodiscard]] auto ReplaceAll(String inputString, const String& searchString, const String& replacementString) -> String
        {
            usize position = 0u;

            for (position = inputString.find(searchString, position); position != String::npos; position = inputString.find(searchString, position))
            {
                inputString.replace(position, searchString.length(), replacementString);
                position += replacementString.length();
            }

            return inputString;
        }
    }
}
