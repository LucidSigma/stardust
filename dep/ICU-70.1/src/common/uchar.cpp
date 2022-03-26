// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/*
********************************************************************************
*   Copyright (C) 1996-2016, International Business Machines
*   Corporation and others.  All Rights Reserved.
********************************************************************************
*
* File UCHAR.C
*
* Modification History:
*
*   Date        Name        Description
*   04/02/97    aliu        Creation.
*   4/15/99     Madhu       Updated all the function definitions for C Implementation
*   5/20/99     Madhu       Added the function u_getVersion()
*   8/19/1999   srl         Upgraded scripts to Unicode3.0 
*   11/11/1999  weiv        added u_isalnum(), cleaned comments
*   01/11/2000  helena      Renamed u_getVersion to u_getUnicodeVersion.
*   06/20/2000  helena      OS/400 port changes; mostly typecast.
******************************************************************************
*/

#include "unicode/utypes.h"
#include "unicode/uchar.h"
#include "unicode/uscript.h"
#include "unicode/udata.h"
#include "uassert.h"
#include "cmemory.h"
#include "ucln_cmn.h"
#include "utrie2.h"
#include "udataswp.h"
#include "uprops.h"
#include "ustr_imp.h"

/* uchar_props_data.h is machine-generated by genprops --csource */
#define INCLUDED_FROM_UCHAR_C
#include "uchar_props_data.h"

/* constants and macros for access to the data ------------------------------ */

/* getting a uint32_t properties word from the data */
#define GET_PROPS(c, result) ((result)=UTRIE2_GET16(&propsTrie, c))

/* API functions ------------------------------------------------------------ */

/* Gets the Unicode character's general category.*/
U_CAPI int8_t U_EXPORT2
u_charType(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (int8_t)GET_CATEGORY(props);
}

/* Enumerate all code points with their general categories. */
struct _EnumTypeCallback {
    UCharEnumTypeRange *enumRange;
    const void *context;
};

static uint32_t U_CALLCONV
_enumTypeValue(const void *context, uint32_t value) {
    (void)context;
    return GET_CATEGORY(value);
}

static UBool U_CALLCONV
_enumTypeRange(const void *context, UChar32 start, UChar32 end, uint32_t value) {
    /* just cast the value to UCharCategory */
    return ((struct _EnumTypeCallback *)context)->
        enumRange(((struct _EnumTypeCallback *)context)->context,
                  start, end+1, (UCharCategory)value);
}

U_CAPI void U_EXPORT2
u_enumCharTypes(UCharEnumTypeRange *enumRange, const void *context) {
    struct _EnumTypeCallback callback;

    if(enumRange==NULL) {
        return;
    }

    callback.enumRange=enumRange;
    callback.context=context;
    utrie2_enum(&propsTrie, _enumTypeValue, _enumTypeRange, &callback);
}

/* Checks if ch is a lower case letter.*/
U_CAPI UBool U_EXPORT2
u_islower(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)==U_LOWERCASE_LETTER);
}

/* Checks if ch is an upper case letter.*/
U_CAPI UBool U_EXPORT2
u_isupper(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)==U_UPPERCASE_LETTER);
}

/* Checks if ch is a title case letter; usually upper case letters.*/
U_CAPI UBool U_EXPORT2
u_istitle(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)==U_TITLECASE_LETTER);
}

/* Checks if ch is a decimal digit. */
U_CAPI UBool U_EXPORT2
u_isdigit(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)==U_DECIMAL_DIGIT_NUMBER);
}

U_CAPI UBool U_EXPORT2
u_isxdigit(UChar32 c) {
    uint32_t props;

    /* check ASCII and Fullwidth ASCII a-fA-F */
    if(
        (c<=0x66 && c>=0x41 && (c<=0x46 || c>=0x61)) ||
        (c>=0xff21 && c<=0xff46 && (c<=0xff26 || c>=0xff41))
    ) {
        return TRUE;
    }

    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)==U_DECIMAL_DIGIT_NUMBER);
}

/* Checks if the Unicode character is a letter.*/
U_CAPI UBool U_EXPORT2
u_isalpha(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&U_GC_L_MASK)!=0);
}

U_CAPI UBool U_EXPORT2
u_isUAlphabetic(UChar32 c) {
    return (u_getUnicodeProperties(c, 1)&U_MASK(UPROPS_ALPHABETIC))!=0;
}

