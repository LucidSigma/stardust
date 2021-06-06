#include "stardust/utility/Utility.h"

#include <SDL2/SDL.h>

#include "stardust/data/Types.h"

namespace stardust
{
    namespace utility
    {
        namespace
        {
            constexpr const char* Base64Characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        }

        [[nodiscard]] String ToBase64(const StringView& data)
        {
            String encodedString;

            u32 bitStream = 0u;
            u32 offset = 0u;

            for (usize i = 0u; i < data.size(); ++i)
            {
                const u32 currentCharacterCode = static_cast<u32>(data[i]);

                offset = 16u - i % 3u * 8u;
                bitStream += currentCharacterCode << offset;

                if (offset == 16u)
                {
                    encodedString += Base64Characters[bitStream >> 18u & 0x3F];
                }

                if (offset == 8u)
                {
                    encodedString += Base64Characters[bitStream >> 12u & 0x3F];
                }

                if (offset == 0u && i != 3u)
                {
                    encodedString += Base64Characters[bitStream >> 6u & 0x3F];
                    encodedString += Base64Characters[bitStream & 0x3F];

                    bitStream = 0u;
                }
            }

            if (offset == 16u)
            {
                encodedString += Base64Characters[bitStream >> 12u & 0x3F];
                encodedString += "==";
            }

            if (offset == 8u)
            {
                encodedString += Base64Characters[bitStream >> 6u & 0x3F];
                encodedString += "=";
            }

            return encodedString;
        }

        [[nodiscard]] String FromBase64(const StringView& base64)
        {
            String decodedString;

            u32 bitStream = 0u;
            u32 offset = 0u;

            static const String base64CharactersString = String(Base64Characters);

            for (usize i = 0u; i < base64.size(); ++i)
            {
                if (const usize characterIndex = base64CharactersString.find(base64[i]);
                    characterIndex != String::npos)
                {
                    offset = 18u - i % 4u * 6u;
                    bitStream += static_cast<u32>(characterIndex) << offset;

                    if (offset == 12u)
                    {
                        decodedString += static_cast<char>(bitStream >> 16u & 0xFF);
                    }

                    if (offset == 6u)
                    {
                        decodedString += static_cast<char>(bitStream >> 8u & 0xFF);
                    }

                    if (offset == 0u && i != 4u)
                    {
                        decodedString += static_cast<char>(bitStream & 0xFF);
                        bitStream = 0u;
                    }
                }
                else if (base64[i] != '=')
                {
                    return "";
                }
            }

            return decodedString;
        }

        void OpenURL(const StringView& url)
        {
            SDL_OpenURL(url.data());
        }
    }
}