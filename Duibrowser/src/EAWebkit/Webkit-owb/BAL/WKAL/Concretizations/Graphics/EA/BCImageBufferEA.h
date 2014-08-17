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
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#ifndef ImageBuffer_h
#define ImageBuffer_h

#include "Image.h"
#include "IntSize.h"
#include <wtf/OwnPtr.h>
#include <wtf/PassRefPtr.h>
#include <memory>

#include "BALBase.h"

namespace WKAL {

    class GraphicsContext;
    class ImageData;
    class IntPoint;
    class IntRect;
    class RenderObject;
    class String;

    class ImageBuffer : Noncopyable, public WKALBase {
    public:
        static std::auto_ptr<ImageBuffer> create(const IntSize&, bool grayScale);
        ~ImageBuffer();

        IntSize size() const { return m_size; }
        GraphicsContext* context() const;

        BalSurface* surface() const;

        Image* image() const;

        void clearImage() { m_image.clear(); }

        PassRefPtr<ImageData> getImageData(const IntRect& rect) const;
        void putImageData(ImageData* source, const IntRect& sourceRect, const IntPoint& destPoint);

        String toDataURL(const String& mimeType) const;

    private:
        void* m_data;
        IntSize m_size;

        OwnPtr<GraphicsContext> m_context;
        mutable OwnPtr<Image> m_image;

        ImageBuffer(BalSurface* surface);
        mutable BalSurface* m_surface;
    };

} // namespace WebCore

#endif // ImageBuffer_h
