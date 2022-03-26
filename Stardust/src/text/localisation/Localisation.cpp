#include "stardust/text/localisation/Localisation.h"

#include <icu/unicode/ubidi.h>
#include <icu/unicode/utypes.h>

#include "stardust/math/Math.h"
#include "stardust/utility/unicode/Unicode.h"

namespace stardust
{
    namespace localisation
    {
        [[nodiscard]] auto PerformBiDiTransformation(const String& text, const bool reverseOutput) -> String
        {
            UBiDi* biDi = ubidi_open();

            if (biDi == nullptr)
            {
                return String{ };
            }

            const UTF16String utf16String = unicode::UTF8ToUTF16(text);
            UErrorCode errorCode = U_ZERO_ERROR;

            ubidi_setPara(
                biDi,
                utf16String.data(),
                static_cast<i32>(utf16String.length()),
                UBIDI_DEFAULT_LTR,
                nullptr,
                &errorCode
            );

            if (U_SUCCESS(errorCode) == 0)
            {
                return String{ };
            }

            const usize processedCharacterCount = static_cast<usize>(ubidi_getProcessedLength(biDi));
            UTF16String transformedUTF16String(processedCharacterCount, u'\0');

            ubidi_writeReordered(
                biDi,
                transformedUTF16String.data(),
                static_cast<i32>(processedCharacterCount),
                reverseOutput ? UBIDI_OUTPUT_REVERSE : 0u,
                &errorCode
            );

            if (U_SUCCESS(errorCode) == 0)
            {
                return String{ };
            }

            ubidi_close(biDi);
            biDi = nullptr;

            return unicode::UTF16ToStandardUTF8(transformedUTF16String);
        }

        [[nodiscard]] auto PerformBiDiTransformation(const UTF8String& text, const bool reverseOutput) -> String
        {
            return PerformBiDiTransformation(unicode::UTF8ToStandardString(text), reverseOutput);
        }
    }
}

namespace std
{
    [[nodiscard]] auto hash<stardust::localisation::TextLocalisationInfo>::operator ()(const stardust::localisation::TextLocalisationInfo& localisationInfo) const noexcept -> stardust::usize
    {
        stardust::usize seed = 0u;

        glm::detail::hash_combine(seed, std::hash<stardust::localisation::Direction>()(localisationInfo.direction));
        glm::detail::hash_combine(seed, std::hash<stardust::localisation::Script>()(localisationInfo.script));
        glm::detail::hash_combine(seed, std::hash<stardust::String>()(localisationInfo.language));
        glm::detail::hash_combine(seed, std::hash<bool>()(localisationInfo.isStartOfText));
        glm::detail::hash_combine(seed, std::hash<bool>()(localisationInfo.isEndOfText));

        return seed;
    }
}