/* Checks if c is a letter or a decimal digit */
U_CAPI UBool U_EXPORT2
u_isalnum(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&(U_GC_L_MASK|U_GC_ND_MASK))!=0);
}

/**
 * Checks if c is alphabetic, or a decimal digit; implements UCHAR_POSIX_ALNUM.
 * @internal
 */
U_CFUNC UBool
u_isalnumPOSIX(UChar32 c) {
    return (UBool)(u_isUAlphabetic(c) || u_isdigit(c));
}

/* Checks if ch is a unicode character with assigned character type.*/
U_CAPI UBool U_EXPORT2
u_isdefined(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)!=0);
}

/* Checks if the Unicode character is a base form character that can take a diacritic.*/
U_CAPI UBool U_EXPORT2
u_isbase(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&(U_GC_L_MASK|U_GC_N_MASK|U_GC_MC_MASK|U_GC_ME_MASK))!=0);
}

/* Checks if the Unicode character is a control character.*/
U_CAPI UBool U_EXPORT2
u_iscntrl(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&(U_GC_CC_MASK|U_GC_CF_MASK|U_GC_ZL_MASK|U_GC_ZP_MASK))!=0);
}

U_CAPI UBool U_EXPORT2
u_isISOControl(UChar32 c) {
    return (uint32_t)c<=0x9f && (c<=0x1f || c>=0x7f);
}

/* Some control characters that are used as space. */
#define IS_THAT_CONTROL_SPACE(c) \
    (c<=0x9f && ((c>=TAB && c<=CR) || (c>=0x1c && c <=0x1f) || c==0x85))

/* Java has decided that U+0085 New Line is not whitespace any more. */
#define IS_THAT_ASCII_CONTROL_SPACE(c) \
    (c<=0x1f && c>=TAB && (c<=CR || c>=0x1c))

/* Checks if the Unicode character is a space character.*/
U_CAPI UBool U_EXPORT2
u_isspace(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&U_GC_Z_MASK)!=0 || IS_THAT_CONTROL_SPACE(c));
}

U_CAPI UBool U_EXPORT2
u_isJavaSpaceChar(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&U_GC_Z_MASK)!=0);
}

/* Checks if the Unicode character is a whitespace character.*/
U_CAPI UBool U_EXPORT2
u_isWhitespace(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(
                ((CAT_MASK(props)&U_GC_Z_MASK)!=0 &&
                    c!=NBSP && c!=FIGURESP && c!=NNBSP) || /* exclude no-break spaces */
                IS_THAT_ASCII_CONTROL_SPACE(c)
           );
}

U_CAPI UBool U_EXPORT2
u_isblank(UChar32 c) {
    if((uint32_t)c<=0x9f) {
        return c==9 || c==0x20; /* TAB or SPACE */
    } else {
        /* Zs */
        uint32_t props;
        GET_PROPS(c, props);
        return (UBool)(GET_CATEGORY(props)==U_SPACE_SEPARATOR);
    }
}

U_CAPI UBool U_EXPORT2
u_isUWhiteSpace(UChar32 c) {
    return (u_getUnicodeProperties(c, 1)&U_MASK(UPROPS_WHITE_SPACE))!=0;
}

/* Checks if the Unicode character is printable.*/
U_CAPI UBool U_EXPORT2
u_isprint(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    /* comparing ==0 returns FALSE for the categories mentioned */
    return (UBool)((CAT_MASK(props)&U_GC_C_MASK)==0);
}

/**
 * Checks if c is in \p{graph}\p{blank} - \p{cntrl}.
 * Implements UCHAR_POSIX_PRINT.
 * @internal
 */
U_CFUNC UBool
u_isprintPOSIX(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    /*
     * The only cntrl character in graph+blank is TAB (in blank).
     * Here we implement (blank-TAB)=Zs instead of calling u_isblank().
     */
    return (UBool)((GET_CATEGORY(props)==U_SPACE_SEPARATOR) || u_isgraphPOSIX(c));
}

U_CAPI UBool U_EXPORT2
u_isgraph(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    /* comparing ==0 returns FALSE for the categories mentioned */
    return (UBool)((CAT_MASK(props)&
                    (U_GC_CC_MASK|U_GC_CF_MASK|U_GC_CS_MASK|U_GC_CN_MASK|U_GC_Z_MASK))
                   ==0);
}

