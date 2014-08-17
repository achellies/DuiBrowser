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
// BCImageEA.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "BitmapImage.h"

#include "AffineTransform.h"
#include "FloatRect.h"
#include "GraphicsContext.h"
#include "ImageObserver.h"
#include <math.h>
#include <EARaster/EARaster.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitConfig.h>
#include <EARaster/EARaster.h>
#include <EARaster/EARasterColor.h>
#include "BCImageCompressionEA.h"

// This function loads resources from WebKit.
Vector<char> loadResourceIntoArray(const char*);


namespace WKAL {


// Inline 16x16 "?" missing image icon binary. 
static const int kBrokenImageWidth = 16; 
static const int kBrokenImageHeight = 16; 
static const int brokenImageBin[] =
{
 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4
,0xff92afd4, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff92afd4
,0xff90aed3, 0xffffffff, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xffffffff, 0xff90aed3
,0xff8dacd2, 0xffffffff, 0xff2e8ac9, 0xff2e8ac9, 0xff2e8ac9, 0xff2c86c6, 0xff2c86c6, 0xff2c85c5, 0xff2c85c5, 0xff2c85c5, 0xff2c86c6, 0xff2e89c8, 0xff2e8ac9, 0xff2e8ac9, 0xffffffff, 0xff8dacd2
,0xff8aaad0, 0xffffffff, 0xff2d88c7, 0xff2d88c7, 0xff2b84c4, 0xff2a81c2, 0xff8ebde0, 0xff8ebde0, 0xffb9d6ec, 0xff9cc6e3, 0xff4692ca, 0xff2a81c2, 0xff2d88c7, 0xff2d88c7, 0xffffffff, 0xff8aaad0
,0xff87a7cf, 0xffffffff, 0xff2c86c5, 0xff2c86c5, 0xff2a83c2, 0xff8dbcde, 0xffe3eff7, 0xff2980c0, 0xff4691c8, 0xffffffff, 0xffe3eff7, 0xff297fc0, 0xff2c86c5, 0xff2c86c5, 0xffffffff, 0xff87a7cf
,0xff84a5ce, 0xffffffff, 0xff2b83c3, 0xff2b83c3, 0xff2a80c1, 0xff5498cc, 0xff8dbbdd, 0xff287dbf, 0xff3686c2, 0xffffffff, 0xfff1f7fb, 0xff287dbe, 0xff2b83c3, 0xff2b83c3, 0xffffffff, 0xff84a5ce
,0xff80a2cc, 0xffffffff, 0xff2a80c1, 0xff2a80c1, 0xff297fc0, 0xff297dbf, 0xff297dbf, 0xff287dbe, 0xff9bc2e1, 0xffc6ddee, 0xff5395ca, 0xff277abd, 0xff2a80c1, 0xff2a80c1, 0xffffffff, 0xff80a2cc
,0xff7da0cb, 0xffffffff, 0xff287dbf, 0xff287dbf, 0xff287dbf, 0xff287dbf, 0xff277cbe, 0xff5295ca, 0xff8cb8dc, 0xff2679bc, 0xff2679bc, 0xff277cbe, 0xff287dbf, 0xff287dbf, 0xffffffff, 0xff7da0cb
,0xff7a9eca, 0xffffffff, 0xff277abd, 0xff277abd, 0xff277abd, 0xff277abd, 0xff2576ba, 0xff347fbf, 0xff347fbf, 0xff2576ba, 0xff277abd, 0xff277abd, 0xff277abd, 0xff277abd, 0xffffffff, 0xff7a9eca
,0xff769bc8, 0xffffffff, 0xff2577ba, 0xff2577ba, 0xff2577ba, 0xff2577ba, 0xff2373b7, 0xffd3e4f1, 0xffd3e4f1, 0xff2373b7, 0xff2577ba, 0xff2577ba, 0xff2577ba, 0xff2577ba, 0xffffffff, 0xff769bc8
,0xff7399c7, 0xffffffff, 0xff2474b8, 0xff2474b8, 0xff2474b8, 0xff2474b8, 0xff2270b5, 0xffd3e3f1, 0xffd3e3f1, 0xff2270b5, 0xff2474b8, 0xff2474b8, 0xff2474b8, 0xff2474b8, 0xffffffff, 0xff7399c7
,0xff7096c6, 0xffffffff, 0xff2372b7, 0xff2372b7, 0xff2372b7, 0xff2372b7, 0xff216fb4, 0xff216fb4, 0xff216fb4, 0xff216fb4, 0xff2372b7, 0xff2372b7, 0xff2372b7, 0xff2372b7, 0xffffffff, 0xff7096c6
,0xff6d94c4, 0xffffffff, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xffffffff, 0xff6d94c4
,0xff6a92c3, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff6a92c3
,0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2
};

void FrameData::clear()
{
    if (m_frame)
    {
        EA::WebKit::GetEARasterInstance()->DestroySurface(m_frame);

        m_frame    = 0;
        m_duration = 0;
        m_hasAlpha = true;
    }
}


BitmapImage::BitmapImage(BalSurface* pSurface, ImageObserver* pObserver)
    : Image(pObserver)
    , m_currentFrame(0)
    , m_frames(0)
    , m_frameTimer(0)
    , m_repetitionCount(0)
    , m_repetitionsComplete(0)
    , m_isSolidColor(false)
    , m_animatingImageType(false)
    , m_animationFinished(true)
    , m_allDataReceived(true)
    , m_haveSize(true)
    , m_sizeAvailable(true)
    , m_decodedSize(0)
    , m_haveFrameCount(true)
    , m_frameCount(1)
{
    initPlatformData();

    const int width  = pSurface->GetWidth();
    const int height = pSurface->GetHeight();

    m_decodedSize = width * height * 4;
    m_size        = IntSize(width, height);

    m_frames.grow(1);
    m_frames[0].m_frame = pSurface;

    checkForSolidColor();
}


void BitmapImage::draw(GraphicsContext* context, const FloatRect& dst, const FloatRect& src, CompositeOperator op)
{
    // 11/09/09 CSidhall Added notify start of process to user
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImage, EA::WebKit::kVProcessStatusStarted);
	
