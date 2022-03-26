#pragma once
#ifndef STARDUST_STRING_H
#define STARDUST_STRING_H

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace string
    {
        [[nodiscard]] extern auto Split(const String& string, const char delimiter = ' ', const bool keepEmptyStrings = false) -> List<String>;
        [[nodiscard]] extern auto Split(const String& string, const List<char>& delimiters, const bool keepEmptyStrings = false) -> List<String>;
        [[nodiscard]] extern auto Split(const String& string, const HashSet<u32>& utf8CodepointDelimiters, const bool keepEmptyStrings = false) -> List<String>;

        [[nodiscard]] extern auto SplitWhitespace(const String& string, const bool keepEmptyStrings = false) -> List<String>;
        [[nodiscard]] extern auto SplitASCIIWhitespace(const String& string, const bool keepEmptyStrings = false) -> List<String>;

        [[nodiscard]] extern auto IsASCII(const String& string) -> bool;

        [[nodiscard]] extern auto ToUppercase(const String& string, const Optional<String>& locale = None) -> String;
        [[nodiscard]] extern auto ToSimpleUppercase(const String& string) -> String;
        [[nodiscard]] extern auto ToASCIIUppercase(String string) -> String;
        [[nodiscard]] extern auto ToLowercase(const String& string, const Optional<String>& locale = None) -> String;
        [[nodiscard]] extern auto ToSimpleLowercase(const String& string) -> String;
        [[nodiscard]] extern auto ToASCIILowercase(String string) -> String;

        [[nodiscard]] extern auto IsUppercase(const String& string) -> bool;
        [[nodiscard]] extern auto IsASCIIUppercase(const String& string) -> bool;
        [[nodiscard]] extern auto IsLowercase(const String& string) -> bool;
        [[nodiscard]] extern auto IsASCIILowercase(const String& string) -> bool;

        [[nodiscard]] extern auto ReplaceAll(String inputString, const char searchCharacter, const char replacementCharacter)->String;
        [[nodiscard]] extern auto ReplaceAll(String inputString, const String& searchString, const String& replacementString)->String;
    }
}

#endif
