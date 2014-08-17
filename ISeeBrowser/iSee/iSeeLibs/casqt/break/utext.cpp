/* Portions of this code based on ICU. */
/*
 ********************************************************************************
 *   Copyright (C) 1996-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 ********************************************************************************
 */

struct UText;
typedef struct UText UText; /**< C typedef for struct UText. @stable ICU 3.6 */


/*****************************************************************************
 *
 *   Functions to work with the text represeted by a UText wrapper
 *
 *****************************************************************************/

/**
  * Get the length of the text.  Depending on the characteristics
  * of the underlying text representation, this may be expensive.  
  * @see  utext_isLengthExpensive()
  *
  *
  * @param ut  the text to be accessed.
  * @return the length of the text, expressed in native units.
  *
  * @stable ICU 3.4
  */
U_STABLE int64_t U_EXPORT2
utext_nativeLength(UText *ut);

/**
 *  Return TRUE if calculating the length of the text could be expensive.
 *  Finding the length of NUL terminated strings is considered to be expensive.
 *
 *  Note that the value of this function may change
 *  as the result of other operations on a UText.
 *  Once the length of a string has been discovered, it will no longer
 *  be expensive to report it.
 *
 * @param ut the text to be accessed.
 * @return TRUE if determining the length of the text could be time consuming.
 * @stable ICU 3.4
 */
U_STABLE UBool U_EXPORT2
utext_isLengthExpensive(const UText *ut);

/**
 * Returns the code point at the requested index,
 * or U_SENTINEL (-1) if it is out of bounds.
 *
 * If the specified index points to the interior of a multi-unit
 * character - one of the trail bytes of a UTF-8 sequence, for example -
 * the complete code point will be returned.
 *
 * The iteration position will be set to the start of the returned code point.
 *
 * This function is roughly equivalent to the the sequence
 *    utext_setNativeIndex(index);
 *    utext_current32();
 * (There is a subtle difference if the index is out of bounds by being less than zero - 
 * utext_setNativeIndex(negative value) sets the index to zero, after which utext_current()
 * will return the char at zero.  utext_char32At(negative index), on the other hand, will
 * return the U_SENTINEL value of -1.)
 * 
 * @param ut the text to be accessed
 * @param nativeIndex the native index of the character to be accessed.  If the index points
 *        to other than the first unit of a multi-unit character, it will be adjusted
 *        to the start of the character.
 * @return the code point at the specified index.
 * @stable ICU 3.4
 */
U_STABLE UChar32 U_EXPORT2
utext_char32At(UText *ut, int64_t nativeIndex);


/**
 *
 * Get the code point at the current iteration position,
 * or U_SENTINEL (-1) if the iteration has reached the end of
 * the input text.
 *
 * @param ut the text to be accessed.
 * @return the Unicode code point at the current iterator position.
 * @stable ICU 3.4
 */
U_STABLE UChar32 U_EXPORT2
utext_current32(UText *ut);


/**
 * Get the code point at the current iteration position of the UText, and
 * advance the position to the first index following the character.
 *
 * If the position is at the end of the text (the index following
 * the last character, which is also the length of the text), 
 * return U_SENTINEL (-1) and do not advance the index. 
 *
 * This is a post-increment operation.
 *
 * An inline macro version of this function, UTEXT_NEXT32(), 
 * is available for performance critical use.
 *
 * @param ut the text to be accessed.
 * @return the Unicode code point at the iteration position.
 * @see UTEXT_NEXT32
 * @stable ICU 3.4
 */
U_STABLE UChar32 U_EXPORT2
utext_next32(UText *ut);


/**
 *  Move the iterator position to the character (code point) whose
 *  index precedes the current position, and return that character.
 *  This is a pre-decrement operation.
 *
 *  If the initial position is at the start of the text (index of 0) 
 *  return U_SENTINEL (-1), and leave the position unchanged.
 *
 *  An inline macro version of this function, UTEXT_PREVIOUS32(), 
 *  is available for performance critical use.
 *
 *  @param ut the text to be accessed.
 *  @return the previous UChar32 code point, or U_SENTINEL (-1) 
 *          if the iteration has reached the start of the text.
 *  @see UTEXT_PREVIOUS32
 *  @stable ICU 3.4
 */
U_STABLE UChar32 U_EXPORT2
utext_previous32(UText *ut);


/**
  * Set the iteration index and return the code point at that index. 
  * Leave the iteration index at the start of the following code point.
  *
  * This function is the most efficient and convenient way to
  * begin a forward iteration.  The results are identical to the those
  * from the sequence
  * \code
  *    utext_setIndex();
  *    utext_next32();
  * \endcode
  *
  *  @param ut the text to be accessed.
  *  @param nativeIndex Iteration index, in the native units of the text provider.
  *  @return Code point which starts at or before index,
  *         or U_SENTINEL (-1) if it is out of bounds.
  * @stable ICU 3.4
  */
U_STABLE UChar32 U_EXPORT2
utext_next32From(UText *ut, int64_t nativeIndex);



/**
  * Set the iteration index, and return the code point preceding the
  * one specified by the initial index.  Leave the iteration position
  * at the start of the returned code point.
  *
  * This function is the most efficient and convenient way to
  * begin a backwards iteration.
  *
  * @param ut the text to be accessed.
  * @param nativeIndex Iteration index in the native units of the text provider.
  * @return Code point preceding the one at the initial index,
  *         or U_SENTINEL (-1) if it is out of bounds.
  *
  * @stable ICU 3.4
  */
U_STABLE UChar32 U_EXPORT2
utext_previous32From(UText *ut, int64_t nativeIndex);

/**
  * Get the current iterator position, which can range from 0 to 
  * the length of the text.
  * The position is a native index into the input text, in whatever format it
  * may have (possibly UTF-8 for example), and may not always be the same as
  * the corresponding UChar (UTF-16) index.
  * The returned position will always be aligned to a code point boundary. 
  *
  * @param ut the text to be accessed.
  * @return the current index position, in the native units of the text provider.
  * @stable ICU 3.4
  */
U_STABLE int64_t U_EXPORT2
utext_getNativeIndex(const UText *ut);

/**
 * Set the current iteration position to the nearest code point
 * boundary at or preceding the specified index.
 * The index is in the native units of the original input text.
 * If the index is out of range, it will be pinned to be within
 * the range of the input text.
 * <p>
 * It will usually be more efficient to begin an iteration
 * using the functions utext_next32From() or utext_previous32From()
 * rather than setIndex().
 * <p>
 * Moving the index position to an adjacent character is best done
 * with utext_next32(), utext_previous32() or utext_moveIndex32().
 * Attempting to do direct arithmetic on the index position is
 * complicated by the fact that the size (in native units) of a
 * character depends on the underlying representation of the character
 * (UTF-8, UTF-16, UTF-32, arbitrary codepage), and is not
 * easily knowable.
 *
 * @param ut the text to be accessed.
 * @param nativeIndex the native unit index of the new iteration position.
 * @stable ICU 3.4
 */
U_STABLE void U_EXPORT2
utext_setNativeIndex(UText *ut, int64_t nativeIndex);

/**
 * Move the iterator postion by delta code points.  The number of code points
 * is a signed number; a negative delta will move the iterator backwards,
 * towards the start of the text.
 * <p>
 * The index is moved by <code>delta</code> code points
 * forward or backward, but no further backward than to 0 and
 * no further forward than to utext_nativeLength().
 * The resulting index value will be in between 0 and length, inclusive.
 *
 * @param ut the text to be accessed.
 * @param delta the signed number of code points to move the iteration position.
 * @return TRUE if the position could be moved the requested number of positions while
 *              staying within the range [0 - text length].
 * @stable ICU 3.4
 */
U_STABLE UBool U_EXPORT2
utext_moveIndex32(UText *ut, int32_t delta);

/**
 * Get the native index of the character preceeding the current position.
 * If the iteration position is already at the start of the text, zero
 * is returned.
 * The value returned is the same as that obtained from the following sequence,
 * but without the side effect of changing the iteration position.
 *   
 * \code
 *    UText  *ut = whatever;
 *      ...
 *    utext_previous(ut)
 *    utext_getNativeIndex(ut);
 * \endcode
 *
 * This function is most useful during forwards iteration, where it will get the
 *   native index of the character most recently returned from utext_next().
 *
 * @param ut the text to be accessed
 * @return the native index of the character preceeding the current index position,
 *         or zero if the current position is at the start of the text.
 * @stable ICU 3.6
 */
U_STABLE int64_t U_EXPORT2
utext_getPreviousNativeIndex(UText *ut); 


