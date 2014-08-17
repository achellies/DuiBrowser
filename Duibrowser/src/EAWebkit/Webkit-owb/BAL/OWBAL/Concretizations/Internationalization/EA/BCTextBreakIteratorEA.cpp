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
// TextBreakIterator.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "TextBreakIterator.h"
#include "TextBreakIteratorInternalICU.h"
#include "PlatformString.h"
#include <wtf/Assertions.h>
#include "BALBase.h"
#include <EAWebKit/EAWebKitTextInterface.h>
#include <EAWebKit/EAWebKit.h>   

namespace OWBAL {

// The sole purpose of this class is to wrap our own type of break iterator.
// We can possibly avoid this wrapper class if we go to the header file and:
//     typedef EA::Text::BreakIteratorBase TextBreakIterator;
// But let's wait until we do the other modifications listed below first.
#include <wtf/FastAllocBase.h>

class TextBreakIterator/*: public WTF::FastAllocBase*/
{
public:
    void* mpBreakIterator;
};


// To do: Make these no longer be static or global when we update the EATextBreak interface as discussed below. 
static TextBreakIterator gTextBreakIterator;

TextBreakIterator* characterBreakIterator(const UChar* pText, int length)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    gTextBreakIterator.mpBreakIterator = pServer->CharacterBreakIterator((EA::WebKit::Char *) pText, length);    
    return &gTextBreakIterator;
}

TextBreakIterator* wordBreakIterator(const UChar* pText, int length)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    gTextBreakIterator.mpBreakIterator =pServer->WordBreakIterator((EA::WebKit::Char *) pText, length);
    return &gTextBreakIterator;
}

TextBreakIterator* lineBreakIterator(const UChar* pText, int length)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    gTextBreakIterator.mpBreakIterator =pServer->LineBreakIterator((EA::WebKit::Char *) pText, length);
    return &gTextBreakIterator;
}

TextBreakIterator* sentenceBreakIterator(const UChar* pText, int length)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    gTextBreakIterator.mpBreakIterator =pServer->SentenceBreakIterator((EA::WebKit::Char *) pText, length);
    return &gTextBreakIterator;
}

int textBreakFirst(TextBreakIterator* pIterator)
{
    // We have a problem: EATextBreak doesn't implement a pure-virtual NextBreak/PrevBreak function.
    // It's easy to do this but it requires us to update EAText. We do something of a hack in the 
    // short run to allow this to work now by checking each of the global variables manually.
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;    
    return pServer->TextBreakFirst(gTextBreakIterator.mpBreakIterator);   
}

int textBreakNext(TextBreakIterator* pIterator)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    return pServer->TextBreakNext(gTextBreakIterator.mpBreakIterator);
 }

int textBreakCurrent(TextBreakIterator* pIterator)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    return pServer->TextBreakCurrent(gTextBreakIterator.mpBreakIterator);
}

int textBreakPreceding(TextBreakIterator* pIterator, int position)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    return pServer->TextBreakPreceding(gTextBreakIterator.mpBreakIterator, position);
}

int textBreakFollowing(TextBreakIterator* pIterator, int position)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    return pServer->TextBreakFollowing(gTextBreakIterator.mpBreakIterator, position);
}

bool isTextBreak(TextBreakIterator* pIterator, int position)
{
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return true;
    return pServer->IsTextBreak(gTextBreakIterator.mpBreakIterator, position);  
}

#if 0 

    // Basic but not Unicode-conforming version.

    class TextBreakIterator/*: public WTF::FastAllocBase*/
    {
    public:
        virtual ~TextBreakIterator(){};
        virtual int first() = 0;
        virtual int next() = 0;
        virtual int previous() = 0;
        inline int following(int pos) {
            currentPos = pos;
            return next();
        }
        inline int preceding(int pos) {
            currentPos = pos;
            return previous();
        }
        int currentPos;
        const UChar *string;
        int length;
    };


    class WordBreakIterator : public TextBreakIterator
    {
    public:
        virtual int first();
        virtual int next();
        virtual int previous();
    };


    class CharBreakIterator : public TextBreakIterator
    {
    public:
        virtual int first();
        virtual int next();
        virtual int previous();
        Vector<UChar> layout;
    };

    int WordBreakIterator::first() {
        currentPos = 0;
        return currentPos;
    }

    int WordBreakIterator::next() {
        if (currentPos == length) {
            currentPos = -1;
            return currentPos;
        }

        bool  haveSpace = false;
        UChar c(' ');

        while (currentPos < length) {
            if (haveSpace && !(string[currentPos] == 0x20) )
                break;
            if (string[currentPos] == 0x20)
                haveSpace = true;
            ++currentPos;
        }
        return currentPos;
    }

    int WordBreakIterator::previous() {
        if (currentPos == 0) {
            currentPos = -1;
            return currentPos;
        }

        bool haveSpace = false;

        while (currentPos > 0) {
            if (haveSpace && !string[currentPos] == 0x20)
                break;
            if (string[currentPos] == 0x20)
                haveSpace = true;
            --currentPos;
        }
        return currentPos;
    }

    int CharBreakIterator::first() {
        currentPos = 0;
        return currentPos;
    }

    int CharBreakIterator::next() {
        if (currentPos == length)
            return -1;
        currentPos++;
        return currentPos;
    }

    int CharBreakIterator::previous() {
        if (currentPos == 0)
            return -1;
        currentPos--;
        return currentPos;
    }


    TextBreakIterator* wordBreakIterator(const UChar* string, int length)
    {
        static WordBreakIterator *iterator = 0;

        if (!iterator)
            iterator = new WordBreakIterator;

        iterator->string = string;
        iterator->length = length;
        iterator->currentPos = 0;

        return iterator;
    }

    TextBreakIterator* characterBreakIterator(const UChar* string, int length)
    {
        static CharBreakIterator *iterator = 0;

        if (!iterator)
            iterator = new CharBreakIterator;

        iterator->string = string;
        iterator->length = length;
        iterator->currentPos = 0;

        for( int i=0; i< length; i++ )
            iterator->layout.append( string[i] );

        return iterator;
    }

    TextBreakIterator* lineBreakIterator(const UChar*, int)
    {
        return NULL;
    }

    TextBreakIterator* sentenceBreakIterator(const UChar*, int)
    {
        return NULL;
    }

    int textBreakFirst(TextBreakIterator* bi)
    {
        return bi->first();
    }

    int textBreakNext(TextBreakIterator* bi)
    {
        return bi->next();
    }

    int textBreakPreceding(TextBreakIterator* bi, int pos)
    {
        return bi->preceding(pos);
    }

    int textBreakFollowing(TextBreakIterator* bi, int pos)
    {
        return bi->following(pos);
    }

    int textBreakCurrent(TextBreakIterator* bi)
    {
        return bi->currentPos;
    }

    bool isTextBreak(TextBreakIterator*, int)
    {
        return true;
    }

    #endif // #if 0

} // namespace
