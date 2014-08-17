/* Portions of this code based on ICU. */
/*
 ********************************************************************************
 *   Copyright (C) 1996-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 ********************************************************************************
 */

#include "utext.cpp"

// The state number of the starting state
#define START_STATE 1

// The state-transition value indicating "stop"
#define STOP_STATE  0

class BreakIterator
{
public:
	BreakIterator(const DataHeader* pData, UErrorCode &status);

	void setText(const UChar* text, int32_t textLength, UErrorCode &status);

	enum {
        /**
         * DONE is returned by previous() and next() after all valid
         * boundaries have been returned.
         * @stable ICU 2.0
         */
        DONE = (int32_t)-1
    };

	/**
     * Sets the current iteration position to the beginning of the text.
     * @return The offset of the beginning of the text.
     *  @stable ICU 2.0
     */
    virtual int32_t first(void);

    /**
     * Sets the current iteration position to the end of the text.
     * @return The text's past-the-end offset.
     *  @stable ICU 2.0
     */
    virtual int32_t last(void);

    /**
     * Advances the iterator either forward or backward the specified number of steps.
     * Negative values move backward, and positive values move forward.  This is
     * equivalent to repeatedly calling next() or previous().
     * @param n The number of steps to move.  The sign indicates the direction
     * (negative is backwards, and positive is forwards).
     * @return The character offset of the boundary position n boundaries away from
     * the current one.
     *  @stable ICU 2.0
     */
    virtual int32_t next(int32_t n);

    /**
     * Advances the iterator to the next boundary position.
     * @return The position of the first boundary after this one.
     *  @stable ICU 2.0
     */
    virtual int32_t next(void);

    /**
     * Moves the iterator backwards, to the last boundary preceding this one.
     * @return The position of the last boundary position preceding this one.
     *  @stable ICU 2.0
     */
    virtual int32_t previous(void);

    /**
     * Sets the iterator to refer to the first boundary position following
     * the specified position.
     * @param offset The position from which to begin searching for a break position.
     * @return The position of the first break after the current position.
     *  @stable ICU 2.0
     */
    virtual int32_t following(int32_t offset);

    /**
     * Sets the iterator to refer to the last boundary position before the
     * specified position.
     * @param offset The position to begin searching for a break from.
     * @return The position of the last boundary before the starting position.
     *  @stable ICU 2.0
     */
    virtual int32_t preceding(int32_t offset);

    /**
     * Returns true if the specfied position is a boundary position.  As a side
     * effect, leaves the iterator pointing to the first boundary position at
     * or after "offset".
     * @param offset the offset to check.
     * @return True if "offset" is a boundary position.
     *  @stable ICU 2.0
     */
    virtual UBool isBoundary(int32_t offset);

    /**
     * Returns the current iteration position.
     * @return The current iteration position.
     * @stable ICU 2.0
     */
    virtual int32_t current(void) const;

private:
	void init(const RBBIDataHeader* data, UErrorCode &status);

private:
	const DataHeader         *fData;
	const RBBIDataHeader     *fHeader;

	const RBBIStateTable     *fForwardTable;
    const RBBIStateTable     *fReverseTable;
    const RBBIStateTable     *fSafeFwdTable;
    const RBBIStateTable     *fSafeRevTable;
    const UChar              *fRuleSource;
    const int32_t            *fRuleStatusTable; 

    /* number of int32_t values in the rule status table.   Used to sanity check indexing */
    int32_t                  fStatusMaxIdx;

    UTrie                    fTrie;

	UnicodeString            fRuleString;

	UText                    fTextData;
	UText                    *fText;

private:
	 /** Index of the Rule {tag} values for the most recent match.
     *  @internal
    */
    int32_t             fLastRuleStatusIndex;

    /**
     * Rule tag value valid flag.
     * Some iterator operations don't intrinsically set the correct tag value.
     * This flag lets us lazily compute the value if we are ever asked for it.
     * @internal
     */
    UBool               fLastStatusIndexValid;

    /**
     * Counter for the number of characters encountered with the "dictionary"
     *   flag set.
     * @internal
     */
    uint32_t            fDictionaryCharCount;

	/**
     * When a range of characters is divided up using the dictionary, the break
     * positions that are discovered are stored here, preventing us from having
     * to use either the dictionary or the state table again until the iterator
     * leaves this range of text. Has the most impact for line breaking.
     * @internal
     */
    int32_t*            fCachedBreakPositions;

    /**
     * The number of elements in fCachedBreakPositions
     * @internal
     */
    int32_t             fNumCachedBreakPositions;

    /**
     * if fCachedBreakPositions is not null, this indicates which item in the
     * cache the current iteration position refers to
     * @internal
     */
    int32_t             fPositionInCache;

private:
	/**
     * Dumps caches and performs other actions associated with a complete change
     * in text or iteration position.
     * @internal
     */
    virtual void reset(void);

	/**
     * This method backs the iterator back up to a "safe position" in the text.
     * This is a position that we know, without any context, must be a break position.
     * The various calling methods then iterate forward from this safe position to
     * the appropriate position to return.  (For more information, see the description
     * of buildBackwardsStateTable() in BreakIterator.Builder.)
     * @param statetable state table used of moving backwards
     * @internal
     */
    int32_t handlePrevious(const RBBIStateTable *statetable);