/**
 *
 * Extract text from a UText into a UChar buffer.  The range of text to be extracted
 * is specified in the native indices of the UText provider.  These may not necessarily
 * be UTF-16 indices.
 * <p>
 * The size (number of 16 bit UChars) of the data to be extracted is returned.  The
 * full number of UChars is returned, even when the extracted text is truncated
 * because the specified buffer size is too small.
 * <p>
 * The extracted string will (if you are a user) / must (if you are a text provider)
 * be NUL-terminated if there is sufficient space in the destination buffer.  This
 * terminating NUL is not included in the returned length.
 * <p>
 * The iteration index is left at the position following the last extracted character.
 *
 * @param  ut    the UText from which to extract data.
 * @param  nativeStart the native index of the first character to extract.\
 *               If the specified index is out of range,
 *               it will be pinned to to be within 0 <= index <= textLength
 * @param  nativeLimit the native string index of the position following the last
 *               character to extract.  If the specified index is out of range,
 *               it will be pinned to to be within 0 <= index <= textLength.
 *               nativeLimit must be >= nativeStart.
 * @param  dest  the UChar (UTF-16) buffer into which the extracted text is placed
 * @param  destCapacity  The size, in UChars, of the destination buffer.  May be zero
 *               for precomputing the required size.
 * @param  status receives any error status.
 *         U_BUFFER_OVERFLOW_ERROR: the extracted text was truncated because the 
 *         buffer was too small.  Returns number of UChars for preflighting.
 * @return Number of UChars in the data to be extracted.  Does not include a trailing NUL.
 *
 * @stable ICU 3.4
 */
U_STABLE int32_t U_EXPORT2
utext_extract(UText *ut,
             int64_t nativeStart, int64_t nativeLimit,
             UChar *dest, int32_t destCapacity,
             UErrorCode *status);


/************************************************************************************
 *
 *  #define inline versions of selected performance-critical text access functions
 *          Caution:  do not use auto increment++ or decrement-- expressions
 *                    as parameters to these macros.
 *
 *          For most use, where there is no extreme performance constraint, the
 *          normal, non-inline functions are a better choice.  The resulting code
 *          will be smaller, and, if the need ever arises, easier to debug.
 *
 *          These are implemented as #defines rather than real functions
 *          because there is no fully portable way to do inline functions in plain C.
 *
 ************************************************************************************/

/**
 * inline version of utext_next32(), for performance-critical situations.
 *
 * Get the code point at the current iteration position of the UText, and
 * advance the position to the first index following the character.
 * This is a post-increment operation.
 * Returns U_SENTINEL (-1) if the position is at the end of the
 * text.
 *
 * @stable ICU 3.4
 */
#define UTEXT_NEXT32(ut)  \
    ((ut)->chunkOffset < (ut)->chunkLength && ((ut)->chunkContents)[(ut)->chunkOffset]<0xd800 ? \
    ((ut)->chunkContents)[((ut)->chunkOffset)++] : utext_next32(ut))

/**
 * inline version of utext_previous32(), for performance-critical situations.
 *
 *  Move the iterator position to the character (code point) whose
 *  index precedes the current position, and return that character.
 *  This is a pre-decrement operation.
 *  Returns U_SENTINEL (-1) if the position is at the start of the  text.
 *
 * @stable ICU 3.4
 */
#define UTEXT_PREVIOUS32(ut)  \
    ((ut)->chunkOffset > 0 && \
     (ut)->chunkContents[(ut)->chunkOffset-1] < 0xd800 ? \
          (ut)->chunkContents[--((ut)->chunkOffset)]  :  utext_previous32(ut))

/**
  *  inline version of utext_getNativeIndex(), for performance-critical situations.
  *
  * Get the current iterator position, which can range from 0 to 
  * the length of the text.
  * The position is a native index into the input text, in whatever format it
  * may have (possibly UTF-8 for example), and may not always be the same as
  * the corresponding UChar (UTF-16) index.
  * The returned position will always be aligned to a code point boundary. 
  *
  * @stable ICU 3.6
  */
#define UTEXT_GETNATIVEINDEX(ut)                       \
    ((ut)->chunkOffset <= (ut)->nativeIndexingLimit?   \
        (ut)->chunkNativeStart+(ut)->chunkOffset :     \
        (ut)->pFuncs->mapOffsetToNative(ut))    

/**
  *  inline version of utext_setNativeIndex(), for performance-critical situations.
  *
  * Set the current iteration position to the nearest code point
  * boundary at or preceding the specified index.
  * The index is in the native units of the original input text.
  * If the index is out of range, it will be pinned to be within
  * the range of the input text.
  *
  * @stable ICU 4.0
  */
#define UTEXT_SETNATIVEINDEX(ut, ix)                       \
    { int64_t __offset = (ix) - (ut)->chunkNativeStart; \
      if (__offset>=0 && __offset<=(int64_t)(ut)->nativeIndexingLimit) { \
          (ut)->chunkOffset=(int32_t)__offset; \
      } else { \
          utext_setNativeIndex((ut), (ix)); } }

/**
 * UText provider properties (bit field indexes).
 *
 * @see UText
 * @stable ICU 3.4
 */
enum {
    /**
     * It is potentially time consuming for the provider to determine the length of the text.
     * @stable ICU 3.4
     */
    UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE = 1,
    /**
     * Text chunks remain valid and usable until the text object is modified or
     * deleted, not just until the next time the access() function is called
     * (which is the default).
     * @stable ICU 3.4
     */
    UTEXT_PROVIDER_STABLE_CHUNKS = 2,
    /**
     * The provider supports modifying the text via the replace() and copy()
     * functions.
     * @see Replaceable
     * @stable ICU 3.4
     */
    UTEXT_PROVIDER_WRITABLE = 3,
    /**
     * There is meta data associated with the text.
     * @see Replaceable::hasMetaData()
     * @stable ICU 3.4
     */ 
    UTEXT_PROVIDER_HAS_META_DATA = 4,
    /**
     * Text provider owns the text storage.
     *  Generally occurs as the result of a deep clone of the UText.
     *  When closing the UText, the associated text must
     *  also be closed/deleted/freed/ whatever is appropriate.
     * @stable ICU 3.6
     */
     UTEXT_PROVIDER_OWNS_TEXT = 5
};

/**
  * Function type declaration for UText.clone().
  *
  *  clone a UText.  Much like opening a UText where the source text is itself
  *  another UText.
  *
  *  A deep clone will copy both the UText data structures and the underlying text.
  *  The original and cloned UText will operate completely independently; modifications
  *  made to the text in one will not effect the other.  Text providers are not
  *  required to support deep clones.  The user of clone() must check the status return
  *  and be prepared to handle failures.
  *
  *  A shallow clone replicates only the UText data structures; it does not make
  *  a copy of the underlying text.  Shallow clones can be used as an efficient way to 
  *  have multiple iterators active in a single text string that is not being
  *  modified.
  *
  *  A shallow clone operation must not fail except for truly exceptional conditions such
  *  as memory allocation failures.
  *
  *  A UText and its clone may be safely concurrently accessed by separate threads.
  *  This is true for both shallow and deep clones.
  *  It is the responsibility of the Text Provider to ensure that this thread safety
  *  constraint is met.

  *
  *  @param dest   A UText struct to be filled in with the result of the clone operation,
  *                or NULL if the clone function should heap-allocate a new UText struct.
  *  @param src    The UText to be cloned.
  *  @param deep   TRUE to request a deep clone, FALSE for a shallow clone.
  *  @param status Errors are returned here.  For deep clones, U_UNSUPPORTED_ERROR
  *                should be returned if the text provider is unable to clone the
  *                original text.
  *  @return       The newly created clone, or NULL if the clone operation failed.
  *
  * @stable ICU 3.4
  */
typedef UText * U_CALLCONV
UTextClone(UText *dest, const UText *src, UBool deep, UErrorCode *status);


/**
 * Function type declaration for UText.nativeLength().
 *
 * @param ut the UText to get the length of.
 * @return the length, in the native units of the original text string.
 * @see UText
 * @stable ICU 3.4
 */
typedef int64_t U_CALLCONV
UTextNativeLength(UText *ut);

