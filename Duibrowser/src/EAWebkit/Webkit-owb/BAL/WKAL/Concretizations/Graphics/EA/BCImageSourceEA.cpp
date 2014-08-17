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
// BCImageSourceEA.cpp
// Modified by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "ImageSource.h"

#include "BMPImageDecoder.h"
#include "GIFImageDecoder.h"
#include "ICOImageDecoder.h"
#include "JPEGImageDecoder.h"
#include "PNGImageDecoder.h"
#include "XBMImageDecoder.h"
#include "SharedBuffer.h"
#include "Cache.h"
#include <EARaster/EARaster.h>
#include <EAWebKit/EAWebKit.h>

namespace WKAL {

ImageDecoder* createDecoder(const Vector<char>& data)
{
// We need at least 4 bytes to figure out what kind of image we're dealing with.
    int length = data.size();
    if (length < 4)
        return 0;

    const unsigned char* uContents = (const unsigned char*)data.data();
    const char* contents = data.data();

    // GIFs begin with GIF8(7 or 9).
    if (strncmp(contents, "GIF8", 4) == 0)
        return new GIFImageDecoder();

    // Test for PNG.
    if (uContents[0]==0x89 &&
        uContents[1]==0x50 &&
        uContents[2]==0x4E &&
        uContents[3]==0x47)
        return new PNGImageDecoder();

    // JPEG
    if (uContents[0]==0xFF &&
        uContents[1]==0xD8 &&
        uContents[2]==0xFF)
        return new JPEGImageDecoder();

    // BMP
    if (strncmp(contents, "BM", 2) == 0)
        return new BMPImageDecoder();

    // ICOs always begin with a 2-byte 0 followed by a 2-byte 1.
    // CURs begin with 2-byte 0 followed by 2-byte 2.
    if (!memcmp(contents, "\000\000\001\000", 4) ||
        !memcmp(contents, "\000\000\002\000", 4))
        return new ICOImageDecoder();

    // XBMs require 8 bytes of info.
    if (length >= 8 && strncmp(contents, "#define ", 8) == 0)
        return new XBMImageDecoder();

    // Give up. We don't know what the heck this is.
    return 0;
}

ImageSource::ImageSource()
    : m_decoder(0)
{
}

ImageSource::~ImageSource()
{
    clear();
}

void ImageSource::clear()
{
    delete m_decoder;
    m_decoder = 0;
}

bool ImageSource::initialized() const
{
    return m_decoder;
}

void ImageSource::setData(SharedBuffer* data, bool allDataReceived)
{
    // Make the decoder by sniffing the bytes.
    // This method will examine the data and instantiate an instance of the appropriate decoder plugin.
    // If insufficient bytes are available to determine the image type, no decoder plugin will be
    // made.
    if (!m_decoder)
        m_decoder = createDecoder(data->buffer());

    if (!m_decoder)
        return;

    m_decoder->setData(data, allDataReceived);
}

bool ImageSource::isSizeAvailable()
{
    if (!m_decoder)
        return false;

    return m_decoder->isSizeAvailable();
}

IntSize ImageSource::size() const
{
    if (!m_decoder)
        return IntSize();

    return m_decoder->size();
}

int ImageSource::repetitionCount()
{
    if (!m_decoder)
        return cAnimationNone;

    return m_decoder->repetitionCount();
}

size_t ImageSource::frameCount() const
{
    return m_decoder ? m_decoder->frameCount() : 0;
}

NativeImagePtr ImageSource::createFrameAtIndex(size_t index)
{
    if (!initialized())
        return 0;

    if (!m_decoder)
        return 0;

    // 11/09/09 CSidhall Added notify start of process to user
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeImageDecoder, EA::WebKit::kVProcessStatusStarted);
	
    RGBA32Buffer* const buffer = m_decoder->frameBufferAtIndex(index);
   
    
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeImageDecoder, EA::WebKit::kVProcessStatusEnded);

    
    if (!buffer || buffer->status() == RGBA32Buffer::FrameEmpty)
        return 0;

    // If we have a zero height image, just pretend we don't have enough data yet.
    if (!buffer->height())
        return 0;

    void* p = buffer->bytes().data();
    int   w = size().width();
    int   h = buffer->height();

    // This version will use (share) the decoder buffer directly instead of making another copy.
    EA::Raster::ISurface* pSurface  = 0;
    
    // 7/13/09 CSidhall - set copy to false so we use the decode buffer directly
    bool copyBuffer;
    if(frameCount() <= 1)
        //copyBuffer = true; // Activate this to get the old system with duplicate buffers 
        copyBuffer = false;
    else
        copyBuffer = true;
    pSurface =   EA::WebKit::GetEARasterInstance()->CreateSurface(p, w, h, w * 4, EA::Raster::kPixelFormatTypeARGB, copyBuffer, false, EA::Raster::kSurfaceCategoryImage);
    return pSurface;
}


bool ImageSource::frameIsCompleteAtIndex(size_t index)
{
    if (!m_decoder)
        return false;

    RGBA32Buffer* buffer = m_decoder->frameBufferAtIndex(index);

    return buffer && (buffer->status() == RGBA32Buffer::FrameComplete);
}


float ImageSource::frameDurationAtIndex(size_t index)
{
    if (!m_decoder)
        return 0;

    RGBA32Buffer* buffer = m_decoder->frameBufferAtIndex(index);
    RGBA32Buffer::FrameStatus status = buffer->status();

    if (!buffer || status == RGBA32Buffer::FrameEmpty)
        return 0;

    // Many annoying ads specify a 0 duration to make an image flash as quickly
    // as possible.  We follow WinIE's behavior and use a duration of 100 ms
    // for any frames that specify a duration of <= 50 ms.  See
    // <http://bugs.webkit.org/show_bug.cgi?id=14413> or Radar 4051389 for
    // more.
    const float duration = buffer->duration() / 1000.0f;
    return (duration < 0.051f) ? 0.100f : duration;
}


bool ImageSource::frameHasAlphaAtIndex(size_t index)
{
    // When a frame has not finished decoding, always mark it as having alpha,
    // so we don't get a black background for the undecoded sections.
    // TODO: A better solution is probably to have the underlying buffer's
    // hasAlpha() return true in these cases, since it is, in fact, technically
    // true.
    if (!frameIsCompleteAtIndex(index))
        return true;

    return m_decoder->frameBufferAtIndex(index)->hasAlpha();
}

// CSidhall 7/10/09 - Added to resize decoder buffer
void ImageSource::resizeDecoderBufferAtIndex(int index, int size)
{
    RGBA32Buffer* pBuffer = m_decoder->frameBufferAtIndex(index);
    if(pBuffer){
        RGBA32Array& bytes = pBuffer->bytes();
        bytes.resize(size);
    }
}

// CSidhall 7/14/09 - Added image lock for prune from decoder (false is not locked)
bool ImageSource::imagePruneLockStatus() const
{
    if(!m_decoder)
        return false;
    return m_decoder->imagePruneLockStatus();
}

}

