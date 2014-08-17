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
// BalTypeEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


// This header is specific to the EA port of OWB WebKit and is #included by BAL/Base/BalBase.h


#ifndef BALType_h
#define BALType_h

#include <wtf/FastAllocBase.h>

#include <EABase/eabase.h>
#include <EAWebKit/EAWebKitInput.h>
#include <EAWebKit/EAWebKitTextInterface.h>
#include <EARaster/EARaster.h>

struct BalResizeEvent
{
    int w;
    int h;
};


struct BalQuitEvent/*: public WTF::FastAllocBase*/
{
};


struct BalEventExpose/*: public WTF::FastAllocBase*/
{
};


struct BalUserEvent/*: public WTF::FastAllocBase*/
{
    int   code;
    void* data1;
    void* data2;
};


// See EAWebKit/EAWebKitInput.h
typedef EA::WebKit::KeyboardEvent    BalEventKey;
typedef EA::WebKit::MouseButtonEvent BalEventButton;
typedef EA::WebKit::MouseMoveEvent   BalEventMotion;
typedef EA::WebKit::MouseWheelEvent  BalEventScroll;


// Fonts
typedef EA::WebKit::IFont BalFontFace;
typedef void             BalFont;


// Graphics
namespace EA {
    namespace Raster {
        class  Surface;
        struct Rect;
        struct Point;
    }
}

namespace WKAL {
    class Color;
}

typedef WKAL::Color          BalColor;
typedef EA::Raster::Point    BalPoint;
typedef EA::Raster::Rect     BalRectangle;
typedef EA::Raster::ISurface  BalSurface;
typedef EA::Raster::ISurface  BalMenu;
typedef EA::Raster::ISurface  BalWidget;
typedef EA::Raster::ISurface  PlatformGraphicsContext;


// UI
struct _FcPattern;

typedef struct _FcPattern   BalPattern;
typedef void                BalScaledFont;
typedef void                BalDrawable;
typedef void                BalMenuItem;
typedef void                BalClipboard;
typedef void                BalTargetList;
typedef void                BalAdjustment;
typedef void                BalContainer;
typedef void                BalPixbuf;
typedef struct _BalMatrix{} BalMatrix;


namespace WKAL {
    class FloatSize;

    typedef void*           PlatformGradient;
    typedef void*           PlatformPath;
    typedef int             PlatformCursor;     // This is a guid or enum id.
    typedef BalWidget*      PlatformWidget;     // PlatformWidget refers to the platform-specfic viewport. For Windows this would typically be HWND. In the simplest case it is an ARGB buffer.
    typedef void*           DragImageRef;
    typedef void*           DragDataRef;
    typedef unsigned short  GlyphBufferGlyph;
    typedef WKAL::FloatSize GlyphBufferAdvance;
}

#endif // Header include guard