/**
 * Function type declaration for UText.access().  Get the description of the text chunk
 *  containing the text at a requested native index.  The UText's iteration
 *  position will be left at the requested index.  If the index is out
 *  of bounds, the iteration position will be left at the start or end
 *  of the string, as appropriate.
 *
 *  Chunks must begin and end on code point boundaries.  A single code point
 *  comprised of multiple storage units must never span a chunk boundary.
 *
 *
 * @param ut          the UText being accessed.
 * @param nativeIndex Requested index of the text to be accessed.
 * @param forward     If TRUE, then the returned chunk must contain text
 *                    starting from the index, so that start<=index<limit.
 *                    If FALSE, then the returned chunk must contain text
 *                    before the index, so that start<index<=limit.
 * @return            True if the requested index could be accessed.  The chunk
 *                    will contain the requested text.
 *                    False value if a chunk cannot be accessed
 *                    (the requested index is out of bounds).
 *
 * @see UText
 * @stable ICU 3.4
 */
typedef UBool U_CALLCONV
UTextAccess(UText *ut, int64_t nativeIndex, UBool forward);

/**
 * Function type declaration for UText.extract().
 *
 * Extract text from a UText into a UChar buffer.  The range of text to be extracted
 * is specified in the native indices of the UText provider.  These may not necessarily
 * be UTF-16 indices.
 * <p>
 * The size (number of 16 bit UChars) in the data to be extracted is returned.  The
 * full amount is returned, even when the specified buffer size is smaller.
 * <p>
 * The extracted string will (if you are a user) / must (if you are a text provider)
 * be NUL-terminated if there is sufficient space in the destination buffer.
 *
 * @param  ut            the UText from which to extract data.
 * @param  nativeStart   the native index of the first characer to extract.
 * @param  nativeLimit   the native string index of the position following the last
 *                       character to extract.
 * @param  dest          the UChar (UTF-16) buffer into which the extracted text is placed
 * @param  destCapacity  The size, in UChars, of the destination buffer.  May be zero
 *                       for precomputing the required size.
 * @param  status        receives any error status.
 *                       If U_BUFFER_OVERFLOW_ERROR: Returns number of UChars for
 *                       preflighting.
 * @return Number of UChars in the data.  Does not include a trailing NUL.
 *
 * @stable ICU 3.4
 */
typedef int32_t U_CALLCONV
UTextExtract(UText *ut,
             int64_t nativeStart, int64_t nativeLimit,
             UChar *dest, int32_t destCapacity,
             UErrorCode *status);

/**
 * Function type declaration for UText.replace().
 *
 * Replace a range of the original text with a replacement text.
 *
 * Leaves the current iteration position at the position following the
 *  newly inserted replacement text.
 *
 * This function need only be implemented on UText types that support writing.
 *
 * When using this function, there should be only a single UText opened onto the
 * underlying native text string.  The function is responsible for updating the
 * text chunk within the UText to reflect the updated iteration position,
 * taking into account any changes to the underlying string's structure caused
 * by the replace operation.
 *
 * @param ut               the UText representing the text to be operated on.
 * @param nativeStart      the index of the start of the region to be replaced
 * @param nativeLimit      the index of the character following the region to be replaced.
 * @param replacementText  pointer to the replacement text
 * @param replacmentLength length of the replacement text in UChars, or -1 if the text is NUL terminated.
 * @param status           receives any error status.  Possible errors include
 *                         U_NO_WRITE_PERMISSION
 *
 * @return The signed number of (native) storage units by which
 *         the length of the text expanded or contracted.
 *
 * @stable ICU 3.4
 */
typedef int32_t U_CALLCONV
UTextReplace(UText *ut,
             int64_t nativeStart, int64_t nativeLimit,
             const UChar *replacementText, int32_t replacmentLength,
             UErrorCode *status);

/**
 * Function type declaration for UText.copy().
 *
 * Copy or move a substring from one position to another within the text,
 * while retaining any metadata associated with the text.
 * This function is used to duplicate or reorder substrings.
 * The destination index must not overlap the source range.
 *
 * The text to be copied or moved is inserted at destIndex;
 * it does not replace or overwrite any existing text.
 *
 * This function need only be implemented for UText types that support writing.
 *
 * When using this function, there should be only a single UText opened onto the
 * underlying native text string.  The function is responsible for updating the
 * text chunk within the UText to reflect the updated iteration position,
 * taking into account any changes to the underlying string's structure caused
 * by the replace operation.
 *
 * @param ut           The UText representing the text to be operated on.
 * @param nativeStart  The index of the start of the region to be copied or moved
 * @param nativeLimit  The index of the character following the region to be replaced.
 * @param nativeDest   The destination index to which the source substring is copied or moved.
 * @param move         If TRUE, then the substring is moved, not copied/duplicated.
 * @param status       receives any error status.  Possible errors include U_NO_WRITE_PERMISSION
 *
 * @stable ICU 3.4
 */
typedef void U_CALLCONV
UTextCopy(UText *ut,
          int64_t nativeStart, int64_t nativeLimit,
          int64_t nativeDest,
          UBool move,
          UErrorCode *status);

/**
 * Function type declaration for UText.mapOffsetToNative().
 * Map from the current UChar offset within the current text chunk to
 *  the corresponding native index in the original source text.
 *
 * This is required only for text providers that do not use native UTF-16 indexes.
 *
 * @param ut     the UText.
 * @return Absolute (native) index corresponding to chunkOffset in the current chunk.
 *         The returned native index should always be to a code point boundary.
 *
 * @stable ICU 3.4
 */
typedef int64_t U_CALLCONV
UTextMapOffsetToNative(const UText *ut);

/**
 * Function type declaration for UText.mapIndexToUTF16().
 * Map from a native index to a UChar offset within a text chunk.
 * Behavior is undefined if the native index does not fall within the
 *   current chunk.
 *
 * This function is required only for text providers that do not use native UTF-16 indexes.
 *
 * @param ut          The UText containing the text chunk.
 * @param nativeIndex Absolute (native) text index, chunk->start<=index<=chunk->limit.
 * @return            Chunk-relative UTF-16 offset corresponding to the specified native
 *                    index.
 *
 * @stable ICU 3.4
 */
typedef int32_t U_CALLCONV
UTextMapNativeIndexToUTF16(const UText *ut, int64_t nativeIndex);


/**
 * Function type declaration for UText.utextClose().
 *
 * A Text Provider close function is only required for provider types that make
 *  allocations in their open function (or other functions) that must be 
 *  cleaned when the UText is closed.
 *
 * The allocation of the UText struct itself and any "extra" storage
 * associated with the UText is handled by the common UText implementation
 * and does not require provider specific cleanup in a close function.
 *
 * Most UText provider implementations do not need to implement this function.
 *
 * @param ut A UText object to be closed.
 *
 * @stable ICU 3.4
 */
typedef void U_CALLCONV
UTextClose(UText *ut);


/**
  *   (public)  Function dispatch table for UText.
  *             Conceptually very much like a C++ Virtual Function Table.
  *             This struct defines the organization of the table.
  *             Each text provider implementation must provide an
  *              actual table that is initialized with the appropriate functions
  *              for the type of text being handled.
  *   @stable ICU 3.6
  */
struct UTextFuncs {
    /**
     *   (public)  Function table size, sizeof(UTextFuncs)
     *             Intended for use should the table grow to accomodate added
     *             functions in the future, to allow tests for older format
     *             function tables that do not contain the extensions.
     *
     *             Fields are placed for optimal alignment on
     *             32/64/128-bit-pointer machines, by normally grouping together
     *             4 32-bit fields,
     *             4 pointers,
     *             2 64-bit fields
     *             in sequence.
     *   @stable ICU 3.6
     */
    int32_t       tableSize;

    /**
      *   (private)  Alignment padding.
      *              Do not use, reserved for use by the UText framework only.
      *   @internal
      */
    int32_t       reserved1, /** @internal */ reserved2, /** @internal */ reserved3;


    /**
     * (public) Function pointer for UTextClone
     *
     * @see UTextClone
     * @stable ICU 3.6
     */
    UTextClone *clone;

    /**
     * (public) function pointer for UTextLength
     * May be expensive to compute!
     *
     * @see UTextLength
     * @stable ICU 3.6
     */
    UTextNativeLength *nativeLength;

    /**
     * (public) Function pointer for UTextAccess.
     *
     * @see UTextAccess
     * @stable ICU 3.6
     */
    UTextAccess *access;

    /**
     * (public) Function pointer for UTextExtract.
     *
     * @see UTextExtract
     * @stable ICU 3.6
     */
    UTextExtract *extract;

    /**
     * (public) Function pointer for UTextReplace.
     *
     * @see UTextReplace
     * @stable ICU 3.6
     */
    UTextReplace *replace;

