#include "stardust/data/Containers.h"

#include <algorithm>
#include <locale>
#include <sstream>

namespace stardust
{
    namespace string
    {
        [[nodiscard]] Vector<String> Split(const String& string, const char delimiter)
        {
            String currentToken;
            std::istringstream reader(string);

            Vector<String> tokens;

            while (std::getline(reader, currentToken, delimiter))
            {
                tokens.push_back(currentToken);
            }

            return tokens;
        }

        [[nodiscard]] String MakeUpper(String string)
        {
            std::transform(
                std::cbegin(string), std::cend(string),
                std::begin(string),
                [](const char letter) -> char
                {
                    return static_cast<char>(std::toupper(static_cast<unsigned char>(letter), std::locale()));
                }
            );

            return string;
        }

        [[nodiscard]] String MakeLower(String string)
        {
            std::transform(
                std::cbegin(string), std::cend(string),
                std::begin(string),
                [](const char letter) -> char
                {
                    return static_cast<char>(std::tolower(static_cast<unsigned char>(letter), std::locale()));
                }
            );

            return string;
        }

        [[nodiscard]] String RemoveFirstCharacter(const String& string)
        {
            return string.empty() ? "" : string.substr(1u);
        }

        [[nodiscard]] String RemoveLastCharacter(const String& string)
        {
            return string.empty() ? "" : string.substr(0u, string.length() - 2u);
        }

        [[nodiscard]] String RemoveFirstAndLastCharacters(const String& string)
        {
            return string.length() > 2u ? string.substr(1u, string.length() - 2u) : "";
        }

        [[nodiscard]] String LeftTrim(String string)
        {
            string.erase(
                std::cbegin(string),
                std::find_if(std::cbegin(string), std::cend(string), [](const unsigned char character)
                {
                    return !std::isspace(character, std::locale());
                })
            );

            return string;
        }

        [[nodiscard]] String RightTrim(String string)
        {
            string.erase(
                std::find_if(std::crbegin(string), std::crend(string), [](const unsigned char character)
                {
                    return !std::isspace(character, std::locale());
                }).base(),
                std::cend(string)
            );

            return string;
        }

        [[nodiscard]] String Trim(String string)
        {
            string = LeftTrim(string);

            return RightTrim(string);
        }
    }
}