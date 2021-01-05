/// BSD 2-Clause License
///
/// Copyright (c) 2020, Robin Berg Pettersen
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this
///    list of conditions and the following disclaimer.
///
/// 2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation
///    and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef TILESON_TILESON_MIN_H
#define TILESON_TILESON_MIN_H

// This is the minimal version of Tileson. You will have to provide your own version of nlohmann/json and make sure it's included before anything
// Tileson related.
#include <nlohmann/json.hpp>


/*** Start of inlined file: tileson_parser.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILESON_PARSER_HPP
#define TILESON_TILESON_PARSER_HPP


/*** Start of inlined file: Tools.hpp ***/
//
// Created by robin on 31.07.2020.
//

#ifndef TILESON_TOOLS_HPP
#define TILESON_TOOLS_HPP

#include <cstdint>
#include <vector>
#include <string_view>
namespace tson
{
	class Tools
	{

		public:
			Tools() = delete;
			~Tools() = delete;
			inline static std::vector<uint8_t> Base64DecodedStringToBytes(std::string_view str);
			inline static std::vector<uint32_t> BytesToUnsignedInts(const std::vector<uint8_t> &bytes);
			//inline static std::vector<int> BytesToInts(const std::vector<uint8_t> &bytes);
	};

	/*!
	 * When you have decoded a Base64 string, you'll get a string representing bytes. This function turns them into actual bytes.
	 * @param str
	 * @return
	 */
	std::vector<uint8_t> Tools::Base64DecodedStringToBytes(std::string_view str)
	{
		std::vector<uint8_t> bytes;
		for(size_t i = 0; i < str.size(); ++i)
		{
			uint8_t u8 = static_cast<uint8_t>(str[i]);
			bytes.push_back(u8);
		}
		return bytes;
	}

	/*!
	 * Converts bytes into unsigned int values. The bytes are converted in the Little Endian byte order to fit Tiled's specs.
	 * @param bytes A vector of bytes.
	 * @return Bytes converted to unsigned ints
	 */
	std::vector<uint32_t> Tools::BytesToUnsignedInts(const std::vector<uint8_t> &bytes)
	{
		std::vector<uint32_t> uints;
		std::vector<uint8_t> toConvert;
		//uint32_t size8 = (compressed[55] << 24) | (compressed[56] << 16) | (compressed[57] << 8) | compressed[58]; //Should be 66000

		for(size_t i = 0; i < bytes.size(); ++i)
		{
			toConvert.push_back(bytes[i]);
			if(toConvert.size() == 4)
			{
				uint32_t u32 = (toConvert[3] << 24) | (toConvert[2] << 16) | (toConvert[1] << 8) | toConvert[0];
				uints.push_back(u32);
				toConvert.clear();
			}
		}

		return uints;
	}

	/*!
	 * While the Tiled specification uses unsigned ints for their tiles, Tileson uses regular ints.
	 * This may be changed in the future, but should in reality never really become an issue.
	 *
	 * Update 2020-11-09: This will cause problems when tiles has flip flags!
	 *
	 * int differences:
	 * int max:  2147483647
	 * uint max: 4294967295
	 *
	 * @param bytes A vector of bytes.
	 * @return Bytes converted to ints
	 */
	/*std::vector<int> Tools::BytesToInts(const std::vector<uint8_t> &bytes)
	{
		std::vector<int> ints;
		std::vector<uint8_t> toConvert;
		//uint32_t size8 = (compressed[55] << 24) | (compressed[56] << 16) | (compressed[57] << 8) | compressed[58]; //Should be 66000

		for(size_t i = 0; i < bytes.size(); ++i)
		{
			toConvert.push_back(bytes[i]);
			if(toConvert.size() == 4)
			{
				uint32_t u32 = (toConvert[3] << 24) | (toConvert[2] << 16) | (toConvert[1] << 8) | toConvert[0];
				ints.push_back(u32);
				toConvert.clear();
			}
		}

		return ints;
	}*/
}

#endif //TILESON_TOOLS_HPP

/*** End of inlined file: Tools.hpp ***/


/*** Start of inlined file: Base64Decompressor.hpp ***/
//
// Created by robin on 29.07.2020.
// The Base64 decoding logic is heavily based on: https://github.com/ReneNyffenegger/cpp-base64
//

#ifndef TILESON_BASE64DECOMPRESSOR_HPP
#define TILESON_BASE64DECOMPRESSOR_HPP


/*** Start of inlined file: IDecompressor.hpp ***/
//
// Created by robin on 29.07.2020.
//

#ifndef TILESON_IDECOMPRESSOR_HPP
#define TILESON_IDECOMPRESSOR_HPP

#include <string_view>

namespace tson
{
	class IDecompressor
	{
		public:
			/*!
			 * If the name matches with 'compression' or 'encoding' the decompress() function will
			 * be called automatically for the actual Layer. Encoding-related matching is handled first!
			 *
			 * Known values:
			 *
			 * compression: zlib, gzip, zstd (since Tiled 1.3) or empty (default) (tilelayer only).
			 * encoding: csv (default) or base64 (tilelayer only).
			 *
			 * @return
			 */
			virtual const std::string &name() const = 0;

			virtual std::string decompress(std::string_view s) = 0;
	};
}

#endif //TILESON_IDECOMPRESSOR_HPP

/*** End of inlined file: IDecompressor.hpp ***/

#include <string>

namespace tson
{
	class Base64Decompressor : public IDecompressor
	{
		public:
			[[nodiscard]] inline const std::string &name() const override;

			inline std::string decompress(std::string_view s) override;

		private:
			inline unsigned int pos_of_char(const unsigned char chr);
			inline static const std::string NAME = "base64";
	};

	const std::string &Base64Decompressor::name() const
	{
		return NAME;
	}

	std::string Base64Decompressor::decompress(std::string_view s)
	{

		size_t length_of_string = s.length();
		if (!length_of_string) return std::string("");

		size_t in_len = length_of_string;
		size_t pos = 0;

		//
		// The approximate length (bytes) of the decoded string might be one ore
		// two bytes smaller, depending on the amount of trailing equal signs
		// in the encoded string. This approximation is needed to reserve
		// enough space in the string to be returned.
		//
		size_t approx_length_of_decoded_string = length_of_string / 4 * 3;
		std::string ret;
		ret.reserve(approx_length_of_decoded_string);

		while (pos < in_len) {

			unsigned int pos_of_char_1 = pos_of_char(s[pos+1] );

			ret.push_back(static_cast<std::string::value_type>( ( (pos_of_char(s[pos+0]) ) << 2 ) + ( (pos_of_char_1 & 0x30 ) >> 4)));

			if (s[pos+2] != '=' && s[pos+2] != '.') { // accept URL-safe base 64 strings, too, so check for '.' also.

				unsigned int pos_of_char_2 = pos_of_char(s[pos+2] );
				ret.push_back(static_cast<std::string::value_type>( (( pos_of_char_1 & 0x0f) << 4) + (( pos_of_char_2 & 0x3c) >> 2)));

				if (s[pos+3] != '=' && s[pos+3] != '.') {
					ret.push_back(static_cast<std::string::value_type>( ( (pos_of_char_2 & 0x03 ) << 6 ) + pos_of_char(s[pos+3])   ));
				}
			}

			pos += 4;
		}

		return ret;
	}

	unsigned int Base64Decompressor::pos_of_char(const unsigned char chr)
	{
		//
		// Return the position of chr within base64_encode()
		//

		if      (chr >= 'A' && chr <= 'Z') return chr - 'A';
		else if (chr >= 'a' && chr <= 'z') return chr - 'a' + ('Z' - 'A')               + 1;
		else if (chr >= '0' && chr <= '9') return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
		else if (chr == '+' || chr == '-') return 62; // Be liberal with input and accept both url ('-') and non-url ('+') base 64 characters (
		else if (chr == '/' || chr == '_') return 63; // Ditto for '/' and '_'

		throw "If input is correct, this line should never be reached.";
	}
}

#endif //TILESON_BASE64DECOMPRESSOR_HPP

/*** End of inlined file: Base64Decompressor.hpp ***/


/*** Start of inlined file: DecompressorContainer.hpp ***/
//
// Created by robin on 30.07.2020.
//

#ifndef TILESON_DECOMPRESSORCONTAINER_HPP
#define TILESON_DECOMPRESSORCONTAINER_HPP

#include <memory>
#include <vector>
#include <string_view>
#include <functional>
namespace tson
{
	class DecompressorContainer
	{
		public:
			inline DecompressorContainer() = default;
			template <typename T, typename... Args>
			inline void add(Args &&... args);
			inline void remove(std::string_view name);
			inline bool contains(std::string_view name) const;
			inline bool empty() const;
			inline size_t size() const;
			inline void clear();

			inline IDecompressor *get(std::string_view name);
		private:
			//Key: name,
			std::vector<std::unique_ptr<IDecompressor>> m_decompressors;
	};

	template<typename T, typename... Args>
	void DecompressorContainer::add(Args &&... args)
	{
		m_decompressors.emplace_back(new T(args...));
	}

	/*!
	 *
	 * @param name The name of the decompressor to check whether exists.
	 * @return Whether a decompressor with the given name exists or not.
	 */
	bool DecompressorContainer::contains(std::string_view name) const
	{
		auto iter = std::find_if(m_decompressors.begin(), m_decompressors.end(), [&](const auto &item)
		{
			return item->name() == name;
		});

		return iter != m_decompressors.end();
	}

	/*!
	 * Removed an element with the given name.
	 * @param name The name of the decompressor
	 */
	void DecompressorContainer::remove(std::string_view name)
	{
		auto iter = std::remove_if(m_decompressors.begin(), m_decompressors.end(), [&](const auto &item)
		{
			return item->name() == name;
		});
		m_decompressors.erase(iter);
	}

	size_t DecompressorContainer::size() const
	{
		return m_decompressors.size();
	}

	/*!
	 *
	 * @param name The name of the container
	 * @return An ICompressor pointer if it exists. nullptr otherwise.
	 */
	IDecompressor *DecompressorContainer::get(std::string_view name)
	{
		auto iter = std::find_if(m_decompressors.begin(), m_decompressors.end(), [&](const auto &item)
		{
			return item->name() == name;
		});

		return (iter != m_decompressors.end()) ? iter->get() : nullptr;
	}

	/*!
	 * Check if container is empty
	 * @return Whether or not the container is empty
	 */
	bool DecompressorContainer::empty() const
	{
		return m_decompressors.empty();
	}

	/*!
	 * Clears all IDecompressor elements in the container
	 */
	void DecompressorContainer::clear()
	{
		m_decompressors.clear();
	}
}
#endif //TILESON_DECOMPRESSORCONTAINER_HPP

/*** End of inlined file: DecompressorContainer.hpp ***/


/*** Start of inlined file: Map.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_MAP_HPP
#define TILESON_MAP_HPP


/*** Start of inlined file: Color.hpp ***/
//
// Created by robin on 09.08.2019.
//

#ifndef TILESON_COLOR_HPP
#define TILESON_COLOR_HPP

#include <type_traits>
#include <cstdint>
#include <string>

namespace tson
{

	template<typename T>
	class Color
	{

		public:
			/*!
			 * Parses color from Tiled's own color format, which is #aarrggbb in hex format or optionally #rrggbb.
			 * @param color Color in "#rrggbbaa" hex format.
			 * @example "#ffaa07ff" and "#aa07ff". In cases where alpha is not a value, it is set to 255.
			*/
			inline explicit Color(const std::string &color)
			{
				parseHexString(color);
			}
			inline Color(T red, T green, T blue, T alpha);
			inline Color() { r = g = b = 0; a = 255; }

			inline bool operator==(const Color &rhs) const;
			inline bool operator==(const std::string &rhs) const;
			inline bool operator!=(const Color &rhs) const;

			inline Color<float> asFloat();
			inline Color<uint8_t> asInt();

			/*! Red */
			T r;
			/*! Green */
			T g;
			/*! Blue */
			T b;
			/*! Alpha */
			T a;

		private:
			void parseHexString(const std::string &color)
			{
				if constexpr (std::is_same<T, float>::value)
				{
					if (color.size() == 9)
					{
						a = (float) std::stoi(color.substr(1, 2), nullptr, 16) / 255;
						r = (float) std::stoi(color.substr(3, 2), nullptr, 16) / 255;
						g = (float) std::stoi(color.substr(5, 2), nullptr, 16) / 255;
						b = (float) std::stoi(color.substr(7, 2), nullptr, 16) / 255;
					}
					else if (color.size() == 7)
					{
						r = (float) std::stoi(color.substr(1, 2), nullptr, 16) / 255;
						g = (float) std::stoi(color.substr(3, 2), nullptr, 16) / 255;
						b = (float) std::stoi(color.substr(5, 2), nullptr, 16) / 255;
						a = 1.f;
					}
				}
				else
				{
					if (color.size() == 9)
					{
						a = std::stoi(color.substr(1, 2), nullptr, 16);
						r = std::stoi(color.substr(3, 2), nullptr, 16);
						g = std::stoi(color.substr(5, 2), nullptr, 16);
						b = std::stoi(color.substr(7, 2), nullptr, 16);
					}
					else if (color.size() == 7)
					{
						r = std::stoi(color.substr(1, 2), nullptr, 16);
						g = std::stoi(color.substr(3, 2), nullptr, 16);
						b = std::stoi(color.substr(5, 2), nullptr, 16);
						a = 255;
					}
				}
			}

	};

	typedef Color<uint8_t> Colori;
	typedef Color<float> Colorf;

	/*!
	 * Gets the Color as a float. Only useful if the template related to the current color is NOT float
	 * @tparam T The template type
	 * @return If the T type is float, the value will be returned as a copy of itself. Else: All values will be divided by 255
	 * before returning.
	 */
	template<typename T>
	tson::Colorf Color<T>::asFloat()
	{
		if constexpr (std::is_same<T, float>::value)
			*this;
		else
			return tson::Colorf((float) r / 255, (float) g / 255, (float) b / 255, (float) a / 255);
	}

	/*!
	 * Gets the Color as an 32-bit variable, where each channel is 8-bit.
	 * Only useful if the template related to the current color is NOT already 8-bit int
	 * @tparam T The template type
	 * @return If the T type is float, the value of each channel will be multiplied by 255. Else: The value will be returned as a copy of itself.
	 */
	template<typename T>
	tson::Colori Color<T>::asInt()
	{
		if constexpr (std::is_same<T, float>::value)
			return tson::Colori((float) r * 255, (float) g * 255, (float) b * 255, (float) a * 255);
		else
			*this;
	}

	/*!
	 * Create a new color in rgba (red, green, blue, alpha) format
	 * @tparam T the template type for each channel. Usually uint8_t (8-bit int) or float.
	 * @param red Red channel
	 * @param green Green channel
	 * @param blue Blue channel
	 * @param alpha Alpha channel
	 */
	template<typename T>
	Color<T>::Color(T red, T green, T blue, T alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	template<typename T>
	bool Color<T>::operator==(const std::string &rhs) const {
		Color other {rhs};
		return *this == other;
	}

	template<typename T>
	bool Color<T>::operator==(const Color &rhs) const
	{
		return r == rhs.r &&
			   g == rhs.g &&
			   b == rhs.b &&
			   a == rhs.a;
	}

	template<typename T>
	bool Color<T>::operator!=(const Color &rhs) const
	{
		return !(rhs == *this);
	}

}

#endif //TILESON_COLOR_HPP

/*** End of inlined file: Color.hpp ***/


/*** Start of inlined file: Vector2.hpp ***/
//
// Created by robin on 31.07.2019.
//

#ifndef TILESON_VECTOR2_HPP
#define TILESON_VECTOR2_HPP

namespace tson
{
	template<typename T>
	class Vector2
	{

		public:
			inline Vector2(T xPos, T yPos);
			inline Vector2() { x = y = 0; }

			inline bool operator==(const Vector2 &rhs) const;
			inline bool operator!=(const Vector2 &rhs) const;

			T x;
			T y;
	};

	/*!
	 *
	 * @tparam T template type
	 * @param xPos x-position
	 * @param yPos y-position
	 */
	template<typename T>
	Vector2<T>::Vector2(T xPos, T yPos)
	{
		x = xPos;
		y = yPos;
	}

	template<typename T>
	bool Vector2<T>::operator==(const Vector2 &rhs) const
	{
		return x == rhs.x &&
			   y == rhs.y;
	}

	template<typename T>
	bool Vector2<T>::operator!=(const Vector2 &rhs) const
	{
		return !(rhs == *this);
	}

	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;
}

#endif //TILESON_VECTOR2_HPP

/*** End of inlined file: Vector2.hpp ***/

//#include "../external/json.hpp"

/*** Start of inlined file: Layer.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_LAYER_HPP
#define TILESON_LAYER_HPP

#include <set>
//#include "../external/json.hpp"


/*** Start of inlined file: Chunk.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_CHUNK_HPP
#define TILESON_CHUNK_HPP

//#include "../external/json.hpp"

namespace tson
{
	class Chunk
	{
		public:
			inline Chunk() = default;
			inline explicit Chunk(const nlohmann::json &json);
			inline bool parse(const nlohmann::json &json);

			[[nodiscard]] inline const std::vector<int> &getData() const;
			[[nodiscard]] inline const std::string &getBase64Data() const;
			[[nodiscard]] inline const Vector2i &getSize() const;
			[[nodiscard]] inline const Vector2i &getPosition() const;

		private:
			std::vector<int> m_data;        /*! 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only. */
			std::string      m_base64Data;  /*! 'data' (when string): Array of unsigned int (GIDs) or base64-encoded data. */
			tson::Vector2i   m_size;        /*!  x='width' (in tiles) and y='height' (in tiles): */
			tson::Vector2i   m_position;    /*! 'x' and 'y' position in tiles */
	};
}

#endif //TILESON_CHUNK_HPP

/*!
 * Parses 'chunk' data from Tiled json and stores the values in this class
 * @param json json-data
 */
tson::Chunk::Chunk(const nlohmann::json &json)
{
	parse(json);
}