    /**
     * (public) Function pointer for UTextCopy.
     *
     * @see UTextCopy
     * @stable ICU 3.6
     */
    UTextCopy *copy;

    /**
     * (public) Function pointer for UTextMapOffsetToNative.
     *
     * @see UTextMapOffsetToNative
     * @stable ICU 3.6
     */
    UTextMapOffsetToNative *mapOffsetToNative;

    /**
     * (public) Function pointer for UTextMapNativeIndexToUTF16.
     *
     * @see UTextMapNativeIndexToUTF16
     * @stable ICU 3.6
     */
    UTextMapNativeIndexToUTF16 *mapNativeIndexToUTF16;

    /**
     * (public) Function pointer for UTextClose.
      *
      * @see UTextClose
      * @stable ICU 3.6
      */
    UTextClose  *close;

    /**
      * (private)  Spare function pointer
      * @internal
      */

    UTextClose  *spare1;
    /**
      * (private)  Spare function pointer
      * @internal
      */
    UTextClose  *spare2;

    /**
      * (private)  Spare function pointer
      * @internal
      */
    UTextClose  *spare3;

};
/**
 * Function dispatch table for UText
 * @see UTextFuncs
 */
typedef struct UTextFuncs UTextFuncs;

 /**
  *   UText struct.  Provides the interface between the generic UText access code
  *                  and the UText provider code that works on specific kinds of
  *                  text  (UTF-8, noncontiguous UTF-16, whatever.)
  *
  *                  Applications that are using predefined types of text providers
  *                  to pass text data to ICU services will have no need to view the
  *                  internals of the UText structs that they open.
  *
  * @stable ICU 3.6
  */
struct UText {
    /**
     *     (private)  Magic.  Used to help detect when UText functions are handed
     *                        invalid or unitialized UText structs.
     *                        utext_openXYZ() functions take an initialized,
     *                        but not necessarily open, UText struct as an
     *                        optional fill-in parameter.  This magic field
     *                        is used to check for that initialization.
     *                        Text provider close functions must NOT clear
     *                        the magic field because that would prevent
     *                        reuse of the UText struct.
     * @internal
     */
    uint32_t       magic;


    /**
     *     (private)  Flags for managing the allocation and freeing of
     *                memory associated with this UText.
     * @internal
     */
    int32_t        flags;


    /**
      *  Text provider properties.  This set of flags is maintainted by the
      *                             text provider implementation.
      *  @stable ICU 3.4
      */
    int32_t         providerProperties;

    /**
     * (public) sizeOfStruct=sizeof(UText)
     * Allows possible backward compatible extension.
     *
     * @stable ICU 3.4
     */
    int32_t         sizeOfStruct;
    
    /* ------ 16 byte alignment boundary -----------  */
    

    /**
      *  (protected) Native index of the first character position following
      *              the current chunk.
      *  @stable ICU 3.6
      */
    int64_t         chunkNativeLimit;

    /**
     *   (protected)  Size in bytes of the extra space (pExtra).
     *  @stable ICU 3.4
     */
    int32_t        extraSize;

    /**
      *    (protected) The highest chunk offset where native indexing and
      *    chunk (UTF-16) indexing correspond.  For UTF-16 sources, value
      *    will be equal to chunkLength.
      *
      *    @stable ICU 3.6
      */
    int32_t         nativeIndexingLimit;

    /* ---- 16 byte alignment boundary------ */
    
    /**
     *  (protected) Native index of the first character in the text chunk.
     *  @stable ICU 3.6
     */
    int64_t         chunkNativeStart;

    /**
     *  (protected) Current iteration position within the text chunk (UTF-16 buffer).
     *  This is the index to the character that will be returned by utext_next32().
     *  @stable ICU 3.6
     */
    int32_t         chunkOffset;

    /**
     *  (protected) Length the text chunk (UTF-16 buffer), in UChars.
     *  @stable ICU 3.6
     */
    int32_t         chunkLength;

    /* ---- 16  byte alignment boundary-- */
    

    /**
     *  (protected)  pointer to a chunk of text in UTF-16 format.
     *  May refer either to original storage of the source of the text, or
     *  if conversion was required, to a buffer owned by the UText.
     *  @stable ICU 3.6
     */
    const UChar    *chunkContents;

     /**
      * (public)     Pointer to Dispatch table for accessing functions for this UText.
      * @stable ICU 3.6
      */
    const UTextFuncs     *pFuncs;

    /**
     *  (protected)  Pointer to additional space requested by the
     *               text provider during the utext_open operation.
     * @stable ICU 3.4
     */
    void          *pExtra;

    /**
     * (protected) Pointer to string or text-containin object or similar.
     * This is the source of the text that this UText is wrapping, in a format
     *  that is known to the text provider functions.
     * @stable ICU 3.4
     */
    const void   *context;

    /* --- 16 byte alignment boundary--- */

    /**
     * (protected) Pointer fields available for use by the text provider.
     * Not used by UText common code.
     * @stable ICU 3.6
     */
    const void     *p; 
    /**
     * (protected) Pointer fields available for use by the text provider.
     * Not used by UText common code.
     * @stable ICU 3.6
     */
    const void     *q;
     /**
     * (protected) Pointer fields available for use by the text provider.
     * Not used by UText common code.
     * @stable ICU 3.6
      */
    const void     *r;

    /**
      *  Private field reserved for future use by the UText framework
      *     itself.  This is not to be touched by the text providers.
      * @internal ICU 3.4
      */
    void           *privP;


    /* --- 16 byte alignment boundary--- */
    

    /**
      * (protected) Integer field reserved for use by the text provider.
      * Not used by the UText framework, or by the client (user) of the UText.
      * @stable ICU 3.4
      */
    int64_t         a;

    /**
      * (protected) Integer field reserved for use by the text provider.
      * Not used by the UText framework, or by the client (user) of the UText.
      * @stable ICU 3.4
      */
    int32_t         b;

    /**
      * (protected) Integer field reserved for use by the text provider.
      * Not used by the UText framework, or by the client (user) of the UText.
      * @stable ICU 3.4
      */
    int32_t         c;

    /*  ---- 16 byte alignment boundary---- */


    /**
      *  Private field reserved for future use by the UText framework
      *     itself.  This is not to be touched by the text providers.
      * @internal ICU 3.4
      */
    int64_t         privA;
    /**
      *  Private field reserved for future use by the UText framework
      *     itself.  This is not to be touched by the text providers.
      * @internal ICU 3.4
      */
    int32_t         privB;
    /**
      *  Private field reserved for future use by the UText framework
      *     itself.  This is not to be touched by the text providers.
      * @internal ICU 3.4
      */
    int32_t         privC;
};

/**
  * @internal
  *  Value used to help identify correctly initialized UText structs.
  *  Note:  must be publicly visible so that UTEXT_INITIALIZER can access it.
  */
enum {
    UTEXT_MAGIC = 0x345ad82c
};

/**
 * initializer to be used with local (stack) instances of a UText
 *  struct.  UText structs must be initialized before passing
 *  them to one of the utext_open functions.
 *
 * @stable ICU 3.6
 */
#define UTEXT_INITIALIZER {                                        \
                  UTEXT_MAGIC,          /* magic                */ \
                  0,                    /* flags                */ \
                  0,                    /* providerProps        */ \
                  sizeof(UText),        /* sizeOfStruct         */ \
                  0,                    /* chunkNativeLimit     */ \
                  0,                    /* extraSize            */ \
                  0,                    /* nativeIndexingLimit  */ \
                  0,                    /* chunkNativeStart     */ \
                  0,                    /* chunkOffset          */ \
                  0,                    /* chunkLength          */ \
                  NULL,                 /* chunkContents        */ \
                  NULL,                 /* pFuncs               */ \
                  NULL,                 /* pExtra               */ \
                  NULL,                 /* context              */ \
                  NULL, NULL, NULL,     /* p, q, r              */ \
                  NULL,                 /* privP                */ \
                  0, 0, 0,              /* a, b, c              */ \
                  0, 0, 0               /* privA,B,C,           */ \
                  }



#define I32_FLAG(bitIndex) ((int32_t)1<<(bitIndex))


static UBool
utext_access(UText *ut, int64_t index, UBool forward) {
    return ut->pFuncs->access(ut, index, forward);
}



