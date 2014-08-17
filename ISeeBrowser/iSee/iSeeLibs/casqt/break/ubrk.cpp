/* Portions of this code based on ICU. */
/*
 ********************************************************************************
 *   Copyright (C) 1996-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 ********************************************************************************
 */

#include "rbbidata.h"
#include "ubrk.h"
#include "utils.h"

#define U_CALLCONV

#include "unistr.cpp"
#include "utrie.cpp"
#include "rbbi.cpp" // From rbbidata.cpp

#include "brkdata.c"
//------------------------------------------------------------------------------
//
//    ubrk_open      Create a canned type of break iterator based on type (word, line, etc.)
//                   and locale.
//
//------------------------------------------------------------------------------
U_CAPI UBreakIterator* U_EXPORT2
ubrk_open(UBreakIteratorType type,
      const char *locale,
      const UChar *text,
      int32_t textLength,
      UErrorCode *status)
{
	DataHeader* pDataHeader = 0;
	switch (type)
	{
	case UBRK_CHARACTER:
		pDataHeader = (DataHeader *)charBrkData;
        break;
    case UBRK_WORD:
		pDataHeader = (DataHeader *)wordBrkData;
        break;
    case UBRK_LINE:
		pDataHeader = (DataHeader *)lineBrkData;
        break;
    case UBRK_SENTENCE:
		pDataHeader = (DataHeader *)sentBrkData;
        break;
	default:
		return 0;
	}
	return (UBreakIterator*)(new BreakIterator(pDataHeader, *status));
}

U_CAPI void U_EXPORT2
ubrk_close(UBreakIterator *bi)
{
    BreakIterator *ubi = (BreakIterator*) bi;
    if (ubi) delete ubi;
}

U_CAPI void U_EXPORT2
ubrk_setText(UBreakIterator* bi,
             const UChar*    text,
             int32_t         textLength,
             UErrorCode*     status)
{
    ((BreakIterator *)bi)->setText(text, textLength, *status);
}


U_CAPI int32_t U_EXPORT2
ubrk_current(const UBreakIterator *bi)
{
	return ((BreakIterator *)bi)->current();
}

U_CAPI int32_t U_EXPORT2
ubrk_next(UBreakIterator *bi)
{
	return ((BreakIterator *)bi)->next();
}

U_CAPI int32_t U_EXPORT2
ubrk_previous(UBreakIterator *bi)
{
	return ((BreakIterator *)bi)->previous();
}

U_CAPI int32_t U_EXPORT2
ubrk_first(UBreakIterator *bi)
{
	return ((BreakIterator *)bi)->first();
}

U_CAPI int32_t U_EXPORT2
ubrk_last(UBreakIterator *bi)
{
	return ((BreakIterator *)bi)->last();
}

U_CAPI int32_t U_EXPORT2
ubrk_preceding(UBreakIterator *bi,
           int32_t offset)
{
	return ((BreakIterator *)bi)->preceding(offset);
}

U_CAPI int32_t U_EXPORT2
ubrk_following(UBreakIterator *bi,
           int32_t offset)
{
	return ((BreakIterator *)bi)->following(offset);
}

U_CAPI  UBool U_EXPORT2
ubrk_isBoundary(UBreakIterator *bi, int32_t offset)
{
	BreakIterator *brit = (BreakIterator *)bi;
	return brit->isBoundary(offset);
}