    // CSidhall 1/14/09 Removed pImage as const pointer so it can be replace by a decompressed version when needed
    EA::Raster::ISurface* pImage = frameAtIndex(m_currentFrame);

    if (pImage) // If it's too early we won't have an image yet.
    {
        if (mayFillWithSolidColor())
            fillWithSolidColor(context, dst, solidColor(), op); // This will skip for a 0 alpha
        else
        {
            // Set the compositing operation.
            if (op == CompositeSourceOver && !frameHasAlphaAtIndex(m_currentFrame))
                context->setCompositeOperation(CompositeCopy);
            else
                context->setCompositeOperation(op);

            EA::Raster::ISurface* pDstSurface = context->platformContext();
            
             // Draw the image.
            EA::Raster::Rect srcRect, dstRect;

            srcRect.x = (int)src.x();
            srcRect.y = (int)src.y();

            if (0 == src.width())
                srcRect.w = pImage->GetWidth();
            else
                srcRect.w = (int)src.width();

            if (0 == src.height())
                srcRect.h = pImage->GetHeight();
            else
                srcRect.h = (int)src.height();

            dstRect.x = (int)(dst.x() + context->origin().width());
            dstRect.y = (int)(dst.y() + context->origin().height());
            dstRect.w = (int)dst.width();
            dstRect.h = (int)dst.height();

            float scaleX = dst.width()  / src.width();
            float scaleY = dst.height() / src.height();      
            const bool bScaled = (scaleX != 1.0) || (scaleY != 1.0);
            if (bScaled)
            {
                srcRect.x = (int)(src.x() * scaleX);  // Note by Paul Pedriana: Is this right? It seems wrong to me. Why is it modifing the source rect x/y?
                srcRect.y = (int)(src.y() * scaleY);
                srcRect.w = dstRect.w;
                srcRect.h = dstRect.h;
            }

            // To do: Create scratch surfaces that are used for these operations. 

            // 12/17/10 CSidhall - Added transform support and grouped for a single blit draw call.         
            EA::Raster::ISurface* pZoomedSurface = 0; 
            EA::Raster::ISurface* pAlphadSurface = 0;
            EA::Raster::ISurface *pTransformSurface = 0;   
            EA::Raster::ISurface* pDecompressedImage = 0;

            // CSidhall 1/14//09 Added image decompression.  
            // The actual compression is in BitmapImage::cacheFrame() after an image has been fully loaded
            // This here just unpacks the full image into an allocated surface (which needs to be removed after the draw).
            #if EAWEBKIT_USE_RLE_COMPRESSION || EAWEBKIT_USE_YCOCGDXT5_COMPRESSION            
            pDecompressedImage = BCImageCompressionEA::UnpackCompressedImage(pImage);         
            if(pDecompressedImage != NULL)
                pImage = pDecompressedImage;
            #endif 

            EA::Raster::IEARaster* pRaster =EA::WebKit::GetEARasterInstance();

            // Alpha surface
            if (context->transparencyLayer() != 1.0f)
            {
                // Note by Paul Pedriana: Why is it applying transparency via a new image when instead it could 
                // just do the blit with surface transparency added in? Is that not possible with SDL?
                pAlphadSurface = pRaster->CreateTransparentSurface(pImage, static_cast<int>(context->transparencyLayer() * 255));
                
                // Reduce memory right away when possible
                if(pDecompressedImage) 
                {
                    pRaster->DestroySurface(pDecompressedImage);    
                    pDecompressedImage = 0;
                }
                pImage = pAlphadSurface;
            }

            // Zoom surface 
            if (bScaled)
            {
                // Note: Consider combining this instead with the transform matrix when there is an active tranform.
                pZoomedSurface = pRaster->ZoomSurface(pImage, scaleX, scaleY, 0);
                
                // Reduce memory right away when possible
                if(pDecompressedImage) 
                {
                    pRaster->DestroySurface(pDecompressedImage);   
                    pDecompressedImage = 0;
                }
                if(pAlphadSurface)
                {
                     pRaster->DestroySurface(pAlphadSurface);
                     pAlphadSurface =0; 
                }
                pImage = pZoomedSurface;    
            }


            // Transform surface
            if(context->hasTransform())
            {
                pTransformSurface = context->transform(pImage, srcRect, dstRect);
                
                // No point in cleaning up mem here now for what was allocated is already done and we want to reduce frags.
                pImage = pTransformSurface;
            }

            // Draw to the buffer
            if(pImage)
                 pRaster->Blit(pImage, &srcRect, pDstSurface, &dstRect, NULL);


            // Surface clean up (in reverse order)
            if(pTransformSurface)
                 pRaster->DestroySurface(pTransformSurface);
            if(pZoomedSurface)
                 pRaster->DestroySurface(pZoomedSurface);
            if(pAlphadSurface)
                 pRaster->DestroySurface(pAlphadSurface);
            if(pDecompressedImage)
                pRaster->DestroySurface(pDecompressedImage);

            startAnimation();

            if (imageObserver())
                imageObserver()->didDraw(this);
        }
    }

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImage, EA::WebKit::kVProcessStatusEnded);
}