/*!
 * Parses 'chunk' data from Tiled json and stores the values in this class
 * @param json json-data
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Chunk::parse(const nlohmann::json &json)
{
	bool allFound = true;

	if(json.count("width") > 0 && json.count("height") > 0)
		m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
	if(json.count("x") > 0 && json.count("y") > 0)
		m_position = {json["x"].get<int>(), json["y"].get<int>()}; else allFound = false;

	//Handle DATA (Optional)
	if(json.count("data") > 0)
	{
		if(json["data"].is_array())
		{
			std::for_each(json["data"].begin(), json["data"].end(), [&](const nlohmann::json &item) { m_data.push_back(item.get<int>()); });
		}
		else
			m_base64Data = json["data"].get<std::string>();
	}

	return allFound;
}

/*!
 * 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
 * @return list of tile ids
 */
const std::vector<int> &tson::Chunk::getData() const
{
	return m_data;
}

/*!
 * 'data' (when string): Array of unsigned int (GIDs) or base64-encoded data.
 * @return base64 string
 */
const std::string &tson::Chunk::getBase64Data() const
{
	return m_base64Data;
}

/*!
 * x='width' (in tiles) and y='height' (in tiles).
 * @return Size (x and y), containing the values from the fields 'width' and 'height' in Tiled
 */
const tson::Vector2i &tson::Chunk::getSize() const
{
	return m_size;
}

/*!
 * 'x' and 'y' position in tiles
 * @return Position in int
 */
const tson::Vector2i &tson::Chunk::getPosition() const
{
	return m_position;
}
/*** End of inlined file: Chunk.hpp ***/


/*** Start of inlined file: Object.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_OBJECT_HPP
#define TILESON_OBJECT_HPP

//#include "../external/json.hpp"


/*** Start of inlined file: PropertyCollection.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_PROPERTYCOLLECTION_HPP
#define TILESON_PROPERTYCOLLECTION_HPP


/*** Start of inlined file: Property.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_PROPERTY_HPP
#define TILESON_PROPERTY_HPP

//#include "../../TilesonConfig.h"

//#if USE_CPP17_FILESYSTEM

#ifndef DISABLE_CPP17_FILESYSTEM
	#if _MSC_VER && !__INTEL_COMPILER
		#include <filesystem>
		namespace fs = std::filesystem;
	#elif __MINGW64__
		#if __MINGW64_VERSION_MAJOR > 6
			#include <filesystem>
			namespace fs = std::filesystem;
		#else
			#include <experimental/filesystem>
			namespace fs = std::experimental::filesystem;
		#endif
	#elif __clang__
		#if __clang_major__ < 8
			#include <experimental/filesystem>
			namespace fs = std::experimental::filesystem;
		#else
			#include <filesystem>
			namespace fs = std::filesystem;
		#endif
	#else //Linux
		#if __GNUC__ < 8 //GCC major version less than 8
			#include <experimental/filesystem>
			namespace fs = std::experimental::filesystem;
		#else
			#include <filesystem>
			namespace fs = std::filesystem;
		#endif
	#endif
#endif

#include <any>
#include <string>

/*** Start of inlined file: Enums.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_ENUMS_HPP
#define TILESON_ENUMS_HPP
#include <cstdint>

/*** Start of inlined file: EnumBitflags.hpp ***/
//
// Created by robin on 08.11.2020.
//

#ifndef TILESON_ENUMBITFLAGS_HPP
#define TILESON_ENUMBITFLAGS_HPP

#include <type_traits>
#include <iostream>

namespace tson
{
	#define ENABLE_BITMASK_OPERATORS(x)  \
	template<>                           \
	struct EnableBitMaskOperators<x>     \
	{                                    \
		static const bool enable = true; \
	};

	template<typename Enum>
	struct EnableBitMaskOperators
	{
		static const bool enable = false;
	};

	template<typename Enum>
	typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
	operator |(Enum lhs, Enum rhs)
	{
		static_assert(std::is_enum<Enum>::value,
					  "template parameter is not an enum type");

		using underlying = typename std::underlying_type<Enum>::type;

		return static_cast<Enum> (
				static_cast<underlying>(lhs) |
				static_cast<underlying>(rhs)
		);
	}

	//Permissions operator &(Permissions lhs, Permissions rhs)
	template<typename Enum>
	typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
	operator &(Enum lhs, Enum rhs)
	{
		static_assert(std::is_enum<Enum>::value,
					  "template parameter is not an enum type");

		using underlying = typename std::underlying_type<Enum>::type;

		return static_cast<Enum> (
				static_cast<underlying>(lhs) &
				static_cast<underlying>(rhs)
		);
	}

	//Permissions operator ^(Permissions lhs, Permissions rhs)
	template<typename Enum>
	typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
	operator ^(Enum lhs, Enum rhs)
	{
		static_assert(std::is_enum<Enum>::value,
					  "template parameter is not an enum type");

		using underlying = typename std::underlying_type<Enum>::type;

		return static_cast<Enum> (
				static_cast<underlying>(lhs) ^
				static_cast<underlying>(rhs)
		);
	}

	//Permissions operator ~(Permissions rhs)
	template<typename Enum>
	typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
	operator ~(Enum rhs)
	{
		static_assert(std::is_enum<Enum>::value,
					  "template parameter is not an enum type");

		using underlying = typename std::underlying_type<Enum>::type;

		return static_cast<Enum> (
				~static_cast<underlying>(rhs)
		);
	}

	//Permissions& operator |=(Permissions &lhs, Permissions rhs)
	template<typename Enum>
	typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
	&operator |=(Enum &lhs, Enum rhs)
	{
		static_assert(std::is_enum<Enum>::value,
					  "template parameter is not an enum type");

		using underlying = typename std::underlying_type<Enum>::type;

		lhs = static_cast<Enum> (
				static_cast<underlying>(lhs) |
				static_cast<underlying>(rhs)
		);

		return lhs;
	}

	//Permissions& operator &=(Permissions &lhs, Permissions rhs)
	template<typename Enum>
	typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
	&operator &=(Enum &lhs, Enum rhs)
	{
		static_assert(std::is_enum<Enum>::value,
					  "template parameter is not an enum type");

		using underlying = typename std::underlying_type<Enum>::type;

		lhs = static_cast<Enum> (
				static_cast<underlying>(lhs) &
				static_cast<underlying>(rhs)
		);

		return lhs;
	}

	//Permissions& operator ^=(Permissions &lhs, Permissions rhs)
	template<typename Enum>
	typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
	&operator ^=(Enum &lhs, Enum rhs)
	{
		static_assert(std::is_enum<Enum>::value,
					  "template parameter is not an enum type");

		using underlying = typename std::underlying_type<Enum>::type;

		lhs = static_cast<Enum> (
				static_cast<underlying>(lhs) ^
				static_cast<underlying>(rhs)
		);

		return lhs;
	}
}

#endif //TILESON_ENUMBITFLAGS_HPP

/*** End of inlined file: EnumBitflags.hpp ***/


namespace tson
{
	/*!
	 * Type used in Property.hpp
	 */
	enum class Type : uint8_t
	{
			Undefined = 0,
			Color = 1, /*! color */
			File = 2, /*! file */
			Int = 3, /*! int */
			Boolean = 4, /*! bool */
			Float = 5, /*! float */
			String = 6 /*! string */
	};

	/*!
	 * Layer.hpp - LayerType
	 * //'type': tilelayer, objectgroup, imagelayer or group
	 */
	enum class LayerType : uint8_t
	{
			Undefined = 0,
			TileLayer = 1,
			ObjectGroup = 2,
			ImageLayer = 3,
			Group = 4
	};

	/*!
	 * Map.hpp - ParseStatus
	 */
	enum class ParseStatus : uint8_t
	{
			OK = 0, //OK unless otherwise stated
			FileNotFound = 1,
			ParseError = 2,
			MissingData = 3
	};

	/*!
	 * Object.hpp - ObjectType
	 */
	enum class ObjectType : uint8_t
	{
			Undefined = 0,
			Object = 1,
			Ellipse = 2,
			Rectangle = 3,
			Point = 4,
			Polygon = 5,
			Polyline = 6,
			Text = 7,
			Template = 8
	};

	static constexpr uint32_t FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	static constexpr uint32_t FLIPPED_VERTICALLY_FLAG   = 0x40000000;
	static constexpr uint32_t FLIPPED_DIAGONALLY_FLAG   = 0x20000000;
	/*!
	 * Object.hpp - ObjectFlipFlags
	 */
	enum class TileFlipFlags : uint32_t
	{
			None = 0,
			Diagonally = FLIPPED_DIAGONALLY_FLAG,
			Vertically = FLIPPED_VERTICALLY_FLAG,
			Horizontally = FLIPPED_HORIZONTALLY_FLAG
	};

	/*!
	 * Tileset.hpp - ObjectAlignment
	 */
	enum class ObjectAlignment : uint8_t
	{
			Unspecified = 0,    //unspecified
			TopLeft = 1,        //topleft
			Top = 2,            //top
			TopRight = 3,       //topright
			Left = 4,           //left
			Center = 5,         //center
			Right = 6,          //right
			BottomLeft = 7,     //bottomleft
			Bottom = 8,         //bottom
			BottomRight = 9     //bottomright
	};

	ENABLE_BITMASK_OPERATORS(TileFlipFlags)
}

#endif //TILESON_ENUMS_HPP

/*** End of inlined file: Enums.hpp ***/


//#include "../external/json.hpp"

namespace tson
{
	class Property
	{
		public:

			//enum class Type : uint8_t
			//{
			//        Undefined = 0,
			//        Color = 1, /*! color */
			//        File = 2, /*! file */
			//        Int = 3, /*! int */
			//        Boolean = 4, /*! bool */
			//        Float = 5, /*! float */
			//        String = 6 /*! string */
			//};

			inline Property();
			inline Property(const nlohmann::json &json);
			inline Property(std::string name, std::any value, Type type);

			inline void setValue(const std::any &value);
			inline void setStrValue(const std::string &value);
			inline void setName(const std::string &name);

			[[nodiscard]] inline const std::type_info& getValueType() const;
			inline std::string getValueTypeInfo();
			[[nodiscard]]inline const std::any &getValue() const;
			template <typename T>
			inline T getValue() const;
			[[nodiscard]] inline const std::string &getName() const;
			[[nodiscard]] inline Type getType() const;

		protected:
			inline void setTypeByString(const std::string &str);
			inline void setValueByType(const nlohmann::json &json);

			Type m_type = Type::Undefined;
			std::string m_name;
			std::any m_value; //Using std::any to assign any type
	};

	template<typename T>
	T Property::getValue() const
	{
		bool isCorrectType = (m_value.type() == typeid(T));

		if(isCorrectType)
		{
			T value = std::any_cast<T>(m_value);
			return value;
		}
		else
		{
			static T defaultValue;
			return defaultValue;
		}
	}
}

tson::Property::Property() : m_name {"unnamed"}
{

}

tson::Property::Property(const nlohmann::json &json)
{
	setTypeByString(json["type"].get<std::string>());
	setValueByType(json["value"]);
	m_name = json["name"].get<std::string>();
}

tson::Property::Property(std::string name, std::any value, Type type) : m_name { move(name) }, m_value { move(value) }, m_type {type}
{

}

void tson::Property::setValue(const std::any &value)
{
	m_value = value;
}

/*!
 * Sets the value specifically as string.
 * When not specified as std::string, the default is that the value will be set as char * when adding a value like "test"
 * This function is to make sure the value is added as string.
 * @param value
 */
void tson::Property::setStrValue(const std::string &value)
{
	m_value = value;
}

const std::any &tson::Property::getValue() const
{
	return m_value;
}

void tson::Property::setName(const std::string &name)
{
	m_name = name;
}

const std::string &tson::Property::getName() const
{
	return m_name;
}

/*!
 * Gets the value type as std::value_info.
 * This can easily be compared to types like this:
 * Check if int: getValueType() == typeid(int)
 * @return
 */

const std::type_info &tson::Property::getValueType() const
{
	return m_value.type();
}

/*!
 * Gets the value type as std::string
 * Examples of known types:
 * "i" = int
 * "f" = float
 * "b" = bool
 * @return
 */
std::string tson::Property::getValueTypeInfo()
{
	return m_value.type().name();
}

tson::Type tson::Property::getType() const
{
	return m_type;
}

void tson::Property::setTypeByString(const std::string &str)
{
	if(str == "color")
		m_type = tson::Type::Color;
	else if(str == "file")
		m_type = tson::Type::File;
	else if(str == "int")
		m_type = tson::Type::Int;
	else if(str == "bool")
		m_type = tson::Type::Boolean;
	else if(str == "float")
		m_type = tson::Type::Float;
	else if(str == "string")
		m_type = tson::Type::String;
	else
		m_type = tson::Type::Undefined;
}

void tson::Property::setValueByType(const nlohmann::json &json)
{
	switch(m_type)
	{
		case Type::Color:
			m_value = Colori(json.get<std::string>());
			break;

		case Type::File:
			#ifndef DISABLE_CPP17_FILESYSTEM
			m_value = fs::path(json.get<std::string>());
			#else
			m_value = json.get<std::string>();
			#endif
			break;

		case Type::Int:
			m_value = json.get<int>();
			break;

		case Type::Boolean:
			m_value = json.get<bool>();
			break;

		case Type::Float:
			m_value = json.get<float>();
			break;

		case Type::String:
			setStrValue(json.get<std::string>());
			break;

		default:
			setStrValue(json.get<std::string>());
			break;

	}
}

#endif //TILESON_PROPERTY_HPP

/*** End of inlined file: Property.hpp ***/

//#include "../external/json.hpp"
#include <map>

namespace tson
{
	class PropertyCollection
	{
		public:
			inline PropertyCollection() = default;

			inline explicit PropertyCollection(std::string id);

			inline tson::Property * add(const tson::Property &property);
			inline tson::Property * add(const nlohmann::json &json);
			inline tson::Property * add(const std::string &name, const std::any &value, tson::Type type);

			inline void remove(const std::string &name);

			inline void setValue(const std::string &name, const std::any &value);
			inline void setId(const std::string &id);

			inline bool hasProperty(const std::string &name);
			inline tson::Property * getProperty(const std::string &name);
			inline std::map<std::string, Property> &getProperties();
			inline std::vector<Property*> get();
			template <typename T>
			inline T getValue(const std::string &name);
			[[nodiscard]] inline const std::string &getId() const;
			[[nodiscard]] inline size_t getSize() const;

		protected:
			std::string m_id;
			std::map<std::string, tson::Property> m_properties;
	};
}

template<typename T>
T tson::PropertyCollection::getValue(const std::string &name)
{
	static T defaultT;
	return (m_properties.count(name) > 0) ? m_properties[name].getValue<T>() : defaultT;
}

tson::PropertyCollection::PropertyCollection(std::string id) : m_id {std::move(id)}
{

}

tson::Property *tson::PropertyCollection::add(const tson::Property &property)
{
	m_properties[property.getName()] = property;
	return &m_properties[property.getName()];
}

tson::Property *tson::PropertyCollection::add(const nlohmann::json &json)
{
	tson::Property property = tson::Property(json);
	std::string name = property.getName();
	m_properties[name] = std::move(property);
	return &m_properties[name];
}

tson::Property *tson::PropertyCollection::add(const std::string &name, const std::any &value, tson::Type type)
{
	m_properties[name] = {name, value, type};
	return &m_properties[name];
}

void tson::PropertyCollection::remove(const std::string &name)
{
	m_properties.erase(name);
}

/*!
 * Sets a value IF the property already exists. Does nothing otherwise.
 * See add() for adding new properties
 * @param name
 * @param value
 */
void tson::PropertyCollection::setValue(const std::string &name, const std::any &value)
{
	if(m_properties.count(name) > 0)
		m_properties[name].setValue(value);
}

void tson::PropertyCollection::setId(const std::string &id)
{
	m_id = id;
}

bool tson::PropertyCollection::hasProperty(const std::string &name)
{
	return m_properties.count(name) > 0;
}

tson::Property *tson::PropertyCollection::getProperty(const std::string &name)
{
	return (m_properties.count(name) > 0) ? &m_properties[name] : nullptr;
}

std::map<std::string, tson::Property> &tson::PropertyCollection::getProperties()
{
	return m_properties;
}

/*!
 * Gets vector of pointers to all the existing properties
 * @return
 */
std::vector<tson::Property *> tson::PropertyCollection::get()
{
	std::vector<tson::Property *> props;
	for(auto &i : m_properties)
		props.emplace_back(&i.second);

	return props;
}

const std::string &tson::PropertyCollection::getId() const
{
	return m_id;
}

size_t tson::PropertyCollection::getSize() const
{
	return m_properties.size();
}

#endif //TILESON_PROPERTYCOLLECTION_HPP

/*** End of inlined file: PropertyCollection.hpp ***/


/*** Start of inlined file: Text.hpp ***/
//
// Created by robin on 05.08.2019.
//

#ifndef TILESON_TEXT_HPP
#define TILESON_TEXT_HPP

#include <string>

namespace tson
{
	class Text
	{
		public:
			inline Text() = default;
			/*!
			 *
			 * @param _text Text
			 * @param _wrap If the text is marked as wrapped
			 */
			inline Text(std::string _text, bool _wrap, tson::Colori _color) : text {std::move(_text)}, wrap {_wrap}, color {_color} {};
			//Just make it simple
			std::string text;
			tson::Colori color;
			bool wrap{};
	};
}

#endif //TILESON_TEXT_HPP

/*** End of inlined file: Text.hpp ***/

namespace tson
{
	class Object
	{
		public:
			//enum class Type : uint8_t
			//{
			//        Undefined = 0,
			//        Object = 1,
			//        Ellipse = 2,
			//        Rectangle = 3,
			//        Point = 4,
			//        Polygon = 5,
			//        Polyline = 6,
			//        Text = 7,
			//        Template = 8
			//};

			inline Object() = default;
			inline explicit Object(const nlohmann::json &json);
			inline bool parse(const nlohmann::json &json);

