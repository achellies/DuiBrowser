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
// EAWebKitGraphics.h
// Created by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITGRAPHICS_H
#define EAWEBKIT_EAWEBKITGRAPHICS_H


#include <EAWebKit/EAWebKitConfig.h>


namespace EA
{
    namespace WebKit
    {
        // Currently this is a copy of the CursorId enum in BCCursorEA.h.
        // We need to implement a clean way whereby we can share a declaration
        // between these two without exposing the WebKit internal headers 
        // to the user. There are a few options for this, but in the meantime
        // we have compile-time checks which validate that these enumerations
        // are in sync with each other. 

        enum CursorId
        {
            kCursorIdPointer,
            kCursorIdCross,
            kCursorIdHand,
            kCursorIdMove,
            kCursorIdIBeam,
            kCursorIdWait,
            kCursorIdHelp,
            kCursorIdResizeE,
            kCursorIdResizeN,
            kCursorIdResizeNE,
            kCursorIdResizeNW,
            kCursorIdResizeS,
            kCursorIdResizeSE,
            kCursorIdResizeSW,
            kCursorIdResizeW,
            kCursorIdResizeNS,
            kCursorIdResizeEW,
            kCursorIdResizeNESW,
            kCursorIdResizeNWSE,
            kCursorIdResizeColumn,
            kCursorIdResizeRow,
            kCursorIdVerticalText,
            kCursorIdCell,
            kCursorIdContextMenu,
            kCursorIdNoDrop,
            kCursorIdNotAllowed,
            kCursorIdProgress,
            kCursorIdZAlias,
            kCursorIdZoomIn,
            kCursorIdZoomOut,
            kCursorIdCopy,
            kCursorIdNone,

            kCursorIdCount
        };

    } // namespace WebKit

} // namespace EA





#endif // Header include guard










