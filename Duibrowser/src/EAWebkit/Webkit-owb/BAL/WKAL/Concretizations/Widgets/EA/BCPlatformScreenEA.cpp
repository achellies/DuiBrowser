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
// BCPlatformScreenEA.cpp
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "PlatformScreen.h"
#include "Assertions.h"
#include "Widget.h"

#include <EARaster/EARaster.h>


namespace WKAL {

int screenDepth(Widget* widget)
{
    ASSERT(widget->containingWindow());
    return widget->containingWindow()->GetPixelFormat().mBytesPerPixel * 8;
}

int screenDepthPerComponent(Widget*)
{
    return 8;  // We assume A8R8G8B8 (or similar)
}

bool screenIsMonochrome(Widget* /*widget*/)
{
    // We will never be monochrome.
    // return screenDepth(widget) < 2;
    return false;
}

FloatRect screenRect(Widget* widget)
{
    ASSERT(widget->containingWindow());
    const EA::Raster::Rect& rect = widget->containingWindow()->GetClipRect();
    return FloatRect(rect.x, rect.y, rect.w, rect.h);
}

FloatRect screenAvailableRect(Widget* widget)
{
	//NotImplemented();
	//return FloatRect();
	
	// Update 10/14/2010 - Note by Arpit Baldeva: Commented out the code above. It should simply return our screenRect(Which is View surface for us) as the entire surface can be used
	// for display purpose. The Windows port of WebKit returns MONITORINFOEX.rcWork here and MONITORINFOEX.rcMonitor for screenRect.
	// The difference between two is explained at http://msdn.microsoft.com/en-us/library/dd145066(VS.85).aspx.  
	return screenRect(widget);
	
}

} // namespace WebCore