			[[nodiscard]] inline ObjectType getObjectType() const;
			[[nodiscard]] inline bool isEllipse() const;
			[[nodiscard]] inline uint32_t getGid() const;
			[[nodiscard]] inline const Vector2i &getSize() const;
			[[nodiscard]] inline int getId() const;
			[[nodiscard]] inline const std::string &getName() const;
			[[nodiscard]] inline bool isPoint() const;
			[[nodiscard]] inline float getRotation() const;
			[[nodiscard]] inline const std::string &getTemplate() const;
			[[nodiscard]] inline const std::string &getType() const;
			[[nodiscard]] inline bool isVisible() const;
			[[nodiscard]] inline const Vector2i &getPosition() const;

			[[nodiscard]] inline const std::vector<tson::Vector2i> &getPolygons() const;
			[[nodiscard]] inline const std::vector<tson::Vector2i> &getPolylines() const;
			[[nodiscard]] inline PropertyCollection &getProperties();
			[[nodiscard]] inline const Text &getText() const;

			template <typename T>
			inline T get(const std::string &name);
			inline tson::Property * getProp(const std::string &name);

			//v1.2.0-stuff
			[[nodiscard]] inline TileFlipFlags getFlipFlags() const;
			inline bool hasFlipFlags(TileFlipFlags flags);

		private:
			inline void setObjectTypeByJson(const nlohmann::json &json);

			ObjectType                        m_objectType = ObjectType::Undefined;    /*! Says with object type this is */
			bool                              m_ellipse {};                            /*! 'ellipse': Used to mark an object as an ellipse */
			uint32_t                          m_gid {};                                /*! 'gid': GID, only if object comes from a Tilemap */
			tson::Vector2i                    m_size;                                  /*! x = 'width' (Width in pixels), y = 'height' (Height in pixels). Ignored if using a gid.)*/
			int                               m_id{};                                  /*! 'id': Incremental id - unique across all objects */
			std::string                       m_name;                                  /*! 'name':  String assigned to name field in editor*/
			bool                              m_point {};                              /*! 'point': Used to mark an object as a point */
			std::vector<tson::Vector2i>       m_polygon; 	                           /*! 'polygon': A list of x,y coordinates in pixels */
			std::vector<tson::Vector2i>       m_polyline; 	                           /*! 'polyline': A list of x,y coordinates in pixels */
			tson::PropertyCollection          m_properties; 	                       /*! 'properties': A list of properties (name, value, type). */
			float                             m_rotation {};                           /*! 'rotation': Angle in degrees clockwise */
			std::string                       m_template;                              /*! 'template': Reference to a template file, in case object is a template instance */
			tson::Text                        m_text; 	                               /*! first: 'text' second: 'wrap' */
			std::string                       m_type;                                  /*! 'type': String assigned to type field in editor */
			bool                              m_visible {};                            /*! 'visible': Whether object is shown in editor. */
			tson::Vector2i                    m_position;                              /*! 'x' and 'y': coordinate in pixels */

			//v1.2.0-stuff
			tson::TileFlipFlags               m_flipFlags = TileFlipFlags::None;       /*! Resolved using bit 32, 31 and 30 from gid */
	};

	/*!
	 * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
	 * @tparam T The template value
	 * @param name Name of the property
	 * @return The actual value, if it exists. Otherwise: The default value of the type.
	 */
	template<typename T>
	T tson::Object::get(const std::string &name)
	{
		return m_properties.getValue<T>(name);
	}
}

/*!
 * Parses a json Tiled object
 * @param json
 */
tson::Object::Object(const nlohmann::json &json)
{
	parse(json);
}

/*!
 * Parses a json Tiled object and autoamtically determines the object type based on the data presented.
 * Call getObjectType() to see what object type it is.
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Object::parse(const nlohmann::json &json)
{
	bool allFound = true;

	if(json.count("ellipse") > 0) m_ellipse = json["ellipse"].get<bool>(); //Optional
	if(json.count("gid") > 0)
	{
		uint32_t gid = json["gid"].get<uint32_t>(); //Optional
		if (gid & FLIPPED_HORIZONTALLY_FLAG) m_flipFlags |= TileFlipFlags::Horizontally;
		if (gid & FLIPPED_VERTICALLY_FLAG) m_flipFlags |= TileFlipFlags::Vertically;
		if (gid & FLIPPED_DIAGONALLY_FLAG) m_flipFlags |= TileFlipFlags::Diagonally;

		// Clear flags
		gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

		m_gid = gid;
	}
	if(json.count("id") > 0) m_id = json["id"].get<int>(); else allFound = false;
	if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
	if(json.count("point") > 0) m_point = json["point"].get<bool>(); //Optional
	if(json.count("rotation") > 0) m_rotation = json["rotation"].get<float>(); else allFound = false;
	if(json.count("template") > 0) m_template = json["template"].get<std::string>(); //Optional
	if(json.count("type") > 0) m_type = json["type"].get<std::string>(); else allFound = false;
	if(json.count("visible") > 0) m_visible = json["visible"].get<bool>(); else allFound = false;

	if(json.count("width") > 0 && json.count("height") > 0)
		m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
	if(json.count("x") > 0 && json.count("y") > 0)
		m_position = {json["x"].get<int>(), json["y"].get<int>()}; else allFound = false;

	if(json.count("text") > 0)
	{
		bool hasColor = json["text"].count("color") > 0;
		tson::Color c = (hasColor) ? tson::Colori(json["text"]["color"].get<std::string>()) : tson::Colori();
		m_text = {json["text"]["text"].get<std::string>(), json["text"]["wrap"].get<bool>(), c}; //Optional
	}

	setObjectTypeByJson(json);

	if(m_objectType == ObjectType::Template)
		allFound = true; //Just accept anything with this type

	//More advanced data
	if(json.count("polygon") > 0 && json["polygon"].is_array())
		std::for_each(json["polygon"].begin(), json["polygon"].end(),
					  [&](const nlohmann::json &item) { m_polygon.emplace_back(item["x"].get<int>(), item["y"].get<int>()); });

	if(json.count("polyline") > 0 && json["polyline"].is_array())
		std::for_each(json["polyline"].begin(), json["polyline"].end(),
					  [&](const nlohmann::json &item) { m_polyline.emplace_back(item["x"].get<int>(), item["y"].get<int>()); });

	if(json.count("properties") > 0 && json["properties"].is_array())
		std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

	return allFound;
}

/*!
 * Sets an object type based on json data.
 * @param json
 */
void tson::Object::setObjectTypeByJson(const nlohmann::json &json)
{
	m_objectType = ObjectType::Undefined;
	if(m_ellipse)
		m_objectType = ObjectType::Ellipse;
	else if(m_point)
		m_objectType = ObjectType::Point;
	else if(json.count("polygon") > 0)
		m_objectType = ObjectType::Polygon;
	else if(json.count("polyline") > 0)
		m_objectType = ObjectType::Polyline;
	else if(json.count("text") > 0)
		m_objectType = ObjectType::Text;
	else if(json.count("gid") > 0)
		m_objectType = ObjectType::Object;
	else if(json.count("template") > 0)
		m_objectType = ObjectType::Template;
	else
		m_objectType = ObjectType::Rectangle;
}

/*!
 * Gets what type of object this is.
 * @return
 */

tson::ObjectType tson::Object::getObjectType() const
{
	return m_objectType;
}

/*!
 * 'ellipse': Used to mark an object as an ellipse
 * @return
 */
bool tson::Object::isEllipse() const
{
	return m_ellipse;
}

/*!
 * 'gid': GID, only if object comes from a Tilemap
 * @return
 */
uint32_t tson::Object::getGid() const
{
	return m_gid;
}

/*!
 * x = 'width' (Width in pixels), y = 'height' (Height in pixels). Ignored if using a gid.)
 * @return
 */
const tson::Vector2i &tson::Object::getSize() const
{
	return m_size;
}

/*!
 * 'id': Incremental id - unique across all objects
 * @return
 */
int tson::Object::getId() const
{
	return m_id;
}

/*!
 * 'name': String assigned to name field in editor
 * @return
 */
const std::string &tson::Object::getName() const
{
	return m_name;
}

/*!
 * 'point': Used to mark an object as a point
 * @return true if the object is of type point
 */
bool tson::Object::isPoint() const
{
	return m_point;
}

/*!
 * 'rotation': Angle in degrees clockwise
 * @return
 */
float tson::Object::getRotation() const
{
	return m_rotation;
}

/*!
 * 'template': Reference to a template file, in case object is a template instance
 * @return
 */
const std::string &tson::Object::getTemplate() const
{
	return m_template;
}

/*!
 * 'type': String assigned to type field in editor
 * @return
 */
const std::string &tson::Object::getType() const
{
	return m_type;
}

/*!
 * 'visible': Whether object is shown in editor.
 * @return
 */
bool tson::Object::isVisible() const
{
	return m_visible;
}

/*!
 * 'x' and 'y': coordinate in pixels
 * @return
 */
const tson::Vector2i &tson::Object::getPosition() const
{
	return m_position;
}

/*!
 * 'polygon': A list of x,y coordinates in pixels.
 * If this is a Polygon type, this function will return the points used to create it
 * @return
 */
const std::vector<tson::Vector2i> &tson::Object::getPolygons() const
{
	return m_polygon;
}

/*!
 * 'polyline': A list of x,y coordinates in pixels
 * If this is a Polyline type, this function will return the points used to create it
 * @return
 */