    /**
     * This method is the actual implementation of the next() method.  All iteration
     * vectors through here.  This method initializes the state machine to state 1
     * and advances through the text character by character until we reach the end
     * of the text or the state machine transitions to state 0.  We update our return
     * value every time the state machine passes through a possible end state.
     * @param statetable state table used of moving forwards
     * @internal
     */
    int32_t handleNext(const RBBIStateTable *statetable);

	/**
     * This is the function that actually implements dictionary-based
     * breaking.  Covering at least the range from startPos to endPos,
     * it checks for dictionary characters, and if it finds them determines
     * the appropriate object to deal with them. It may cache found breaks in
     * fCachedBreakPositions as it goes. It may well also look at text outside
     * the range startPos to endPos.
     * If going forward, endPos is the normal Unicode break result, and
     * if goind in reverse, startPos is the normal Unicode break result
     * @param startPos  The start position of a range of text
     * @param endPos    The end position of a range of text
     * @param reverse   The call is for the reverse direction
     * @internal
     */
    int32_t checkDictionary(int32_t startPos, int32_t endPos, UBool reverse);
};

static int32_t U_CALLCONV
getFoldingOffset(uint32_t data) {
    /* if bit 15 is set, then the folding offset is in bits 14..0 of the 16-bit trie result */
    if(data&0x8000) {
        return (int32_t)(data&0x7fff);
    } else {
        return 0;
    }
}

BreakIterator::BreakIterator(const DataHeader* data, UErrorCode &status)
: fText(&fTextData), fCachedBreakPositions(0)
{
	UText ut = UTEXT_INITIALIZER;
	fTextData = ut;
	const RBBIDataHeader *d = (const RBBIDataHeader *)
        ((char *)data + data->dataHeader.headerSize);
    init(d, status);
	fData = data;
}

void BreakIterator::init(const RBBIDataHeader* data, UErrorCode &status)
{
	if (U_FAILURE(status)) {
        return;
    }
	fHeader = data;
    if (fHeader->fMagic != 0xb1a0 || 
        !(fHeader->fFormatVersion[0] == 3 ||         // ICU 3.4 
          *(int32_t *)fHeader->fFormatVersion == 1))  // ICU 3.2 and earlier.
    {
        status = U_INVALID_FORMAT_ERROR;
        return;
    }

    fReverseTable = NULL;
    fSafeFwdTable = NULL;
    fSafeRevTable = NULL;
    if (data->fFTableLen != 0) {
        fForwardTable = (RBBIStateTable *)((char *)data + fHeader->fFTable);
    }
    if (data->fRTableLen != 0) {
        fReverseTable = (RBBIStateTable *)((char *)data + fHeader->fRTable);
    }
    if (data->fSFTableLen != 0) {
        fSafeFwdTable = (RBBIStateTable *)((char *)data + fHeader->fSFTable);
    }
    if (data->fSRTableLen != 0) {
        fSafeRevTable = (RBBIStateTable *)((char *)data + fHeader->fSRTable);
    }

    utrie_unserialize(&fTrie,
                       (uint8_t *)data + fHeader->fTrie,
                       fHeader->fTrieLen,
                       &status);
    if (U_FAILURE(status)) {
        return;
    }
    fTrie.getFoldingOffset=getFoldingOffset;


    fRuleSource   = (UChar *)((char *)data + fHeader->fRuleSource);
    fRuleString.setTo(TRUE, fRuleSource, -1);
    U_ASSERT(data->fRuleSourceLen > 0);

    fRuleStatusTable = (int32_t *)((char *)data + fHeader->fStatusTable);
    fStatusMaxIdx    = data->fStatusTableLen / sizeof(int32_t);
}

void BreakIterator::setText(const UChar* text, int32_t textLength, UErrorCode &status)
{
	utext_openUChars(fText, text, textLength, &status);
	this->first();
}

/**
 * Sets the current iteration position to the beginning of the text.
 * @return The offset of the beginning of the text.
 */
int32_t BreakIterator::first(void) {
    reset();
    fLastRuleStatusIndex  = 0;
    fLastStatusIndexValid = TRUE;
    //if (fText == NULL)
    //    return BreakIterator::DONE;

    utext_setNativeIndex(fText, 0);
    return 0;
}

/**
 * Sets the current iteration position to the end of the text.
 * @return The text's past-the-end offset.
 */
int32_t BreakIterator::last(void) {
    reset();
    if (fText == NULL) {
        fLastRuleStatusIndex  = 0;
        fLastStatusIndexValid = TRUE;
        return BreakIterator::DONE;
    }

    fLastStatusIndexValid = FALSE;
    int32_t pos = (int32_t)utext_nativeLength(fText);
    utext_setNativeIndex(fText, pos);
    return pos;
}

/**
 * Advances the iterator either forward or backward the specified number of steps.
 * Negative values move backward, and positive values move forward.  This is
 * equivalent to repeatedly calling next() or previous().
 * @param n The number of steps to move.  The sign indicates the direction
 * (negative is backwards, and positive is forwards).
 * @return The character offset of the boundary position n boundaries away from
 * the current one.
 */
int32_t BreakIterator::next(int32_t n) {
    int32_t result = current();
    while (n > 0) {
        result = next();
        --n;
    }
    while (n < 0) {
        result = previous();
        ++n;
    }
    return result;
}

/**
 * Advances the iterator to the next boundary position.
 * @return The position of the first boundary after this one.
 */
