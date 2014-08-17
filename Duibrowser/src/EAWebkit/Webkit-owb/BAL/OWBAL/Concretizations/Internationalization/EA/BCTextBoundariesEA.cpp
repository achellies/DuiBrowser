/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

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
// BCTextBoundariesEA.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "TextBoundaries.h"
#include <EAWebKit/EAWebKitTextInterface.h>
#include <EAWebKit/EAWebKit.h>  // For font server

namespace OWBAL {

int findNextWordFromIndex(const UChar* chars, int len, int position, bool forward)
{
    /// The arrows below show the positions that would be returned by 
    /// successive calls to GetNextWordBreak with the default wordBreakType.
    ///    "Hello   world   hello   world"
    ///     ^    ^  ^    ^  ^    ^  ^    ^

    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer)
        return 0;
    return pServer->FindNextWordFromIndex((EA::WebKit::Char*) chars, len, position, forward);
 }

void findWordBoundary(const UChar* chars, int len, int position, int* start, int* end)
{
    /// The arrows below show the positions that would be returned by 
    /// successive calls to GetNextWordBreak with the default wordBreakType.
    ///    "Hello   world   hello   world"
    ///     ^    ^  ^    ^  ^    ^  ^    ^

    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(pServer)    
        pServer->FindWordBoundary( (EA::WebKit::Char*)chars, len, position, start, end);
}

} // namespace WebCore
