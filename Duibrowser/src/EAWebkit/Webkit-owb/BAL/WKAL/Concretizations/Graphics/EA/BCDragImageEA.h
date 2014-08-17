/*
 * Copyright (C) 2008 Pleyo.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Pleyo nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY PLEYO AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PLEYO OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2008-2009
*/

///////////////////////////////////////////////////////////////////////////////
// Modified by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef DragImage_h
#define DragImage_h

#include "IntSize.h"
#include "FloatSize.h"
#include "BALBase.h"

//We need to #define YOffset as it needs to be shared with WebKit
#define DragLabelBorderYOffset 2

namespace WKAL {
    
    class CachedImage;
    class Frame;
    class Image;
    class KURL;
    class Range;
    class String;
    
    IntSize dragImageSize(DragImageRef);
    
    //These functions should be memory neutral, eg. if they return a newly allocated image, 
    //they should release the input image.  As a corollary these methods don't guarantee
    //the input image ref will still be valid after they have been called
    DragImageRef fitDragImageToMaxSize(DragImageRef image, const IntSize& srcSize, const IntSize& size);
    DragImageRef scaleDragImage(DragImageRef, FloatSize scale);
    DragImageRef dissolveDragImageToFraction(DragImageRef image, float delta);
    
    DragImageRef createDragImageFromImage(Image*);
    DragImageRef createDragImageForSelection(Frame*);    
    DragImageRef createDragImageIconForCachedImage(CachedImage*);
    void deleteDragImage(DragImageRef);
}


#endif //!DragImage_h