void Image::drawPattern(GraphicsContext* context, const FloatRect& tileRect, const AffineTransform& patternTransform,
                        const FloatPoint& phase, CompositeOperator op, const FloatRect& destRect)
{
    if (destRect.isEmpty())
        return;

    // Note: For drawPattern, 1x1 texel textures detected with mayFillWithSolidColor() are already handled by Image::drawTiled().

    NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusStarted);
	
    // CSidhall - Removed pImage as const pointer so it can be changed for decompression
    EA::Raster::ISurface* pImage = nativeImageForCurrentFrame();

    if (!pImage) // If it's too early we won't have an image yet.
	{
		//abaldeva -  Long term, we may want to revise the process status to be something inspired from RAII pattern. For now, this will work.
		NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusEnded);
		return;
	}

    EA::Raster::ISurface* const cr = context->platformContext();
    context->save();
    
    context->setCompositeOperation(op);

  
    // 2/9/09 CSidhall Height and width adjustments     
    // This is to fix a background clip bug when scrolling
    FloatRect adjDestRect = destRect;
    IntSize origin = context->origin();       
    float dy =  destRect.y() + origin.height();
    if(dy < 0.0f)
    {
        float heightAdj = adjDestRect.height() + dy;
        if(heightAdj <= 0.0f)
        {
            // Added to fix negative or 0 height clip bug 
            context->restore();
			//abaldeva -  Long term, we may want to revise the process status to be something inspired from RAII pattern. For now, this will work.
			NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusEnded);
			return;
        }
        adjDestRect.setHeight(heightAdj);
    }
    float dx =  destRect.x() + origin.width();
    if(dx < 0.0f)
    {
        float widthAdj = adjDestRect.width() + dx;
        if(widthAdj <= 0.0f)
        {
            context->restore();
			//abaldeva -  Long term, we may want to revise the process status to be something inspired from RAII pattern. For now, this will work.
			NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusEnded);
			return;
        }
        adjDestRect.setWidth(widthAdj);
    }

    // 2/11/09 CS Note: We want to grab the current clip rect before context->clip()
    EA::Raster::Rect clipRect = cr->GetClipRect();

    context->clip(IntRect(adjDestRect)); // don't draw outside this

    IntRect dest(IntPoint(), IntSize(pImage->GetWidth(), pImage->GetHeight()));
    IntRect src(static_cast<int>(phase.x()), static_cast<int>(phase.y()), static_cast<int>(tileRect.size().width()), static_cast<int>(tileRect.size().height()));

    int xMax = static_cast<int>(destRect.x() + destRect.width());
    int yMax = static_cast<int>(destRect.y() + destRect.height());

    EA::Raster::Rect srcRect, dstRect;

    srcRect.x = 0;
    srcRect.y = 0;

    if (0 == src.width())
        srcRect.w = pImage->GetWidth();
    else
        srcRect.w = src.width();

    if (0 == src.height())
        srcRect.h = pImage->GetHeight();
    else
        srcRect.h = src.height();

    dstRect.x = dest.x();
    dstRect.y = dest.y();
    dstRect.w = dest.width();
    dstRect.h = dest.height();

    // compute ratio of the zoomed part:
    const double ratioW = (double)dest.width()  / (double)srcRect.w;
    const double ratioH = (double)dest.height() / (double)srcRect.h;

   // CSidhall 1/14//09 Added image decompression.
    #if EAWEBKIT_USE_RLE_COMPRESSION || EAWEBKIT_USE_YCOCGDXT5_COMPRESSION            
    
    EA::Raster::ISurface* pDecompressedImage = BCImageCompressionEA::UnpackCompressedImage(pImage);         
    
    // Note: we are changing the image pointer here to the decompressed image!      
    if(pDecompressedImage != NULL)
        pImage = pDecompressedImage;
    
    #endif 

    EA::Raster::ISurface* pSurface = NULL;
    EA::Raster::IEARaster* pRaster =EA::WebKit::GetEARasterInstance();

    if ((ratioW != 1.0) || (ratioH != 1.0))
    {
        pSurface = pRaster->ZoomSurface(pImage, ratioW, ratioH, 0);

        // Adjust offset to the new referentiel (zoomed)
        srcRect.x = (int)(src.x() * ratioW);
        srcRect.y = (int)(src.y() * ratioH);
    }

    // EA/Alex Mole, 12/22/09: Just create the transparent surface once, rather than once per tile
    // Create a transparency layer if we need one
    EA::Raster::ISurface* pAlphadSurface = NULL;
    if( context->transparencyLayer() != 1.0 )
    {
        if( pSurface )
        {
            pAlphadSurface = pRaster->CreateTransparentSurface(pSurface, static_cast<int>(context->transparencyLayer() * 255));
        }
        else
        {
            pAlphadSurface = pRaster->CreateTransparentSurface(pImage, static_cast<int>(context->transparencyLayer() * 255));
        }
    }

    // Now decide on which surface we want to blit
    EA::Raster::ISurface* pSurfaceToBlit = pAlphadSurface;
    if( !pSurfaceToBlit )
    {
        pSurfaceToBlit = pSurface;
    }
    if( !pSurfaceToBlit )
    {
        pSurfaceToBlit = pImage;
    }

    for (int x = static_cast<int>(phase.x()); x <= xMax; x += pImage->GetWidth())
    {
        for (int y = static_cast<int>(phase.y()); y <= yMax; y += pImage->GetHeight())
        {
            dest.setLocation(IntPoint(x, y) + context->origin());
            
            dstRect.x = dest.x();
            dstRect.y = dest.y();
            dstRect.w = dest.width();
            dstRect.h = dest.height();

            pRaster->Blit(pSurfaceToBlit, &srcRect, cr, &dstRect, &clipRect);
        }
    }

    if( pAlphadSurface )
    {
        pRaster->DestroySurface( pAlphadSurface );
    }

    if(pSurface)
        pRaster->DestroySurface(pSurface);

       #if EAWEBKIT_USE_RLE_COMPRESSION || EAWEBKIT_USE_YCOCGDXT5_COMPRESSION             
        
        // Remove the full ARBG buffer of decompressed data        
        if(pDecompressedImage != NULL)
            pRaster->DestroySurface(pDecompressedImage);
        
        #endif

    context->restore();

    if (imageObserver())
        imageObserver()->didDraw(this);

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusEnded);
}


