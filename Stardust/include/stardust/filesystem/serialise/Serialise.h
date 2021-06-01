#pragma once
#ifndef STARDUST_SERIALISE_H
#define STARDUST_SERIALISE_H

#include "stardust/data/Containers.h"
#include "stardust/utility/interfaces/ISerialisable.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    namespace serialise
    {
        enum class ArchiveType
        {
            Binary,
            PortableBinary,
            JSON,
            XML,
        };

        [[nodiscard]] Status SerialiseToFile(ISerialisable& object, const StringView& filepath, const ArchiveType archiveType);
        [[nodiscard]] Status DeserialiseFromFile(ISerialisable& object, const StringView& filepath, const ArchiveType archiveType);
    }

    namespace serial = serialise;
}

#endif