U_CAPI UBool U_EXPORT2
utext_moveIndex32(UText *ut, int32_t delta) {
    UChar32  c;
    if (delta > 0) {
        do {
            if(ut->chunkOffset>=ut->chunkLength && !utext_access(ut, ut->chunkNativeLimit, TRUE)) {
                return FALSE;
            }
            c = ut->chunkContents[ut->chunkOffset];
            if (U16_IS_SURROGATE(c)) {
                c = utext_next32(ut);
                if (c == U_SENTINEL) {
                    return FALSE;
                }
            } else {
                ut->chunkOffset++;
            }
        } while(--delta>0);

    } else if (delta<0) {
        do {
            if(ut->chunkOffset<=0 && !utext_access(ut, ut->chunkNativeStart, FALSE)) {
                return FALSE;
            }
            c = ut->chunkContents[ut->chunkOffset-1];
            if (U16_IS_SURROGATE(c)) {
                c = utext_previous32(ut);
                if (c == U_SENTINEL) {
                    return FALSE;
                }
            } else {
                ut->chunkOffset--;
            }
        } while(++delta<0);
    }

    return TRUE;
}


U_CAPI int64_t U_EXPORT2
utext_nativeLength(UText *ut) {
    return ut->pFuncs->nativeLength(ut);
}


U_CAPI UBool U_EXPORT2
utext_isLengthExpensive(const UText *ut) {
    UBool r = (ut->providerProperties & I32_FLAG(UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE)) != 0;
    return r;
}


U_CAPI int64_t U_EXPORT2
utext_getNativeIndex(const UText *ut) {
    if(ut->chunkOffset <= ut->nativeIndexingLimit) {
        return ut->chunkNativeStart+ut->chunkOffset;
    } else {
        return ut->pFuncs->mapOffsetToNative(ut);
    }
}


U_CAPI void U_EXPORT2
utext_setNativeIndex(UText *ut, int64_t index) {
    if(index<ut->chunkNativeStart || index>=ut->chunkNativeLimit) {
        // The desired position is outside of the current chunk.
        // Access the new position.  Assume a forward iteration from here,
        // which will also be optimimum for a single random access.
        // Reverse iterations may suffer slightly.
        ut->pFuncs->access(ut, index, TRUE);
    } else if((int32_t)(index - ut->chunkNativeStart) <= ut->nativeIndexingLimit) {
        // utf-16 indexing.
        ut->chunkOffset=(int32_t)(index-ut->chunkNativeStart);
    } else {
         ut->chunkOffset=ut->pFuncs->mapNativeIndexToUTF16(ut, index);
    }
    // The convention is that the index must always be on a code point boundary.
    // Adjust the index position if it is in the middle of a surrogate pair.
    if (ut->chunkOffset<ut->chunkLength) {
        UChar c= ut->chunkContents[ut->chunkOffset];
        if (UTF16_IS_TRAIL(c)) {
            if (ut->chunkOffset==0) {
                ut->pFuncs->access(ut, ut->chunkNativeStart, FALSE);
            }
            if (ut->chunkOffset>0) {
                UChar lead = ut->chunkContents[ut->chunkOffset-1];
                if (UTF16_IS_LEAD(lead)) {
                    ut->chunkOffset--;
                }
            }
        }
    }
}



U_CAPI int64_t U_EXPORT2
utext_getPreviousNativeIndex(UText *ut) {
    //
    //  Fast-path the common case.
    //     Common means current position is not at the beginning of a chunk
    //     and the preceding character is not supplementary.
    //
    int32_t i = ut->chunkOffset - 1;
    int64_t result;
    if (i >= 0) {
        UChar c = ut->chunkContents[i];
        if (U16_IS_TRAIL(c) == FALSE) {
            if (i <= ut->nativeIndexingLimit) {
                result = ut->chunkNativeStart + i;
            } else {
                ut->chunkOffset = i;
                result = ut->pFuncs->mapOffsetToNative(ut);
                ut->chunkOffset++;
            }
            return result;
        }
    }

    // If at the start of text, simply return 0.
    if (ut->chunkOffset==0 && ut->chunkNativeStart==0) {
        return 0;
    }

    // Harder, less common cases.  We are at a chunk boundary, or on a surrogate.
    //    Keep it simple, use other functions to handle the edges.
    //
    utext_previous32(ut);
    result = UTEXT_GETNATIVEINDEX(ut);
    utext_next32(ut);
    return result;
}


//
//  utext_current32.  Get the UChar32 at the current position.
//                    UText iteration position is always on a code point boundary,
//                    never on the trail half of a surrogate pair.
//
U_CAPI UChar32 U_EXPORT2
utext_current32(UText *ut) {
    UChar32  c;
    if (ut->chunkOffset==ut->chunkLength) {
        // Current position is just off the end of the chunk.
        if (ut->pFuncs->access(ut, ut->chunkNativeLimit, TRUE) == FALSE) {
            // Off the end of the text.
            return U_SENTINEL;
        }
    }

    c = ut->chunkContents[ut->chunkOffset];
    if (U16_IS_LEAD(c) == FALSE) {
        // Normal, non-supplementary case.
        return c;
    }

    //
    //  Possible supplementary char.
    //
    UChar32   trail = 0;
    UChar32   supplementaryC = c;
    if ((ut->chunkOffset+1) < ut->chunkLength) {
        // The trail surrogate is in the same chunk.
        trail = ut->chunkContents[ut->chunkOffset+1];
    } else {
        //  The trail surrogate is in a different chunk.
        //     Because we must maintain the iteration position, we need to switch forward
        //     into the new chunk, get the trail surrogate, then revert the chunk back to the
        //     original one.
        //     An edge case to be careful of:  the entire text may end with an unpaired
        //        leading surrogate.  The attempt to access the trail will fail, but
        //        the original position before the unpaired lead still needs to be restored.
        int64_t  nativePosition = ut->chunkNativeLimit;
        int32_t  originalOffset = ut->chunkOffset;
        if (ut->pFuncs->access(ut, nativePosition, TRUE)) {
            trail = ut->chunkContents[ut->chunkOffset];
        }
        UBool r = ut->pFuncs->access(ut, nativePosition, FALSE);  // reverse iteration flag loads preceding chunk
        U_ASSERT(r==TRUE);
        ut->chunkOffset = originalOffset;
        if(!r) {
            return U_SENTINEL;
        }
    }

    if (U16_IS_TRAIL(trail)) {
        supplementaryC = U16_GET_SUPPLEMENTARY(c, trail);
    }
    return supplementaryC;

}


U_CAPI UChar32 U_EXPORT2
utext_char32At(UText *ut, int64_t nativeIndex) {
    UChar32 c = U_SENTINEL;

    // Fast path the common case.
    if (nativeIndex>=ut->chunkNativeStart && nativeIndex < ut->chunkNativeStart + ut->nativeIndexingLimit) {
        ut->chunkOffset = (int32_t)(nativeIndex - ut->chunkNativeStart);
        c = ut->chunkContents[ut->chunkOffset];
        if (U16_IS_SURROGATE(c) == FALSE) {
            return c;
        }
    }


    utext_setNativeIndex(ut, nativeIndex);
    if (nativeIndex>=ut->chunkNativeStart && ut->chunkOffset<ut->chunkLength) {
        c = ut->chunkContents[ut->chunkOffset];
        if (U16_IS_SURROGATE(c)) {
            // For surrogates, let current32() deal with the complications
            //    of supplementaries that may span chunk boundaries.
            c = utext_current32(ut);
        }
    }
    return c;
}


U_CAPI UChar32 U_EXPORT2
utext_next32(UText *ut) {
    UChar32       c;

    if (ut->chunkOffset >= ut->chunkLength) {
        if (ut->pFuncs->access(ut, ut->chunkNativeLimit, TRUE) == FALSE) {
            return U_SENTINEL;
        }
    }

    c = ut->chunkContents[ut->chunkOffset++];
    if (U16_IS_LEAD(c) == FALSE) {
        // Normal case, not supplementary.
        //   (A trail surrogate seen here is just returned as is, as a surrogate value.
        //    It cannot be part of a pair.)
        return c;
    }

    if (ut->chunkOffset >= ut->chunkLength) {
        if (ut->pFuncs->access(ut, ut->chunkNativeLimit, TRUE) == FALSE) {
            // c is an unpaired lead surrogate at the end of the text.
            // return it as it is.
            return c;
        }
    }
    UChar32 trail = ut->chunkContents[ut->chunkOffset];
    if (U16_IS_TRAIL(trail) == FALSE) {
        // c was an unpaired lead surrogate, not at the end of the text.
        // return it as it is (unpaired).  Iteration position is on the
        // following character, possibly in the next chunk, where the
        //  trail surrogate would have been if it had existed.
        return c;
    }

    UChar32 supplementary = U16_GET_SUPPLEMENTARY(c, trail);
    ut->chunkOffset++;   // move iteration position over the trail surrogate.
    return supplementary;
    }