void BitmapImage::initPlatformData()
{
}


void BitmapImage::invalidatePlatformData()
{
}

Image* Image::loadPlatformResource(const char* name)
{
    BitmapImage* img=NULL; 
    const char* kMissingName="missingImage";  

   // 11/30/10 CSidhall - Added support for missing image by providing an inline error icon texture (brokenImageBin).
   if (strcmp(name,kMissingName) == 0)
   {
       // We create a surface that uses the inline image data directly
       EA::Raster::ISurface* pSurface =   EA::WebKit::GetEARasterInstance()->CreateSurface((void*) &brokenImageBin[0],
                                                                                            kBrokenImageWidth,kBrokenImageHeight, 
                                                                                            kBrokenImageWidth * 4,            
                                                                                            EA::Raster::kPixelFormatTypeARGB, false, false, EA::Raster::kSurfaceCategoryImage);  
       // This will also set up the first frame and the image size
       img = new BitmapImage(pSurface); 
   }
   else 
   {
        // CSidhall - Don't think this option actually would work with our images as the size and frame set up are private.
        // Also the buffer passed/shared here is a Vector.
        Vector<char> arr = loadResourceIntoArray(name);  // Note by Paul Pedriana: The current version of loadResourceIntoArray is a no-op.
        img = new BitmapImage;
        RefPtr<SharedBuffer> buffer = SharedBuffer::create(arr.data(), arr.size());
        img->setData(buffer, true);
   }
   return img;

}