const std::vector<tson::Vector2i> &tson::Object::getPolylines() const
{
	return m_polyline;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Object::getProperties()
{
	return m_properties;
}

/*!
 * 'type': String assigned to type field in editor
 * @return
 */
const tson::Text &tson::Object::getText() const
{
	return m_text;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Object::getProp(const std::string &name)
{
	if(m_properties.hasProperty(name))
		return m_properties.getProperty(name);
	return nullptr;
}

/*!
 * Get all flip flags
 * @return
 */
tson::TileFlipFlags tson::Object::getFlipFlags() const
{
	return m_flipFlags;
}

/*!
 *
 * @param flags Which flags to check for. Several flags can be checked at once using the bitwise or operator.
 * Example:
 * hasFlipFlags(TileFlipFlags::Vertically | TileFlipFlags::Horizontally)
 *
 * @return true if the flag(s) specified are set
 */
bool tson::Object::hasFlipFlags(TileFlipFlags flags)
{
	return ((m_flipFlags & flags) == flags) ? true : false;
}

#endif //TILESON_OBJECT_HPP

/*** End of inlined file: Object.hpp ***/


/*** Start of inlined file: TileObject.hpp ***/
//
// Created by robin on 26.07.2020.
//

#ifndef TILESON_TILEOBJECT_HPP
#define TILESON_TILEOBJECT_HPP


/*** Start of inlined file: Rect.hpp ***/
//
// Created by robin on 24.07.2020.
//

#ifndef TILESON_RECT_HPP
#define TILESON_RECT_HPP

namespace tson
{
	class Rect
	{
		public:

			inline Rect();
			inline Rect(int x_, int y_, int width_, int height_);

			inline bool operator==(const Rect &rhs) const;
			inline bool operator!=(const Rect &rhs) const;

			int x;
			int y;
			int width;
			int height;
	};

	Rect::Rect()
	{

	}

	Rect::Rect(int x_, int y_, int width_, int height_)
	{
		x = x_;
		y = y_;
		width = width_;
		height = height_;
	}

	bool Rect::operator==(const Rect &rhs) const
	{
		return x == rhs.x &&
			   y == rhs.y &&
			   width == rhs.width &&
			   height == rhs.height;
	}

	bool Rect::operator!=(const Rect &rhs) const
	{
		return !(rhs == *this);
	}
}

#endif //TILESON_RECT_HPP

/*** End of inlined file: Rect.hpp ***/

namespace tson
{
	class Tile;
	class TileObject
	{
		public:
			inline TileObject() = default;
			inline TileObject(const std::tuple<int, int> &posInTileUnits, tson::Tile *tile);

			inline void initialize(const std::tuple<int, int> &posInTileUnits, tson::Tile *tile); //Defined in tileson_forward.hpp

			inline Tile *getTile() const;
			inline const Vector2i &getPositionInTileUnits() const;
			inline const Vector2f &getPosition() const;
			inline const tson::Rect &getDrawingRect() const; //Defined in tileson_forward.hpp

		private:
			tson::Tile *m_tile;
			tson::Vector2i m_posInTileUnits;
			tson::Vector2f m_position;

	};

	TileObject::TileObject(const std::tuple<int, int> &posInTileUnits, tson::Tile *tile)
	{
		initialize(posInTileUnits, tile);
	}

	/*!
	 * Get a pointer to the related tile
	 * @return
	 */
	Tile *TileObject::getTile() const
	{
		return m_tile;
	}

	/*!
	 * Gets the position of the tile in tile units
	 * @return
	 */
	const Vector2i &TileObject::getPositionInTileUnits() const
	{
		return m_posInTileUnits;
	}

	/*!
	 * Gets the position of the tile in pixels.
	 * @return
	 */
	const Vector2f &TileObject::getPosition() const
	{
		return m_position;
	}
}

#endif //TILESON_TILEOBJECT_HPP

/*** End of inlined file: TileObject.hpp ***/


/*** Start of inlined file: FlaggedTile.hpp ***/
//
// Created by robin on 13.11.2020.
//

#ifndef TILESON_FLAGGEDTILE_HPP
#define TILESON_FLAGGEDTILE_HPP

namespace tson
{
	class FlaggedTile
	{

		public:
			FlaggedTile(size_t x_, size_t y_, uint32_t id_, uint32_t tileId_) : x {x_}, y {y_}, id {id_}, tileId {tileId_}
			{

			}
			size_t x;
			size_t y;
			/*! Full ID, including flag */
			uint32_t id;
			/*! ID of the flagged tile */
			uint32_t tileId;
	};
}
#endif //TILESON_FLAGGEDTILE_HPP

/*** End of inlined file: FlaggedTile.hpp ***/

namespace tson
{
	class Tile;
	class Map;

	class Layer
	{
		public:
			inline Layer() = default;
			inline Layer(const nlohmann::json &json, tson::Map *map);
			inline bool parse(const nlohmann::json &json, tson::Map *map);

			[[nodiscard]] inline const std::string &getCompression() const;
			[[nodiscard]] inline const std::vector<uint32_t> &getData() const;
			[[nodiscard]] inline const std::string &getBase64Data() const;
			[[nodiscard]] inline const std::string &getDrawOrder() const;
			[[nodiscard]] inline const std::string &getEncoding() const;
			[[nodiscard]] inline int getId() const;
			[[nodiscard]] inline const std::string &getImage() const;
			[[nodiscard]] inline const std::string &getName() const;
			[[nodiscard]] inline const Vector2f &getOffset() const;
			[[nodiscard]] inline float getOpacity() const;
			[[nodiscard]] inline const Vector2i &getSize() const;
			[[nodiscard]] inline const Colori &getTransparentcolor() const;

			[[nodiscard]] inline LayerType getType() const;

			[[nodiscard]] inline const std::string &getTypeStr() const;
			[[nodiscard]] inline bool isVisible() const;
			[[nodiscard]] inline int getX() const;
			[[nodiscard]] inline int getY() const;

			[[nodiscard]] inline std::vector<tson::Chunk> &getChunks();
			[[nodiscard]] inline std::vector<tson::Layer> &getLayers();
			[[nodiscard]] inline std::vector<tson::Object> &getObjects();
			[[nodiscard]] inline PropertyCollection &getProperties();

			inline tson::Object *getObj(int id);
			inline tson::Object *firstObj(const std::string &name);
			inline std::vector<tson::Object> getObjectsByName(const std::string &name);
			inline std::vector<tson::Object> getObjectsByType(tson::ObjectType type);

			template <typename T>
			inline T get(const std::string &name);
			inline tson::Property * getProp(const std::string &name);

			inline void assignTileMap(std::map<uint32_t, tson::Tile*> *tileMap);
			inline void createTileData(const Vector2i &mapSize, bool isInfiniteMap);

			[[nodiscard]] inline const std::map<std::tuple<int, int>, tson::Tile *> &getTileData() const;
			inline tson::Tile * getTileData(int x, int y);

			//v1.2.0-stuff
			[[nodiscard]] inline const Colori &getTintColor() const;
			[[nodiscard]] inline tson::Map *getMap() const;

			[[nodiscard]] inline const std::map<std::tuple<int, int>, tson::TileObject> &getTileObjects() const;
			inline tson::TileObject * getTileObject(int x, int y);
			[[nodiscard]] inline const std::set<uint32_t> &getUniqueFlaggedTiles() const;
			inline void resolveFlaggedTiles();

		private:
			inline void setTypeByString();

			std::vector<tson::Chunk>                       m_chunks; 	                      /*! 'chunks': Array of chunks (optional). tilelayer only. */
			std::string                                    m_compression;                     /*! 'compression': zlib, gzip or empty (default). tilelayer only. */
			std::vector<uint32_t>                          m_data;                            /*! 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded
																							   *   data. tilelayer only. */
			std::string                                    m_base64Data;                      /*! 'data' (when string):     Array of unsigned int (GIDs) or base64-encoded
																							   *   data. tilelayer only. */
			std::string                                    m_drawOrder;                       /*! 'draworder': topdown (default) or index. objectgroup only. */
			std::string                                    m_encoding;                        /*! 'encoding': csv (default) or base64. tilelayer only. */
			int                                            m_id{};                            /*! 'id': Incremental id - unique across all layers */
			std::string                                    m_image;                           /*! 'image': Image used by this layer. imagelayer only. */
			std::vector<tson::Layer>                       m_layers; 	                      /*! 'layers': Array of layers. group on */
			std::string                                    m_name;                            /*! 'name': Name assigned to this layer */
			std::vector<tson::Object>                      m_objects;                         /*! 'objects': Array of objects. objectgroup only. */
			tson::Vector2f                                 m_offset;                          /*! 'offsetx' and 'offsety': Horizontal and Vertical layer offset in pixels
																							   *  (default: {0, 0}) */
			float                                          m_opacity{};                       /*! 'opacity': Value between 0 and 1 */
			tson::PropertyCollection                       m_properties; 	                  /*! 'properties': A list of properties (name, value, type). */
			tson::Vector2i                                 m_size;                            /*! x = 'width': (Column count. Same as map width for fixed-size maps.)
																								  y = 'height': Row count. Same as map height for fixed-size maps. */
			tson::Colori                                   m_transparentcolor;                /*! 'transparentcolor': Hex-formatted color (#RRGGBB) (optional, imagelayer only */
			std::string                                    m_typeStr;                         /*! 'type': tilelayer, objectgroup, imagelayer or group */
			LayerType                                      m_type {LayerType::Undefined};     /*! Layer type as enum*/
			bool                                           m_visible{};                       /*! 'visible': Whether layer is shown or hidden in editor */
			int                                            m_x{};                             /*! 'x': Horizontal layer offset in tiles. Always 0. */
			int                                            m_y{};                             /*! 'y': Vertical layer offset in tiles. Always 0. */

			std::map<uint32_t, tson::Tile*>                *m_tileMap;
			std::map<std::tuple<int, int>, tson::Tile*>    m_tileData;                        /*! Key: Tuple of x and y pos in tile units. */

			//v1.2.0-stuff
			tson::Colori                                        m_tintcolor;                  /*! 'tintcolor': Hex-formatted color (#RRGGBB or #AARRGGBB) that is multiplied with
																							   *        any graphics drawn by this layer or any child layers (optional). */
			inline void decompressData();                                                     /*! Defined in tileson_forward.hpp */
			inline void queueFlaggedTile(size_t x, size_t y, uint32_t id);                    /*! Queue a flagged tile */

			tson::Map *                                         m_map;                        /*! The map who owns this layer */
			std::map<std::tuple<int, int>, tson::TileObject>    m_tileObjects;
			std::set<uint32_t>                                  m_uniqueFlaggedTiles;
			std::vector<tson::FlaggedTile>                      m_flaggedTiles;

	};

	/*!
	 * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
	 * @tparam T The template value
	 * @param name Name of the property
	 * @return The actual value, if it exists. Otherwise: The default value of the type.
	 */
	template<typename T>
	T Layer::get(const std::string &name)
	{
		return m_properties.getValue<T>(name);
	}
}

/*!
 * Parses a Tiled layer from json
 * @param json
 */
tson::Layer::Layer(const nlohmann::json &json, tson::Map *map)
{
	parse(json, map);
}

void tson::Layer::queueFlaggedTile(size_t x, size_t y, uint32_t id)
{
	uint32_t tileId = id;
	tileId &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
	m_uniqueFlaggedTiles.insert(id);
	m_flaggedTiles.emplace_back(x, y, id, tileId);
}

/*!
 * Parses a Tiled layer from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Layer::parse(const nlohmann::json &json, tson::Map *map)
{
	m_map = map;

	bool allFound = true;
	if(json.count("tintcolor") > 0) m_tintcolor = tson::Colori(json["tintcolor"].get<std::string>()); //Optional
	if(json.count("compression") > 0) m_compression = json["compression"].get<std::string>(); //Optional
	if(json.count("draworder") > 0) m_drawOrder = json["draworder"].get<std::string>(); //Optional
	if(json.count("encoding") > 0) m_encoding = json["encoding"].get<std::string>(); //Optional
	if(json.count("id") > 0) m_id = json["id"].get<int>(); //Optional
	if(json.count("image") > 0) m_image = json["image"].get<std::string>(); //Optional
	if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
	if(json.count("offsetx") > 0 && json.count("offsety") > 0)
		m_offset = {json["offsetx"].get<float>(), json["offsety"].get<float>()}; //Optional
	if(json.count("opacity") > 0) m_opacity = json["opacity"].get<float>(); else allFound = false;
	if(json.count("width") > 0 && json.count("height") > 0)
		m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
	if(json.count("transparentcolor") > 0) m_transparentcolor = tson::Colori(json["transparentcolor"].get<std::string>()); //Optional
	if(json.count("type") > 0) m_typeStr = json["type"].get<std::string>(); else allFound = false;
	if(json.count("visible") > 0) m_visible = json["visible"].get<bool>(); else allFound = false;
	if(json.count("x") > 0) m_x = json["x"].get<int>(); else allFound = false;
	if(json.count("y") > 0) m_y = json["y"].get<int>(); else allFound = false;

	//Handle DATA (Optional)
	if(json.count("data") > 0)
	{
		if(json["data"].is_array())
		{
			std::for_each(json["data"].begin(), json["data"].end(), [&](const nlohmann::json &item) { m_data.push_back(item.get<uint32_t>()); });
		}
		else
		{
			m_base64Data = json["data"].get<std::string>();
			decompressData();
		}
	}

	//More advanced data
	if(json.count("chunks") > 0 && json["chunks"].is_array())
		std::for_each(json["chunks"].begin(), json["chunks"].end(), [&](const nlohmann::json &item) { m_chunks.emplace_back(item); });
	if(json.count("layers") > 0 && json["layers"].is_array())
		std::for_each(json["layers"].begin(), json["layers"].end(), [&](const nlohmann::json &item) { m_layers.emplace_back(item, m_map); });
	if(json.count("objects") > 0 && json["objects"].is_array())
		std::for_each(json["objects"].begin(), json["objects"].end(), [&](const nlohmann::json &item) { m_objects.emplace_back(item); });
	if(json.count("properties") > 0 && json["properties"].is_array())
		std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

	setTypeByString();

	return allFound;
}

/*!
 * Copies all objects with a name that equals the parameter
 * @param name Name of the objects to return
 * @return All objects with a matching name
 */
std::vector<tson::Object> tson::Layer::getObjectsByName(const std::string &name)
{
	std::vector<tson::Object> found;

	std::copy_if(m_objects.begin(), m_objects.end(), std::back_inserter(found), [&](const tson::Object &item)
	{
		return item.getName() == name;
	});

	return found;
}

/*!
 * Copies all objects with a type that equals the parameter
 * @param type LayerType of the objects to return
 * @return All objects with a matching type
 */
std::vector<tson::Object> tson::Layer::getObjectsByType(tson::ObjectType type)
{
	std::vector<tson::Object> found;

	std::copy_if(m_objects.begin(), m_objects.end(), std::back_inserter(found), [&](const tson::Object &item)
	{
		return item.getObjectType() == type;
	});

	return found;
}

/*!
 * Returns the first object with the given name
 * @param name Name of the object to find.
 * @return A pointer to the object if found. nullptr otherwise.
 */
tson::Object *tson::Layer::firstObj(const std::string &name)
{
	auto result = std::find_if(m_objects.begin(), m_objects.end(), [&](const tson::Object &obj){return obj.getName() == name; });
	if(result == m_objects.end())
		return nullptr;

	return &result.operator*();
}

/*!
 * Get an object by ID
 * @param id Unique ID of the object
 * @return A pointer to the object if found. nullptr otherwise.
 */
tson::Object *tson::Layer::getObj(int id)
{
	auto result = std::find_if(m_objects.begin(), m_objects.end(), [&](const tson::Object &obj){return obj.getId() == id; });
	if(result == m_objects.end())
		return nullptr;

	return &result.operator*();
}

/*!
 * Set type by string
 * tilelayer, objectgroup, imagelayer or group
 */
void tson::Layer::setTypeByString()
{
	if(m_typeStr == "tilelayer") m_type = LayerType::TileLayer;
	else if(m_typeStr == "objectgroup") m_type = LayerType::ObjectGroup;
	else if(m_typeStr == "imagelayer") m_type = LayerType::ImageLayer;
	else if(m_typeStr == "group") m_type = LayerType::Group;
	else m_type = LayerType::Undefined;
}

/*!
 * 'compression': zlib, gzip or empty (default). tilelayer only.
 * @return
 */
const std::string &tson::Layer::getCompression() const
{
	return m_compression;
}

/*!
 * 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
 * @return
 */
const std::vector<uint32_t> &tson::Layer::getData() const
{
	return m_data;
}

/*!
 * 'data' (when string): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
 * @return
 */
const std::string &tson::Layer::getBase64Data() const
{
	return m_base64Data;
}

/*!
 * 'draworder': topdown (default) or index. objectgroup only.
 * @return
 */
const std::string &tson::Layer::getDrawOrder() const
{
	return m_drawOrder;
}

/*!
 * 'encoding': csv (default) or base64. tilelayer only.
 * @return
 */
const std::string &tson::Layer::getEncoding() const
{
	return m_encoding;
}

/*!
 * 'id': Incremental id - unique across all layers
 * @return
 */
int tson::Layer::getId() const
{
	return m_id;
}

/*!
 * 'image': Image used by this layer. imagelayer only.
 * @return
 */
const std::string &tson::Layer::getImage() const
{
	return m_image;
}

/*!
 * 'name': Name assigned to this layer
 * @return
 */
const std::string &tson::Layer::getName() const
{
	return m_name;
}

/*!
 * 'offsetx' and 'offsety': Horizontal and Vertical layer offset in pixels (default: {0, 0})
 * @return
 */
const tson::Vector2f &tson::Layer::getOffset() const
{
	return m_offset;
}

/*!
 * 'opacity': Value between 0 and 1
 * @return
 */
float tson::Layer::getOpacity() const
{
	return m_opacity;
}

/*!
 * x = 'width': (Column count. Same as map width for fixed-size maps.)
 * y = 'height': Row count. Same as map height for fixed-size maps.
 * @return width and height as a single size
 */
const tson::Vector2i &tson::Layer::getSize() const
{
	return m_size;
}

/*!
 * 'transparentcolor': Color created from a hex color (#RRGGBB) (optional, imagelayer only)
 * @return color as color object with rgba channel.
 */
const tson::Colori &tson::Layer::getTransparentcolor() const
{
	return m_transparentcolor;
}

/*!
 * 'type': tilelayer, objectgroup, imagelayer or group
 * @return string with the object type
 */
const std::string &tson::Layer::getTypeStr() const
{
	return m_typeStr;
}

/*!
 * 'visible': Whether layer is shown or hidden in editor
 * @return
 */
bool tson::Layer::isVisible() const
{
	return m_visible;
}

/*!
 * 'x': Horizontal layer offset in tiles. Always 0.
 * @return x value (always 0 for layer)
 */
int tson::Layer::getX() const
{
	return m_x;
}

/*!
 * 'y': Horizontal layer offset in tiles. Always 0.
 * @return y value (always 0 for layer)
 */
int tson::Layer::getY() const
{
	return m_y;
}

/*!
 * 'chunks': Array of chunks (optional). tilelayer only.
 * @return
 */
std::vector<tson::Chunk> &tson::Layer::getChunks()
{
	return m_chunks;
}

/*!
 * 'layers': Array of layers. group on
 * @return
 */
std::vector<tson::Layer> &tson::Layer::getLayers()
{
	return m_layers;
}

/*!
 * 'objects': Array of objects. objectgroup only.
 * @return
 */
std::vector<tson::Object> &tson::Layer::getObjects()
{
	return m_objects;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Layer::getProperties()
{
	return m_properties;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Layer::getProp(const std::string &name)
{
	if(m_properties.hasProperty(name))
		return m_properties.getProperty(name);
	return nullptr;
}

/*!
 * Get layer type
 * @return Layer type as enum
 */
tson::LayerType tson::Layer::getType() const
{
	return m_type;
}

/*!
 * Assigns a tilemap of pointers to existing tiles.
 * @param tileMap The tilemap. key: tile id, value: pointer to Tile.
 */
void tson::Layer::assignTileMap(std::map<uint32_t, tson::Tile *> *tileMap)
{
	m_tileMap = tileMap;
}

/*!
 * Get tile data as some kind of map with x and y position with pointers to existing tiles.
 * Map only contains tiles that are not empty. x and y position is in tile units.
 *
 * Example of getting tile from the returned map:
 *
 * Tile *tile = tileData[{0, 4}];
 *
 * @return A map that represents the data returned from getData() in a 2D map with Tile pointers.
 */
const std::map<std::tuple<int, int>, tson::Tile *> &tson::Layer::getTileData() const
{
	return m_tileData;
}

/*!
 * A safe way to get tile data
 * Get tile data as some kind of map with x and y position with pointers to existing tiles.
 * Map only contains tiles that are not empty. x and y position is in tile units.
 *
 * Example of getting tile:
 * Tile *tile = layer->getTileData(0, 4)
 *
 * @param x X position in tile units
 * @param y Y position in tile units
 * @return pointer to tile, if it exists. nullptr otherwise.
 */
tson::Tile *tson::Layer::getTileData(int x, int y)
{
	return (m_tileData.count({x, y}) > 0) ? m_tileData[{x,y}] : nullptr;
}

/*!
 * Used for getting the tson::Map who is the parent of this Layer.
 * @return a pointer to the tson::Map where this layer is contained.
 */
tson::Map *tson::Layer::getMap() const
{
	return m_map;
}

/*!
 *
 * This is only supported for non-infinite maps!
 *
 * @param mapSize The size of the map
 * @param isInfiniteMap Whether or not the current map is infinte.
 */
void tson::Layer::createTileData(const Vector2i &mapSize, bool isInfiniteMap)
{
	size_t x = 0;
	size_t y = 0;
	if(!isInfiniteMap)
	{
		std::for_each(m_data.begin(), m_data.end(), [&](uint32_t tileId)
		{
			if (x == mapSize.x)
			{
				++y;
				x = 0;
			}

			if (tileId > 0 && m_tileMap->count(tileId) > 0)
			{
				const std::tuple<int, int> tile{
					static_cast<int>(x),
					static_cast<int>(y),
				};

				m_tileData[tile] = m_tileMap->at(tileId);
				m_tileObjects[tile] = { tile, m_tileData[tile]};
			}
			else if(tileId > 0 && m_tileMap->count(tileId) == 0) //Tile with flip flags!
			{
				queueFlaggedTile(x, y, tileId);
			}
			x++;
		});

	}
}

const std::map<std::tuple<int, int>, tson::TileObject> &tson::Layer::getTileObjects() const
{
	return m_tileObjects;
}

tson::TileObject *tson::Layer::getTileObject(int x, int y)
{
	return (m_tileObjects.count({x, y}) > 0) ? &m_tileObjects[{x,y}] : nullptr;
}

const std::set<uint32_t> &tson::Layer::getUniqueFlaggedTiles() const
{
	return m_uniqueFlaggedTiles;
}

void tson::Layer::resolveFlaggedTiles()
{
	std::for_each(m_flaggedTiles.begin(), m_flaggedTiles.end(), [&](const tson::FlaggedTile &tile)
	{
		if (tile.id > 0 && m_tileMap->count(tile.id) > 0)
		{
			const std::tuple<int, int> currentTile{
				static_cast<int>(tile.x),
				static_cast<int>(tile.y),
			};

			m_tileData[currentTile] = m_tileMap->at(tile.id);
			m_tileObjects[currentTile] = { currentTile, m_tileData[currentTile] };
		}
	});
}

/*!
 * 'tintcolor': Hex-formatted color (#RRGGBB or #AARRGGBB) that is multiplied with any graphics drawn by this layer or any child layers (optional).
 *
 * @return tintcolor
 */
const tson::Colori &tson::Layer::getTintColor() const
{
	return m_tintcolor;
}

#endif //TILESON_LAYER_HPP

/*** End of inlined file: Layer.hpp ***/



/*** Start of inlined file: Tileset.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILESET_HPP
#define TILESON_TILESET_HPP

//#include "../external/json.hpp"


/*** Start of inlined file: WangSet.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGSET_HPP
#define TILESON_WANGSET_HPP

//#include "../external/json.hpp"

/*** Start of inlined file: WangColor.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGCOLOR_HPP
#define TILESON_WANGCOLOR_HPP

//#include "../external/json.hpp"

namespace tson
{
	class WangColor
	{
		public:
			inline WangColor() = default;
			inline explicit WangColor(const nlohmann::json &json);
			inline bool parse(const nlohmann::json &json);

			[[nodiscard]] inline const Colori &getColor() const;
			[[nodiscard]] inline const std::string &getName() const;
			[[nodiscard]] inline float getProbability() const;
			[[nodiscard]] inline int getTile() const;

		private:
			tson::Colori      m_color;              /*! 'color': Hex-formatted color (#RRGGBB or #AARRGGBB) */
			std::string       m_name;               /*! 'name': Name of the Wang color */
			float             m_probability{};      /*! 'probability': Probability used when randomizing */
			int               m_tile{};             /*! 'tile': Local ID of tile representing the Wang color */
	};
}

tson::WangColor::WangColor(const nlohmann::json &json)
{
	parse(json);
}

bool tson::WangColor::parse(const nlohmann::json &json)
{
	bool allFound = true;

	if(json.count("color") > 0) m_color = tson::Colori(json["color"].get<std::string>()); else allFound = false;
	if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
	if(json.count("probability") > 0) m_probability = json["probability"].get<float>(); else allFound = false;
	if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;

	return allFound;
}

/*!
 * 'color': Color object created from hex-formatted string (#RRGGBB or #AARRGGBB)
 * @return
 */
const tson::Colori &tson::WangColor::getColor() const
{
	return m_color;
}

/*!
 * 'name': Name of the Wang color
 * @return
 */
const std::string &tson::WangColor::getName() const
{
	return m_name;
}

/*!
 * 'probability': Probability used when randomizing
 * @return
 */
float tson::WangColor::getProbability() const
{
	return m_probability;
}

/*!
 * 'tile': Local ID of tile representing the Wang color
 * @return
 */
int tson::WangColor::getTile() const
{
	return m_tile;
}

#endif //TILESON_WANGCOLOR_HPP

/*** End of inlined file: WangColor.hpp ***/



/*** Start of inlined file: WangTile.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGTILE_HPP
#define TILESON_WANGTILE_HPP

//#include "../external/json.hpp"

namespace tson
{
	class WangTile
	{
		public:
			inline WangTile() = default;
			inline explicit WangTile(const nlohmann::json &json);
			inline bool parse(const nlohmann::json &json);

			[[nodiscard]] inline bool hasDFlip() const;
			[[nodiscard]] inline bool hasHFlip() const;
			[[nodiscard]] inline int getTileid() const;
			[[nodiscard]] inline bool hasVFlip() const;

			[[nodiscard]] inline const std::vector<int> &getWangIds() const;

		private:
			bool                 m_dflip{};     /*! 'dflip': Tile is flipped diagonally */
			bool                 m_hflip{};     /*! 'hflip': Tile is flipped horizontally */
			int                  m_tileid{};    /*! 'tileid': Local ID of tile */
			bool                 m_vflip{};     /*! 'vflip': Tile is flipped vertically */
			std::vector<int>     m_wangId;      /*! 'wangid': Array of Wang color indexes (uchar[8])*/
	};
}

