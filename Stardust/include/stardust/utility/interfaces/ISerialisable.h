#pragma once
#ifndef STARDUST_I_SERIALISABLE_H
#define STARDUST_I_SERIALISABLE_H

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

namespace stardust
{
    class ISerialisable
    {
    public:
        virtual void Serialise(cereal::BinaryOutputArchive& archive) { }
        virtual void Deserialise(cereal::BinaryInputArchive& archive) { }

        virtual void Serialise(cereal::PortableBinaryOutputArchive& archive) { }
        virtual void Deserialise(cereal::PortableBinaryInputArchive& archive) { }

        virtual void Serialise(cereal::JSONOutputArchive& archive) { }
        virtual void Deserialise(cereal::JSONInputArchive& archive) { }

        virtual void Serialise(cereal::XMLOutputArchive& archive) { }
        virtual void Deserialise(cereal::XMLInputArchive& archive) { }

    protected:
        ISerialisable() = default;
        virtual ~ISerialisable() noexcept = default;
    };
}

#endif