/**
 * Checks if c is in
 * [^\p{space}\p{gc=Control}\p{gc=Surrogate}\p{gc=Unassigned}]
 * with space=\p{Whitespace} and Control=Cc.
 * Implements UCHAR_POSIX_GRAPH.
 * @internal
 */
U_CFUNC UBool
u_isgraphPOSIX(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    /* \p{space}\p{gc=Control} == \p{gc=Z}\p{Control} */
    /* comparing ==0 returns FALSE for the categories mentioned */
    return (UBool)((CAT_MASK(props)&
                    (U_GC_CC_MASK|U_GC_CS_MASK|U_GC_CN_MASK|U_GC_Z_MASK))
                   ==0);
}

U_CAPI UBool U_EXPORT2
u_ispunct(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&U_GC_P_MASK)!=0);
}

/* Checks if the Unicode character can start a Unicode identifier.*/
U_CAPI UBool U_EXPORT2
u_isIDStart(UChar32 c) {
    /* same as u_isalpha() */
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&(U_GC_L_MASK|U_GC_NL_MASK))!=0);
}

/* Checks if the Unicode character can be a Unicode identifier part other than starting the
 identifier.*/
U_CAPI UBool U_EXPORT2
u_isIDPart(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(
           (CAT_MASK(props)&
            (U_GC_ND_MASK|U_GC_NL_MASK|
             U_GC_L_MASK|
             U_GC_PC_MASK|U_GC_MC_MASK|U_GC_MN_MASK)
           )!=0 ||
           u_isIDIgnorable(c));
}

/*Checks if the Unicode character can be ignorable in a Java or Unicode identifier.*/
U_CAPI UBool U_EXPORT2
u_isIDIgnorable(UChar32 c) {
    if(c<=0x9f) {
        return u_isISOControl(c) && !IS_THAT_ASCII_CONTROL_SPACE(c);
    } else {
        uint32_t props;
        GET_PROPS(c, props);
        return (UBool)(GET_CATEGORY(props)==U_FORMAT_CHAR);
    }
}

/*Checks if the Unicode character can start a Java identifier.*/
U_CAPI UBool U_EXPORT2
u_isJavaIDStart(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&(U_GC_L_MASK|U_GC_SC_MASK|U_GC_PC_MASK))!=0);
}

/*Checks if the Unicode character can be a Java identifier part other than starting the
 * identifier.
 */
U_CAPI UBool U_EXPORT2
u_isJavaIDPart(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(
           (CAT_MASK(props)&
            (U_GC_ND_MASK|U_GC_NL_MASK|
             U_GC_L_MASK|
             U_GC_SC_MASK|U_GC_PC_MASK|
             U_GC_MC_MASK|U_GC_MN_MASK)
           )!=0 ||
           u_isIDIgnorable(c));
}

U_CAPI int32_t U_EXPORT2
u_charDigitValue(UChar32 c) {
    uint32_t props;
    int32_t value;
    GET_PROPS(c, props);
    value=(int32_t)GET_NUMERIC_TYPE_VALUE(props)-UPROPS_NTV_DECIMAL_START;
    if(value<=9) {
        return value;
    } else {
        return -1;
    }
}