int32_t BreakIterator::next(void) {
    // if we have cached break positions and we're still in the range
    // covered by them, just move one step forward in the cache
    if (fCachedBreakPositions != NULL) {
        if (fPositionInCache < fNumCachedBreakPositions - 1) {
            ++fPositionInCache;
            int32_t pos = fCachedBreakPositions[fPositionInCache];
            utext_setNativeIndex(fText, pos);
            return pos;
        }
        else {
            reset();
        }
    }

    int32_t startPos = current();
    int32_t result = handleNext(fForwardTable);
    if (fDictionaryCharCount > 0) {
        result = checkDictionary(startPos, result, FALSE);
    }
    return result;
}

/**
 * Advances the iterator backwards, to the last boundary preceding this one.
 * @return The position of the last boundary position preceding this one.
 */
int32_t BreakIterator::previous(void) {
    int32_t result;
    int32_t startPos;

    // if we have cached break positions and we're still in the range
    // covered by them, just move one step backward in the cache
    if (fCachedBreakPositions != NULL) {
        if (fPositionInCache > 0) {
            --fPositionInCache;
            // If we're at the beginning of the cache, need to reevaluate the
            // rule status
            if (fPositionInCache <= 0) {
                fLastStatusIndexValid = FALSE;
            }
            int32_t pos = fCachedBreakPositions[fPositionInCache];
            utext_setNativeIndex(fText, pos);
            return pos;
        }
        else {
            reset();
        }
    }

    // if we're already sitting at the beginning of the text, return DONE
    if (fText == NULL || (startPos = current()) == 0) {
        fLastRuleStatusIndex  = 0;
        fLastStatusIndexValid = TRUE;
        return BreakIterator::DONE;
    }

    if (fSafeRevTable != NULL || fSafeFwdTable != NULL) {
        result = handlePrevious(fReverseTable);
        if (fDictionaryCharCount > 0) {
            result = checkDictionary(result, startPos, TRUE);
        }
        return result;
    }

    // old rule syntax
    // set things up.  handlePrevious() will back us up to some valid
    // break position before the current position (we back our internal
    // iterator up one step to prevent handlePrevious() from returning
    // the current position), but not necessarily the last one before

    // where we started

    int32_t start = current();

    UTEXT_PREVIOUS32(fText);
    int32_t lastResult    = handlePrevious(fReverseTable);
    if (lastResult == UBRK_DONE) {
        lastResult = 0;
        utext_setNativeIndex(fText, 0);
    }
    result = lastResult;
    int32_t lastTag       = 0;
    UBool   breakTagValid = FALSE;

    // iterate forward from the known break position until we pass our
    // starting point.  The last break position before the starting
    // point is our return value

    for (;;) {
        result         = next();
        if (result == BreakIterator::DONE || result >= start) {
            break;
        }
        lastResult     = result;
        lastTag        = fLastRuleStatusIndex;
        breakTagValid  = TRUE;
    }

    // fLastBreakTag wants to have the value for section of text preceding
    // the result position that we are to return (in lastResult.)  If
    // the backwards rules overshot and the above loop had to do two or more
    // next()s to move up to the desired return position, we will have a valid
    // tag value. But, if handlePrevious() took us to exactly the correct result positon,
    // we wont have a tag value for that position, which is only set by handleNext().

    // set the current iteration position to be the last break position
    // before where we started, and then return that value
    utext_setNativeIndex(fText, lastResult);
    fLastRuleStatusIndex  = lastTag;       // for use by getRuleStatus()
    fLastStatusIndexValid = breakTagValid;

    // No need to check the dictionary; it will have been handled by
    // next()

    return lastResult;
}

/**
 * Sets the iterator to refer to the first boundary position following
 * the specified position.
 * @offset The position from which to begin searching for a break position.
 * @return The position of the first break after the current position.
 */
int32_t BreakIterator::following(int32_t offset) {
    // if we have cached break positions and offset is in the range
    // covered by them, use them
    // TODO: could use binary search
    // TODO: what if offset is outside range, but break is not?
    if (fCachedBreakPositions != NULL) {
        if (offset >= fCachedBreakPositions[0]
                && offset < fCachedBreakPositions[fNumCachedBreakPositions - 1]) {
            fPositionInCache = 0;
            // We are guaranteed not to leave the array due to range test above
            while (offset >= fCachedBreakPositions[fPositionInCache]) {
                ++fPositionInCache;
            }
            int32_t pos = fCachedBreakPositions[fPositionInCache];
            utext_setNativeIndex(fText, pos);
            return pos;
        }
        else {
            reset();
        }
    }

    // if the offset passed in is already past the end of the text,
    // just return DONE; if it's before the beginning, return the
    // text's starting offset
    fLastRuleStatusIndex  = 0;
    fLastStatusIndexValid = TRUE;
    if (fText == NULL || offset >= utext_nativeLength(fText)) {
        last();
        return next();
    }
    else if (offset < 0) {
        return first();
    }

    // otherwise, set our internal iteration position (temporarily)
    // to the position passed in.  If this is the _beginning_ position,
    // then we can just use next() to get our return value

    int32_t result = 0;

    if (fSafeRevTable != NULL) {
        // new rule syntax
        utext_setNativeIndex(fText, offset);
        // move forward one codepoint to prepare for moving back to a
        // safe point.
        // this handles offset being between a supplementary character
        UTEXT_NEXT32(fText);
        // handlePrevious will move most of the time to < 1 boundary away
        handlePrevious(fSafeRevTable);
        int32_t result = next();
        while (result <= offset) {
            result = next();
        }
        return result;
    }
    if (fSafeFwdTable != NULL) {
        // backup plan if forward safe table is not available
        utext_setNativeIndex(fText, offset);
        UTEXT_PREVIOUS32(fText);
        // handle next will give result >= offset
        handleNext(fSafeFwdTable);
        // previous will give result 0 or 1 boundary away from offset,
        // most of the time
        // we have to
        int32_t oldresult = previous();
        while (oldresult > offset) {
            int32_t result = previous();
            if (result <= offset) {
                return oldresult;
            }
            oldresult = result;
        }
        int32_t result = next();
        if (result <= offset) {
            return next();
        }
        return result;
    }
    // otherwise, we have to sync up first.  Use handlePrevious() to back
    // up to a known break position before the specified position (if
    // we can determine that the specified position is a break position,
    // we don't back up at all).  This may or may not be the last break
    // position at or before our starting position.  Advance forward
    // from here until we've passed the starting position.  The position
    // we stop on will be the first break position after the specified one.
    // old rule syntax

    utext_setNativeIndex(fText, offset);
    if (offset==0 || 
        offset==1  && utext_getNativeIndex(fText)==0) {
        return next();
    }
    result = previous();

    while (result != BreakIterator::DONE && result <= offset) {
        result = next();
    }

    return result;
}

