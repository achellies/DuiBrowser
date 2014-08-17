/*
Copyright (C) 2004,2009-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// TextBreakIterator.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef TextBreakIterator_h
#define TextBreakIterator_h

#include <wtf/unicode/Unicode.h>

namespace OWBAL {

    class TextBreakIterator;

    // Note: The returned iterator is good only until you get another iterator.

    // Note by Paul Pedriana: It seems that WebKit accesses these iterators only 
    // ever one at a time, and not recursively or by multiple threads. The implication
    // is that we can have a single version of each 

    TextBreakIterator* characterBreakIterator(const UChar*, int length);
    TextBreakIterator* wordBreakIterator     (const UChar*, int length);
    TextBreakIterator* lineBreakIterator     (const UChar*, int length);
    TextBreakIterator* sentenceBreakIterator (const UChar*, int length);

    int  textBreakFirst    (TextBreakIterator*);
    int  textBreakNext     (TextBreakIterator*);
    int  textBreakCurrent  (TextBreakIterator*);
    int  textBreakPreceding(TextBreakIterator*, int);
    int  textBreakFollowing(TextBreakIterator*, int);
    bool isTextBreak       (TextBreakIterator*, int);

    const int TextBreakDone = -1;

}

#endif