U_CAPI double U_EXPORT2
u_getNumericValue(UChar32 c) {
    uint32_t props;
    int32_t ntv;
    GET_PROPS(c, props);
    ntv=(int32_t)GET_NUMERIC_TYPE_VALUE(props);

    if(ntv==UPROPS_NTV_NONE) {
        return U_NO_NUMERIC_VALUE;
    } else if(ntv<UPROPS_NTV_DIGIT_START) {
        /* decimal digit */
        return ntv-UPROPS_NTV_DECIMAL_START;
    } else if(ntv<UPROPS_NTV_NUMERIC_START) {
        /* other digit */
        return ntv-UPROPS_NTV_DIGIT_START;
    } else if(ntv<UPROPS_NTV_FRACTION_START) {
        /* small integer */
        return ntv-UPROPS_NTV_NUMERIC_START;
    } else if(ntv<UPROPS_NTV_LARGE_START) {
        /* fraction */
        int32_t numerator=(ntv>>4)-12;
        int32_t denominator=(ntv&0xf)+1;
        return (double)numerator/denominator;
    } else if(ntv<UPROPS_NTV_BASE60_START) {
        /* large, single-significant-digit integer */
        double numValue;
        int32_t mant=(ntv>>5)-14;
        int32_t exp=(ntv&0x1f)+2;
        numValue=mant;

        /* multiply by 10^exp without math.h */
        while(exp>=4) {
            numValue*=10000.;
            exp-=4;
        }
        switch(exp) {
        case 3:
            numValue*=1000.;
            break;
        case 2:
            numValue*=100.;
            break;
        case 1:
            numValue*=10.;
            break;
        case 0:
        default:
            break;
        }

        return numValue;
    } else if(ntv<UPROPS_NTV_FRACTION20_START) {
        /* sexagesimal (base 60) integer */
        int32_t numValue=(ntv>>2)-0xbf;
        int32_t exp=(ntv&3)+1;

        switch(exp) {
        case 4:
            numValue*=60*60*60*60;
            break;
        case 3:
            numValue*=60*60*60;
            break;
        case 2:
            numValue*=60*60;
            break;
        case 1:
            numValue*=60;
            break;
        case 0:
        default:
            break;
        }

        return numValue;
    } else if(ntv<UPROPS_NTV_FRACTION32_START) {
        // fraction-20 e.g. 3/80
        int32_t frac20=ntv-UPROPS_NTV_FRACTION20_START;  // 0..0x17
        int32_t numerator=2*(frac20&3)+1;
        int32_t denominator=20<<(frac20>>2);
        return (double)numerator/denominator;
    } else if(ntv<UPROPS_NTV_RESERVED_START) {
        // fraction-32 e.g. 3/64
        int32_t frac32=ntv-UPROPS_NTV_FRACTION32_START;  // 0..15
        int32_t numerator=2*(frac32&3)+1;
        int32_t denominator=32<<(frac32>>2);
        return (double)numerator/denominator;
    } else {
        /* reserved */
        return U_NO_NUMERIC_VALUE;
    }
}

U_CAPI int32_t U_EXPORT2
u_digit(UChar32 ch, int8_t radix) {
    int8_t value;
    if((uint8_t)(radix-2)<=(36-2)) {
        value=(int8_t)u_charDigitValue(ch);
        if(value<0) {
            /* ch is not a decimal digit, try latin letters */
            if(ch>=0x61 && ch<=0x7A) {
                value=(int8_t)(ch-0x57);  /* ch - 'a' + 10 */
            } else if(ch>=0x41 && ch<=0x5A) {
                value=(int8_t)(ch-0x37);  /* ch - 'A' + 10 */
            } else if(ch>=0xFF41 && ch<=0xFF5A) {
                value=(int8_t)(ch-0xFF37);  /* fullwidth ASCII a-z */
            } else if(ch>=0xFF21 && ch<=0xFF3A) {
                value=(int8_t)(ch-0xFF17);  /* fullwidth ASCII A-Z */
            }
        }
    } else {
        value=-1;   /* invalid radix */
    }
    return (int8_t)((value<radix) ? value : -1);
}

U_CAPI UChar32 U_EXPORT2
u_forDigit(int32_t digit, int8_t radix) {
    if((uint8_t)(radix-2)>(36-2) || (uint32_t)digit>=(uint32_t)radix) {
        return 0;
    } else if(digit<10) {
        return (UChar32)(0x30+digit);
    } else {
        return (UChar32)((0x61-10)+digit);
    }
}

/* miscellaneous, and support for uprops.cpp -------------------------------- */

U_CAPI void U_EXPORT2
u_getUnicodeVersion(UVersionInfo versionArray) {
    if(versionArray!=NULL) {
        uprv_memcpy(versionArray, dataVersion, U_MAX_VERSION_LENGTH);
    }
}

U_CFUNC uint32_t
u_getMainProperties(UChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return props;
}

U_CFUNC uint32_t
u_getUnicodeProperties(UChar32 c, int32_t column) {
    U_ASSERT(column>=0);
    if(column>=propsVectorsColumns) {
        return 0;
    } else {
        uint16_t vecIndex=UTRIE2_GET16(&propsVectorsTrie, c);
        return propsVectors[vecIndex+column];
    }
}