// Checks to see if the image is of a solid color. We optimize these images and just do a fill rect instead.
void BitmapImage::checkForSolidColor()
{
    if (frameCount() == 1) // We could check multiple frames, but generally all that matters is the special case of a 1x1 single frame.
    {
        // CS - Removed pSurface pointer as const so can change it for decompressed versions
        EA::Raster::ISurface* pSurface = frameAtIndex(0);

       // CSidhall 1/15//09 Added image decompression support here in case image is compressed already
        #if EAWEBKIT_USE_RLE_COMPRESSION || EAWEBKIT_USE_YCOCGDXT5_COMPRESSION            
        
        EA::Raster::ISurface* pDecompressedImage = BCImageCompressionEA::UnpackCompressedImage(pSurface);         
        
        // Note: we are changing the image pointer here to the decompressed image instead      
        if(pDecompressedImage != NULL)
            pSurface = pDecompressedImage;
        
        #endif 

        EA::Raster::IEARaster* pRaster = EA::WebKit::GetEARasterInstance();

        if(pSurface && (pSurface->GetWidth() == 1) && (pSurface->GetHeight() == 1))
        {
            int      bpp = pSurface->GetPixelFormat().mBytesPerPixel;
            uint8_t* p   = (uint8_t*)pSurface->GetData(); //  + (0 * pSurface->pitch) + (0 * bpp);
            uint32_t color;

            switch (bpp)
            {
                case 3:
                    // Note by Paul Pedriana: I'm not sure this is correct.
                    color = (p[0] << 16) | (p[1] <<  8) | (p[0] << 0);  // ARGB
                    break;

                default:
                case 4:
                    color = *(uint32_t*)p; // This assumes p is 32 bit aligned.
                    break;
            }

            int r, g, b, a;
            pRaster->ConvertColor(color, pSurface->GetPixelFormat(), r, g, b, a);
            m_solidColor.setRGB( pRaster->makeRGBA(r, g, b, a) );
            if((a >= 255) || (a == 0))
                m_isSolidColor = true;  // This will fill instead of texture draw to safe some time. In case of 0 alpha, will exit the fill.
        }
            
        // CSidhall 1/14/09 Added image decompression
        #if EAWEBKIT_USE_RLE_COMPRESSION || EAWEBKIT_USE_YCOCGDXT5_COMPRESSION             
        
        // Remove the full ARBG buffer of decompressed data        
        if(pDecompressedImage != NULL)
              pRaster->DestroySurface(pDecompressedImage);
        
        #endif
    }
}

// CSidhall 7/14/09 - Added image lock for prune from decoder (false is not locked)
 bool BitmapImage::imagePruneLockStatus() const
 {
     return m_source.imagePruneLockStatus();
 }


} // namespace

