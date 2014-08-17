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
// EAWebKitForwardDeclarations.h
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITFORWARDDECLARATIONS_H
#define EAWEBKIT_EAWEBKITFORWARDDECLARATIONS_H


///////////////////////////////////////////////////////////////////////
// The purpose of this section is to forward-declare things that 
// EAWebKit.h uses so that EAWebKit.h doesn't get too messy by 
// having these declarations in it.
///////////////////////////////////////////////////////////////////////

extern "C" void WTFLogEvent(const char* format, ...);


class WebView;
class WebFrame;


namespace WebCore
{
    class Frame;
    class FrameView;
    class Page;
    class Document;
}


namespace EA
{
    namespace Raster
    {
        class Surface;
    }

    namespace WebKit
    {
        class View;
    }
}


#endif // Header include guard
