#pragma once
#ifndef STARDUST_LOCALISATION_H
#define STARDUST_LOCALISATION_H

#include <functional>
#include <type_traits>

#include <harfbuzz/hb.h>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace localisation
    {
        enum class Direction
            : std::underlying_type_t<hb_direction_t>
        {
            LeftToRight = HB_DIRECTION_LTR,
            RightToLeft = HB_DIRECTION_RTL,
            TopToBottom = HB_DIRECTION_TTB,
            BottomToTop = HB_DIRECTION_BTT,
        };

        enum class Script
            : std::underlying_type_t<hb_script_t>
        {
            Common = HB_SCRIPT_COMMON,
            Inherited = HB_SCRIPT_INHERITED,
            Unknown = HB_SCRIPT_UNKNOWN,

            Arabic = HB_SCRIPT_ARABIC,
            Armenian = HB_SCRIPT_ARMENIAN,
            Bengali = HB_SCRIPT_BENGALI,
            Cyrillic = HB_SCRIPT_CYRILLIC,
            Devanagari = HB_SCRIPT_DEVANAGARI,
            Georgian = HB_SCRIPT_GEORGIAN,
            Greek = HB_SCRIPT_GREEK,
            Gujarati = HB_SCRIPT_GUJARATI,
            Gurmukhi = HB_SCRIPT_GURMUKHI,
            Hangul = HB_SCRIPT_HANGUL,
            Han = HB_SCRIPT_HAN,
            Hebrew = HB_SCRIPT_HEBREW,
            Hiragana = HB_SCRIPT_HIRAGANA,
            Kannada = HB_SCRIPT_KANNADA,
            Katakana = HB_SCRIPT_KATAKANA,
            Lao = HB_SCRIPT_LAO,
            Latin = HB_SCRIPT_LATIN,
            Malayalam = HB_SCRIPT_MALAYALAM,
            Oriya = HB_SCRIPT_ORIYA,
            Tamil = HB_SCRIPT_TAMIL,
            Telugu = HB_SCRIPT_TELUGU,
            Thai = HB_SCRIPT_THAI,

            Tibetan = HB_SCRIPT_TIBETAN,

            Bopomofo = HB_SCRIPT_BOPOMOFO,
            Braille = HB_SCRIPT_BRAILLE,
            CanadianSyllabics = HB_SCRIPT_CANADIAN_SYLLABICS,
            Cherokee = HB_SCRIPT_CHEROKEE,
            Ethiopic = HB_SCRIPT_ETHIOPIC,
            Khmer = HB_SCRIPT_KHMER,
            Monoglian = HB_SCRIPT_MONGOLIAN,
            Myanmar = HB_SCRIPT_MYANMAR,
            Ogham = HB_SCRIPT_OGHAM,
            Runic = HB_SCRIPT_RUNIC,
            Sinhala = HB_SCRIPT_SINHALA,
            Syriac = HB_SCRIPT_SYRIAC,
            Thaana = HB_SCRIPT_THAANA,
            Yi = HB_SCRIPT_YI,

            Deseret = HB_SCRIPT_DESERET,
            Gothic = HB_SCRIPT_GOTHIC,
            OldItalic = HB_SCRIPT_OLD_ITALIC,

            Buhid = HB_SCRIPT_BUHID,
            Hanunoo = HB_SCRIPT_HANUNOO,
            Tagalog = HB_SCRIPT_TAGALOG,
            Tagbanwa = HB_SCRIPT_TAGBANWA,

            Cypriot = HB_SCRIPT_CYPRIOT,
            Limbu = HB_SCRIPT_LIMBU,
            LinearB = HB_SCRIPT_LINEAR_B,
            Osmanya = HB_SCRIPT_OSMANYA,
            Shavian = HB_SCRIPT_SHAVIAN,
            TaiLe = HB_SCRIPT_TAI_LE,
            Ugaritic = HB_SCRIPT_UGARITIC,

            Buginese = HB_SCRIPT_BUGINESE,
            Coptic = HB_SCRIPT_COPTIC,
            Glagolitic = HB_SCRIPT_GLAGOLITIC,
            Kharoshthi = HB_SCRIPT_KHAROSHTHI,
            NewTaiLue = HB_SCRIPT_NEW_TAI_LUE,
            OldPersian = HB_SCRIPT_OLD_PERSIAN,
            SylotiNagri = HB_SCRIPT_SYLOTI_NAGRI,
            Tifinagh = HB_SCRIPT_TIFINAGH,

            Balinese = HB_SCRIPT_BALINESE,
            Cuneiform = HB_SCRIPT_CUNEIFORM,
            NKo = HB_SCRIPT_NKO,
            PhagsPa = HB_SCRIPT_PHAGS_PA,
            Phoenician = HB_SCRIPT_PHOENICIAN,

            Carian = HB_SCRIPT_CARIAN,
            Cham = HB_SCRIPT_CHAM,
            KayahLi = HB_SCRIPT_KAYAH_LI,
            Lepcha = HB_SCRIPT_LEPCHA,
            Lycian = HB_SCRIPT_LYCIAN,
            Lydian = HB_SCRIPT_LYDIAN,
            OlChiki = HB_SCRIPT_OL_CHIKI,
            Rejang = HB_SCRIPT_REJANG,
            Saurashtra = HB_SCRIPT_SAURASHTRA,
            Sudanese = HB_SCRIPT_SUNDANESE,
            Vai = HB_SCRIPT_VAI,

            Avestan = HB_SCRIPT_AVESTAN,
            Bamum = HB_SCRIPT_BAMUM,
            EgyptianHieroglyphs = HB_SCRIPT_EGYPTIAN_HIEROGLYPHS,
            ImperialAramaic = HB_SCRIPT_IMPERIAL_ARAMAIC,
            InscriptionalPahlavi = HB_SCRIPT_INSCRIPTIONAL_PAHLAVI,
            InscriptionalParthian = HB_SCRIPT_INSCRIPTIONAL_PARTHIAN,
            Javanese = HB_SCRIPT_JAVANESE,
            Kaithi = HB_SCRIPT_KAITHI,
            Lisu = HB_SCRIPT_LISU,
            MeeteiMayek = HB_SCRIPT_MEETEI_MAYEK,
            OldSouthArabian = HB_SCRIPT_OLD_SOUTH_ARABIAN,
            OldTurkic = HB_SCRIPT_OLD_TURKIC,
            Samaritan = HB_SCRIPT_SAMARITAN,
            TaiTham = HB_SCRIPT_TAI_THAM,
            TaiViet = HB_SCRIPT_TAI_VIET,

            Batak = HB_SCRIPT_BATAK,
            Brahmi = HB_SCRIPT_BRAHMI,
            Mandaic = HB_SCRIPT_MANDAIC,

            Chakma = HB_SCRIPT_CHAKMA,
            MeroiticCursive = HB_SCRIPT_MEROITIC_CURSIVE,
            MeroiticHieroglyphs = HB_SCRIPT_MEROITIC_HIEROGLYPHS,
            Miao = HB_SCRIPT_MIAO,
            Sharada = HB_SCRIPT_SHARADA,
            SoraSompeng = HB_SCRIPT_SORA_SOMPENG,
            Takri = HB_SCRIPT_TAKRI,

            BassaVah = HB_SCRIPT_BASSA_VAH,
            CaucasianAlbanian = HB_SCRIPT_CAUCASIAN_ALBANIAN,
            Duployan = HB_SCRIPT_DUPLOYAN,
            Elbasan = HB_SCRIPT_ELBASAN,
            Grantha = HB_SCRIPT_GRANTHA,
            Khojki = HB_SCRIPT_KHOJKI,
            Khudawadi = HB_SCRIPT_KHUDAWADI,
            LinearA = HB_SCRIPT_LINEAR_A,
            Mahajani = HB_SCRIPT_MAHAJANI,
            Manichaean = HB_SCRIPT_MANICHAEAN,
            MendeKikakui = HB_SCRIPT_MENDE_KIKAKUI,
            Modi = HB_SCRIPT_MODI,
            Mro = HB_SCRIPT_MRO,
            Nabataean = HB_SCRIPT_NABATAEAN,
            OldNorthArabian = HB_SCRIPT_OLD_NORTH_ARABIAN,
            OldPermic = HB_SCRIPT_OLD_PERMIC,
            PahawhHmong = HB_SCRIPT_PAHAWH_HMONG,
            Palmyrene = HB_SCRIPT_PALMYRENE,
            PauCinHau = HB_SCRIPT_PAU_CIN_HAU,
            PsalterPahlavi = HB_SCRIPT_PSALTER_PAHLAVI,
            Siddham = HB_SCRIPT_SIDDHAM,
            Tirhuta = HB_SCRIPT_TIRHUTA,
            WarangCiti = HB_SCRIPT_WARANG_CITI,

            Ahom = HB_SCRIPT_AHOM,
            AnatolianHieroglyphs = HB_SCRIPT_ANATOLIAN_HIEROGLYPHS,
            Hatran = HB_SCRIPT_HATRAN,
            Multani = HB_SCRIPT_MULTANI,
            OldHungarian = HB_SCRIPT_OLD_HUNGARIAN,
            SignWriting = HB_SCRIPT_SIGNWRITING,

            Adlam = HB_SCRIPT_ADLAM,
            Bhaiksuki = HB_SCRIPT_BHAIKSUKI,
            Marchen = HB_SCRIPT_MARCHEN,
            Osage = HB_SCRIPT_OSAGE,
            Tangut = HB_SCRIPT_TANGUT,
            Newa = HB_SCRIPT_NEWA,

            MasaramGondi = HB_SCRIPT_MASARAM_GONDI,
            Nushu = HB_SCRIPT_NUSHU,
            Soyombo = HB_SCRIPT_SOYOMBO,
            ZanabazarSquare = HB_SCRIPT_ZANABAZAR_SQUARE,

            Dogra = HB_SCRIPT_DOGRA,
            GunjalaGondi = HB_SCRIPT_GUNJALA_GONDI,
            HanifiRohingya = HB_SCRIPT_HANIFI_ROHINGYA,
            Makasar = HB_SCRIPT_MAKASAR,
            Medefaidrin = HB_SCRIPT_MEDEFAIDRIN,
            OldSogdian = HB_SCRIPT_OLD_SOGDIAN,
            Sogdian = HB_SCRIPT_SOGDIAN,

            Elymaic = HB_SCRIPT_ELYMAIC,
            Nandinagari = HB_SCRIPT_NANDINAGARI,
            NyiakengPuachueHmong = HB_SCRIPT_NYIAKENG_PUACHUE_HMONG,
            Wancho = HB_SCRIPT_WANCHO,

            Chorasmian = HB_SCRIPT_CHORASMIAN,
            DivesAkuru = HB_SCRIPT_DIVES_AKURU,
            KhitanSmallScript = HB_SCRIPT_KHITAN_SMALL_SCRIPT,
            Yezidi = HB_SCRIPT_YEZIDI,

            CyproMinoan = HB_SCRIPT_CYPRO_MINOAN,
            OldUyghur = HB_SCRIPT_OLD_UYGHUR,
            Tangsa = HB_SCRIPT_TANGSA,
            Toto = HB_SCRIPT_TOTO,
            Vithkuqi = HB_SCRIPT_VITHKUQI,

            Math = HB_SCRIPT_MATH,

            Invalid = HB_SCRIPT_INVALID,
        };

        struct TextLocalisationInfo final
        {
            Direction direction = Direction::LeftToRight;
            Script script = Script::Latin;
            String language = "en";

            bool isStartOfText = true;
            bool isEndOfText = true;

            [[nodiscard]] auto operator ==(const TextLocalisationInfo&) const noexcept -> bool = default;
            [[nodiscard]] auto operator !=(const TextLocalisationInfo&) const noexcept -> bool = default;
        };

        [[nodiscard]] extern auto PerformBiDiTransformation(const String& text, const bool reverseOutput = false) -> String;
        [[nodiscard]] extern auto PerformBiDiTransformation(const UTF8String& text, const bool reverseOutput = false) -> String;
    }

    namespace l10n = localisation;
}

namespace std
{
    template <>
    struct hash<stardust::localisation::TextLocalisationInfo> final
    {
        [[nodiscard]] auto operator ()(const stardust::localisation::TextLocalisationInfo& localisationInfo) const noexcept -> stardust::usize;
    };
}

#endif