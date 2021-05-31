#include "stardust/utility/hash/Hash.h"

#include <hash/md5.h>
#include <hash/sha1.h>
#include <hash/sha256.h>

namespace stardust
{
    namespace
    {
        MD5 md5Hasher;
        SHA1 sha1Hasher;
        SHA256 sha256Hasher;
    }

    namespace utility
    {
        [[nodiscard]] String GetMD5Hash(const StringView& text)
        {
            return md5Hasher(text.data());
        }

        [[nodiscard]] String GetSHA1Hash(const StringView& text)
        {
            return sha1Hasher(text.data());
        }

        [[nodiscard]] String GetSHA256Hash(const StringView& text)
        {
            return sha256Hasher(text.data());
        }
    }
}