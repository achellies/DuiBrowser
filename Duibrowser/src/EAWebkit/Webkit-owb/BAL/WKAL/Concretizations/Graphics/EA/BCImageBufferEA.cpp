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
* This file was modified by Electronic Arts Inc Copyright © 2008-2010
*/

///////////////////////////////////////////////////////////////////////////////
// BCImageBufferEA.cpp
// Modified by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "ImageBuffer.h"

#include "BitmapImage.h"
#include "GraphicsContext.h"
#include "ImageData.h"
#include "NotImplemented.h"

#include <EARaster/EARaster.h>
#include <EAWebKit/EAWebKit.h>

#define WIDTH_MAX  8192
#define HEIGHT_MAX 8192

using namespace std;


namespace WKAL {


auto_ptr<ImageBuffer> ImageBuffer::create(const IntSize& size, bool)
{
    int width, height;

    if( size.width() >= WIDTH_MAX )
        width = WIDTH_MAX - 1;
    else
        width = size.width();

    if( size.height() >= HEIGHT_MAX )
        height = HEIGHT_MAX - 1;
    else
        height = size.height();

    EA::Raster::ISurface* pSurface =   EA::WebKit::GetEARasterInstance()->CreateSurface(width, height, EA::Raster::kPixelFormatTypeARGB, EA::Raster::kSurfaceCategoryImage);

    return auto_ptr<ImageBuffer>(new ImageBuffer(pSurface));
}


ImageBuffer::ImageBuffer(BalSurface* surface)
    : m_surface(surface)
{
    m_context.set(new GraphicsContext(m_surface));
}


ImageBuffer::~ImageBuffer()
{
    if (!m_image.get()) 
    {
          EA::WebKit::GetEARasterInstance()->DestroySurface(m_surface);
    }
}


GraphicsContext* ImageBuffer::context() const
{
    return m_context.get();
}


BalSurface* ImageBuffer::surface() const
{
    return m_surface;
}


Image* ImageBuffer::image() const
{
    if (!m_image) {
        // It's assumed that if image() is called, the actual rendering to the
        // GraphicsContext must be done.
        ASSERT(context());
        // BitmapImage will release the passed in surface on destruction
        m_image.set(new BitmapImage(m_surface));
    }
    return m_image.get();
}


PassRefPtr<ImageData> ImageBuffer::getImageData(const IntRect&) const
{
    notImplemented();
    return 0;
}


void ImageBuffer::putImageData(ImageData*, const IntRect&, const IntPoint&)
{
    OWB_PRINTF("ImageBuffer::putImageData\n");
    notImplemented();
}


String ImageBuffer::toDataURL(const String&) const
{
    OWB_PRINTF("ImageBuffer::toDataURL\n");
    notImplemented();
    return String();
}

} // namespace WebCore