tson::WangTile::WangTile(const nlohmann::json &json)
{
	parse(json);
}

/*!
 * Parses a wang tile from Tiled json.
 * @param json A Tiled json file
 * @return true if all mandatory fields were found. False otherwise.
 */
bool tson::WangTile::parse(const nlohmann::json &json)
{
	bool allFound = true;

	if(json.count("dflip") > 0) m_dflip = json["dflip"].get<bool>(); else allFound = false;
	if(json.count("hflip") > 0) m_hflip = json["hflip"].get<bool>(); else allFound = false;
	if(json.count("vflip") > 0) m_vflip = json["vflip"].get<bool>(); else allFound = false;
	if(json.count("tileid") > 0) m_tileid = json["tileid"].get<int>(); else allFound = false;

	if(json.count("wangid") > 0 && json["wangid"].is_array())
		std::for_each(json["wangid"].begin(), json["wangid"].end(), [&](const nlohmann::json &item) { m_wangId.emplace_back(item.get<int>()); });

	return allFound;
}

/*!
 * 'dflip': Tile is flipped diagonally
 * @return
 */
bool tson::WangTile::hasDFlip() const
{
	return m_dflip;
}

/*!
 * 'hflip': Tile is flipped horizontally
 * @return
 */
bool tson::WangTile::hasHFlip() const
{
	return m_hflip;
}

/*!
 * 'tileid': Local ID of tile
 * @return
 */
int tson::WangTile::getTileid() const
{
	return m_tileid;
}

/*!
 * 'vflip': Tile is flipped vertically
 * @return
 */
bool tson::WangTile::hasVFlip() const
{
	return m_vflip;
}

/*!
 * 'wangid': Array of Wang color indexes (uchar[8])
 * @return
 */
const std::vector<int> &tson::WangTile::getWangIds() const
{
	return m_wangId;
}

#endif //TILESON_WANGTILE_HPP

/*** End of inlined file: WangTile.hpp ***/

namespace tson
{
	class WangSet
	{
		public:
			inline WangSet() = default;
			inline explicit WangSet(const nlohmann::json &json);
			inline bool parse(const nlohmann::json &json);

			[[nodiscard]] inline const std::string &getName() const;
			[[nodiscard]] inline int getTile() const;

			[[nodiscard]] inline const std::vector<tson::WangTile> &getWangTiles() const;
			[[nodiscard]] inline const std::vector<tson::WangColor> &getCornerColors() const;
			[[nodiscard]] inline const std::vector<tson::WangColor> &getEdgeColors() const;

			inline PropertyCollection &getProperties();

			template <typename T>
			inline T get(const std::string &name);
			inline tson::Property * getProp(const std::string &name);

		private:
			std::string                  m_name;          /*! 'name': Name of the Wang set */
			int                          m_tile{};        /*! 'tile': Local ID of tile representing the Wang set */
			std::vector<tson::WangTile>  m_wangTiles;     /*! 'wangtiles': Array of Wang tiles */
			std::vector<tson::WangColor> m_cornerColors;  /*! 'cornercolors': Array of Wang colors */
			std::vector<tson::WangColor> m_edgeColors;    /*! 'edgecolors': Array of Wang colors */
			tson::PropertyCollection     m_properties; 	  /*! 'properties': A list of properties (name, value, type). */

	};

	/*!
	 * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
	 * @tparam T The template value
	 * @param name Name of the property
	 * @return The actual value, if it exists. Otherwise: The default value of the type.
	 */
	template<typename T>
	T tson::WangSet::get(const std::string &name)
	{
		return m_properties.getValue<T>(name);
	}
}

tson::WangSet::WangSet(const nlohmann::json &json)
{
	parse(json);
}

bool tson::WangSet::parse(const nlohmann::json &json)
{
	bool allFound = true;

	if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;
	if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;

	//More advanced data
	if(json.count("wangtiles") > 0 && json["wangtiles"].is_array())
		std::for_each(json["wangtiles"].begin(), json["wangtiles"].end(), [&](const nlohmann::json &item) { m_wangTiles.emplace_back(item); });
	if(json.count("cornercolors") > 0 && json["cornercolors"].is_array())
		std::for_each(json["cornercolors"].begin(), json["cornercolors"].end(), [&](const nlohmann::json &item) { m_cornerColors.emplace_back(item); });
	if(json.count("edgecolors") > 0 && json["edgecolors"].is_array())
		std::for_each(json["edgecolors"].begin(), json["edgecolors"].end(), [&](const nlohmann::json &item) { m_edgeColors.emplace_back(item); });
	if(json.count("properties") > 0 && json["properties"].is_array())
		std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

	return allFound;
}

/*!
 * 'name': Name of the Wang set
 * @return
 */
const std::string &tson::WangSet::getName() const
{
	return m_name;
}

/*!
 * 'tile': Local ID of tile representing the Wang set
 * @return
 */
int tson::WangSet::getTile() const
{
	return m_tile;
}

/*!
 * 'wangtiles': Array of Wang tiles
 * @return
 */
const std::vector<tson::WangTile> &tson::WangSet::getWangTiles() const
{
	return m_wangTiles;
}

/*!
 * 'cornercolors': Array of Wang colors
 * @return
 */
const std::vector<tson::WangColor> &tson::WangSet::getCornerColors() const
{
	return m_cornerColors;
}

/*!
 * 'edgecolors': Array of Wang colors
 * @return
 */
const std::vector<tson::WangColor> &tson::WangSet::getEdgeColors() const
{
	return m_edgeColors;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::WangSet::getProperties()
{
	return m_properties;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::WangSet::getProp(const std::string &name)
{
	if(m_properties.hasProperty(name))
		return m_properties.getProperty(name);

	return nullptr;
}

#endif //TILESON_WANGSET_HPP

/*** End of inlined file: WangSet.hpp ***/


/*** Start of inlined file: Tile.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILE_HPP
#define TILESON_TILE_HPP

//#include "../external/json.hpp"


/*** Start of inlined file: Frame.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_FRAME_HPP
#define TILESON_FRAME_HPP

//#include "../external/json.hpp"

namespace tson
{
	class Frame
	{
		public:
			inline Frame() = default;
			inline Frame(int duration, int tileId);
			inline Frame(const nlohmann::json &json);

			inline bool parse(const nlohmann::json &json);

			[[nodiscard]] inline int getDuration() const;
			[[nodiscard]] inline int getTileId() const;

		private:
			int m_duration {};  /*! 'duration': Frame duration in milliseconds */
			int m_tileId {};    /*! 'tileid': Local tile ID representing this frame */
	};
}

/*!
 *
 * @param duration duration in milliseconds
 * @param tileId TileId
 */
tson::Frame::Frame(int duration, int tileId) : m_duration {duration}, m_tileId {tileId}
{

}

/*!
 * Parses frame data from json
 * @param json
 */
tson::Frame::Frame(const nlohmann::json &json)
{
	parse(json);
}

/*!
 * Parses frame data from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Frame::parse(const nlohmann::json &json)
{
	bool allFound = true;

	if(json.count("duration") > 0) m_duration = json["duration"].get<int>(); else allFound = false;
	if(json.count("tileid") > 0) m_tileId = json["tileid"].get<int>(); else allFound = false;

	return allFound;
}

/*!
 * 'duration': Frame duration in milliseconds
 * @return Duration in milliseconds
 */
int tson::Frame::getDuration() const
{
	return m_duration;
}

/*!
 * 'tileid': Local tile ID representing this frame
 * @return tile id
 */
int tson::Frame::getTileId() const
{
	return m_tileId;
}

#endif //TILESON_FRAME_HPP

/*** End of inlined file: Frame.hpp ***/

namespace tson
{
	class Tileset;

	class Tile
	{
		public:
			inline Tile() = default;
			inline Tile(const nlohmann::json &json, tson::Tileset *tileset, tson::Map *map);
			inline Tile(uint32_t id, tson::Tileset *tileset, tson::Map *map);
			inline Tile(uint32_t id, tson::Map *map); //v1.2.0
			inline bool parse(const nlohmann::json &json, tson::Tileset *tileset, tson::Map *map);

			[[nodiscard]] inline uint32_t getId() const;
			#ifndef DISABLE_CPP17_FILESYSTEM
			[[nodiscard]] inline const fs::path &getImage() const;
			#else
			[[nodiscard]] inline const std::string &getImage() const;
			#endif
			[[nodiscard]] inline const Vector2i &getImageSize() const;
			[[nodiscard]] inline const std::string &getType() const;

			[[nodiscard]] inline const std::vector<tson::Frame> &getAnimation() const;
			[[nodiscard]] inline const Layer &getObjectgroup() const;
			[[nodiscard]] inline PropertyCollection &getProperties();
			[[nodiscard]] inline const std::vector<int> &getTerrain() const;

			template <typename T>
			inline T get(const std::string &name);
			inline tson::Property * getProp(const std::string &name);

			//v1.2.0-stuff
			inline void setProperties(const tson::PropertyCollection &properties);

			inline tson::Tileset * getTileset() const;
			inline tson::Map * getMap() const;
			inline const tson::Rect &getDrawingRect() const;
			inline const tson::Vector2f getPosition(const std::tuple<int, int> &tileDataPos);
			inline const tson::Vector2i getPositionInTileUnits(const std::tuple<int, int> &tileDataPos);
			inline const tson::Vector2i getTileSize() const;                       /*! Declared in tileson_forward.hpp */

			[[nodiscard]] inline TileFlipFlags getFlipFlags() const;
			inline bool hasFlipFlags(TileFlipFlags flags);
			[[nodiscard]] inline uint32_t getGid() const;

			inline void addTilesetAndPerformCalculations(tson::Tileset *tileset); //v1.2.0

		private:
			std::vector<tson::Frame>    m_animation; 	    /*! 'animation': Array of Frames */
			uint32_t                    m_id {};            /*! 'id': Local ID of the tile */
			#ifndef DISABLE_CPP17_FILESYSTEM
			fs::path                    m_image;            /*! 'image': Image representing this tile (optional)*/
			#else
			std::string                 m_image;
			#endif
			tson::Vector2i              m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
			tson::Layer                 m_objectgroup; 	 	/*! 'objectgroup': Layer with type objectgroup (optional) */
			tson::PropertyCollection    m_properties; 	    /*! 'properties': A list of properties (name, value, type). */
			std::vector<int>            m_terrain;          /*! 'terrain': Index of terrain for each corner of tile */
			std::string                 m_type;             /*! 'type': The type of the tile (optional) */

			//v1.2.0-stuff
			uint32_t                    m_gid {};                                    /*! id without flip flags */
			tson::Tileset *             m_tileset;                                   /*! A pointer to the tileset where this Tile comes from */
			tson::Map *                 m_map;                                       /*! A pointer to the map where this tile is contained */
			tson::Rect                  m_drawingRect;                               /*! A rect that shows which part of the tileset that is used for this tile */
			tson::TileFlipFlags         m_flipFlags = TileFlipFlags::None;           /*! Resolved using bit 32, 31 and 30 from gid */
			inline void performDataCalculations();                                   /*! Declared in tileson_forward.hpp - Calculate all the values used in the tile class. */
			inline void manageFlipFlagsByIdThenRemoveFlags(uint32_t &id);
			friend class Layer;
	};

	/*!
	 * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
	 * @tparam T The template value
	 * @param name Name of the property
	 * @return The actual value, if it exists. Otherwise: The default value of the type.
	 */
	template<typename T>
	T tson::Tile::get(const std::string &name)
	{
		return m_properties.getValue<T>(name);
	}
}

tson::Tile::Tile(const nlohmann::json &json, tson::Tileset *tileset, tson::Map *map)
{
	parse(json, tileset, map);
}

/*!
 * Used in cases where you have a tile without any property
 * @param id
 */
tson::Tile::Tile(uint32_t id, tson::Tileset *tileset, tson::Map *map) : m_id {id}, m_gid {id}
{
	m_tileset = tileset;
	m_map = map;
	manageFlipFlagsByIdThenRemoveFlags(m_gid);
	performDataCalculations();
}

/*!
 * Used in cases where you have a FLIP FLAGGED tile
 * @param id
 */
tson::Tile::Tile(uint32_t id, tson::Map *map) : m_id {id}, m_gid {id}
{
	m_map = map;
	manageFlipFlagsByIdThenRemoveFlags(m_gid);
}

/*!
 * For flip flagged tiles, tilesets must be resolved later.
 * @param tileset
 */
void tson::Tile::addTilesetAndPerformCalculations(tson::Tileset *tileset)
{
	m_tileset = tileset;
	performDataCalculations();
}

/*!
 * Parses a tile from a Tiled json. id on tile is store as id + 1 to match the references in data containers.
 * @param json
 * @return
 */
bool tson::Tile::parse(const nlohmann::json &json, tson::Tileset *tileset, tson::Map *map)
{
	m_tileset = tileset;
	m_map = map;

	bool allFound = true;
	#ifndef DISABLE_CPP17_FILESYSTEM
	if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); //Optional
	#else
	if(json.count("image") > 0) m_image = json["image"].get<std::string>(); //Optional
	#endif
	if(json.count("id") > 0)
	{
		m_id = json["id"].get<uint32_t>() + 1;
		m_gid = m_id;
		manageFlipFlagsByIdThenRemoveFlags(m_gid);
	}
	else
		allFound = false;

	if(json.count("type") > 0) m_type = json["type"].get<std::string>(); //Optional
	if(json.count("objectgroup") > 0) m_objectgroup = tson::Layer(json["objectgroup"], m_map); //Optional

	if(json.count("imagewidth") > 0 && json.count("imageheight") > 0)
		m_imageSize = {json["imagewidth"].get<int>(), json["imageheight"].get<int>()}; //Optional

	//More advanced data
	if(json.count("animation") > 0 && json["animation"].is_array())
		std::for_each(json["animation"].begin(), json["animation"].end(), [&](const nlohmann::json &item) { m_animation.emplace_back(item); });
	if(json.count("terrain") > 0 && json["terrain"].is_array())
		std::for_each(json["terrain"].begin(), json["terrain"].end(), [&](const nlohmann::json &item) { m_terrain.emplace_back(item.get<int>()); });

	if(json.count("properties") > 0 && json["properties"].is_array())
		std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

	performDataCalculations();

	return allFound;
}

/*!
 * 'id': Local ID of the tile
 * @return
 */
uint32_t tson::Tile::getId() const
{
	return m_id;
}

/*!
 * 'image': Image representing this tile (optional)
 * @return
 */
#ifndef DISABLE_CPP17_FILESYSTEM
const fs::path &tson::Tile::getImage() const { return m_image; }
#else
const std::string &tson::Tile::getImage() const { return m_image; }
#endif
/*!
 * x = 'imagewidth' and y = 'imageheight': in pixels
 * @return
 */
const tson::Vector2i &tson::Tile::getImageSize() const
{
	return m_imageSize;
}

/*!
 * 'type': The type of the tile (optional)
 * @return
 */
const std::string &tson::Tile::getType() const
{
	return m_type;
}

/*!
 * 'animation': Array of Frames
 * @return
 */
const std::vector<tson::Frame> &tson::Tile::getAnimation() const
{
	return m_animation;
}

/*!
 * 'objectgroup': Layer with type objectgroup (optional)
 * @return
 */
const tson::Layer &tson::Tile::getObjectgroup() const
{
	return m_objectgroup;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Tile::getProperties()
{
	return m_properties;
}

/*!
 * 'terrain': Index of terrain for each corner of tile
 * @return
 */
const std::vector<int> &tson::Tile::getTerrain() const
{
	return m_terrain;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Tile::getProp(const std::string &name)
{
	if(m_properties.hasProperty(name))
		return m_properties.getProperty(name);

	return nullptr;
}

/*!
 * Used for getting the tson::Tileset who is the parent of this Tile.
 * @return a pointer to the tson::Tileset where this tile is contained.
 */
tson::Tileset *tson::Tile::getTileset() const
{
	return m_tileset;
}

/*!
 * Used for getting the tson::Map who is the parent of this Tile.
 * @return a pointer to the tson::Map where this tile is contained.
 */
tson::Map *tson::Tile::getMap() const
{
	return m_map;
}

/*!
 * Get the information needed to draw the Tile based on its current tileset
 * @return a tson::Rect containing the information needed to draw the tile.
 */
const tson::Rect &tson::Tile::getDrawingRect() const
{
	return m_drawingRect;
}

/*!
 * Helper function.
 *
 * Get the position of the tile in tile units.
 * The size of each unit is determined by the tile size property of the map.
 * Example: If the tile size is 16x16 in the map, a tile unit of [2, 4] would be [32, 64] in pixels.
 * If you want the position in pixels: use getPosition() instead.
 *
 * @return Position of tile in tile units.
 */
const tson::Vector2i tson::Tile::getPositionInTileUnits(const std::tuple<int, int> &tileDataPos)
{
	return {std::get<0>(tileDataPos), std::get<1>(tileDataPos)};
}

void tson::Tile::manageFlipFlagsByIdThenRemoveFlags(uint32_t &id)
{
	if (id & FLIPPED_HORIZONTALLY_FLAG) m_flipFlags |= TileFlipFlags::Horizontally;
	if (id & FLIPPED_VERTICALLY_FLAG) m_flipFlags |= TileFlipFlags::Vertically;
	if (id & FLIPPED_DIAGONALLY_FLAG) m_flipFlags |= TileFlipFlags::Diagonally;

	id &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
}

tson::TileFlipFlags tson::Tile::getFlipFlags() const
{
	return m_flipFlags;
}

/*!
 *
 * @param flags Which flags to check for. Several flags can be checked at once using the bitwise or operator.
 * Example:
 * hasFlipFlags(TileFlipFlags::Vertically | TileFlipFlags::Horizontally)
 *
 * @return true if the flag(s) specified are set
 */
bool tson::Tile::hasFlipFlags(tson::TileFlipFlags flags)
{
	return ((m_flipFlags & flags) == flags) ? true : false;
}

uint32_t tson::Tile::getGid() const
{
	return m_gid;
}

void tson::Tile::setProperties(const tson::PropertyCollection &properties)
{
	m_properties = properties;
}

#endif //TILESON_TILE_HPP

/*** End of inlined file: Tile.hpp ***/


/*** Start of inlined file: Terrain.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TERRAIN_HPP
#define TILESON_TERRAIN_HPP

//#include "../external/json.hpp"

namespace tson
{
	class Terrain
	{
		public:
			inline Terrain() = default;
			inline Terrain(std::string name, int tile);
			inline explicit Terrain(const nlohmann::json &json);

			inline bool parse(const nlohmann::json &json);

			[[nodiscard]] inline const std::string &getName() const;
			[[nodiscard]] inline int getTile() const;
			[[nodiscard]] inline PropertyCollection &getProperties();

			template <typename T>
			inline T get(const std::string &name);
			inline tson::Property * getProp(const std::string &name);

		private:
			std::string                 m_name;        /*! 'name': Name of terrain */
			int                         m_tile {};     /*! 'tile': Local ID of tile representing terrain */
			tson::PropertyCollection    m_properties;  /*! 'properties': A list of properties (name, value, type). */
	};

	/*!
	 * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
	 * @tparam T The template value
	 * @param name Name of the property
	 * @return The actual value, if it exists. Otherwise: The default value of the type.
	 */
	template<typename T>
	T tson::Terrain::get(const std::string &name)
	{
		return m_properties.getValue<T>(name);
	}
}

