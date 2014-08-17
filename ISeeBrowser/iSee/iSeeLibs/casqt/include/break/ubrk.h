/* Portions of this code based on ICU. */
/*
 ********************************************************************************
 *   Copyright (C) 1996-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 ********************************************************************************
 */

#ifndef _ICUBRK_H_
#define _ICUBRK_H_

#include "utypes.h"

// From icu\source\common\unicode\ubrk.h
typedef void UBreakIterator;

/** The possible types of text boundaries.  @stable ICU 2.0 */
typedef enum UBreakIteratorType {
  /** Character breaks  @stable ICU 2.0 */
  UBRK_CHARACTER = 0,
  /** Word breaks @stable ICU 2.0 */
  UBRK_WORD = 1,
  /** Line breaks @stable ICU 2.0 */
  UBRK_LINE = 2,
  /** Sentence breaks @stable ICU 2.0 */
  UBRK_SENTENCE = 3,

#ifndef U_HIDE_DEPRECATED_API
  /**
   * Title Case breaks
   * The iterator created using this type locates title boundaries as described for
   * Unicode 3.2 only. For Unicode 4.0 and above title boundary iteration,
   * please use Word Boundary iterator.
   *
   * @deprecated ICU 2.8 Use the word break iterator for titlecasing for Unicode 4 and later.
   */
  UBRK_TITLE = 4,
#endif /* U_HIDE_DEPRECATED_API */
  UBRK_COUNT = 5
} UBreakIteratorType;

/** Value indicating all text boundaries have been returned.
 *  @stable ICU 2.0
 */
#define UBRK_DONE ((int32_t) -1)


/**
 *  Enum constants for the word break tags returned by
 *  getRuleStatus().  A range of values is defined for each category of
 *  word, to allow for further subdivisions of a category in future releases.
 *  Applications should check for tag values falling within the range, rather
 *  than for single individual values.
 *  @stable ICU 2.2
*/
typedef enum UWordBreak {
    /** Tag value for "words" that do not fit into any of other categories.
     *  Includes spaces and most punctuation. */
    UBRK_WORD_NONE           = 0,
    /** Upper bound for tags for uncategorized words. */
    UBRK_WORD_NONE_LIMIT     = 100,
    /** Tag value for words that appear to be numbers, lower limit.    */
    UBRK_WORD_NUMBER         = 100,
    /** Tag value for words that appear to be numbers, upper limit.    */
    UBRK_WORD_NUMBER_LIMIT   = 200,
    /** Tag value for words that contain letters, excluding
     *  hiragana, katakana or ideographic characters, lower limit.    */
    UBRK_WORD_LETTER         = 200,
    /** Tag value for words containing letters, upper limit  */
    UBRK_WORD_LETTER_LIMIT   = 300,
    /** Tag value for words containing kana characters, lower limit */
    UBRK_WORD_KANA           = 300,
    /** Tag value for words containing kana characters, upper limit */
    UBRK_WORD_KANA_LIMIT     = 400,
    /** Tag value for words containing ideographic characters, lower limit */
    UBRK_WORD_IDEO           = 400,
    /** Tag value for words containing ideographic characters, upper limit */
    UBRK_WORD_IDEO_LIMIT     = 500
} UWordBreak;

/**
 *  Enum constants for the line break tags returned by getRuleStatus().
 *  A range of values is defined for each category of
 *  word, to allow for further subdivisions of a category in future releases.
 *  Applications should check for tag values falling within the range, rather
 *  than for single individual values.
 *  @stable ICU 2.8
*/
typedef enum ULineBreakTag {
    /** Tag value for soft line breaks, positions at which a line break
      *  is acceptable but not required                */
    UBRK_LINE_SOFT            = 0,
    /** Upper bound for soft line breaks.              */
    UBRK_LINE_SOFT_LIMIT      = 100,
    /** Tag value for a hard, or mandatory line break  */
    UBRK_LINE_HARD            = 100,
    /** Upper bound for hard line breaks.              */
    UBRK_LINE_HARD_LIMIT      = 200
} ULineBreakTag;



/**
 *  Enum constants for the sentence break tags returned by getRuleStatus().
 *  A range of values is defined for each category of
 *  sentence, to allow for further subdivisions of a category in future releases.
 *  Applications should check for tag values falling within the range, rather
 *  than for single individual values.
 *  @stable ICU 2.8
*/
typedef enum USentenceBreakTag {
    /** Tag value for for sentences  ending with a sentence terminator
      * ('.', '?', '!', etc.) character, possibly followed by a
      * hard separator (CR, LF, PS, etc.)
      */
    UBRK_SENTENCE_TERM       = 0,
    /** Upper bound for tags for sentences ended by sentence terminators.    */
    UBRK_SENTENCE_TERM_LIMIT = 100,
    /** Tag value for for sentences that do not contain an ending
      * sentence terminator ('.', '?', '!', etc.) character, but
      * are ended only by a hard separator (CR, LF, PS, etc.) or end of input.
      */
    UBRK_SENTENCE_SEP        = 100,
    /** Upper bound for tags for sentences ended by a separator.              */
    UBRK_SENTENCE_SEP_LIMIT  = 200
    /** Tag value for a hard, or mandatory line break  */
} USentenceBreakTag;


