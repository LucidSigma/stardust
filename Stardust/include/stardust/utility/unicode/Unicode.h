#pragma once
#ifndef STARDUST_UNICODE_H
#define STARDUST_UNICODE_H

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace unicode
    {
        [[nodiscard]] extern auto CodepointToUTF8(const u32 codepoint) -> Array<char, 4u>;

        [[nodiscard]] extern auto UTF8ToUTF16(const String& utf8String) -> UTF16String;
        [[nodiscard]] extern auto UTF8ToUTF32(const String& utf8String) -> UTF32String;

        [[nodiscard]] extern auto UTF16ToUTF8(const UTF16String& utf16String) -> UTF8String;
        [[nodiscard]] extern auto UTF16ToStandardUTF8(const UTF16String& utf16String) -> String;
        [[nodiscard]] extern auto UTF32ToUTF8(const UTF32String& utf32String) -> UTF8String;
        [[nodiscard]] extern auto UTF32ToStandardUTF8(const UTF32String& utf32String) -> String;

        [[nodiscard]] extern auto UTF16ToUTF32(const UTF16String& utf16String) -> UTF32String;
        [[nodiscard]] extern auto UTF32ToUTF16(const UTF32String& utf32String) -> UTF16String;

        [[nodiscard]] extern auto UTF8ToStandardString(const UTF8String& utf8String) -> String;
    }
}

#endif
