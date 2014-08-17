/* Portions of this code based on ICU. */
/*
 ********************************************************************************
 *   Copyright (C) 1996-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 ********************************************************************************
 */

// Internal file
#ifndef __RBBIDATA_H__
#define __RBBIDATA_H__

#include "utypes.h"

/* icu\source\common\unicode\udata.h */
/**
 * UDataInfo contains the properties about the requested data.
 * This is meta data.
 *
 * <p>This structure may grow in the future, indicated by the
 * <code>size</code> field.</p>
 *
 * <p>The platform data property fields help determine if a data
 * file can be efficiently used on a given machine.
 * The particular fields are of importance only if the data
 * is affected by the properties - if there is integer data
 * with word sizes > 1 byte, char* text, or UChar* text.</p>
 *
 * <p>The implementation for the <code>udata_open[Choice]()</code>
 * functions may reject data based on the value in <code>isBigEndian</code>.
 * No other field is used by the <code>udata</code> API implementation.</p>
 *
 * <p>The <code>dataFormat</code> may be used to identify
 * the kind of data, e.g. a converter table.</p>
 *
 * <p>The <code>formatVersion</code> field should be used to
 * make sure that the format can be interpreted.
 * I may be a good idea to check only for the one or two highest
 * of the version elements to allow the data memory to
 * get more or somewhat rearranged contents, for as long
 * as the using code can still interpret the older contents.</p>
 *
 * <p>The <code>dataVersion</code> field is intended to be a
 * common place to store the source version of the data;
 * for data from the Unicode character database, this could
 * reflect the Unicode version.</p>
 * @stable ICU 2.0
 */
typedef struct {
    /** sizeof(UDataInfo)
     *  @stable ICU 2.0 */
    uint16_t size;

    /** unused, set to 0 
     *  @stable ICU 2.0*/
    uint16_t reservedWord;

    /* platform data properties */
    /** 0 for little-endian machine, 1 for big-endian
     *  @stable ICU 2.0 */
    uint8_t isBigEndian;

    /** see U_CHARSET_FAMILY values in utypes.h 
     *  @stable ICU 2.0*/
    uint8_t charsetFamily;

    /** sizeof(UChar), one of { 1, 2, 4 } 
     *  @stable ICU 2.0*/
    uint8_t sizeofUChar;

    /** unused, set to 0 
     *  @stable ICU 2.0*/
    uint8_t reservedByte;

    /** data format identifier 
     *  @stable ICU 2.0*/
    uint8_t dataFormat[4];

    /** versions: [0] major [1] minor [2] milli [3] micro 
     *  @stable ICU 2.0*/
    uint8_t formatVersion[4];

    /** versions: [0] major [1] minor [2] milli [3] micro 
     *  @stable ICU 2.0*/
    uint8_t dataVersion[4];
} UDataInfo;

// From icu\source\common\ucmndata.h
typedef struct  {
    uint16_t    headerSize;
    uint8_t     magic1;
    uint8_t     magic2;
} MappedData;


typedef struct  {
    MappedData  dataHeader;
    UDataInfo   info;
} DataHeader;

// From icu\source\common\rbbidata.h
/*  
 *   The following structs map exactly onto the raw data from ICU common data file. 
 */
struct RBBIDataHeader {
    uint32_t         fMagic;           /*  == 0xbla0                                               */
    uint8_t          fFormatVersion[4]; /* Data Format.  Same as the value in struct UDataInfo      */
                                       /*   if there is one associated with this data.             */
                                       /*     (version originates in rbbi, is copied to UDataInfo) */
                                       /*   For ICU 3.2 and earlier, this field was                */
                                       /*       uint32_t  fVersion                                 */
                                       /*   with a value of 1.                                     */
    uint32_t         fLength;          /*  Total length in bytes of this RBBI Data,                */
                                       /*      including all sections, not just the header.        */
    uint32_t         fCatCount;        /*  Number of character categories.                         */

    /*                                                                        */
    /*  Offsets and sizes of each of the subsections within the RBBI data.    */
    /*  All offsets are bytes from the start of the RBBIDataHeader.           */
    /*  All sizes are in bytes.                                               */
    /*                                                                        */
    uint32_t         fFTable;         /*  forward state transition table. */
    uint32_t         fFTableLen;
    uint32_t         fRTable;         /*  Offset to the reverse state transition table. */
    uint32_t         fRTableLen;
    uint32_t         fSFTable;        /*  safe point forward transition table */
    uint32_t         fSFTableLen;
    uint32_t         fSRTable;        /*  safe point reverse transition table */
    uint32_t         fSRTableLen;
    uint32_t         fTrie;           /*  Offset to Trie data for character categories */
    uint32_t         fTrieLen;
    uint32_t         fRuleSource;     /*  Offset to the source for for the break */
    uint32_t         fRuleSourceLen;  /*    rules.  Stored UChar *. */
    uint32_t         fStatusTable;    /* Offset to the table of rule status values */
    uint32_t         fStatusTableLen;

    uint32_t         fReserved[6];    /*  Reserved for expansion */

};



struct  RBBIStateTableRow {
    int16_t          fAccepting;    /*  Non-zero if this row is for an accepting state.   */
                                    /*  Value 0: not an accepting state.                  */
                                    /*       -1: Unconditional Accepting state.           */
                                    /*    positive:  Look-ahead match has completed.      */
                                    /*           Actual boundary position happened earlier */
                                    /*           Value here == fLookAhead in earlier      */
                                    /*              state, at actual boundary pos.        */
    int16_t          fLookAhead;    /*  Non-zero if this row is for a state that          */
                                    /*    corresponds to a '/' in the rule source.        */
                                    /*    Value is the same as the fAccepting             */
                                    /*      value for the rule (which will appear         */
                                    /*      in a different state.                         */
    int16_t          fTagIdx;       /*  Non-zero if this row covers a {tagged} position   */
                                    /*     from a rule.  Value is the index in the        */
                                    /*     StatusTable of the set of matching             */
                                    /*     tags (rule status values)                      */
    int16_t          fReserved;
    uint16_t         fNextState[2]; /*  Next State, indexed by char category.             */
                                    /*    Array Size is fNumCols from the                 */
                                    /*    state table header.                             */
                                    /*    CAUTION:  see RBBITableBuilder::getTableSize()  */
                                    /*              before changing anything here.        */
};


struct RBBIStateTable {
    uint32_t         fNumStates;    /*  Number of states.                                 */
    uint32_t         fRowLen;       /*  Length of a state table row, in bytes.            */
    uint32_t         fFlags;        /*  Option Flags for this state table                 */
    uint32_t         fReserved;     /*  reserved                                          */
    char             fTableData[4]; /*  First RBBIStateTableRow begins here.              */
                                    /*    (making it char[] simplifies ugly address       */
                                    /*     arithmetic for indexing variable length rows.) */
};

typedef enum {
    RBBI_LOOKAHEAD_HARD_BREAK = 1,
    RBBI_BOF_REQUIRED = 2
} RBBIStateTableFlags;

#endif // __RBBIDATA_H__
