/* Portions of this code based on ICU. */
/*
 ********************************************************************************
 *   Copyright (C) 1996-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 ********************************************************************************
 */

/**
 * Trie constants, defining shift widths, index array lengths, etc.
 */
enum {
    /** Shift size for shifting right the input index. 1..9 */
    UTRIE_SHIFT=5,

    /** Number of data values in a stage 2 (data array) block. 2, 4, 8, .., 0x200 */
    UTRIE_DATA_BLOCK_LENGTH=1<<UTRIE_SHIFT,

    /** Mask for getting the lower bits from the input index. */
    UTRIE_MASK=UTRIE_DATA_BLOCK_LENGTH-1,

    /**
     * Lead surrogate code points' index displacement in the index array.
     * 0x10000-0xd800=0x2800
     */
    UTRIE_LEAD_INDEX_DISP=0x2800>>UTRIE_SHIFT,

    /**
     * Shift size for shifting left the index array values.
     * Increases possible data size with 16-bit index values at the cost
     * of compactability.
     * This requires blocks of stage 2 data to be aligned by UTRIE_DATA_GRANULARITY.
     * 0..UTRIE_SHIFT
     */
    UTRIE_INDEX_SHIFT=2,

    /** The alignment size of a stage 2 data block. Also the granularity for compaction. */
    UTRIE_DATA_GRANULARITY=1<<UTRIE_INDEX_SHIFT,

    /** Number of bits of a trail surrogate that are used in index table lookups. */
    UTRIE_SURROGATE_BLOCK_BITS=10-UTRIE_SHIFT,

    /**
     * Number of index (stage 1) entries per lead surrogate.
     * Same as number of index entries for 1024 trail surrogates,
     * ==0x400>>UTRIE_SHIFT
     */
    UTRIE_SURROGATE_BLOCK_COUNT=(1<<UTRIE_SURROGATE_BLOCK_BITS),

    /** Length of the BMP portion of the index (stage 1) array. */
    UTRIE_BMP_INDEX_LENGTH=0x10000>>UTRIE_SHIFT
};

/**
 * Runtime UTrie callback function.
 * Extract from a lead surrogate's data the
 * index array offset of the indexes for that lead surrogate.
 *
 * @param data data value for a surrogate from the trie, including the folding offset
 * @return offset>=UTRIE_BMP_INDEX_LENGTH, or 0 if there is no data for the lead surrogate
 */
typedef int32_t U_CALLCONV
UTrieGetFoldingOffset(uint32_t data);

/**
 * Run-time Trie structure.
 *
 * Either the data table is 16 bits wide and accessed via the index
 * pointer, with each index item increased by indexLength;
 * in this case, data32==NULL.
 *
 * Or the data table is 32 bits wide and accessed via the data32 pointer.
 */
struct UTrie {
    const uint16_t *index;
    const uint32_t *data32; /* NULL if 16b data is used via index */

    /**
     * This function is not used in _FROM_LEAD, _FROM_BMP, and _FROM_OFFSET_TRAIL macros.
     * If convenience macros like _GET16 or _NEXT32 are used, this function must be set.
     *
     * utrie_unserialize() sets a default function which simply returns
     * the lead surrogate's value itself - which is the inverse of the default
     * folding function used by utrie_serialize().
     *
     * @see UTrieGetFoldingOffset
     */
    UTrieGetFoldingOffset *getFoldingOffset;

    int32_t indexLength, dataLength;
    uint32_t initialValue;
    UBool isLatin1Linear;
};

typedef struct UTrie UTrie;

/** Internal trie getter from an offset (0 if c16 is a BMP/lead units) and a 16-bit unit */
#define _UTRIE_GET_RAW(trie, data, offset, c16) \
    (trie)->data[ \
        ((int32_t)((trie)->index[(offset)+((c16)>>UTRIE_SHIFT)])<<UTRIE_INDEX_SHIFT)+ \
        ((c16)&UTRIE_MASK) \
    ]

/** Internal trie getter from a pair of surrogates */
#define _UTRIE_GET_FROM_PAIR(trie, data, c, c2, result, resultType) { \
    int32_t __offset; \
\
    /* get data for lead surrogate */ \
    (result)=_UTRIE_GET_RAW((trie), data, 0, (c)); \
    __offset=(trie)->getFoldingOffset(result); \
\
    /* get the real data from the folded lead/trail units */ \
    if(__offset>0) { \
        (result)=_UTRIE_GET_RAW((trie), data, __offset, (c2)&0x3ff); \
    } else { \
        (result)=(resultType)((trie)->initialValue); \
    } \
}

/** Internal trie getter from a BMP code point, treating a lead surrogate as a normal code point */
#define _UTRIE_GET_FROM_BMP(trie, data, c16) \
    _UTRIE_GET_RAW(trie, data, 0xd800<=(c16) && (c16)<=0xdbff ? UTRIE_LEAD_INDEX_DISP : 0, c16);

/**
 * Internal trie getter from a code point.
 * Could be faster(?) but longer with
 *   if((c32)<=0xd7ff) { (result)=_UTRIE_GET_RAW(trie, data, 0, c32); }
 */
#define _UTRIE_GET(trie, data, c32, result, resultType) \
    if((uint32_t)(c32)<=0xffff) { \
        /* BMP code points */ \
        (result)=_UTRIE_GET_FROM_BMP(trie, data, c32); \
    } else if((uint32_t)(c32)<=0x10ffff) { \
        /* supplementary code point */ \
        UChar __lead16=UTF16_LEAD(c32); \
        _UTRIE_GET_FROM_PAIR(trie, data, __lead16, c32, result, resultType); \
    } else { \
        /* out of range */ \
        (result)=(resultType)((trie)->initialValue); \
    }

