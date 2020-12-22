#include "stardust/data/Containers.h"

#include <algorithm>
#include <locale>
#include <sstream>

namespace stardust
{
	[[nodiscard]] Vector<String> SplitString(const String& string, const char delimiter)
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
}