/**
 * Sets the iterator to refer to the last boundary position before the
 * specified position.
 * @offset The position to begin searching for a break from.
 * @return The position of the last boundary before the starting position.
 */
int32_t BreakIterator::preceding(int32_t offset) {
    // if we have cached break positions and offset is in the range
    // covered by them, use them
    if (fCachedBreakPositions != NULL) {
        // TODO: binary search?
        // TODO: What if offset is outside range, but break is not?
        if (offset > fCachedBreakPositions[0]
                && offset <= fCachedBreakPositions[fNumCachedBreakPositions - 1]) {
            fPositionInCache = 0;
            while (fPositionInCache < fNumCachedBreakPositions
                   && offset > fCachedBreakPositions[fPositionInCache])
                ++fPositionInCache;
            --fPositionInCache;
            // If we're at the beginning of the cache, need to reevaluate the
            // rule status
            if (fPositionInCache <= 0) {
                fLastStatusIndexValid = FALSE;
            }
            utext_setNativeIndex(fText, fCachedBreakPositions[fPositionInCache]);
            return fCachedBreakPositions[fPositionInCache];
        }
        else {
            reset();
        }
    }

    // if the offset passed in is already past the end of the text,
    // just return DONE; if it's before the beginning, return the
    // text's starting offset
    if (fText == NULL || offset > utext_nativeLength(fText)) {
        // return BreakIterator::DONE;
        return last();
    }
    else if (offset < 0) {
        return first();
    }

    // if we start by updating the current iteration position to the
    // position specified by the caller, we can just use previous()
    // to carry out this operation

    if (fSafeFwdTable != NULL) {
        // new rule syntax
        utext_setNativeIndex(fText, offset);
        int32_t newOffset = (int32_t)UTEXT_GETNATIVEINDEX(fText);
        if (newOffset != offset) {
            // Will come here if specified offset was not a code point boundary AND
            //   the underlying implmentation is using UText, which snaps any non-code-point-boundary
            //   indices to the containing code point.
            // For breakitereator::preceding only, these non-code-point indices need to be moved
            //   up to refer to the following codepoint.
            UTEXT_NEXT32(fText);
            offset = (int32_t)UTEXT_GETNATIVEINDEX(fText);
        }

        // TODO:  (synwee) would it be better to just check for being in the middle of a surrogate pair,
        //        rather than adjusting the position unconditionally?
        //        (Change would interact with safe rules.)
        // TODO:  change RBBI behavior for off-boundary indices to match that of UText?
        //        affects only preceding(), seems cleaner, but is slightly different.
        UTEXT_PREVIOUS32(fText);
        handleNext(fSafeFwdTable);
        int32_t result = (int32_t)UTEXT_GETNATIVEINDEX(fText);
        while (result >= offset) {
            result = previous();
        }
        return result;
    }
    if (fSafeRevTable != NULL) {
        // backup plan if forward safe table is not available
        //  TODO:  check whether this path can be discarded
        //         It's probably OK to say that rules must supply both safe tables
        //            if they use safe tables at all.  We have certainly never described
        //            to anyone how to work with just one safe table.
        utext_setNativeIndex(fText, offset);
        UTEXT_NEXT32(fText);
        
        // handle previous will give result <= offset
        handlePrevious(fSafeRevTable);

        // next will give result 0 or 1 boundary away from offset,
        // most of the time
        // we have to
        int32_t oldresult = next();
        while (oldresult < offset) {
            int32_t result = next();
            if (result >= offset) {
                return oldresult;
            }
            oldresult = result;
        }
        int32_t result = previous();
        if (result >= offset) {
            return previous();
        }
        return result;
    }

    // old rule syntax
    utext_setNativeIndex(fText, offset);
    return previous();
}

/**
 * Returns true if the specfied position is a boundary position.  As a side
 * effect, leaves the iterator pointing to the first boundary position at
 * or after "offset".
 * @param offset the offset to check.
 * @return True if "offset" is a boundary position.
 */