/**
 * Get a 16-bit trie value from a code point.
 * Even lead surrogate code points are treated as normal code points,
 * with unfolded values that may differ from _FROM_LEAD() macro results for them.
 *
 * @param trie (const UTrie *, in) a pointer to the runtime trie structure
 * @param c32 (UChar32, in) the input code point
 * @param result (uint16_t, out) uint16_t variable for the trie lookup result
 */
#define UTRIE_GET16(trie, c32, result) _UTRIE_GET(trie, index, c32, result, uint16_t)

/**
 * Trie data structure in serialized form:
 *
 * UTrieHeader header;
 * uint16_t index[header.indexLength];
 * uint16_t data[header.dataLength];
 * @internal
 */
typedef struct UTrieHeader {
    /** "Trie" in big-endian US-ASCII (0x54726965) */
    uint32_t signature;

    /**
     * options bit field:
     *     9    1=Latin-1 data is stored linearly at data+UTRIE_DATA_BLOCK_LENGTH
     *     8    0=16-bit data, 1=32-bit data
     *  7..4    UTRIE_INDEX_SHIFT   // 0..UTRIE_SHIFT
     *  3..0    UTRIE_SHIFT         // 1..9
     */
    uint32_t options;

    /** indexLength is a multiple of UTRIE_SURROGATE_BLOCK_COUNT */
    int32_t indexLength;

    /** dataLength>=UTRIE_DATA_BLOCK_LENGTH */
    int32_t dataLength;
} UTrieHeader;

/**
 * Constants for use with UTrieHeader.options.
 * @internal
 */
enum {
    /** Mask to get the UTRIE_SHIFT value from options. */
    UTRIE_OPTIONS_SHIFT_MASK=0xf,

    /** Shift options right this much to get the UTRIE_INDEX_SHIFT value. */
    UTRIE_OPTIONS_INDEX_SHIFT=4,

    /** If set, then the data (stage 2) array is 32 bits wide. */
    UTRIE_OPTIONS_DATA_IS_32_BIT=0x100,

    /**
     * If set, then Latin-1 data (for U+0000..U+00ff) is stored in the data (stage 2) array
     * as a simple, linear array at data+UTRIE_DATA_BLOCK_LENGTH.
     */
    UTRIE_OPTIONS_LATIN1_IS_LINEAR=0x200
};

/* inverse to defaultGetFoldedValue() */
U_CAPI int32_t U_EXPORT2
utrie_defaultGetFoldingOffset2(uint32_t data) {
    return (int32_t)data;
}

U_CAPI int32_t U_EXPORT2
utrie_unserialize(UTrie *trie, const void *data, int32_t length, UErrorCode *pErrorCode) {
    const UTrieHeader *header;
    const uint16_t *p16;
    uint32_t options;

    if(pErrorCode==NULL || U_FAILURE(*pErrorCode)) {
        return -1;
    }

    /* enough data for a trie header? */
    if(length<sizeof(UTrieHeader)) {
        *pErrorCode=U_INVALID_FORMAT_ERROR;
        return -1;
    }

    /* check the signature */
    header=(const UTrieHeader *)data;
    if(header->signature!=0x54726965) {
        *pErrorCode=U_INVALID_FORMAT_ERROR;
        return -1;
    }

    /* get the options and check the shift values */
    options=header->options;
    if( (options&UTRIE_OPTIONS_SHIFT_MASK)!=UTRIE_SHIFT ||
        ((options>>UTRIE_OPTIONS_INDEX_SHIFT)&UTRIE_OPTIONS_SHIFT_MASK)!=UTRIE_INDEX_SHIFT
    ) {
        *pErrorCode=U_INVALID_FORMAT_ERROR;
        return -1;
    }
    trie->isLatin1Linear= (UBool)((options&UTRIE_OPTIONS_LATIN1_IS_LINEAR)!=0);

    /* get the length values */
    trie->indexLength=header->indexLength;
    trie->dataLength=header->dataLength;

    length-=(int32_t)sizeof(UTrieHeader);

    /* enough data for the index? */
    if(length<2*trie->indexLength) {
        *pErrorCode=U_INVALID_FORMAT_ERROR;
        return -1;
    }
    p16=(const uint16_t *)(header+1);
    trie->index=p16;
    p16+=trie->indexLength;
    length-=2*trie->indexLength;

    /* get the data */
    if(options&UTRIE_OPTIONS_DATA_IS_32_BIT) {
        if(length<4*trie->dataLength) {
            *pErrorCode=U_INVALID_FORMAT_ERROR;
            return -1;
        }
        trie->data32=(const uint32_t *)p16;
        trie->initialValue=trie->data32[0];
        length=(int32_t)sizeof(UTrieHeader)+2*trie->indexLength+4*trie->dataLength;
    } else {
        if(length<2*trie->dataLength) {
            *pErrorCode=U_INVALID_FORMAT_ERROR;
            return -1;
        }

        /* the "data16" data is used via the index pointer */
        trie->data32=NULL;
        trie->initialValue=trie->index[trie->indexLength];
        length=(int32_t)sizeof(UTrieHeader)+2*trie->indexLength+2*trie->dataLength;
    }

    trie->getFoldingOffset=utrie_defaultGetFoldingOffset2;

    return length;
}