U_CAPI UChar32 U_EXPORT2
utext_previous32(UText *ut) {
    UChar32       c;

    if (ut->chunkOffset <= 0) {
        if (ut->pFuncs->access(ut, ut->chunkNativeStart, FALSE) == FALSE) {
            return U_SENTINEL;
        }
    }
    ut->chunkOffset--;
    c = ut->chunkContents[ut->chunkOffset];
    if (U16_IS_TRAIL(c) == FALSE) {
        // Normal case, not supplementary.
        //   (A lead surrogate seen here is just returned as is, as a surrogate value.
        //    It cannot be part of a pair.)
        return c;
    }

    if (ut->chunkOffset <= 0) {
        if (ut->pFuncs->access(ut, ut->chunkNativeStart, FALSE) == FALSE) {
            // c is an unpaired trail surrogate at the start of the text.
            // return it as it is.
            return c;
        }
    }

    UChar32 lead = ut->chunkContents[ut->chunkOffset-1];
    if (U16_IS_LEAD(lead) == FALSE) {
        // c was an unpaired trail surrogate, not at the end of the text.
        // return it as it is (unpaired).  Iteration position is at c
        return c;
    }

    UChar32 supplementary = U16_GET_SUPPLEMENTARY(lead, c);
    ut->chunkOffset--;   // move iteration position over the lead surrogate.
    return supplementary;
}



U_CAPI UChar32 U_EXPORT2
utext_next32From(UText *ut, int64_t index) {
    UChar32       c      = U_SENTINEL;

    if(index<ut->chunkNativeStart || index>=ut->chunkNativeLimit) {
        // Desired position is outside of the current chunk.
        if(!ut->pFuncs->access(ut, index, TRUE)) {
            // no chunk available here
            return U_SENTINEL;
        }
    } else if (index - ut->chunkNativeStart  <= (int64_t)ut->nativeIndexingLimit) {
        // Desired position is in chunk, with direct 1:1 native to UTF16 indexing
        ut->chunkOffset = (int32_t)(index - ut->chunkNativeStart);
    } else {
        // Desired position is in chunk, with non-UTF16 indexing.
        ut->chunkOffset = ut->pFuncs->mapNativeIndexToUTF16(ut, index);
    }

    c = ut->chunkContents[ut->chunkOffset++];
    if (U16_IS_SURROGATE(c)) {
        // Surrogates.  Many edge cases.  Use other functions that already
        //              deal with the problems.
        utext_setNativeIndex(ut, index);
        c = utext_next32(ut);
    }
    return c;
}


U_CAPI UChar32 U_EXPORT2
utext_previous32From(UText *ut, int64_t index) {
    //
    //  Return the character preceding the specified index.
    //  Leave the iteration position at the start of the character that was returned.
    //
    UChar32     cPrev;    // The character preceding cCurr, which is what we will return.

    // Address the chunk containg the position preceding the incoming index
    // A tricky edge case:
    //   We try to test the requested native index against the chunkNativeStart to determine
    //    whether the character preceding the one at the index is in the current chunk.
    //    BUT, this test can fail with UTF-8 (or any other multibyte encoding), when the
    //    requested index is on something other than the first position of the first char.
    //
    if(index<=ut->chunkNativeStart || index>ut->chunkNativeLimit) {
        // Requested native index is outside of the current chunk.
        if(!ut->pFuncs->access(ut, index, FALSE)) {
            // no chunk available here
            return U_SENTINEL;
        }
    } else if(index - ut->chunkNativeStart <= (int64_t)ut->nativeIndexingLimit) {
        // Direct UTF-16 indexing.
        ut->chunkOffset = (int32_t)(index - ut->chunkNativeStart);
    } else {
        ut->chunkOffset=ut->pFuncs->mapNativeIndexToUTF16(ut, index);
        if (ut->chunkOffset==0 && !ut->pFuncs->access(ut, index, FALSE)) {
            // no chunk available here
            return U_SENTINEL;
        }
    }

    //
    // Simple case with no surrogates.
    //
    ut->chunkOffset--;
    cPrev = ut->chunkContents[ut->chunkOffset];

    if (U16_IS_SURROGATE(cPrev)) {
        // Possible supplementary.  Many edge cases.
        // Let other functions do the heavy lifting.
        utext_setNativeIndex(ut, index);
        cPrev = utext_previous32(ut);
    }
    return cPrev;
}


U_CAPI int32_t U_EXPORT2
utext_extract(UText *ut,
             int64_t start, int64_t limit,
             UChar *dest, int32_t destCapacity,
             UErrorCode *status) {
    return ut->pFuncs->extract(ut, start, limit, dest, destCapacity, status);
}

//------------------------------------------------------------------------------
//
//   UText common functions implementation
//
//------------------------------------------------------------------------------

//
//  UText.flags bit definitions
//
enum {
    UTEXT_HEAP_ALLOCATED  = 1,      //  1 if ICU has allocated this UText struct on the heap.
                                    //  0 if caller provided storage for the UText.

    UTEXT_EXTRA_HEAP_ALLOCATED = 2, //  1 if ICU has allocated extra storage as a separate
                                    //     heap block.
                                    //  0 if there is no separate allocation.  Either no extra
                                    //     storage was requested, or it is appended to the end
                                    //     of the main UText storage.

    UTEXT_OPEN = 4                  //  1 if this UText is currently open
                                    //  0 if this UText is not open.
};


//
//  Extended form of a UText.  The purpose is to aid in computing the total size required
//    when a provider asks for a UText to be allocated with extra storage.

struct ExtendedUText {
    UText          ut;
    UAlignedMemory extension;
};

static const UText emptyText = UTEXT_INITIALIZER;

U_CAPI UText * U_EXPORT2
utext_setup(UText *ut, int32_t extraSpace, UErrorCode *status) {
    if (U_FAILURE(*status)) {
        return ut;
    }

    if (ut == NULL) {
        // We need to heap-allocate storage for the new UText
        int32_t spaceRequired = sizeof(UText);
        if (extraSpace > 0) {
            spaceRequired = sizeof(ExtendedUText) + extraSpace - sizeof(UAlignedMemory);
        }
        ut = (UText *)uprv_malloc(spaceRequired);
        if (ut == NULL) {
            *status = U_MEMORY_ALLOCATION_ERROR;
            return NULL;
        } else {
            *ut = emptyText;
            ut->flags |= UTEXT_HEAP_ALLOCATED;
            if (spaceRequired>0) {
                ut->extraSize = extraSpace;
                ut->pExtra    = &((ExtendedUText *)ut)->extension;
            }
        }
    } else {
        // We have been supplied with an already existing UText.
        // Verify that it really appears to be a UText.
        if (ut->magic != UTEXT_MAGIC) {
            *status = U_ILLEGAL_ARGUMENT_ERROR;
            return ut;
        }
        // If the ut is already open and there's a provider supplied close
        //   function, call it.
        if ((ut->flags & UTEXT_OPEN) && ut->pFuncs->close != NULL)  {
            ut->pFuncs->close(ut);
        }
        ut->flags &= ~UTEXT_OPEN;

        // If extra space was requested by our caller, check whether
        //   sufficient already exists, and allocate new if needed.
        if (extraSpace > ut->extraSize) {
            // Need more space.  If there is existing separately allocated space,
            //   delete it first, then allocate new space.
            if (ut->flags & UTEXT_EXTRA_HEAP_ALLOCATED) {
                uprv_free(ut->pExtra);
                ut->extraSize = 0;
            }
            ut->pExtra = uprv_malloc(extraSpace);
            if (ut->pExtra == NULL) {
                *status = U_MEMORY_ALLOCATION_ERROR;
            } else {
                ut->extraSize = extraSpace;
                ut->flags |= UTEXT_EXTRA_HEAP_ALLOCATED;
            }
        }
    }
    if (U_SUCCESS(*status)) {
        ut->flags |= UTEXT_OPEN;

        // Initialize all remaining fields of the UText.
        //
        ut->context             = NULL;
        ut->chunkContents       = NULL;
        ut->p                   = NULL;
        ut->q                   = NULL;
        ut->r                   = NULL;
        ut->a                   = 0;
        ut->b                   = 0;
        ut->c                   = 0;
        ut->chunkOffset         = 0;
        ut->chunkLength         = 0;
        ut->chunkNativeStart    = 0;
        ut->chunkNativeLimit    = 0;
        ut->nativeIndexingLimit = 0;
        ut->providerProperties  = 0;
        ut->privA               = 0;
        ut->privB               = 0;
        ut->privC               = 0;
        ut->privP               = NULL;
        if (ut->pExtra!=NULL && ut->extraSize>0)
            uprv_memset(ut->pExtra, 0, ut->extraSize);

    }
    return ut;
}