U_CFUNC int32_t
uprv_getMaxValues(int32_t column) {
    switch(column) {
    case 0:
        return indexes[UPROPS_MAX_VALUES_INDEX];
    case 2:
        return indexes[UPROPS_MAX_VALUES_2_INDEX];
    default:
        return 0;
    }
}

U_CAPI void U_EXPORT2
u_charAge(UChar32 c, UVersionInfo versionArray) {
    if(versionArray!=NULL) {
        uint32_t version=u_getUnicodeProperties(c, 0)>>UPROPS_AGE_SHIFT;
        versionArray[0]=(uint8_t)(version>>4);
        versionArray[1]=(uint8_t)(version&0xf);
        versionArray[2]=versionArray[3]=0;
    }
}

U_CAPI UScriptCode U_EXPORT2
uscript_getScript(UChar32 c, UErrorCode *pErrorCode) {
    if(pErrorCode==NULL || U_FAILURE(*pErrorCode)) {
        return USCRIPT_INVALID_CODE;
    }
    if((uint32_t)c>0x10ffff) {
        *pErrorCode=U_ILLEGAL_ARGUMENT_ERROR;
        return USCRIPT_INVALID_CODE;
    }
    uint32_t scriptX=u_getUnicodeProperties(c, 0)&UPROPS_SCRIPT_X_MASK;
    uint32_t codeOrIndex=uprops_mergeScriptCodeOrIndex(scriptX);
    if(scriptX<UPROPS_SCRIPT_X_WITH_COMMON) {
        return (UScriptCode)codeOrIndex;
    } else if(scriptX<UPROPS_SCRIPT_X_WITH_INHERITED) {
        return USCRIPT_COMMON;
    } else if(scriptX<UPROPS_SCRIPT_X_WITH_OTHER) {
        return USCRIPT_INHERITED;
    } else {
        return (UScriptCode)scriptExtensions[codeOrIndex];
    }
}

U_CAPI UBool U_EXPORT2
uscript_hasScript(UChar32 c, UScriptCode sc) {
    uint32_t scriptX=u_getUnicodeProperties(c, 0)&UPROPS_SCRIPT_X_MASK;
    uint32_t codeOrIndex=uprops_mergeScriptCodeOrIndex(scriptX);
    if(scriptX<UPROPS_SCRIPT_X_WITH_COMMON) {
        return sc==(UScriptCode)codeOrIndex;
    }

    const uint16_t *scx=scriptExtensions+codeOrIndex;
    if(scriptX>=UPROPS_SCRIPT_X_WITH_OTHER) {
        scx=scriptExtensions+scx[1];
    }
    uint32_t sc32=sc;
    if(sc32>0x7fff) {
        /* Guard against bogus input that would make us go past the Script_Extensions terminator. */
        return FALSE;
    }
    while(sc32>*scx) {
        ++scx;
    }
    return sc32==(*scx&0x7fff);
}

U_CAPI int32_t U_EXPORT2
uscript_getScriptExtensions(UChar32 c,
                            UScriptCode *scripts, int32_t capacity,
                            UErrorCode *pErrorCode) {
    if(pErrorCode==NULL || U_FAILURE(*pErrorCode)) {
        return 0;
    }
    if(capacity<0 || (capacity>0 && scripts==NULL)) {
        *pErrorCode=U_ILLEGAL_ARGUMENT_ERROR;
        return 0;
    }
    uint32_t scriptX=u_getUnicodeProperties(c, 0)&UPROPS_SCRIPT_X_MASK;
    uint32_t codeOrIndex=uprops_mergeScriptCodeOrIndex(scriptX);
    if(scriptX<UPROPS_SCRIPT_X_WITH_COMMON) {
        if(capacity==0) {
            *pErrorCode=U_BUFFER_OVERFLOW_ERROR;
        } else {
            scripts[0]=(UScriptCode)codeOrIndex;
        }
        return 1;
    }

    const uint16_t *scx=scriptExtensions+codeOrIndex;
    if(scriptX>=UPROPS_SCRIPT_X_WITH_OTHER) {
        scx=scriptExtensions+scx[1];
    }
    int32_t length=0;
    uint16_t sx;
    do {
        sx=*scx++;
        if(length<capacity) {
            scripts[length]=(UScriptCode)(sx&0x7fff);
        }
        ++length;
    } while(sx<0x8000);
    if(length>capacity) {
        *pErrorCode=U_BUFFER_OVERFLOW_ERROR;
    }
    return length;
}

