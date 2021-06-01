#include "stardust/filesystem/serialise/Serialise.h"

#include <fstream>

#include "stardust/data/Pointers.h"

namespace stardust
{
    namespace serialise
    {
        [[nodiscard]] Status SerialiseToFile(ISerialisable& object, const StringView& filepath, const ArchiveType archiveType)
        {
            std::ios_base::openmode openFlags = std::ios_base::out | std::ios_base::trunc;

            if (archiveType == ArchiveType::Binary || archiveType == ArchiveType::PortableBinary)
            {
                openFlags |= std::ios_base::binary;
            }

            std::ofstream fileStream(filepath.data(), openFlags);

            if (!fileStream.is_open())
            {
                return Status::Fail;
            }

            switch (archiveType)
            {
            case ArchiveType::Binary:
                {
                    const auto archive = std::make_unique<cereal::BinaryOutputArchive>(fileStream);
                    object.Serialise(*archive);

                    break;
                }

            case ArchiveType::PortableBinary:
                {
                    const auto archive = std::make_unique<cereal::PortableBinaryOutputArchive>(fileStream);
                    object.Serialise(*archive);

                    break;
                }

            case ArchiveType::XML:
                {
                    const auto archive = std::make_unique<cereal::XMLOutputArchive>(fileStream);
                    object.Serialise(*archive);

                    break;
                }

            default:
            case ArchiveType::JSON:
                {
                    const auto archive = std::make_unique<cereal::JSONOutputArchive>(fileStream);
                    object.Serialise(*archive);

                    break;
                }
            }

            fileStream.close();

            return Status::Success;
        }

        [[nodiscard]] Status DeserialiseFromFile(ISerialisable& object, const StringView& filepath, const ArchiveType archiveType)
        {
            std::ios_base::openmode openFlags = std::ios_base::in;

            if (archiveType == ArchiveType::Binary || archiveType == ArchiveType::PortableBinary)
            {
                openFlags |= std::ios_base::binary;
            }

            std::ifstream fileStream(filepath.data(), openFlags);

            if (!fileStream.is_open())
            {
                return Status::Fail;
            }

            switch (archiveType)
            {
            case ArchiveType::Binary:
                {
                    const auto archive = std::make_unique<cereal::BinaryInputArchive>(fileStream);
                    object.Deserialise(*archive);

                    break;
                }

            case ArchiveType::PortableBinary:
                {
                    const auto archive = std::make_unique<cereal::PortableBinaryInputArchive>(fileStream);
                    object.Deserialise(*archive);

                    break;
                }

            case ArchiveType::XML:
                {
                    const auto archive = std::make_unique<cereal::XMLInputArchive>(fileStream);
                    object.Deserialise(*archive);

                    break;
                }

            default:
            case ArchiveType::JSON:
                {
                    const auto archive = std::make_unique<cereal::JSONInputArchive>(fileStream);
                    object.Deserialise(*archive);

                    break;
                }
            }

            fileStream.close();

            return Status::Success;
        }
    }
}