U_CAPI UText * U_EXPORT2
utext_close(UText *ut) {
    if (ut==NULL ||
        ut->magic != UTEXT_MAGIC ||
        (ut->flags & UTEXT_OPEN) == 0)
    {
        // The supplied ut is not an open UText.
        // Do nothing.
        return ut;
    }

    // If the provider gave us a close function, call it now.
    // This will clean up anything allocated specifically by the provider.
    if (ut->pFuncs->close != NULL) {
        ut->pFuncs->close(ut);
    }
    ut->flags &= ~UTEXT_OPEN;

    // If we (the framework) allocated the UText or subsidiary storage,
    //   delete it.
    if (ut->flags & UTEXT_EXTRA_HEAP_ALLOCATED) {
        uprv_free(ut->pExtra);
        ut->pExtra = NULL;
        ut->flags &= ~UTEXT_EXTRA_HEAP_ALLOCATED;
        ut->extraSize = 0;
    }

    // Zero out function table of the closed UText.  This is a defensive move,
    //   inteded to cause applications that inadvertantly use a closed
    //   utext to crash with null pointer errors.
    ut->pFuncs        = NULL;

    if (ut->flags & UTEXT_HEAP_ALLOCATED) {
        // This UText was allocated by UText setup.  We need to free it.
        // Clear magic, so we can detect if the user messes up and immediately
        //  tries to reopen another UText using the deleted storage.
        ut->magic = 0;
        uprv_free(ut);
        ut = NULL;
    }
    return ut;
}


//
// invalidateChunk   Reset a chunk to have no contents, so that the next call
//                   to access will cause new data to load.
//                   This is needed when copy/move/replace operate directly on the
//                   backing text, potentially putting it out of sync with the
//                   contents in the chunk.
//
static void
invalidateChunk(UText *ut) {
    ut->chunkLength = 0;
    ut->chunkNativeLimit = 0;
    ut->chunkNativeStart = 0;
    ut->chunkOffset = 0;
    ut->nativeIndexingLimit = 0;
}

//
// pinIndex        Do range pinning on a native index parameter.
//                 64 bit pinning is done in place.
//                 32 bit truncated result is returned as a convenience for
//                        use in providers that don't need 64 bits.
static int32_t
pinIndex(int64_t &index, int64_t limit) {
    if (index<0) {
        index = 0;
    } else if (index > limit) {
        index = limit;
    }
    return (int32_t)index;
}

//
// Pointer relocation function,
//   a utility used by shallow clone.
//   Adjust a pointer that refers to something within one UText (the source)
//   to refer to the same relative offset within a another UText (the target)
//
static void adjustPointer(UText *dest, const void **destPtr, const UText *src) {
    // convert all pointers to (char *) so that byte address arithmetic will work.
    char  *dptr = (char *)*destPtr;
    char  *dUText = (char *)dest;
    char  *sUText = (char *)src;

    if (dptr >= (char *)src->pExtra && dptr < ((char*)src->pExtra)+src->extraSize) {
        // target ptr was to something within the src UText's pExtra storage.
        //   relocate it into the target UText's pExtra region.
        *destPtr = ((char *)dest->pExtra) + (dptr - (char *)src->pExtra);
    } else if (dptr>=sUText && dptr < sUText+src->sizeOfStruct) {
        // target ptr was pointing to somewhere within the source UText itself.
        //   Move it to the same offset within the target UText.
        *destPtr = dUText + (dptr-sUText);
    }
}


//
//  Clone.  This is a generic copy-the-utext-by-value clone function that can be
//          used as-is with some utext types, and as a helper by other clones.
//
static UText * U_CALLCONV
shallowTextClone(UText * dest, const UText * src, UErrorCode * status) {
    if (U_FAILURE(*status)) {
        return NULL;
    }
    int32_t  srcExtraSize = src->extraSize;

    //
    // Use the generic text_setup to allocate storage if required.
    //
    dest = utext_setup(dest, srcExtraSize, status);
    if (U_FAILURE(*status)) {
        return dest;
    }

    //
    //  flags (how the UText was allocated) and the pointer to the
    //   extra storage must retain the values in the cloned utext that
    //   were set up by utext_setup.  Save them separately before
    //   copying the whole struct.
    //
    void *destExtra = dest->pExtra;
    int32_t flags   = dest->flags;


    //
    //  Copy the whole UText struct by value.
    //  Any "Extra" storage is copied also.
    //
    int sizeToCopy = src->sizeOfStruct;
    if (sizeToCopy > dest->sizeOfStruct) {
        sizeToCopy = dest->sizeOfStruct;
    }
    uprv_memcpy(dest, src, sizeToCopy);
    dest->pExtra = destExtra;
    dest->flags  = flags;
    if (srcExtraSize > 0) {
        uprv_memcpy(dest->pExtra, src->pExtra, srcExtraSize);
    }

    //
    // Relocate any pointers in the target that refer to the UText itself
    //   to point to the cloned copy rather than the original source.
    //
    adjustPointer(dest, &dest->context, src);
    adjustPointer(dest, &dest->p, src);
    adjustPointer(dest, &dest->q, src);
    adjustPointer(dest, &dest->r, src);
	adjustPointer(dest, (const void **)&dest->chunkContents, src);

    return dest;
}

static UText * U_CALLCONV
ucstrTextClone(UText *dest, const UText * src, UBool deep, UErrorCode * status) {
    // First do a generic shallow clone.
    dest = shallowTextClone(dest, src, status);

    // For deep clones, make a copy of the string.
    //  The copied storage is owned by the newly created clone.
    //  A non-NULL pointer in UText.p is the signal to the close() function to delete
    //    it.
    //
    if (deep && U_SUCCESS(*status)) {
        U_ASSERT(utext_nativeLength(dest) < INT32_MAX);
        int32_t  len = (int32_t)utext_nativeLength(dest);

        // The cloned string IS going to be NUL terminated, whether or not the original was.
        const UChar *srcStr = (const UChar *)src->context;
        UChar *copyStr = (UChar *)uprv_malloc((len+1) * sizeof(UChar));
        if (copyStr == NULL) {
            *status = U_MEMORY_ALLOCATION_ERROR;
        } else {
            int64_t i;
            for (i=0; i<len; i++) {
                copyStr[i] = srcStr[i];
            }
            copyStr[len] = 0;
            dest->context = copyStr;
            dest->providerProperties |= I32_FLAG(UTEXT_PROVIDER_OWNS_TEXT);
        }
    }
    return dest;
}


static void U_CALLCONV
ucstrTextClose(UText *ut) {
    // Most of the work of close is done by the generic UText framework close.
    // All that needs to be done here is delete the string if the UText
    //  owns it.  This occurs if the UText was created by cloning.
    if (ut->providerProperties & I32_FLAG(UTEXT_PROVIDER_OWNS_TEXT)) {
        UChar *s = (UChar *)ut->context;
        uprv_free(s);
        ut->context = NULL;
    }
}

//------------------------------------------------------------------------------
//
//     UText implementation for const UChar * strings
//
//         Use of UText data members:
//            context    pointer to UnicodeString
//            a          length.  -1 if not yet known.
//
//         TODO:  support 64 bit lengths.
//
//------------------------------------------------------------------------------
static int64_t U_CALLCONV
ucstrTextLength(UText *ut) {
    if (ut->a < 0) {
        // null terminated, we don't yet know the length.  Scan for it.
        //    Access is not convenient for doing this
        //    because the current interation postion can't be changed.
        const UChar  *str = (const UChar *)ut->context;
        for (;;) {
            if (str[ut->chunkNativeLimit] == 0) {
                break;
            }
            ut->chunkNativeLimit++;
        }
        ut->a = ut->chunkNativeLimit;
        ut->chunkLength = (int32_t)ut->chunkNativeLimit;
        ut->nativeIndexingLimit = ut->chunkLength;
        ut->providerProperties &= ~I32_FLAG(UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE);
    }
    return ut->a;
}