/**
 * Open a new UBreakIterator for locating text boundaries for a specified locale.
 * A UBreakIterator may be used for detecting character, line, word,
 * and sentence breaks in text.
 * @param type The type of UBreakIterator to open: one of UBRK_CHARACTER, UBRK_WORD,
 * UBRK_LINE, UBRK_SENTENCE
 * @param locale The locale specifying the text-breaking conventions.
 * @param text The text to be iterated over.
 * @param textLength The number of characters in text, or -1 if null-terminated.
 * @param status A UErrorCode to receive any errors.
 * @return A UBreakIterator for the specified locale.
 * @see ubrk_openRules
 * @stable ICU 2.0
 */
U_STABLE UBreakIterator* U_EXPORT2
ubrk_open(UBreakIteratorType type,
      const char *locale,
      const UChar *text,
      int32_t textLength,
      UErrorCode *status);
/**
  * A recommended size (in bytes) for the memory buffer to be passed to ubrk_saveClone().
  * @stable ICU 2.0
  */
#define U_BRK_SAFECLONE_BUFFERSIZE 512

/**
* Close a UBreakIterator.
* Once closed, a UBreakIterator may no longer be used.
* @param bi The break iterator to close.
 * @stable ICU 2.0
*/
U_STABLE void U_EXPORT2
ubrk_close(UBreakIterator *bi);

/**
 * Sets an existing iterator to point to a new piece of text
 * @param bi The iterator to use
 * @param text The text to be set
 * @param textLength The length of the text
 * @param status The error code
 * @stable ICU 2.0
 */
U_STABLE void U_EXPORT2
ubrk_setText(UBreakIterator* bi,
             const UChar*    text,
             int32_t         textLength,
             UErrorCode*     status);

/**
 * Determine the most recently-returned text boundary.
 *
 * @param bi The break iterator to use.
 * @return The character index most recently returned by \ref ubrk_next, \ref ubrk_previous,
 * \ref ubrk_first, or \ref ubrk_last.
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
ubrk_current(const UBreakIterator *bi);

/**
 * Determine the text boundary following the current text boundary.
 *
 * @param bi The break iterator to use.
 * @return The character index of the next text boundary, or UBRK_DONE
 * if all text boundaries have been returned.
 * @see ubrk_previous
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
ubrk_next(UBreakIterator *bi);

/**
 * Determine the text boundary preceding the current text boundary.
 *
 * @param bi The break iterator to use.
 * @return The character index of the preceding text boundary, or UBRK_DONE
 * if all text boundaries have been returned.
 * @see ubrk_next
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
ubrk_previous(UBreakIterator *bi);

/**
 * Determine the index of the first character in the text being scanned.
 * This is not always the same as index 0 of the text.
 * @param bi The break iterator to use.
 * @return The character index of the first character in the text being scanned.
 * @see ubrk_last
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
ubrk_first(UBreakIterator *bi);

/**
 * Determine the index immediately <EM>beyond</EM> the last character in the text being
 * scanned.
 * This is not the same as the last character.
 * @param bi The break iterator to use.
 * @return The character offset immediately <EM>beyond</EM> the last character in the
 * text being scanned.
 * @see ubrk_first
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
ubrk_last(UBreakIterator *bi);

/**
 * Determine the text boundary preceding the specified offset.
 * The value returned is always smaller than offset, or UBRK_DONE.
 * @param bi The break iterator to use.
 * @param offset The offset to begin scanning.
 * @return The text boundary preceding offset, or UBRK_DONE.
 * @see ubrk_following
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
ubrk_preceding(UBreakIterator *bi,
           int32_t offset);

/**
 * Determine the text boundary following the specified offset.
 * The value returned is always greater than offset, or UBRK_DONE.
 * @param bi The break iterator to use.
 * @param offset The offset to begin scanning.
 * @return The text boundary following offset, or UBRK_DONE.
 * @see ubrk_preceding
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
ubrk_following(UBreakIterator *bi,
           int32_t offset);

/**
* Returns true if the specfied position is a boundary position.  As a side
* effect, leaves the iterator pointing to the first boundary position at
* or after "offset".
* @param bi The break iterator to use.
* @param offset the offset to check.
* @return True if "offset" is a boundary position.
* @stable ICU 2.0
*/
U_STABLE  UBool U_EXPORT2
ubrk_isBoundary(UBreakIterator *bi, int32_t offset);

#endif // _ICUBRK_H_