UBool BreakIterator::isBoundary(int32_t offset) {
    // the beginning index of the iterator is always a boundary position by definition
    if (offset == 0) {
        first();       // For side effects on current position, tag values.
        return TRUE;
    }

    if (offset == (int32_t)utext_nativeLength(fText)) {
        last();       // For side effects on current position, tag values.
        return TRUE;
    }

    // out-of-range indexes are never boundary positions
    if (offset < 0) {
        first();       // For side effects on current position, tag values.
        return FALSE;
    }

    if (offset > utext_nativeLength(fText)) {
        last();        // For side effects on current position, tag values.
        return FALSE;
    }

    // otherwise, we can use following() on the position before the specified
    // one and return true if the position we get back is the one the user
    // specified
    utext_previous32From(fText, offset);
    int32_t backOne = (int32_t)UTEXT_GETNATIVEINDEX(fText);
    UBool    result  = following(backOne) == offset;
    return result;
}

/**
 * Returns the current iteration position.
 * @return The current iteration position.
 */
int32_t BreakIterator::current(void) const {
    int32_t  pos = (int32_t)UTEXT_GETNATIVEINDEX(fText);
    return pos;
}
 
//=======================================================================
// implementation
//=======================================================================

//
// RBBIRunMode  -  the state machine runs an extra iteration at the beginning and end
//                 of user text.  A variable with this enum type keeps track of where we
//                 are.  The state machine only fetches user input while in the RUN mode.
//
enum RBBIRunMode {
    RBBI_START,     // state machine processing is before first char of input
    RBBI_RUN,       // state machine processing is in the user text
    RBBI_END        // state machine processing is after end of user text.
};


//-----------------------------------------------------------------------------------
//
//  handleNext(stateTable)
//     This method is the actual implementation of the rbbi next() method. 
//     This method initializes the state machine to state 1
//     and advances through the text character by character until we reach the end
//     of the text or the state machine transitions to state 0.  We update our return
//     value every time the state machine passes through an accepting state.
//
//-----------------------------------------------------------------------------------
int32_t BreakIterator::handleNext(const RBBIStateTable *statetable) {
    int32_t             state;
    int16_t             category        = 0;
    RBBIRunMode         mode;
    
    RBBIStateTableRow  *row;
    UChar32             c;
    int32_t             lookaheadStatus = 0;
    int32_t             lookaheadTagIdx = 0;
    int32_t             result          = 0;
    int32_t             initialPosition = 0;
    int32_t             lookaheadResult = 0;
    UBool               lookAheadHardBreak = (statetable->fFlags & RBBI_LOOKAHEAD_HARD_BREAK) != 0;
    const char         *tableData       = statetable->fTableData;
    uint32_t            tableRowLen     = statetable->fRowLen;

    #ifdef RBBI_DEBUG
        if (fTrace) {
            RBBIDebugPuts("Handle Next   pos   char  state category");
        }
    #endif

    // No matter what, handleNext alway correctly sets the break tag value.
    fLastStatusIndexValid = TRUE;
    fLastRuleStatusIndex = 0;

    // if we're already at the end of the text, return DONE.
    initialPosition = (int32_t)UTEXT_GETNATIVEINDEX(fText); 
    result          = initialPosition;
    c               = UTEXT_NEXT32(fText);
    if (fData == NULL || c==U_SENTINEL) {
        return BreakIterator::DONE;
    }

    //  Set the initial state for the state machine
    state = START_STATE;
    row = (RBBIStateTableRow *)
            //(statetable->fTableData + (statetable->fRowLen * state));
            (tableData + tableRowLen * state);
            
    
    mode     = RBBI_RUN;
    if (statetable->fFlags & RBBI_BOF_REQUIRED) {
        category = 2;
        mode     = RBBI_START;
    }


    // loop until we reach the end of the text or transition to state 0
    //
    for (;;) {
        if (c == U_SENTINEL) {
            // Reached end of input string.
            if (mode == RBBI_END) {
                // We have already run the loop one last time with the 
                //   character set to the psueudo {eof} value.  Now it is time
                //   to unconditionally bail out.
                if (lookaheadResult > result) {
                    // We ran off the end of the string with a pending look-ahead match.
                    // Treat this as if the look-ahead condition had been met, and return
                    //  the match at the / position from the look-ahead rule.
                    result               = lookaheadResult;
                    fLastRuleStatusIndex = lookaheadTagIdx;
                    lookaheadStatus = 0;
                } 
                break;
            }
            // Run the loop one last time with the fake end-of-input character category.
            mode = RBBI_END;
            category = 1;
        }

        //
        // Get the char category.  An incoming category of 1 or 2 means that
        //      we are preset for doing the beginning or end of input, and
        //      that we shouldn't get a category from an actual text input character.
        //
        if (mode == RBBI_RUN) {
            // look up the current character's character category, which tells us
            // which column in the state table to look at.
            // Note:  the 16 in UTRIE_GET16 refers to the size of the data being returned,
            //        not the size of the character going in, which is a UChar32.
            //
            UTRIE_GET16(&fTrie, c, category);

            // Check the dictionary bit in the character's category.
            //    Counter is only used by dictionary based iterators (subclasses).
            //    Chars that need to be handled by a dictionary have a flag bit set
            //    in their category values.
            //
            if ((category & 0x4000) != 0)  {
                fDictionaryCharCount++;
                //  And off the dictionary flag bit.
                category &= ~0x4000;
            }
        }

        #ifdef RBBI_DEBUG
            if (fTrace) {
                RBBIDebugPrintf("             %4d   ", utext_getNativeIndex(fText));
                if (0x20<=c && c<0x7f) {
                    RBBIDebugPrintf("\"%c\"  ", c);
                } else {
                    RBBIDebugPrintf("%5x  ", c);
                }
                RBBIDebugPrintf("%3d  %3d\n", state, category);
            }
        #endif

        // State Transition - move machine to its next state
        //
        state = row->fNextState[category];
        row = (RBBIStateTableRow *)
            // (statetable->fTableData + (statetable->fRowLen * state));
            (tableData + tableRowLen * state);


        if (row->fAccepting == -1) {
            // Match found, common case.
            if (mode != RBBI_START) {
                result = (int32_t)UTEXT_GETNATIVEINDEX(fText);
            }
            fLastRuleStatusIndex = row->fTagIdx;   // Remember the break status (tag) values.
        }

        if (row->fLookAhead != 0) {
            if (lookaheadStatus != 0
                && row->fAccepting == lookaheadStatus) {
                // Lookahead match is completed.  
                result               = lookaheadResult;
                fLastRuleStatusIndex = lookaheadTagIdx;
                lookaheadStatus      = 0;
                // TODO:  make a standalone hard break in a rule work.
                if (lookAheadHardBreak) {
                    UTEXT_SETNATIVEINDEX(fText, result);
                    return result;
                }
                // Look-ahead completed, but other rules may match further.  Continue on
                //  TODO:  junk this feature?  I don't think it's used anywhwere.
                goto continueOn;
            }

            int32_t  r = (int32_t)UTEXT_GETNATIVEINDEX(fText);
            lookaheadResult = r;
            lookaheadStatus = row->fLookAhead;
            lookaheadTagIdx = row->fTagIdx;
            goto continueOn;
        }


        if (row->fAccepting != 0) {
            // Because this is an accepting state, any in-progress look-ahead match
            //   is no longer relavant.  Clear out the pending lookahead status.
            lookaheadStatus = 0;           // clear out any pending look-ahead match.
        }

continueOn:
        if (state == STOP_STATE) {
            // This is the normal exit from the lookup state machine.
            // We have advanced through the string until it is certain that no
            //   longer match is possible, no matter what characters follow.
            break;
        }
        
        // Advance to the next character.  
        // If this is a beginning-of-input loop iteration, don't advance
        //    the input position.  The next iteration will be processing the
        //    first real input character.
        if (mode == RBBI_RUN) {
            c = UTEXT_NEXT32(fText);
        } else {
            if (mode == RBBI_START) {
                mode = RBBI_RUN;
            }
        }


    }

    // The state machine is done.  Check whether it found a match...

    // If the iterator failed to advance in the match engine, force it ahead by one.
    //   (This really indicates a defect in the break rules.  They should always match
    //    at least one character.)
    if (result == initialPosition) {
        UTEXT_SETNATIVEINDEX(fText, initialPosition);
        UTEXT_NEXT32(fText);
        result = (int32_t)UTEXT_GETNATIVEINDEX(fText);
    }

    // Leave the iterator at our result position.
    UTEXT_SETNATIVEINDEX(fText, result);
    #ifdef RBBI_DEBUG
        if (fTrace) {
            RBBIDebugPrintf("result = %d\n\n", result);
        }
    #endif
    return result;
}