static UBool U_CALLCONV
ucstrTextAccess(UText *ut, int64_t index, UBool  forward) {
    const UChar *str   = (const UChar *)ut->context;

    // pin the requested index to the bounds of the string,
    //  and set current iteration position.
    if (index<0) {
        index = 0;
    } else if (index < ut->chunkNativeLimit) {
        // The request data is within the chunk as it is known so far.
        // Put index on a code point boundary.
        U16_SET_CP_START(str, 0, index);
    } else if (ut->a >= 0) {
        // We know the length of this string, and the user is requesting something
        // at or beyond the length.  Pin the requested index to the length.
        index = ut->a;
    } else {
        // Null terminated string, length not yet known, and the requested index
        //  is beyond where we have scanned so far.
        //  Scan to 32 UChars beyond the requested index.  The strategy here is
        //  to avoid fully scanning a long string when the caller only wants to
        //  see a few characters at its beginning.
        int32_t scanLimit = (int32_t)index + 32;
        if ((index + 32)>INT32_MAX || (index + 32)<0 ) {   // note: int64 expression
            scanLimit = INT32_MAX;
        }

        int32_t chunkLimit = (int32_t)ut->chunkNativeLimit;
        for (; chunkLimit<scanLimit; chunkLimit++) {
            if (str[chunkLimit] == 0) {
                // We found the end of the string.  Remember it, pin the requested index to it,
                //  and bail out of here.
                ut->a = chunkLimit;
                ut->chunkLength = chunkLimit;
                ut->nativeIndexingLimit = chunkLimit;
                if (index >= chunkLimit) {
                    index = chunkLimit;
                } else {
                    U16_SET_CP_START(str, 0, index);
                }

                ut->chunkNativeLimit = chunkLimit;
                ut->providerProperties &= ~I32_FLAG(UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE);
                goto breakout;
            }
        }
        // We scanned through the next batch of UChars without finding the end.
        U16_SET_CP_START(str, 0, index);
        if (chunkLimit == INT32_MAX) {
            // Scanned to the limit of a 32 bit length.
            // Forceably trim the overlength string back so length fits in int32
            //  TODO:  add support for 64 bit strings.
            ut->a = chunkLimit;
            ut->chunkLength = chunkLimit;
            ut->nativeIndexingLimit = chunkLimit;
            if (index > chunkLimit) {
                index = chunkLimit;
            }
            ut->chunkNativeLimit = chunkLimit;
            ut->providerProperties &= ~I32_FLAG(UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE);
        } else {
            // The endpoint of a chunk must not be left in the middle of a surrogate pair.
            // If the current end is on a lead surrogate, back the end up by one.
            // It doesn't matter if the end char happens to be an unpaired surrogate,
            //    and it's simpler not to worry about it.
            if (U16_IS_LEAD(str[chunkLimit-1])) {
                --chunkLimit;
            }
            ut->chunkNativeLimit = chunkLimit;
        }

    }
breakout:
    U_ASSERT(index<=INT32_MAX);
    ut->chunkOffset = (int32_t)index;

    // Check whether request is at the start or end
    UBool retVal = (forward && index<ut->chunkNativeLimit) || (!forward && index>0);
    return retVal;
}


// From ustring.c
/* NUL-termination of strings ----------------------------------------------- */

/**
 * NUL-terminate a string no matter what its type.
 * Set warning and error codes accordingly.
 */
#define __TERMINATE_STRING(dest, destCapacity, length, pErrorCode)      \
    if(pErrorCode!=NULL && U_SUCCESS(*pErrorCode)) {                    \
        /* not a public function, so no complete argument checking */   \
                                                                        \
        if(length<0) {                                                  \
            /* assume that the caller handles this */                   \
        } else if(length<destCapacity) {                                \
            /* NUL-terminate the string, the NUL fits */                \
            dest[length]=0;                                             \
            /* unset the not-terminated warning but leave all others */ \
            if(*pErrorCode==U_STRING_NOT_TERMINATED_WARNING) {          \
                *pErrorCode=U_ZERO_ERROR;                               \
            }                                                           \
        } else if(length==destCapacity) {                               \
            /* unable to NUL-terminate, but the string itself fit - set a warning code */ \
            *pErrorCode=U_STRING_NOT_TERMINATED_WARNING;                \
        } else /* length>destCapacity */ {                              \
            /* even the string itself did not fit - set an error code */ \
            *pErrorCode=U_BUFFER_OVERFLOW_ERROR;                        \
        }                                                               \
    }

U_CAPI int32_t U_EXPORT2
u_terminateUChars(UChar *dest, int32_t destCapacity, int32_t length, UErrorCode *pErrorCode) {
    __TERMINATE_STRING(dest, destCapacity, length, pErrorCode);
    return length;
}

static int32_t U_CALLCONV
ucstrTextExtract(UText *ut,
                  int64_t start, int64_t limit,
                  UChar *dest, int32_t destCapacity,
                  UErrorCode *pErrorCode)
{
    if(U_FAILURE(*pErrorCode)) {
        return 0;
    }
    if(destCapacity<0 || (dest==NULL && destCapacity>0) || start>limit) {
        *pErrorCode=U_ILLEGAL_ARGUMENT_ERROR;
        return 0;
    }

    const UChar *s=(const UChar *)ut->context;
    int32_t si, di;

    int32_t start32;
    int32_t limit32;

    // Access the start.  Does two things we need:
    //   Pins 'start' to the length of the string, if it came in out-of-bounds.
    //   Snaps 'start' to the beginning of a code point.
    ucstrTextAccess(ut, start, TRUE);
    U_ASSERT(start <= INT32_MAX);
    start32 = (int32_t)start;

    int32_t strLength=(int32_t)ut->a;
    if (strLength >= 0) {
        limit32 = pinIndex(limit, strLength);
    } else {
        limit32 = pinIndex(limit, INT32_MAX);
    }

    di = 0;
    for (si=start32; si<limit32; si++) {
        if (strLength<0 && s[si]==0) {
            // Just hit the end of a null-terminated string.
            ut->a = si;               // set string length for this UText
            ut->chunkNativeLimit    = si;
            ut->chunkLength         = si;
            ut->nativeIndexingLimit = si;
            strLength               = si;
            break;
        }
        if (di<destCapacity) {
            // only store if there is space.
            dest[di] = s[si];
        } else {
            if (strLength>=0) {
                // We have filled the destination buffer, and the string length is known.
                //  Cut the loop short.  There is no need to scan string termination.
                di = strLength;
                si = limit32;
                break;
            }
        }
        di++;
    }

    // If the limit index points to a lead surrogate of a pair,
    //   add the corresponding trail surrogate to the destination.
    if (si>0 && U16_IS_LEAD(s[si-1]) &&
        ((si<strLength || strLength<0)  && U16_IS_TRAIL(s[si])))
    {
        if (di<destCapacity) {
            // store only if there is space in the output buffer.
            dest[di++] = s[si++];
        }
    }

    // Put iteration position at the point just following the extracted text
    ut->chunkOffset = si;

    // Add a terminating NUL if space in the buffer permits,
    // and set the error status as required.
    u_terminateUChars(dest, destCapacity, di, pErrorCode);
    return di;
}

static const struct UTextFuncs ucstrFuncs = 
{
    sizeof(UTextFuncs),
    0, 0, 0,           // Reserved alignment padding
    ucstrTextClone,
    ucstrTextLength,
    ucstrTextAccess,
    ucstrTextExtract,
    NULL,              // Replace
    NULL,              // Copy
    NULL,              // MapOffsetToNative,
    NULL,              // MapIndexToUTF16,
    ucstrTextClose,
    NULL,              // spare 1
    NULL,              // spare 2
    NULL,              // spare 3
};


U_CAPI UText * U_EXPORT2
utext_openUChars(UText *ut, const UChar *s, int64_t length, UErrorCode *status) {
    if (U_FAILURE(*status)) {
        return NULL;
    }
    if (length < -1 || length>INT32_MAX) {
        *status = U_ILLEGAL_ARGUMENT_ERROR;
        return NULL;
    }
    ut = utext_setup(ut, 0, status);
    if (U_SUCCESS(*status)) {
        ut->pFuncs               = &ucstrFuncs;
        ut->context              = s;
        ut->providerProperties   = I32_FLAG(UTEXT_PROVIDER_STABLE_CHUNKS);
        if (length==-1) {
            ut->providerProperties |= I32_FLAG(UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE);
        }
        ut->a                    = length;
        ut->chunkContents        = s;
        ut->chunkNativeStart     = 0;
        ut->chunkNativeLimit     = length>=0? length : 0;
        ut->chunkLength          = (int32_t)ut->chunkNativeLimit;
        ut->chunkOffset          = 0;
        ut->nativeIndexingLimit  = ut->chunkLength;
    }
    return ut;
}