U_CAPI UBlockCode U_EXPORT2
ublock_getCode(UChar32 c) {
    return (UBlockCode)((u_getUnicodeProperties(c, 0)&UPROPS_BLOCK_MASK)>>UPROPS_BLOCK_SHIFT);
}

/* property starts for UnicodeSet ------------------------------------------- */

static UBool U_CALLCONV
_enumPropertyStartsRange(const void *context, UChar32 start, UChar32 end, uint32_t value) {
    /* add the start code point to the USet */
    const USetAdder *sa=(const USetAdder *)context;
    sa->add(sa->set, start);
    (void)end;
    (void)value;
    return TRUE;
}

#define USET_ADD_CP_AND_NEXT(sa, cp) sa->add(sa->set, cp); sa->add(sa->set, cp+1)

U_CFUNC void U_EXPORT2
uchar_addPropertyStarts(const USetAdder *sa, UErrorCode *pErrorCode) {
    if(U_FAILURE(*pErrorCode)) {
        return;
    }

    /* add the start code point of each same-value range of the main trie */
    utrie2_enum(&propsTrie, NULL, _enumPropertyStartsRange, sa);

    /* add code points with hardcoded properties, plus the ones following them */

    /* add for u_isblank() */
    USET_ADD_CP_AND_NEXT(sa, TAB);

    /* add for IS_THAT_CONTROL_SPACE() */
    sa->add(sa->set, CR+1); /* range TAB..CR */
    sa->add(sa->set, 0x1c);
    sa->add(sa->set, 0x1f+1);
    USET_ADD_CP_AND_NEXT(sa, 0x85);  // NEXT LINE (NEL)

    /* add for u_isIDIgnorable() what was not added above */
    sa->add(sa->set, 0x7f); /* range DEL..NBSP-1, NBSP added below */
    sa->add(sa->set, HAIRSP);
    sa->add(sa->set, RLM+1);
    sa->add(sa->set, 0x206a);  // INHIBIT SYMMETRIC SWAPPING
    sa->add(sa->set, 0x206f+1);  // NOMINAL DIGIT SHAPES
    USET_ADD_CP_AND_NEXT(sa, ZWNBSP);

    /* add no-break spaces for u_isWhitespace() what was not added above */
    USET_ADD_CP_AND_NEXT(sa, NBSP);
    USET_ADD_CP_AND_NEXT(sa, FIGURESP);
    USET_ADD_CP_AND_NEXT(sa, NNBSP);

    /* add for u_digit() */
    sa->add(sa->set, u'a');
    sa->add(sa->set, u'z'+1);
    sa->add(sa->set, u'A');
    sa->add(sa->set, u'Z'+1);
    // fullwidth
    sa->add(sa->set, u"ａ"[0u]);
    sa->add(sa->set, u"ｚ"[0u] +1);
    sa->add(sa->set, u"Ａ"[0u]);
    sa->add(sa->set, u"Ｚ"[0u] + 1);

    /* add for u_isxdigit() */
    sa->add(sa->set, u'f'+1);
    sa->add(sa->set, u'F'+1);
    // fullwidth
    sa->add(sa->set, u"ｆ"[0u] +1);
    sa->add(sa->set, u"Ｆ"[0u]+1);

    /* add for UCHAR_DEFAULT_IGNORABLE_CODE_POINT what was not added above */
    sa->add(sa->set, 0x2060); /* range 2060..206f */
    sa->add(sa->set, 0xfff0);
    sa->add(sa->set, 0xfffb+1);
    sa->add(sa->set, 0xe0000);
    sa->add(sa->set, 0xe0fff+1);

    /* add for UCHAR_GRAPHEME_BASE and others */
    USET_ADD_CP_AND_NEXT(sa, CGJ);
}

U_CFUNC void U_EXPORT2
upropsvec_addPropertyStarts(const USetAdder *sa, UErrorCode *pErrorCode) {
    if(U_FAILURE(*pErrorCode)) {
        return;
    }

    /* add the start code point of each same-value range of the properties vectors trie */
    utrie2_enum(&propsVectorsTrie, NULL, _enumPropertyStartsRange, sa);
}