//-----------------------------------------------------------------------------------
//
//  handlePrevious()
//
//      Iterate backwards, according to the logic of the reverse rules.
//      This version handles the exact style backwards rules.
//
//      The logic of this function is very similar to handleNext(), above.
//
//-----------------------------------------------------------------------------------
int32_t BreakIterator::handlePrevious(const RBBIStateTable *statetable) {
    int32_t             state;
    int16_t             category        = 0;
    RBBIRunMode         mode;
    RBBIStateTableRow  *row;
    UChar32             c;
    int32_t             lookaheadStatus = 0;
    int32_t             result          = 0;
    int32_t             initialPosition = 0;
    int32_t             lookaheadResult = 0;
    UBool               lookAheadHardBreak = (statetable->fFlags & RBBI_LOOKAHEAD_HARD_BREAK) != 0;

    #ifdef RBBI_DEBUG
        if (fTrace) {
            RBBIDebugPuts("Handle Previous   pos   char  state category");
        }
    #endif

    // handlePrevious() never gets the rule status.
    // Flag the status as invalid; if the user ever asks for status, we will need
    // to back up, then re-find the break position using handleNext(), which does
    // get the status value.
    fLastStatusIndexValid = FALSE;
    fLastRuleStatusIndex = 0;

    // if we're already at the start of the text, return DONE.
    if (fText == NULL || fData == NULL || UTEXT_GETNATIVEINDEX(fText)==0) {
        return BreakIterator::DONE;
    }

    //  Set up the starting char.
    initialPosition = (int32_t)UTEXT_GETNATIVEINDEX(fText);
    result          = initialPosition;
    c               = UTEXT_PREVIOUS32(fText);

    //  Set the initial state for the state machine
    state = START_STATE;
    row = (RBBIStateTableRow *)
            (statetable->fTableData + (statetable->fRowLen * state));
    category = 3;
    mode     = RBBI_RUN;
    if (statetable->fFlags & RBBI_BOF_REQUIRED) {
        category = 2;
        mode     = RBBI_START;
    }


    // loop until we reach the start of the text or transition to state 0
    //
    for (;;) {
        if (c == U_SENTINEL) {
            // Reached end of input string.
            if (mode == RBBI_END || 
                *(int32_t *)fHeader->fFormatVersion == 1 ) {
                // We have already run the loop one last time with the 
                //   character set to the psueudo {eof} value.  Now it is time
                //   to unconditionally bail out.
                //  (Or we have an old format binary rule file that does not support {eof}.)
                if (lookaheadResult < result) {
                    // We ran off the end of the string with a pending look-ahead match.
                    // Treat this as if the look-ahead condition had been met, and return
                    //  the match at the / position from the look-ahead rule.
                    result               = lookaheadResult;
                    lookaheadStatus = 0;
                } else if (result == initialPosition) {
                    // Ran off start, no match found.
                    // move one index one (towards the start, since we are doing a previous())
                    UTEXT_SETNATIVEINDEX(fText, initialPosition);
                    UTEXT_PREVIOUS32(fText);   // TODO:  shouldn't be necessary.  We're already at beginning.  Check.
                }
                break;
            }
            // Run the loop one last time with the fake end-of-input character category.
            mode = RBBI_END;
            category = 1;
        }

        //
        // Get the char category.  An incoming category of 1 or 2 means that
        //      we are preset for doing the beginning or end of input, and
        //      that we shouldn't get a category from an actual text input character.
        //
        if (mode == RBBI_RUN) {
            // look up the current character's character category, which tells us
            // which column in the state table to look at.
            // Note:  the 16 in UTRIE_GET16 refers to the size of the data being returned,
            //        not the size of the character going in, which is a UChar32.
            //
            UTRIE_GET16(&fTrie, c, category);

            // Check the dictionary bit in the character's category.
            //    Counter is only used by dictionary based iterators (subclasses).
            //    Chars that need to be handled by a dictionary have a flag bit set
            //    in their category values.
            //
            if ((category & 0x4000) != 0)  {
                fDictionaryCharCount++;
                //  And off the dictionary flag bit.
                category &= ~0x4000;
            }
        }

        #ifdef RBBI_DEBUG
            if (fTrace) {
                RBBIDebugPrintf("             %4d   ", (int32_t)utext_getNativeIndex(fText));
                if (0x20<=c && c<0x7f) {
                    RBBIDebugPrintf("\"%c\"  ", c);
                } else {
                    RBBIDebugPrintf("%5x  ", c);
                }
                RBBIDebugPrintf("%3d  %3d\n", state, category);
            }
        #endif

        // State Transition - move machine to its next state
        //
        state = row->fNextState[category];
        row = (RBBIStateTableRow *)
            (statetable->fTableData + (statetable->fRowLen * state));

        if (row->fAccepting == -1) {
            // Match found, common case.
            result = (int32_t)UTEXT_GETNATIVEINDEX(fText);
        }

        if (row->fLookAhead != 0) {
            if (lookaheadStatus != 0
                && row->fAccepting == lookaheadStatus) {
                // Lookahead match is completed.  
                result               = lookaheadResult;
                lookaheadStatus      = 0;
                // TODO:  make a standalone hard break in a rule work.
                if (lookAheadHardBreak) {
                    UTEXT_SETNATIVEINDEX(fText, result);
                    return result;
                }
                // Look-ahead completed, but other rules may match further.  Continue on
                //  TODO:  junk this feature?  I don't think it's used anywhwere.
                goto continueOn;
            }

            int32_t  r = (int32_t)UTEXT_GETNATIVEINDEX(fText);
            lookaheadResult = r;
            lookaheadStatus = row->fLookAhead;
            goto continueOn;
        }


        if (row->fAccepting != 0) {
            // Because this is an accepting state, any in-progress look-ahead match
            //   is no longer relavant.  Clear out the pending lookahead status.
            lookaheadStatus = 0;    
        }

continueOn:
        if (state == STOP_STATE) {
            // This is the normal exit from the lookup state machine.
            // We have advanced through the string until it is certain that no
            //   longer match is possible, no matter what characters follow.
            break;
        }

        // Move (backwards) to the next character to process.  
        // If this is a beginning-of-input loop iteration, don't advance
        //    the input position.  The next iteration will be processing the
        //    first real input character.
        if (mode == RBBI_RUN) {
            c = UTEXT_PREVIOUS32(fText);
        } else {            
            if (mode == RBBI_START) {
                mode = RBBI_RUN;
            }
        }
    }

    // The state machine is done.  Check whether it found a match...

    // If the iterator failed to advance in the match engine, force it ahead by one.
    //   (This really indicates a defect in the break rules.  They should always match
    //    at least one character.)
    if (result == initialPosition) {
        UTEXT_SETNATIVEINDEX(fText, initialPosition);
        UTEXT_PREVIOUS32(fText);
        result = (int32_t)UTEXT_GETNATIVEINDEX(fText);
    }

    // Leave the iterator at our result position.
    UTEXT_SETNATIVEINDEX(fText, result);
    #ifdef RBBI_DEBUG
        if (fTrace) {
            RBBIDebugPrintf("result = %d\n\n", result);
        }
    #endif
    return result;
}


