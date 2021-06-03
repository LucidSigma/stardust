#include "stardust/filesystem/serialise/Serialise.h"

#include <concepts>
#include <fstream>
#include <istream>
#include <sstream>
#include <ostream>

#include "stardust/data/Pointers.h"

namespace stardust
{
    namespace serialise
    {
        namespace
        {
            template <std::derived_from<std::ostream> Stream>
            void SerialiseHelper(ISerialisable& object, Stream& stream, const ArchiveType archiveType)
            {
                switch (archiveType)
                {
                case ArchiveType::Binary:
                    {
                        const auto archive = std::make_unique<cereal::BinaryOutputArchive>(stream);
                        object.Serialise(*archive);

                        break;
                    }

                case ArchiveType::PortableBinary:
                    {
                        const auto archive = std::make_unique<cereal::PortableBinaryOutputArchive>(stream);
                        object.Serialise(*archive);

                        break;
                    }

                case ArchiveType::XML:
                    {
                        const auto archive = std::make_unique<cereal::XMLOutputArchive>(stream);
                        object.Serialise(*archive);

                        break;
                    }

                default:
                case ArchiveType::JSON:
                    {
                        const auto archive = std::make_unique<cereal::JSONOutputArchive>(stream);
                        object.Serialise(*archive);

                        break;
                    }
                }
            }

            template <std::derived_from<std::istream> Stream>
            void DeserialiseHelper(ISerialisable& object, Stream& stream, const ArchiveType archiveType)
            {
                switch (archiveType)
                {
                case ArchiveType::Binary:
                    {
                        const auto archive = std::make_unique<cereal::BinaryInputArchive>(stream);
                        object.Deserialise(*archive);

                        break;
                    }

                case ArchiveType::PortableBinary:
                    {
                        const auto archive = std::make_unique<cereal::PortableBinaryInputArchive>(stream);
                        object.Deserialise(*archive);

                        break;
                    }

                case ArchiveType::XML:
                    {
                        const auto archive = std::make_unique<cereal::XMLInputArchive>(stream);
                        object.Deserialise(*archive);

                        break;
                    }

                default:
                case ArchiveType::JSON:
                    {
                        const auto archive = std::make_unique<cereal::JSONInputArchive>(stream);
                        object.Deserialise(*archive);

                        break;
                    }
                }
            }
        }

        [[nodiscard]] Vector<ubyte> SerialiseToMemory(ISerialisable& object, const ArchiveType archiveType)
        {
            std::ostringstream dataStream;

            SerialiseHelper(object, dataStream, archiveType);

            const String data = dataStream.str();

            return Vector<ubyte>(std::cbegin(data), std::cend(data));
        }

        [[nodiscard]] Status DeserialiseFromMemory(ISerialisable& object, const Vector<ubyte>& data, const ArchiveType archiveType)
        {
            const String dataString(std::cbegin(data), std::cend(data));
            std::istringstream dataStream(dataString);

            if (dataStream.fail())
            {
                return Status::Fail;
            }

            DeserialiseHelper(object, dataStream, archiveType);

            return Status::Success;
        }

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

            SerialiseHelper(object, fileStream, archiveType);

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

            DeserialiseHelper(object, fileStream, archiveType);

            fileStream.close();

            return Status::Success;
        }
    }
}