tson::Terrain::Terrain(std::string name, int tile) : m_name {std::move(name)}, m_tile {tile}
{

}

tson::Terrain::Terrain(const nlohmann::json &json)
{
	parse(json);
}

bool tson::Terrain::parse(const nlohmann::json &json)
{
	bool allFound = true;

	if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
	if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;

	if(json.count("properties") > 0 && json["properties"].is_array())
		std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

	return allFound;
}

/*!
 * 'name': Name of terrain
 * @return
 */
const std::string &tson::Terrain::getName() const
{
	return m_name;
}

/*!
 * 'tile': Local ID of tile representing terrain
 * @return
 */
int tson::Terrain::getTile() const
{
	return m_tile;
}

/*!
 * 'properties': A list of properties (name, value, type). *Missing from the official Tiled documentation...*
 * @return
 */
tson::PropertyCollection &tson::Terrain::getProperties()
{
	return m_properties;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Terrain::getProp(const std::string &name)
{
	if(m_properties.hasProperty(name))
		return m_properties.getProperty(name);
	return nullptr;
}

#endif //TILESON_TERRAIN_HPP

/*** End of inlined file: Terrain.hpp ***/


/*** Start of inlined file: Grid.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_GRID_HPP
#define TILESON_GRID_HPP

#include <string>
//#include "../external/json.hpp"

namespace tson
{
	class Grid
	{
		public:
			inline Grid() = default;
			inline explicit Grid(const nlohmann::json &json);

			inline bool parse(const nlohmann::json &json);

			[[nodiscard]] inline const std::string &getOrientation() const;
			[[nodiscard]] inline const Vector2i &getSize() const;

		private:
			std::string m_orientation; /*! 'orientation': Orientation of the grid for the tiles in this tileset (orthogonal or isometric) */
			tson::Vector2i m_size; /*! 'width' and 'height': Size. */
	};
}

/*!
 * Parses Tiled grid data from json
 * @param json
 */
tson::Grid::Grid(const nlohmann::json &json)
{
	parse(json);
}

/*!
 * Parses Tiled grid data from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Grid::parse(const nlohmann::json &json)
{
	bool allFound = true;

	if(json.count("orientation") > 0) m_orientation = json["orientation"].get<std::string>(); //Optional

	if(json.count("width") > 0 && json.count("height") > 0)
		m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;

	return allFound;
}

/*!
 * 'orientation': Orientation of the grid for the tiles in this tileset (orthogonal or isometric)
 * @return orientation as string
 */
const std::string &tson::Grid::getOrientation() const
{
	return m_orientation;
}

/*!
 * 'width' and 'height': Size.
 * @return size as int
 */
const tson::Vector2i &tson::Grid::getSize() const
{
	return m_size;
}

#endif //TILESON_GRID_HPP

/*** End of inlined file: Grid.hpp ***/

#include <functional>

namespace tson
{
	class Map;
	class Tileset
	{
		public:
			inline Tileset() = default;
			inline explicit Tileset(const nlohmann::json &json, tson::Map *map);
			inline bool parse(const nlohmann::json &json, tson::Map *map);

			[[nodiscard]] inline int getColumns() const;
			[[nodiscard]] inline int getFirstgid() const;

			#ifndef DISABLE_CPP17_FILESYSTEM
			[[nodiscard]] inline const fs::path &getImagePath() const;
			[[nodiscard]] inline const fs::path &getImage() const;
			#else
			[[nodiscard]] inline const std::string &getImagePath() const;
			[[nodiscard]] inline const std::string &getImage() const;
			#endif
			[[nodiscard]] inline const Vector2i &getImageSize() const;
			[[nodiscard]] inline int getMargin() const;
			[[nodiscard]] inline const std::string &getName() const;
			[[nodiscard]] inline int getSpacing() const;
			[[nodiscard]] inline int getTileCount() const;
			[[nodiscard]] inline const Vector2i &getTileSize() const;
			[[nodiscard]] inline const Colori &getTransparentColor() const;

			[[nodiscard]] inline const std::string &getType() const;
			[[nodiscard]] inline std::vector<tson::Tile> &getTiles();
			[[nodiscard]] inline const std::vector<tson::WangSet> &getWangsets() const;
			[[nodiscard]] inline PropertyCollection &getProperties();
			[[nodiscard]] inline const std::vector<tson::Terrain> &getTerrains() const;
			[[nodiscard]] inline const Vector2i &getTileOffset() const;
			[[nodiscard]] inline const Grid &getGrid() const;

			inline tson::Tile * getTile(int id);
			inline tson::Terrain * getTerrain(const std::string &name);

			template <typename T>
			inline T get(const std::string &name);
			inline tson::Property * getProp(const std::string &name);

			//v1.2.0-stuff
			[[nodiscard]] inline tson::Map *getMap() const;
			[[nodiscard]] inline ObjectAlignment getObjectAlignment() const;

			inline static tson::ObjectAlignment StringToAlignment(std::string_view str);

		private:
			inline void generateMissingTiles();

			int                           m_columns {};       /*! 'columns': The number of tile columns in the tileset */
			int                           m_firstgid {};      /*! 'firstgid': GID corresponding to the first tile in the set */
			#ifndef DISABLE_CPP17_FILESYSTEM
			fs::path                      m_image;            /*! 'image': Image used for tiles in this set */
			#else
			std::string                   m_image;
			#endif
			tson::Vector2i                m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
			int                           m_margin {};        /*! 'margin': Buffer between image edge and first tile (pixels)*/
			std::string                   m_name;             /*! 'name': Name given to this tileset */
			int                           m_spacing {};       /*! 'spacing': Spacing between adjacent tiles in image (pixels)*/
			int                           m_tileCount {};     /*! 'tilecount': The number of tiles in this tileset */
			tson::Vector2i                m_tileSize;         /*! x = 'tilewidth' and y = 'tileheight': Maximum size of tiles in this set */
			tson::Colori                  m_transparentColor; /*! 'transparentcolor': Hex-formatted color (#RRGGBB) (optional) */
			std::string                   m_type;             /*! 'type': tileset (for tileset files, since 1.0) */

			std::vector<tson::Tile>       m_tiles;            /*! 'tiles': Array of Tiles (optional) */
			std::vector<tson::WangSet>    m_wangsets;         /*! 'wangsets':Array of Wang sets (since 1.1.5) */
			tson::PropertyCollection      m_properties; 	  /*! 'properties': A list of properties (name, value, type). */

			std::vector<tson::Terrain>    m_terrains;         /*! 'terrains': Array of Terrains (optional) */
			tson::Vector2i                m_tileOffset;       /*! 'x' and 'y': See <tileoffset> (optional) */
			tson::Grid                    m_grid;             /*! 'grid': This element is only used in case of isometric orientation, and determines
																   how tile overlays for terrain and collision information are rendered. */

			//v1.2.0-stuff
			tson::ObjectAlignment         m_objectAlignment{tson::ObjectAlignment::Unspecified};  /*! 'objectalignment': Alignment to use for tile objects. Tiled 1.4.*/
			tson::Map *                   m_map;              /*! The map who owns this tileset */
	};

	/*!
	 * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
	 * @tparam T The template value
	 * @param name Name of the property
	 * @return The actual value, if it exists. Otherwise: The default value of the type.
	 */
	template<typename T>
	T tson::Tileset::get(const std::string &name)
	{
		return m_properties.getValue<T>(name);
	}
}

tson::Tileset::Tileset(const nlohmann::json &json, tson::Map *map)
{
	parse(json, map);
}

bool tson::Tileset::parse(const nlohmann::json &json, tson::Map *map)
{
	m_map = map;
	bool allFound = true;

	if(json.count("columns") > 0) m_columns = json["columns"].get<int>(); else allFound = false;
	if(json.count("firstgid") > 0) m_firstgid = json["firstgid"].get<int>(); else allFound = false;
	#ifndef DISABLE_CPP17_FILESYSTEM
	if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); else allFound = false;
	#else
	if(json.count("image") > 0) m_image = json["image"].get<std::string>(); else allFound = false;
	#endif
	if(json.count("margin") > 0) m_margin = json["margin"].get<int>(); else allFound = false;
	if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
	if(json.count("spacing") > 0) m_spacing = json["spacing"].get<int>(); else allFound = false;
	if(json.count("tilecount") > 0) m_tileCount = json["tilecount"].get<int>(); else allFound = false;
	if(json.count("transparentcolor") > 0) m_transparentColor = tson::Colori(json["transparentcolor"].get<std::string>()); //Optional
	if(json.count("type") > 0) m_type = json["type"].get<std::string>();
	if(json.count("grid") > 0) m_grid = tson::Grid(json["grid"]);

	if(json.count("imagewidth") > 0 && json.count("imageheight") > 0)
		m_imageSize = {json["imagewidth"].get<int>(), json["imageheight"].get<int>()}; else allFound = false;
	if(json.count("tilewidth") > 0 && json.count("tileheight") > 0)
		m_tileSize = {json["tilewidth"].get<int>(), json["tileheight"].get<int>()}; else allFound = false;
	if(json.count("tileoffset") > 0)
		m_tileOffset = {json["tileoffset"]["x"].get<int>(), json["tileoffset"]["y"].get<int>()};

	//More advanced data
	if(json.count("wangsets") > 0 && json["wangsets"].is_array())
		std::for_each(json["wangsets"].begin(), json["wangsets"].end(), [&](const nlohmann::json &item) { m_wangsets.emplace_back(item); });
	if(json.count("tiles") > 0 && json["tiles"].is_array())
		std::for_each(json["tiles"].begin(), json["tiles"].end(), [&](const nlohmann::json &item) { m_tiles.emplace_back(item, this, m_map); });
	if(json.count("terrains") > 0 && json["terrains"].is_array())
		std::for_each(json["terrains"].begin(), json["terrains"].end(), [&](const nlohmann::json &item) { m_terrains.emplace_back(item); });

	if(json.count("properties") > 0 && json["properties"].is_array())
		std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

	if(json.count("objectalignment") > 0)
	{
		std::string alignment = json["objectalignment"].get<std::string>();
		m_objectAlignment = StringToAlignment(alignment);
	}

	generateMissingTiles();

	return allFound;
}

/*!
 * 'columns': The number of tile columns in the tileset
 * @return
 */
int tson::Tileset::getColumns() const
{
	return m_columns;
}

/*!
 * 'firstgid': GID corresponding to the first tile in the set
 * @return
 */
int tson::Tileset::getFirstgid() const
{
	return m_firstgid;
}

/*!
 * 'image': Image used for tiles in this set
 * @return
 */
#ifndef DISABLE_CPP17_FILESYSTEM
const fs::path &tson::Tileset::getImagePath() const { return m_image; }
#else
const std::string &tson::Tileset::getImagePath() const { return m_image; }
#endif
/*!
 * x = 'imagewidth' and y = 'imageheight': in pixels
 * @return
 */
const tson::Vector2i &tson::Tileset::getImageSize() const
{
	return m_imageSize;
}

/*!
 * 'margin': Buffer between image edge and first tile (pixels)
 * @return
 */
int tson::Tileset::getMargin() const
{
	return m_margin;
}

/*!
 * 'name': Name given to this tileset
 * @return
 */
const std::string &tson::Tileset::getName() const
{
	return m_name;
}

/*!
 * 'spacing': Spacing between adjacent tiles in image (pixels)
 * @return
 */
int tson::Tileset::getSpacing() const
{
	return m_spacing;
}

/*!
 * 'tilecount': The number of tiles in this tileset
 * @return
 */
int tson::Tileset::getTileCount() const
{
	return m_tileCount;
}

/*!
 * x = 'tilewidth' and y = 'tileheight': Maximum size of tiles in this set
 * @return
 */
const tson::Vector2i &tson::Tileset::getTileSize() const
{
	return m_tileSize;
}

/*!
 * 'transparentcolor': Color object created by hex-formatted color (#RRGGBB) (optional)
 * @return
 */
const tson::Colori &tson::Tileset::getTransparentColor() const
{
	return m_transparentColor;
}

/*!
 * 'type': tileset (for tileset files, since 1.0)
 * @return
 */
const std::string &tson::Tileset::getType() const
{
	return m_type;
}

/*!
 * 'image': Image used for tiles in this set
 * @return
 */
#ifndef DISABLE_CPP17_FILESYSTEM
const fs::path &tson::Tileset::getImage() const { return m_image; }
#else
const std::string &tson::Tileset::getImage() const { return m_image; }
#endif
/*!
 * 'tiles': Array of Tiles (optional)
 * @return
 */
std::vector<tson::Tile> &tson::Tileset::getTiles()
{
	return m_tiles;
}

/*!
 * 'wangsets':Array of Wang sets (since Tiled 1.1.5)
 * @return
 */