void
BreakIterator::reset()
{
    if (fCachedBreakPositions) {
        uprv_free(fCachedBreakPositions);
    }
    fCachedBreakPositions = NULL;
    fNumCachedBreakPositions = 0;
    fDictionaryCharCount = 0;
    fPositionInCache = 0;
}


//-------------------------------------------------------------------------------
//
//  checkDictionary       This function handles all processing of characters in
//                        the "dictionary" set. It will determine the appropriate
//                        course of action, and possibly set up a cache in the
//                        process.
//
//-------------------------------------------------------------------------------
int32_t BreakIterator::checkDictionary(int32_t startPos,
                            int32_t endPos,
                            UBool reverse) {
#if 1
	return reverse ? startPos : endPos;
#else
    // Reset the old break cache first.
    uint32_t dictionaryCount = fDictionaryCharCount;
    reset();

    if (dictionaryCount <= 1 || (endPos - startPos) <= 1) {
        return (reverse ? startPos : endPos);
    }
    
    // Starting from the starting point, scan towards the proposed result,
    // looking for the first dictionary character (which may be the one
    // we're on, if we're starting in the middle of a range).
    utext_setNativeIndex(fText, reverse ? endPos : startPos);
    if (reverse) {
        UTEXT_PREVIOUS32(fText);
    }
    
    int32_t rangeStart = startPos;
    int32_t rangeEnd = endPos;

    uint16_t    category;
    int32_t     current;
    UErrorCode  status = U_ZERO_ERROR;
    UStack      breaks(status);
    int32_t     foundBreakCount = 0;
    UChar32     c = utext_current32(fText);

    UTRIE_GET16(&fData->fTrie, c, category);
    
    // Is the character we're starting on a dictionary character? If so, we
    // need to back up to include the entire run; otherwise the results of
    // the break algorithm will differ depending on where we start. Since
    // the result is cached and there is typically a non-dictionary break
    // within a small number of words, there should be little performance impact.
    if (category & 0x4000) {
        if (reverse) {
            do {
                utext_next32(fText);          // TODO:  recast to work directly with postincrement.
                c = utext_current32(fText);
                UTRIE_GET16(&fData->fTrie, c, category);
            } while (c != U_SENTINEL && (category & 0x4000));
            // Back up to the last dictionary character
            rangeEnd = (int32_t)UTEXT_GETNATIVEINDEX(fText);
            if (c == U_SENTINEL) {
                // c = fText->last32();
                //   TODO:  why was this if needed?
                c = UTEXT_PREVIOUS32(fText);
            }
            else {
                c = UTEXT_PREVIOUS32(fText);
            }
        }
        else {
            do {
                c = UTEXT_PREVIOUS32(fText);
                UTRIE_GET16(&fData->fTrie, c, category);
            }
            while (c != U_SENTINEL && (category & 0x4000));
            // Back up to the last dictionary character
            if (c == U_SENTINEL) {
                // c = fText->first32();
                c = utext_current32(fText);
            }
            else {
                utext_next32(fText);
                c = utext_current32(fText);
            }
            rangeStart = (int32_t)UTEXT_GETNATIVEINDEX(fText);;
        }
        UTRIE_GET16(&fData->fTrie, c, category);
    }
    
    // Loop through the text, looking for ranges of dictionary characters.
    // For each span, find the appropriate break engine, and ask it to find
    // any breaks within the span.
    // Note: we always do this in the forward direction, so that the break
    // cache is built in the right order.
    if (reverse) {
        utext_setNativeIndex(fText, rangeStart);
        c = utext_current32(fText);
        UTRIE_GET16(&fData->fTrie, c, category);
    }
    while(U_SUCCESS(status)) {
        while((current = (int32_t)UTEXT_GETNATIVEINDEX(fText)) < rangeEnd && (category & 0x4000) == 0) {
            utext_next32(fText);           // TODO:  tweak for post-increment operation
            c = utext_current32(fText);
            UTRIE_GET16(&fData->fTrie, c, category);
        }
        if (current >= rangeEnd) {
            break;
        }
        
        // We now have a dictionary character. Get the appropriate language object
        // to deal with it.
        const LanguageBreakEngine *lbe = getLanguageBreakEngine(c);
        
        // Ask the language object if there are any breaks. It will leave the text
        // pointer on the other side of its range, ready to search for the next one.
        if (lbe != NULL) {
            foundBreakCount += lbe->findBreaks(fText, rangeStart, rangeEnd, FALSE, fBreakType, breaks);
        }
        
        // Reload the loop variables for the next go-round
        c = utext_current32(fText);
        UTRIE_GET16(&fData->fTrie, c, category);
    }
    
    // If we found breaks, build a new break cache. The first and last entries must
    // be the original starting and ending position.
    if (foundBreakCount > 0) {
        int32_t totalBreaks = foundBreakCount;
        if (startPos < breaks.elementAti(0)) {
            totalBreaks += 1;
        }
        if (endPos > breaks.peeki()) {
            totalBreaks += 1;
        }
        fCachedBreakPositions = (int32_t *)uprv_malloc(totalBreaks * sizeof(int32_t));
        if (fCachedBreakPositions != NULL) {
            int32_t out = 0;
            fNumCachedBreakPositions = totalBreaks;
            if (startPos < breaks.elementAti(0)) {
                fCachedBreakPositions[out++] = startPos;
            }
            for (int32_t i = 0; i < foundBreakCount; ++i) {
                fCachedBreakPositions[out++] = breaks.elementAti(i);
            }
            if (endPos > fCachedBreakPositions[out-1]) {
                fCachedBreakPositions[out] = endPos;
            }
            // If there are breaks, then by definition, we are replacing the original
            // proposed break by one of the breaks we found. Use following() and
            // preceding() to do the work. They should never recurse in this case.
            if (reverse) {
                return preceding(endPos - 1);
            }
            else {
                return following(startPos);
            }
        }
        // If the allocation failed, just fall through to the "no breaks found" case.
    }

    // If we get here, there were no language-based breaks. Set the text pointer
    // to the original proposed break.
    utext_setNativeIndex(fText, reverse ? startPos : endPos);
    return (reverse ? startPos : endPos);
#endif
}
