/* Portions of this code based on ICU. */
/*
 ********************************************************************************
 *   Copyright (C) 1996-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 ********************************************************************************
 */

class U_COMMON_API UnicodeString
{
public:
	UnicodeString() : fArray(0), fCapacity(0) {}

	UnicodeString &setTo(UBool isTerminated, const UChar *text, int32_t textLength);

private:
	UChar     *fArray;    // the Unicode data (aligned at 12B (32b pointers) or 16B (64b))
    int32_t   fCapacity;  // sizeof fArray
};

UnicodeString &
UnicodeString::setTo(UBool isTerminated,
                     const UChar *text,
                     int32_t textLength)
{
	if (textLength < 0)
	{
		const UChar *c;
		for (c = text; *c; c++);
		textLength = c - text;
	}

	fArray = (UChar *)text;
	fCapacity = isTerminated ? textLength + 1 : textLength;

	return *this;
}