const std::vector<tson::WangSet> &tson::Tileset::getWangsets() const
{
	return m_wangsets;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Tileset::getProperties()
{
	return m_properties;
}

/*!
 * 'terrains': Array of Terrains (optional)
 * @return
 */
const std::vector<tson::Terrain> &tson::Tileset::getTerrains() const
{
	return m_terrains;
}

/*!
 * 'x' and 'y': See <tileoffset> (optional)
 * @return
 */
const tson::Vector2i &tson::Tileset::getTileOffset() const
{
	return m_tileOffset;
}

/*!
 * 'grid': This element is only used in case of isometric orientation, and determines
 * how tile overlays for terrain and collision information are rendered.
 * @return
 */
const tson::Grid &tson::Tileset::getGrid() const
{
	return m_grid;
}

/*!
 * Gets a tile by ID (Tiled ID + 1)
 * @param id The ID of the tile stored in Tiled map + 1. Example: If ID was stored in Tiled map as 0, the corresponding value in Tileson is 1.
 * This is to make sure the IDs of tiles matches their references in containers.
 * @return A pointer to the Tile if found. nullptr otherwise.
 */
tson::Tile *tson::Tileset::getTile(int id)
{
	auto result = std::find_if(m_tiles.begin(), m_tiles.end(), [&](const tson::Tile & item) { return item.getId() == id;});
	if(result == m_tiles.end())
		return nullptr;

	return &result.operator*();
}

/*!
 * Get an existing Terrain object by name
 * @param name
 * @return A pointer to the Terrain if found. nullptr otherwise.
 */
tson::Terrain *tson::Tileset::getTerrain(const std::string &name)
{
	auto result = std::find_if(m_terrains.begin(), m_terrains.end(), [&](const tson::Terrain & item) { return item.getName() == name;});
	if(result == m_terrains.end())
		return nullptr;

	return &result.operator*();
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Tileset::getProp(const std::string &name)
{
	if(m_properties.hasProperty(name))
		return m_properties.getProperty(name);

	return nullptr;
}

/*!
 * Tiled only has tiles with a property stored in the map. This function makes sure even the ones with no properties will exist.
 */
void tson::Tileset::generateMissingTiles()
{
	std::vector<uint32_t> tileIds;
	for(auto &tile : m_tiles)
		tileIds.push_back(tile.getId());

	for(uint32_t i = m_firstgid; i < m_firstgid + m_tileCount; ++i)
	{
		if(std::count(tileIds.begin(), tileIds.end(), i) == 0)
		{
			m_tiles.emplace_back(Tile(i, this, m_map));
		}
	}
}

/*!
 * Used for getting the tson::Map who is the parent of this Tileset.
 * @return a pointer to the tson::Map where this tileset is contained.
 */
tson::Map *tson::Tileset::getMap() const
{
	return m_map;
}

/*!
 *
 * @param str The string you want to convert
 * @return Alignment enum based on the string from the input.
 */
tson::ObjectAlignment tson::Tileset::StringToAlignment(std::string_view str)
{
	if(str == "unspecified") return tson::ObjectAlignment::Unspecified;
	else if(str == "topleft") return tson::ObjectAlignment::TopLeft;
	else if(str == "top") return tson::ObjectAlignment::Top;
	else if(str == "topright") return tson::ObjectAlignment::TopRight;
	else if(str == "left") return tson::ObjectAlignment::Left;
	else if(str == "center") return tson::ObjectAlignment::Center;
	else if(str == "right") return tson::ObjectAlignment::Right;
	else if(str == "bottomleft") return tson::ObjectAlignment::BottomLeft;
	else if(str == "bottom") return tson::ObjectAlignment::Bottom;
	else if(str == "bottomright") return tson::ObjectAlignment::BottomRight;
	else
		return tson::ObjectAlignment::Unspecified;
}

tson::ObjectAlignment tson::Tileset::getObjectAlignment() const
{
	return m_objectAlignment;
}

#endif //TILESON_TILESET_HPP
/*** End of inlined file: Tileset.hpp ***/

namespace tson
{
	class Map
	{
		public:
			inline Map() = default;
			inline Map(ParseStatus status, std::string description);
			inline explicit Map(const nlohmann::json &json, tson::DecompressorContainer *decompressors);
			inline bool parse(const nlohmann::json &json, tson::DecompressorContainer *decompressors);

			[[nodiscard]] inline const Colori &getBackgroundColor() const;
			[[nodiscard]] inline const Vector2i &getSize() const;
			[[nodiscard]] inline int getHexsideLength() const;
			[[nodiscard]] inline bool isInfinite() const;
			[[nodiscard]] inline int getNextLayerId() const;
			[[nodiscard]] inline int getNextObjectId() const;
			[[nodiscard]] inline const std::string &getOrientation() const;
			[[nodiscard]] inline const std::string &getRenderOrder() const;
			[[nodiscard]] inline const std::string &getStaggerAxis() const;
			[[nodiscard]] inline const std::string &getStaggerIndex() const;
			[[nodiscard]] inline const std::string &getTiledVersion() const;
			[[nodiscard]] inline const Vector2i &getTileSize() const;
			[[nodiscard]] inline const std::string &getType() const;
			[[nodiscard]] inline int getVersion() const;

			[[nodiscard]] inline std::vector<tson::Layer> &getLayers();
			[[nodiscard]] inline PropertyCollection &getProperties();
			[[nodiscard]] inline std::vector<tson::Tileset> &getTilesets();

			[[nodiscard]] inline ParseStatus getStatus() const;
			[[nodiscard]] inline const std::string &getStatusMessage() const;
			[[nodiscard]] inline const std::map<uint32_t, tson::Tile *> &getTileMap() const;

			inline Layer * getLayer(const std::string &name);
			inline Tileset * getTileset(const std::string &name);

			template <typename T>
			inline T get(const std::string &name);
			inline tson::Property * getProp(const std::string &name);

			//v1.2.0
			[[nodiscard]] inline int getCompressionLevel() const;
			inline DecompressorContainer *getDecompressors();
			inline Tileset * getTilesetByGid(uint32_t gid);

		private:
			inline void createTilesetData(const nlohmann::json &json);
			inline void processData();

			Colori                                 m_backgroundColor;   /*! 'backgroundcolor': Hex-formatted color (#RRGGBB or #AARRGGBB) (optional)*/;
			Vector2i                               m_size;              /*! 'width' and 'height' of a Tiled map */
			int                                    m_hexsideLength {};  /*! 'hexsidelength': Length of the side of a hex tile in pixels */
			bool                                   m_isInfinite {};     /*! 'infinite': Whether the map has infinite dimensions*/
			std::vector<tson::Layer>               m_layers; 	        /*! 'layers': Array of layers. group on */
			int                                    m_nextLayerId {};    /*! 'nextlayerid': Auto-increments for each layer */
			int                                    m_nextObjectId {};   /*! 'nextobjectid': Auto-increments for each placed object */
			std::string                            m_orientation;       /*! 'orientation': orthogonal, isometric, staggered or hexagonal */
			tson::PropertyCollection               m_properties; 	    /*! 'properties': A list of properties (name, value, type). */
			std::string                            m_renderOrder;       /*! 'renderorder': Rendering direction (orthogonal maps only) */
			std::string                            m_staggerAxis;       /*! 'staggeraxis': x or y (staggered / hexagonal maps only) */
			std::string                            m_staggerIndex;      /*! 'staggerindex': odd or even (staggered / hexagonal maps only) */
			std::string                            m_tiledVersion;      /*! 'tiledversion': The Tiled version used to save the file */
			Vector2i                               m_tileSize;          /*! 'tilewidth': and 'tileheight' of a map */
			std::vector<tson::Tileset>             m_tilesets;          /*! 'tilesets': Array of Tilesets */
			std::string                            m_type;              /*! 'type': map (since 1.0) */
			int                                    m_version{};         /*! 'version': The JSON format version*/

			ParseStatus                            m_status {ParseStatus::OK};
			std::string                            m_statusMessage {"OK"};

			std::map<uint32_t, tson::Tile*>        m_tileMap;           /*! key: Tile ID. Value: Pointer to Tile*/

			//v1.2.0
			int                                    m_compressionLevel {-1};  /*! 'compressionlevel': The compression level to use for tile layer
																			  *     data (defaults to -1, which means to use the algorithm default)
																			  *     Introduced in Tiled 1.3*/
			tson::DecompressorContainer *          m_decompressors;
			std::map<uint32_t, tson::Tile>         m_flaggedTileMap;    /*! key: Tile ID. Value: Tile*/
	};

	/*!
	 * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
	 * @tparam T The template value
	 * @param name Name of the property
	 * @return The actual value, if it exists. Otherwise: The default value of the type.
	 */
	template<typename T>
	T tson::Map::get(const std::string &name)
	{
		return m_properties.getValue<T>(name);
	}
}

/*!
 * When errors have happened before the map starts parsing, just keep the statuses
 * @param status The status
 * @param description Description of the status
 */
tson::Map::Map(tson::ParseStatus status, std::string description) : m_status {status}, m_statusMessage { std::move(description) }
{

}

/*!
 * Parses a json of a Tiled map.
 * @param json A json object with the format of Map
 * @return true if all mandatory fields was found. false otherwise.
 */
tson::Map::Map(const nlohmann::json &json, tson::DecompressorContainer *decompressors)
{
	parse(json, decompressors);
}

/*!
 * Parses a json of a Tiled map.
 * @param json A json object with the format of Map
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Map::parse(const nlohmann::json &json, tson::DecompressorContainer *decompressors)
{
	m_decompressors = decompressors;

	bool allFound = true;
	if(json.count("compressionlevel") > 0) m_compressionLevel = json["compressionlevel"].get<int>(); //Tiled 1.3 - Optional
	if(json.count("backgroundcolor") > 0) m_backgroundColor = Colori(json["backgroundcolor"].get<std::string>()); //Optional
	if(json.count("width") > 0 && json.count("height") > 0 )
		m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
	if(json.count("hexsidelength") > 0) m_hexsideLength = json["hexsidelength"].get<int>();         //Optional
	if(json.count("infinite") > 0) m_isInfinite = json["infinite"].get<bool>();                     //Optional
	if(json.count("nextlayerid") > 0) m_nextLayerId = json["nextlayerid"].get<int>();               //Optional
	if(json.count("nextobjectid") > 0) m_nextObjectId = json["nextobjectid"].get<int>(); else allFound = false;
	if(json.count("orientation") > 0) m_orientation = json["orientation"].get<std::string>(); else allFound = false;
	if(json.count("renderorder") > 0) m_renderOrder = json["renderorder"].get<std::string>();       //Optional
	if(json.count("staggeraxis") > 0) m_staggerAxis = json["staggeraxis"].get<std::string>();       //Optional
	if(json.count("staggerindex") > 0) m_staggerIndex = json["staggerindex"].get<std::string>();    //Optional
	if(json.count("tiledversion") > 0) m_tiledVersion = json["tiledversion"].get<std::string>(); else allFound = false;
	if(json.count("tilewidth") > 0 && json.count("tileheight") > 0 )
		m_tileSize = {json["tilewidth"].get<int>(), json["tileheight"].get<int>()}; else allFound = false;
	if(json.count("type") > 0) m_type = json["type"].get<std::string>();                            //Optional
	if(json.count("version") > 0) m_version = json["version"].get<int>(); else allFound = false;

	//More advanced data
	if(json.count("layers") > 0 && json["layers"].is_array())
		std::for_each(json["layers"].begin(), json["layers"].end(), [&](const nlohmann::json &item) { m_layers.emplace_back(item, this); });

	if(json.count("properties") > 0 && json["properties"].is_array())
		std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

	createTilesetData(json);
	processData();

	return allFound;
}

/*!
 * Tileset data must be created in two steps to prevent malformed tson::Tileset pointers inside tson::Tile
 */
void tson::Map::createTilesetData(const nlohmann::json &json)
{
	if(json.count("tilesets") > 0 && json["tilesets"].is_array())
	{
		//First created tileset objects
		std::for_each(json["tilesets"].begin(), json["tilesets"].end(), [&](const nlohmann::json &item)
		{
			m_tilesets.emplace_back();
		});

		int i = 0;
		//Then do the parsing
		std::for_each(json["tilesets"].begin(), json["tilesets"].end(), [&](const nlohmann::json &item)
		{
			m_tilesets[i].parse(item, this);
			++i;
		});
	}
}

/*!
 * Processes the parsed data and uses the data to create helpful objects, like tile maps.
 */
void tson::Map::processData()
{
	m_tileMap.clear();
	for(auto &tileset : m_tilesets)
	{
		std::for_each(tileset.getTiles().begin(), tileset.getTiles().end(), [&](tson::Tile &tile) { m_tileMap[tile.getId()] = &tile; });
	}
	std::for_each(m_layers.begin(), m_layers.end(), [&](tson::Layer &layer)
	{
		layer.assignTileMap(&m_tileMap);
		layer.createTileData(m_size, m_isInfinite);
		const std::set<uint32_t> &flaggedTiles = layer.getUniqueFlaggedTiles();
		for(uint32_t ftile : flaggedTiles)
		{
			tson::Tile tile {ftile, layer.getMap()};
			if(m_tileMap.count(tile.getGid()))
			{
				tson::Tile *originalTile = m_tileMap[tile.getGid()];
				tile.addTilesetAndPerformCalculations(originalTile->getTileset());
				tile.setProperties(originalTile->getProperties());
				m_flaggedTileMap[ftile] = tile;
				m_tileMap[ftile] = &m_flaggedTileMap[ftile];
			}
		}
		layer.resolveFlaggedTiles();
	});
}

/*!
 * 'backgroundcolor': Color created from a hex-formatted color string (#RRGGBB or #AARRGGBB) (optional)
 * @return string as color
 */
const tson::Colori &tson::Map::getBackgroundColor() const
{
	return m_backgroundColor;
}

/*!
 * 'width' and 'height' of a Tiled map
 * @return
 */
const tson::Vector2<int> &tson::Map::getSize() const
{
	return m_size;
}

/*!
 * 'hexsidelength': Length of the side of a hex tile in pixels
 * @return
 */
int tson::Map::getHexsideLength() const
{
	return m_hexsideLength;
}

/*!
 * 'infinite': Whether the map has infinite dimensions
 * @return
 */
bool tson::Map::isInfinite() const
{
	return m_isInfinite;
}

/*!
 * 'nextlayerid': Auto-increments for each layer
 * @return
 */
int tson::Map::getNextLayerId() const
{
	return m_nextLayerId;
}

/*!
 * 'nextobjectid': Auto-increments for each placed object
 * @return
 */
int tson::Map::getNextObjectId() const
{
	return m_nextObjectId;
}

/*!
 * 'orientation': orthogonal, isometric, staggered or hexagonal
 * @return
 */
const std::string &tson::Map::getOrientation() const
{
	return m_orientation;
}

/*!
 * 'renderorder': Rendering direction (orthogonal maps only)
 * @return
 */
const std::string &tson::Map::getRenderOrder() const
{
	return m_renderOrder;
}

/*!
 * 'staggeraxis': x or y (staggered / hexagonal maps only)
 * @return
 */
const std::string &tson::Map::getStaggerAxis() const
{
	return m_staggerAxis;
}

/*!
 * 'staggerindex': odd or even (staggered / hexagonal maps only)
 * @return
 */
const std::string &tson::Map::getStaggerIndex() const
{
	return m_staggerIndex;
}

/*!
 * 'tiledversion': The Tiled version used to save the file
 * @return
 */
const std::string &tson::Map::getTiledVersion() const
{
	return m_tiledVersion;
}

/*!
 * 'tilewidth': and 'tileheight' of a map
 * @return
 */
const tson::Vector2<int> &tson::Map::getTileSize() const
{
	return m_tileSize;
}

/*!
 * 'type': map (since 1.0)
 * @return
 */
const std::string &tson::Map::getType() const
{
	return m_type;
}

/*!
 * 'version': The JSON format version
 * @return
 */
int tson::Map::getVersion() const
{
	return m_version;
}

/*!
 * 'layers': Array of layers. group on
 * @return
 */
std::vector<tson::Layer> &tson::Map::getLayers()
{
	return m_layers;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Map::getProperties()
{
	return m_properties;
}

/*!
 * 'tilesets': Array of Tilesets
 * @return
 */
std::vector<tson::Tileset> &tson::Map::getTilesets()
{
	return m_tilesets;
}

tson::Layer *tson::Map::getLayer(const std::string &name)
{
	auto result = std::find_if(m_layers.begin(), m_layers.end(), [&](const tson::Layer &item) { return item.getName() == name; });
	if(result == m_layers.end())
		return nullptr;

	return &result.operator*();
}

/*!
 * Gets a tileset by name
 *
 * @param name Name of the tileset
 * @return tileset with the matching name
 */
tson::Tileset *tson::Map::getTileset(const std::string &name)
{
	auto result = std::find_if(m_tilesets.begin(), m_tilesets.end(), [&](const tson::Tileset &item) {return item.getName() == name; });
	if(result == m_tilesets.end())
		return nullptr;

	return &result.operator*();
}

/*!
 * Gets a tileset by gid (graphical ID of a tile). These are always unique, no matter how many tilesets you have
 *
 * @param gid Graphical ID of a tile
 * @return tileset related to the actual gid
 */
tson::Tileset *tson::Map::getTilesetByGid(uint32_t gid)
{
	auto result = std::find_if(m_tilesets.begin(), m_tilesets.end(), [&](const tson::Tileset &tileset)
	{
		int firstId = tileset.getFirstgid(); //First tile id of the tileset
		int lastId = (firstId + tileset.getTileCount()) - 1;

		return (gid >= firstId && gid <= lastId);
	});
	if(result == m_tilesets.end())
		return nullptr;

	return &result.operator*();
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Map::getProp(const std::string &name)
{
	if(m_properties.hasProperty(name))
		return m_properties.getProperty(name);
	return nullptr;
}

tson::ParseStatus tson::Map::getStatus() const
{
	return m_status;
}

const std::string &tson::Map::getStatusMessage() const
{
	return m_statusMessage;
}

/*!
 * Get a tile map with pointers to every existing tile.
 * @return
 */
const std::map<uint32_t, tson::Tile *> &tson::Map::getTileMap() const
{
	return m_tileMap;
}

tson::DecompressorContainer *tson::Map::getDecompressors()
{
	return m_decompressors;
}

/*!
 * 'compressionlevel': The compression level to use for tile layer data (defaults to -1, which means to use the algorithm default)
 *
 * @return The compression level
 */
int tson::Map::getCompressionLevel() const
{
	return m_compressionLevel;
}

#endif //TILESON_MAP_HPP

/*** End of inlined file: Map.hpp ***/


/*** Start of inlined file: Project.hpp ***/
//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_PROJECT_HPP
#define TILESON_PROJECT_HPP

#include <fstream>
#include <sstream>
#include <memory>

/*** Start of inlined file: World.hpp ***/
//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_WORLD_HPP
#define TILESON_WORLD_HPP


/*** Start of inlined file: WorldMapData.hpp ***/
//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_WORLDMAPDATA_HPP
#define TILESON_WORLDMAPDATA_HPP

namespace tson
{
	class WorldMapData
	{
		public:
			inline WorldMapData(const fs::path &folder_, const nlohmann::json &json);
			inline void parse(const fs::path &folder_, const nlohmann::json &json);
			//inline WorldMapData(fs::path folder_, std::string fileName_) : folder {std::move(folder_)}, fileName {fileName_}
			//{
			//    path = folder / fileName;
			//}

			fs::path folder;
			fs::path path;
			std::string fileName;
			tson::Vector2i size;
			tson::Vector2i position;
	};

	WorldMapData::WorldMapData(const fs::path &folder_, const nlohmann::json &json)
	{
		parse(folder_, json);
	}

	void WorldMapData::parse(const fs::path &folder_, const nlohmann::json &json)
	{
		folder = folder_;
		if(json.count("fileName") > 0) fileName = json["fileName"].get<std::string>();
		if(json.count("height") > 0) size = {json["width"].get<int>(), json["height"].get<int>()};
		if(json.count("x") > 0) position = {json["x"].get<int>(), json["y"].get<int>()};

		path = (!fileName.empty()) ? folder / fileName : folder;
	}
}

#endif //TILESON_WORLDMAPDATA_HPP

#endif //DISABLE_CPP17_FILESYSTEM
/*** End of inlined file: WorldMapData.hpp ***/

#include <memory>
namespace tson
{
	class Tileson;
	class World
	{
		public:
			inline World() = default;
			inline explicit World(const fs::path &path);
			inline bool parse(const fs::path &path);
			inline int loadMaps(tson::Tileson *parser); //tileson_forward.hpp
			inline bool contains(std::string_view filename);
			inline const WorldMapData *get(std::string_view filename) const;

			[[nodiscard]] inline const fs::path &getPath() const;
			[[nodiscard]] inline const fs::path &getFolder() const;
			[[nodiscard]] inline const std::vector<WorldMapData> &getMapData() const;
			[[nodiscard]] inline bool onlyShowAdjacentMaps() const;
			[[nodiscard]] inline const std::string &getType() const;
			[[nodiscard]] inline const std::vector<std::unique_ptr<tson::Map>> &getMaps() const;

		private:
			inline void parseJson(const nlohmann::json &json);

			fs::path m_path;
			fs::path m_folder;
			std::vector<WorldMapData> m_mapData;
			std::vector<std::unique_ptr<tson::Map>> m_maps;
			bool m_onlyShowAdjacentMaps;
			std::string m_type;
	};

	World::World(const fs::path &path)
	{
		parse(path);
	}

	bool World::parse(const fs::path &path)
	{
		m_path = path;
		m_folder = m_path.parent_path();
		std::ifstream i(m_path.u8string());
		nlohmann::json json;
		try
		{
			i >> json;
		}
		catch(const nlohmann::json::parse_error &error)
		{
			std::string message = "Parse error: ";
			message += std::string(error.what());
			message += std::string("\n");
			return false;
		}
		parseJson(json);
		return true;
	}

	const fs::path &World::getPath() const
	{
		return m_path;
	}

	const std::vector<WorldMapData> &World::getMapData() const
	{
		return m_mapData;
	}

	bool World::onlyShowAdjacentMaps() const
	{
		return m_onlyShowAdjacentMaps;
	}

	const std::string &World::getType() const
	{
		return m_type;
	}

	void World::parseJson(const nlohmann::json &json)
	{
		if(json.count("onlyShowAdjacentMaps") > 0) m_onlyShowAdjacentMaps = json["onlyShowAdjacentMaps"].get<bool>();
		if(json.count("type") > 0) m_type = json["type"].get<std::string>();

		if(json["maps"].is_array())
		{
			std::for_each(json["maps"].begin(), json["maps"].end(), [&](const nlohmann::json &item) { m_mapData.emplace_back(m_folder, item); });
		}
	}

	const fs::path &World::getFolder() const
	{
		return m_folder;
	}

	/*!
	 * Check if there is WorldMapData in the world that contains the current filename.
	 * Filename = <file>.<extension>
	 * @param filename
	 * @return
	 */
	bool World::contains(std::string_view filename)
	{
		//Note: might be moved to std::ranges from C++20.
		return std::any_of(m_mapData.begin(), m_mapData.end(), [&](const auto &item) { return item.fileName == filename; });
	}

	/*!
	 * Get a map by its filename
	 * @param filename Filename (including extension) - (example: file.json)
	 * @return pointer to WorldMapData or nullptr if not exists
	 */
	const WorldMapData * World::get(std::string_view filename) const
	{
		auto iter = std::find_if(m_mapData.begin(), m_mapData.end(), [&](const auto &item) { return item.fileName == filename; });
		return (iter == m_mapData.end()) ? nullptr : iter.operator->();
	}

	/*!
	 * Get all maps that have been loaded by loadMaps().
	 * NOTE: This is untested, and was a last second addition to Tileson 1.2.0, as I had forgot about the loadMaps() functionality (also untested)
	 * If you find anything malfunctioning - please report.
	 * @return All maps loaded by loadMaps()
	 */
	const std::vector<std::unique_ptr<tson::Map>> &World::getMaps() const
	{
		return m_maps;
	}

}

#endif //TILESON_WORLD_HPP

#endif //DISABLE_CPP17_FILESYSTEM
/*** End of inlined file: World.hpp ***/



/*** Start of inlined file: ProjectFolder.hpp ***/
//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_PROJECTFOLDER_HPP
#define TILESON_PROJECTFOLDER_HPP

namespace tson
{
	class ProjectFolder
	{
		public:
			inline ProjectFolder(const fs::path &path);

			inline const fs::path &getPath() const;
			inline bool hasWorldFile() const;
			inline const std::vector<ProjectFolder> &getSubFolders() const;
			inline const std::vector<fs::path> &getFiles() const;
			inline const World &getWorld() const;

		private:
			inline void loadData();
			fs::path                    m_path;
			bool                        m_hasWorldFile;
			tson::World                 m_world;
			std::vector<ProjectFolder>  m_subFolders;
			std::vector<fs::path>       m_files;

	};

	ProjectFolder::ProjectFolder(const fs::path &path) : m_path {path}
	{
		loadData();
	}

	void ProjectFolder::loadData()
	{
		m_hasWorldFile = false;
		m_subFolders.clear();
		m_files.clear();
		//Search and see if there is a World file .world file
		fs::path worldPath;
		for (const auto & entry : fs::directory_iterator(m_path))
		{
			if(fs::is_regular_file(entry.path()))
			{
				if(entry.path().extension() == ".world")
				{
					m_hasWorldFile = true;
					worldPath = entry.path();
				}
			}
		}

		if(m_hasWorldFile)
			m_world.parse(worldPath);

		for (const auto & entry : fs::directory_iterator(m_path))
		{
			if (fs::is_directory(entry.path()))
				m_subFolders.emplace_back(entry.path());//.loadData(); - loadData() is called in the constructor, so don't call again.
			else if (fs::is_regular_file(entry.path()))
			{
				if(m_hasWorldFile && m_world.contains(reinterpret_cast<const char*>(entry.path().filename().u8string().c_str())))
					m_files.emplace_back(entry.path());
				else if(!m_hasWorldFile)
					m_files.emplace_back(entry.path());
			}
		}

	}

	const fs::path &ProjectFolder::getPath() const
	{
		return m_path;
	}

	bool ProjectFolder::hasWorldFile() const
	{
		return m_hasWorldFile;
	}

	const std::vector<ProjectFolder> &ProjectFolder::getSubFolders() const
	{
		return m_subFolders;
	}

	const std::vector<fs::path> &ProjectFolder::getFiles() const
	{
		return m_files;
	}

	/*!
	 * Only gives useful data if hasWorldFile() is true!
	 * @return
	 */
	const World &ProjectFolder::getWorld() const
	{
		return m_world;
	}
}

#endif //TILESON_PROJECTFOLDER_HPP

#endif //DISABLE_CPP17_FILESYSTEM
/*** End of inlined file: ProjectFolder.hpp ***/


/*** Start of inlined file: ProjectData.hpp ***/
//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_PROJECTDATA_HPP
#define TILESON_PROJECTDATA_HPP

namespace tson
{
	class ProjectData
	{
		public:
			ProjectData() = default;
			std::string automappingRulesFile;
			std::vector<std::string> commands;
			std::string extensionsPath;
			std::vector<std::string> folders;
			std::string objectTypesFile;

			//Tileson specific
			fs::path basePath;
			std::vector<tson::ProjectFolder> folderPaths;
	};
}

#endif //TILESON_PROJECTDATA_HPP

#endif //DISABLE_CPP17_FILESYSTEM
/*** End of inlined file: ProjectData.hpp ***/

namespace tson
{
	class Project
	{
		public:
			inline Project() = default;
			inline explicit Project(const fs::path &path);
			inline bool parse(const fs::path &path);

			[[nodiscard]] inline const ProjectData &getData() const;
			[[nodiscard]] inline const fs::path &getPath() const;
			[[nodiscard]] inline const std::vector<ProjectFolder> &getFolders() const;

		private:
			inline void parseJson(const nlohmann::json &json);
			fs::path m_path;
			std::vector<ProjectFolder> m_folders;
			ProjectData m_data;
	};

	Project::Project(const fs::path &path)
	{
		parse(path);
	}

	bool Project::parse(const fs::path &path)
	{
		m_path = path;
		std::ifstream i(m_path.u8string());
		nlohmann::json json;
		try
		{
			i >> json;
		}
		catch(const nlohmann::json::parse_error &error)
		{
			std::string message = "Parse error: ";
			message += std::string(error.what());
			message += std::string("\n");
			return false;
		}
		parseJson(json);
		return true;
	}

	const ProjectData &Project::getData() const
	{
		return m_data;
	}

	void Project::parseJson(const nlohmann::json &json)
	{
		m_data.basePath = m_path.parent_path(); //The directory of the project file

		if(json.count("automappingRulesFile") > 0) m_data.automappingRulesFile = json["automappingRulesFile"].get<std::string>();
		if(json.count("commands") > 0)
		{
			m_data.commands.clear();
			std::for_each(json["commands"].begin(), json["commands"].end(), [&](const nlohmann::json &item)
			{
				m_data.commands.emplace_back(item.get<std::string>());
			});
		}
		if(json.count("extensionsPath") > 0) m_data.extensionsPath = json["extensionsPath"].get<std::string>();
		if(json.count("folders") > 0)
		{
			m_data.folders.clear();
			m_data.folderPaths.clear();
			std::for_each(json["folders"].begin(), json["folders"].end(), [&](const nlohmann::json &item)
			{
				std::string folder = item.get<std::string>();
				m_data.folders.emplace_back(folder);
				m_data.folderPaths.emplace_back(m_data.basePath / folder);
				m_folders.emplace_back(m_data.basePath / folder);
			});
		}
		if(json.count("objectTypesFile") > 0) m_data.objectTypesFile = json["objectTypesFile"].get<std::string>();

	}

	const fs::path &Project::getPath() const
	{
		return m_path;
	}

	const std::vector<ProjectFolder> &Project::getFolders() const
	{
		return m_folders;
	}

}

#endif //TILESON_PROJECT_HPP

#endif //DISABLE_CPP17_FILESYSTEM

/*** End of inlined file: Project.hpp ***/


/*** Start of inlined file: MemoryStream.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_MEMORYSTREAM_HPP
#define TILESON_MEMORYSTREAM_HPP


/*** Start of inlined file: MemoryBuffer.hpp ***/
//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_MEMORYBUFFER_HPP
#define TILESON_MEMORYBUFFER_HPP

#include <iostream>

namespace tson
{
	class MemoryBuffer : public std::basic_streambuf<char> {
		public:
			MemoryBuffer(const uint8_t *p, size_t l) {
				setg((char*)p, (char*)p, (char*)p + l);
			}
	};
}

#endif //TILESON_MEMORYBUFFER_HPP

/*** End of inlined file: MemoryBuffer.hpp ***/

namespace tson
{
	class MemoryStream : public std::istream {
		public:
			MemoryStream(const uint8_t *p, size_t l) :
					std::istream(&m_buffer),
					m_buffer(p, l) {
				rdbuf(&m_buffer);
			}

		private:
			MemoryBuffer m_buffer;
	};
}

#endif //TILESON_MEMORYSTREAM_HPP

/*** End of inlined file: MemoryStream.hpp ***/

#include <fstream>
#include <sstream>
#include <memory>

namespace tson
{
	class Tileson
	{
		public:
			inline explicit Tileson(bool includeBase64Decoder = true);
			#ifndef DISABLE_CPP17_FILESYSTEM
			inline std::unique_ptr<tson::Map> parse(const fs::path &path);
			#else
			inline std::unique_ptr<tson::Map> parse(const std::string &path);
			#endif
			inline std::unique_ptr<tson::Map> parse(const void * data, size_t size);
			inline tson::DecompressorContainer *decompressors();
		private:
			inline std::unique_ptr<tson::Map> parseJson(const nlohmann::json &json);
			tson::DecompressorContainer m_decompressors;
	};
}

/*!
 *
 * @param includeBase64Decoder Includes the base64-decoder from "Base64Decompressor.hpp" if true.
 * Otherwise no other decompressors/decoders than whatever the user itself have added will be used.
 */
tson::Tileson::Tileson(bool includeBase64Decoder)
{
	if(includeBase64Decoder)
		m_decompressors.add<Base64Decompressor>();
}

/*!
 * Parses Tiled json map data by file
 * @param path path to file
 * @return parsed data as Map
 */
#ifndef DISABLE_CPP17_FILESYSTEM
std::unique_ptr<tson::Map> tson::Tileson::parse(const fs::path &path)
{
	if(fs::exists(path) && fs::is_regular_file(path))
	{
		std::ifstream i(path.u8string());
		nlohmann::json json;
		try
		{
			i >> json;
		}
		catch(const nlohmann::json::parse_error &error)
		{
			std::string message = "Parse error: ";
			message += std::string(error.what());
			message += std::string("\n");
			return std::make_unique<tson::Map>(tson::ParseStatus::ParseError, message);
		}
		return parseJson(json);
	}

	std::string msg = "File not found: ";
	msg += std::string(reinterpret_cast<const char*>(path.u8string().c_str()));
	return std::make_unique<tson::Map>(tson::ParseStatus::FileNotFound, msg);
}
#else
[[deprecated("std::filesystem will be required in future versions and DISABLE_CPP17_FILESYSTEM will be removed")]]
std::unique_ptr<tson::Map> tson::Tileson::parse(const std::string &path)
{

	std::ifstream i(path);
	nlohmann::json json;
	try
	{
		i >> json;
	}
	catch(const nlohmann::json::parse_error &error)
	{
		std::string message = "Parse error: ";
		message += std::string(error.what());
		message += std::string("\n");
		return std::make_unique<tson::Map> (tson::ParseStatus::ParseError, message);
	}
	return std::move(parseJson(json));
}
#endif
/*!
 * Parses Tiled json map data by memory
 * @param data The data to parse
 * @param size The size of the data to parse
 * @return parsed data as Map
 */
std::unique_ptr<tson::Map> tson::Tileson::parse(const void *data, size_t size)
{
	//std::istringstream i;
	//i.rdbuf()->pubsetbuf((char *)data, size);

	tson::MemoryStream mem {(uint8_t *)data, size};

	nlohmann::json json;
	try
	{
		mem >> json;
	}
	catch (const nlohmann::json::parse_error& error)
	{
		std::string message = "Parse error: ";
		message += std::string(error.what());
		message += std::string("\n");
		return std::make_unique<tson::Map>(tson::ParseStatus::ParseError, message);
	}

	return std::move(parseJson(json));
}

/*!
 * Common parsing functionality for doing the json parsing
 * @param json Tiled json to parse
 * @return parsed data as Map
 */
std::unique_ptr<tson::Map> tson::Tileson::parseJson(const nlohmann::json &json)
{
	std::unique_ptr<tson::Map> map = std::make_unique<tson::Map>();
	if(map->parse(json, &m_decompressors))
		return std::move(map);

	return std::make_unique<tson::Map> (tson::ParseStatus::MissingData, "Missing map data...");
}

/*!
 * Gets the decompressor container used when something is either encoded or compressed (regardless: IDecompressor is used as base).
 * These are used specifically for tile layers, and are connected by checking the name of the IDecompressor. If the name of a decompressor
 * matches with an encoding or a compression, its decompress() function will be used.
 *
 * @return The container including all decompressors.
 */
tson::DecompressorContainer *tson::Tileson::decompressors()
{
	return &m_decompressors;
}

#endif //TILESON_TILESON_PARSER_HPP

/*** End of inlined file: tileson_parser.hpp ***/


/*** Start of inlined file: tileson_forward.hpp ***/
//
// Created by robin on 25.07.2020.
//

#ifndef TILESON_TILESON_FORWARD_HPP
#define TILESON_TILESON_FORWARD_HPP
/*!
 * T I L E S O N   F O R W A R D   D E C L A R A T I O N S
 * -------------------------------------------------------
 *
 * Due to cross-references we have forward declarations that cannot be resolved during the
 * implementation, thus the implementations must be done later when the class definition itself is known.
 *
 * All those forward declarations can be found below.
 */

// T i l e . h p p
// ---------------------

/*!
 * Really just a shortcut to retrieve the tile size from the map.
 * @return TileSize based on the map property for tile size.
 */
const tson::Vector2i tson::Tile::getTileSize() const
{
	if(m_map != nullptr)
		return m_map->getTileSize();
	else
		return {0,0};
}

/*!
 * Uses tson::Tileset and tson::Map data to calculate related values for tson::Tile.
 * Added in v1.2.0
 */
void tson::Tile::performDataCalculations()
{
	if(m_tileset == nullptr || m_map == nullptr)
		return;

	int firstId = m_tileset->getFirstgid(); //First tile id of the tileset
	int columns = m_tileset->getColumns();
	int rows = m_tileset->getTileCount() / columns;
	int lastId = (m_tileset->getFirstgid() + m_tileset->getTileCount()) - 1;

	if (getGid() >= firstId && getGid() <= lastId)
	{
		int baseTilePosition = ((int)getGid() - firstId);

		int tileModX = (baseTilePosition % columns);
		int currentRow = (baseTilePosition / columns);
		int offsetX = (tileModX != 0) ? ((tileModX) * m_map->getTileSize().x) : (0 * m_map->getTileSize().x);
		int offsetY =  (currentRow < rows-1) ? (currentRow * m_map->getTileSize().y) : ((rows-1) * m_map->getTileSize().y);

		m_drawingRect = { offsetX, offsetY, m_map->getTileSize().x, m_map->getTileSize().y };
	}
	else
		m_drawingRect = {0, 0, 0, 0};
}

/*!
 * Get the position of the tile in pixels based on the tile data position from the current layer.
 * @return The position of the tile in Pixels
 */
const tson::Vector2f tson::Tile::getPosition(const std::tuple<int, int> &tileDataPos)
{
	return {((float) std::get<0>(tileDataPos)) * m_drawingRect.width, ((float) std::get<1>(tileDataPos)) * m_drawingRect.height};
}

// T i l e O b j e c t . h p p
// ---------------------

/*!
 * In cases where the empty constructor is called, this must be called manually
 * for this class to make sense
 * @param posInTileUnits
 * @param tile
 */
void tson::TileObject::initialize(const std::tuple<int, int> &posInTileUnits, tson::Tile *tile)
{
	m_tile = tile;
	m_posInTileUnits = tile->getPositionInTileUnits(posInTileUnits);
	m_position = tile->getPosition(posInTileUnits);
}

const tson::Rect &tson::TileObject::getDrawingRect() const
{
	return m_tile->getDrawingRect();
}

// L a y e r . h p p
// -------------------

/*!
 * Decompresses data if there are matching decompressors
 */
void tson::Layer::decompressData()
{

	tson::DecompressorContainer *container = m_map->getDecompressors();
	if(container->empty())
		return;

	if(m_encoding.empty() && m_compression.empty())
		return;

	std::string data = m_base64Data;
	bool hasBeenDecoded = false;
	if(!m_encoding.empty() && container->contains(m_encoding))
	{
		data = container->get(m_encoding)->decompress(data);
		hasBeenDecoded = true;
	}

	if(!m_compression.empty() && container->contains(m_compression))
	{
		data = container->get(m_compression)->decompress(data);
	}

	if(hasBeenDecoded)
	{
		std::vector<uint8_t> bytes = tson::Tools::Base64DecodedStringToBytes(data);
		m_data = tson::Tools::BytesToUnsignedInts(bytes);
	}
}

// W o r l d . h p p
// ------------------

/*!
 * Loads the actual maps based on the world data.
 * @param parser A Tileson object used for parsing the maps of the world.
 * @return How many maps who were parsed. Remember to call getStatus() for the actual map to find out if everything went okay.
 */
#ifndef DISABLE_CPP17_FILESYSTEM
int tson::World::loadMaps(tson::Tileson *parser)
{
	m_maps.clear();
	std::for_each(m_mapData.begin(), m_mapData.end(), [&](const tson::WorldMapData &data)
	{
		if(fs::exists(data.path))
		{
			std::unique_ptr<tson::Map> map = parser->parse(data.path);
			m_maps.push_back(std::move(map));
		}
	});

	return m_maps.size();
}
#endif

#endif //TILESON_TILESON_FORWARD_HPP

/*** End of inlined file: tileson_forward.hpp ***/

#endif //TILESON_TILESON_MIN_H

