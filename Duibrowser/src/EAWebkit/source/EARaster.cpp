/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EARaster.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////



#include <EARaster/EARaster.h>
#include <EARaster/EARasterColor.h>
#include "Color.h"
#include "IntRect.h"
#include <EAWebkit/internal/EAWebKitAssert.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>


namespace EA {

namespace Raster {


///////////////////////////////////////////////////////////////////////
// Surface
///////////////////////////////////////////////////////////////////////


Surface::Surface()
{
    InitMembers();
    SetPixelFormat(kPixelFormatTypeARGB);
}


Surface::Surface(int width, int height, PixelFormatType pft)
{
    InitMembers();
    SetPixelFormat(pft);
    Resize(width, height, false);
    // The user can check mpData to see if the resize succeeded.
}


Surface::~Surface()
{
    FreeData();
}


int Surface::AddRef()
{
    // This is not thread-safe.
    return ++mRefCount;
}


int Surface::Release()
{
    // This is not thread-safe.
    if(mRefCount > 1)
        return --mRefCount;

    DestroySurface(this);
    return 0;
}


void Surface::InitMembers()
{
    // mPixelFormat  // Should probably init this.
    mSurfaceFlags  = 0;
    mpData         = NULL;
    mWidth         = 0;
    mHeight        = 0;
    mStride        = 0;
    mLockCount     = 0;
    mRefCount      = 0;
    mpUserData     = NULL;
    mCompressedSize = 0;

    // Draw info.
    mClipRect.x    = 0;
    mClipRect.y    = 0;
    mClipRect.w    = INT_MAX;
    mClipRect.h    = INT_MAX;
    mpBlitDest     = 0;
    mDrawFlags     = 0;
    mpBlitFunction = NULL;
    mCategory      = kSurfaceCategoryDefault; 
}


void Surface::SetPixelFormat(PixelFormatType pft)
{
    mPixelFormat.mPixelFormatType = pft;
    mPixelFormat.mSurfaceAlpha    = 255;

    if (pft == kPixelFormatTypeRGB)
        mPixelFormat.mBytesPerPixel = 3;
    else
        mPixelFormat.mBytesPerPixel = 4;

    switch (pft)
    {
        case kPixelFormatTypeARGB:
            mPixelFormat.mRMask  = 0x00ff0000;
            mPixelFormat.mGMask  = 0x0000ff00;
            mPixelFormat.mBMask  = 0x000000ff;
            mPixelFormat.mAMask  = 0xff000000;
            mPixelFormat.mRShift = 16;
            mPixelFormat.mGShift = 8;
            mPixelFormat.mBShift = 0;
            mPixelFormat.mAShift = 24;
            break;

        case kPixelFormatTypeRGBA:
            mPixelFormat.mRMask  = 0xff000000;
            mPixelFormat.mGMask  = 0x00ff0000;
            mPixelFormat.mBMask  = 0x0000ff00;
            mPixelFormat.mAMask  = 0x000000ff;
            mPixelFormat.mRShift = 24;
            mPixelFormat.mGShift = 16;
            mPixelFormat.mBShift = 8;
            mPixelFormat.mAShift = 0;
            break;

        case kPixelFornatTypeXRGB:
            mPixelFormat.mRMask  = 0x00ff0000;
            mPixelFormat.mGMask  = 0x0000ff00;
            mPixelFormat.mBMask  = 0x000000ff;
            mPixelFormat.mAMask  = 0x00000000;
            mPixelFormat.mRShift = 16;
            mPixelFormat.mGShift = 8;
            mPixelFormat.mBShift = 0;
            mPixelFormat.mAShift = 24;
            break;

        case kPixelFormatTypeRGBX:
            mPixelFormat.mRMask  = 0xff000000;
            mPixelFormat.mGMask  = 0x00ff0000;
            mPixelFormat.mBMask  = 0x0000ff00;
            mPixelFormat.mAMask  = 0x00000000;
            mPixelFormat.mRShift = 24;
            mPixelFormat.mGShift = 16;
            mPixelFormat.mBShift = 8;
            mPixelFormat.mAShift = 0;
            break;

        case kPixelFormatTypeRGB:
            mPixelFormat.mRMask  = 0xff000000;  // I think this is wrong, or at least endian-dependent.
            mPixelFormat.mGMask  = 0x00ff0000;
            mPixelFormat.mBMask  = 0x0000ff00;
            mPixelFormat.mAMask  = 0x00000000;
            mPixelFormat.mRShift = 24;
            mPixelFormat.mGShift = 16;
            mPixelFormat.mBShift = 8;
            mPixelFormat.mAShift = 0;
            break;
    }
}

bool Surface::Set(void* pData, int width, int height, int stride, PixelFormatType pft, bool bCopyData, bool bTakeOwnership, SurfaceCategory category)
{
    FreeData();

    mpBlitDest     = 0;
    mDrawFlags     = 0;
    mpBlitFunction = NULL;

    if(pData)
    {
        SetPixelFormat(pft);
        SetCategory(category);

        if(bCopyData)  // If we are copying the data instead of taking over ownership...
        {
            if(Resize(width, height, false))
            {
                Lock();
                if(mStride == stride)
                {
                    // We could blit pSource to ourself, but we happen to know we are of identical
                    // format and we simply want to replication source onto ourselves. So memcpy it.              
                    memcpy(mpData, pData, height * stride);
                }
                else
                {
                    // Case where the source and dest buffers have different strides so line copy instead                   
                    char* pDestRow = (char*) mpData;     
                    char* pSourceRow = (char*) pData;     
                    int lineSize = width * GetPixelFormat().mBytesPerPixel;

                    for(int h=0; h < height; h++)
                    {
                         memcpy(pDestRow, pSourceRow, lineSize);          
                         pDestRow +=mStride;   
                         pSourceRow +=stride;    
                    }
                }
                Unlock();
            }
            else
                return false;
        }
        else
        {
            mpData  = pData;
            mWidth  = width;
            mHeight = height;
            mStride = stride;

            if(!bTakeOwnership)
                mSurfaceFlags |= kFlagOtherOwner;
        }
    }

    return true;
}


bool Surface::Set(ISurface* pSource)
{
    if(Resize(pSource->GetWidth(), pSource->GetHeight(), false))
    {
        int stride = pSource->GetStride();
        Lock();
        if(mStride == stride)
        {
            // We could blit pSource to ourself, but we happen to know we are of identical
            // format and we simply want to replication source onto ourselves. So memcpy it.        
            memcpy(mpData, pSource->GetData(), pSource->GetHeight() * stride);
        }
        else
        {
            // Case where the source and dest buffers have different strides so line copy instead                   
            char* pDestRow = (char*) mpData;     
            char* pSourceRow = (char*) pSource->GetData();     
            int lineSize = pSource->GetWidth() * GetPixelFormat().mBytesPerPixel;
            int height = pSource->GetHeight();
            for(int h=0; h < height; h++)
            {
                 memcpy(pDestRow, pSourceRow, lineSize);          
                 pDestRow +=mStride;   
                 pSourceRow +=stride;    
            }
        }
        Unlock();
        return true;
    }

    return false;
}


bool Surface::Resize(int width, int height, bool bCopyData)
{
    const size_t kNewMemorySize = width * height * mPixelFormat.mBytesPerPixel;

    if(bCopyData && mpData)
    {
		void* pData = EAWEBKIT_NEW("Surface Resize") char[kNewMemorySize];//WTF::fastNewArray<char>(kNewMemorySize);

        #ifdef EA_DEBUG
            Lock();
            memset(mpData, 0, kNewMemorySize);
            Unlock();
        #endif

        // To do: Copy from mpData to pData.
          EAW_FAIL_MSG("Surface::Resize data copy is not completed.");
        FreeData();
        mpData = pData;
    }
    else
    {
        // We have a separate pathway for this case because it uses less total memory than above.
        FreeData();

        mpData = EAWEBKIT_NEW("Surface Resize") char[kNewMemorySize];//WTF::fastNewArray<char> (kNewMemorySize);

        #ifdef EA_DEBUG
        if(mpData) 
        {
            Lock();         
            memset(mpData, 0, kNewMemorySize);
            Unlock();
        }
        #endif
    }

    if(mpData)
    {
        mWidth  = width;
        mHeight = height;
        mStride = width * mPixelFormat.mBytesPerPixel;

        // Reset the clip rec on resize
        Rect r (0,0,width,height);
        SetClipRect(r);
    
    }

    return (mpData != NULL);
}


bool Surface::FreeData()
{
    bool returnFlag = false;

    // Remove the compressed data buffer which is stored in the user data
    if( ((mSurfaceFlags & (EA::Raster::kFlagCompressedRLE | EA::Raster::kFlagCompressedYCOCGDXT5 )) != 0 ) &&
        (mpUserData) && (mCompressedSize) )
    {
         EAWEBKIT_DELETE[] ((char*)mpUserData);
         mpUserData = NULL;
         mCompressedSize = 0;
         mSurfaceFlags &= ~(EA::Raster::kFlagCompressedRLE | EA::Raster::kFlagCompressedYCOCGDXT5);
         returnFlag = true;  // Success
    }

    if((mSurfaceFlags & kFlagOtherOwner) == 0)  // If we own the pointer...
    {
        EAWEBKIT_DELETE[] ((char*)mpData);//WTF::fastDeleteArray<char> ((char*)mpData);
        mpData = NULL;
 
        // We only want to clear this stuff if there is no other owner for the other owner might still need this info.
        mLockCount    = 0;
        mSurfaceFlags = 0; 
        returnFlag = true;  // Success
    }

    return returnFlag;
}

// Can be used for texture lock notification
void Surface::Lock()
{
    mLockCount++;
}

void Surface::Unlock()
{
    EAW_ASSERT(mLockCount > 0);
    mLockCount--;
}

void Surface::SetClipRect(const Rect* pRect)
{
	Rect fullRect(0, 0, mWidth, mHeight);

	if(pRect)
	    IntersectRect(*pRect, fullRect, mClipRect);
    else
    	mClipRect = fullRect;
}


ISurface* CreateSurface()
{
    
	ISurface* pSurface = EAWEBKIT_NEW("Surface") Surface();//WTF::fastNew<Surface>();
   
    if(pSurface)
        pSurface->AddRef();

    return pSurface;
}


ISurface* CreateSurface(int width, int height, PixelFormatType pft,SurfaceCategory category)
{
    ISurface* pNewSurface = CreateSurface();

    if(pNewSurface)
    {
        // Note that pNewSurface is already AddRef'd.
        pNewSurface->SetPixelFormat(pft);
        pNewSurface->SetCategory(category);
        if(!pNewSurface->Resize(width, height, false))
        {
            DestroySurface(pNewSurface);
            pNewSurface = NULL;
        }
    }

    return pNewSurface;
}


ISurface* CreateSurface(ISurface* pSurface)
{
    ISurface* pNewSurface = CreateSurface();

    if(pNewSurface)
    {
        // Note that pNewSurface is already AddRef'd.
        if(!pNewSurface->Set(pSurface))
        {
            DestroySurface(pNewSurface);
            pNewSurface = NULL;
        }
    }

    return pNewSurface;
}


ISurface* CreateSurface(void* pData, int width, int height, int stride, PixelFormatType pft, bool bCopyData, bool bTakeOwnership,SurfaceCategory category)
{
    ISurface* pNewSurface = CreateSurface();

    if(pNewSurface)
    {
        // Note that pNewSurface is already AddRef'd.
        if(!pNewSurface->Set(pData, width, height, stride, pft, bCopyData, bTakeOwnership, category))
        {
            DestroySurface(pNewSurface);
            pNewSurface = NULL;
        }
    }

    return pNewSurface;
}


void DestroySurface(ISurface* pSurface)
{
    EAWEBKIT_DELETE pSurface;//WTF::fastDelete<Surface>(pSurface);

}






///////////////////////////////////////////////////////////////////////
// Utility functions
///////////////////////////////////////////////////////////////////////

EARASTER_API bool IntersectRect(const Rect& a, const Rect& b, Rect& result)
{
    // Horizontal
    int aMin = a.x;
    int aMax = aMin + a.w;
    int bMin = b.x;
    int bMax = bMin + b.w;

    if(bMin > aMin)
        aMin = bMin;
    result.x = aMin;
    if(bMax < aMax)
        aMax = bMax;
    result.w = (((aMax - aMin) > 0) ? (aMax - aMin) : 0);

    // Vertical
    aMin = a.y;
    aMax = aMin + a.h;
    bMin = b.y;
    bMax = bMin + b.h;

    if(bMin > aMin)
        aMin = bMin;
    result.y = aMin;
    if(bMax < aMax)
        aMax = bMax;
    result.h = (((aMax - aMin) > 0) ? (aMax - aMin) : 0);

    return (result.w && result.h);
}


// To do: Convert this to a .tga writer instead of .ppm, as .tga is more commonly
//        supported yet is still a fairly simple format.
EARASTER_API bool WritePPMFile(const char* pPath, ISurface* pSurface, bool bAlphaOnly)
{
    FILE* const fp = fopen(pPath, "w");

    if(fp)
    {
        const bool bARGB = (pSurface->GetPixelFormat().mPixelFormatType == EA::Raster::kPixelFormatTypeARGB);

        fprintf(fp, "P3\n");
        fprintf(fp, "# %s\n", pPath);
        fprintf(fp, "%d %d\n", (int)pSurface->GetWidth(), (int)pSurface->GetHeight());
        fprintf(fp, "%d\n", 255);

        for(int y = 0; y < pSurface->GetHeight(); y++)
        {
            for(int x = 0; x < pSurface->GetWidth(); x++)
            {
                EA::Raster::Color color; EA::Raster::GetPixel(pSurface, x, y, color);
                const uint32_t    c = color.rgb();
                unsigned          a, r, g, b;

                if(bAlphaOnly)
                {
                    if(bARGB)
                        a = (unsigned)((c >> 24) & 0xff);  // ARGB
                    else
                        a = (unsigned)(c & 0xff);          // RGBA

                    fprintf(fp, "%03u %03u %03u \t", a, a, a);
                }
                else
                {
                    if(bARGB)
                    {
                        r = (unsigned)((c >> 16) & 0xff); // ARGB
                        g = (unsigned)((c >>  8) & 0xff);
                        b = (unsigned)((c >>  0) & 0xff);
                    }
                    else
                    {
                        r = (unsigned)((c >> 24) & 0xff); // RGBA
                        g = (unsigned)((c >> 16) & 0xff);
                        b = (unsigned)((c >>  8) & 0xff);
                    }

                    fprintf(fp, "%03u %03u %03u \t", r, g, b);
                }
            }

            fprintf(fp, "\n");
        }

        fprintf(fp, "\n");
        fclose(fp);

        return true;
    }

    return false;
}


void IntRectToEARect(const WKAL::IntRect& in, EA::Raster::Rect& out)
{
    out = EA::Raster::Rect( in.x(), in.y(), in.width(), in.height() );
}

void EARectToIntRect(const EA::Raster::Rect& in, WKAL::IntRect& out)
{
    out = WebCore::IntRect( in.x, in.y, in.w, in.h );
}

} // namespace Raster

} // namespace EA

namespace EA
{
	namespace Raster
	{
		void EARasterConcrete::IntRectToEARect(const WKAL::IntRect& in, EA::Raster::Rect& out)
		 {
			 EA::Raster::IntRectToEARect(in, out);
		 }
		 void EARasterConcrete::EARectToIntRect(const EA::Raster::Rect& in, WKAL::IntRect& out)
		 {
			EA::Raster::EARectToIntRect(in, out);
		 }
		// Surface management
		 ISurface* EARasterConcrete::CreateSurface()
		 {
			 return EA::Raster::CreateSurface();
		 }
		 ISurface* EARasterConcrete::CreateSurface(int width, int height, PixelFormatType pft,SurfaceCategory category)
		 {
			 return EA::Raster::CreateSurface(width, height, pft, category);
		 }
		 ISurface*    EARasterConcrete::CreateSurface(ISurface* pSurface)
		 {
			 return EA::Raster::CreateSurface(pSurface);
		 }
		 ISurface* EARasterConcrete::CreateSurface(void* pData, int width, int height, int stride, PixelFormatType pft, bool bCopyData, bool bTakeOwnership, SurfaceCategory category)
		 {
			 return EA::Raster::CreateSurface(pData, width, height, stride, pft, bCopyData, bTakeOwnership, category);
		 }
		 void EARasterConcrete::DestroySurface(ISurface* pSurface)
		 {
			 EA::Raster::DestroySurface(pSurface);
		 }

		// Color conversion
		 void EARasterConcrete::ConvertColor(NativeColor c, PixelFormatType cFormat, Color& result)
		 {
			 EA::Raster::ConvertColor(c, cFormat, result);
		 }
		 void EARasterConcrete::ConvertColor(int r, int g, int b, int a, Color& result)
		 {
			 EA::Raster::ConvertColor(r, g, b, a, result);
		 }
		 NativeColor EARasterConcrete::ConvertColor(const Color& c, PixelFormatType resultFormat)
		 {
			 return EA::Raster::ConvertColor(c, resultFormat);
		 }
		 void EARasterConcrete::ConvertColor(const Color& c, int& r, int& g, int& b, int& a)
		 {
			 EA::Raster::ConvertColor(c, r, g, b, a);
		 }
		 void EARasterConcrete::ConvertColor(NativeColor c, const PixelFormat& pf, int& r, int& g, int& b, int& a)
		 {
			 EA::Raster::ConvertColor(c, pf, r, g, b, a);
		 }
		 void EARasterConcrete::ConvertColor(NativeColor c, const PixelFormat& pf, int& r, int& g, int& b)
		 {
			 EA::Raster::ConvertColor(c, pf, r, g, b);
		 }

		// Pixel functions
		 void  EARasterConcrete::GetPixel(ISurface* pSurface, int x, int y, Color& color)
		 {
			 EA::Raster::GetPixel(pSurface, x, y, color);
		 }
		 int EARasterConcrete::SetPixelSolidColor(ISurface* pSurface, int x, int y, const Color& color)
		 {
			 return EA::Raster::SetPixelSolidColor(pSurface, x, y, color);
		 }
		 int EARasterConcrete::SetPixelSolidColorNoClip(ISurface* pSurface, int x, int y, const Color& color)
		 {
			 return EA::Raster::SetPixelSolidColorNoClip(pSurface, x, y, color);
		 }
		 int EARasterConcrete::SetPixelColor(ISurface* pSurface, int x, int y, const Color& color)
		 {
			 return EA::Raster::SetPixelColor(pSurface, x, y, color);
		 }
		 int EARasterConcrete::SetPixelColorNoClip(ISurface* pSurface, int x, int y, const Color& color)
		 {
			 return EA::Raster::SetPixelColorNoClip(pSurface, x, y, color);
		 }
		 int EARasterConcrete::SetPixelRGBA(ISurface* pSurface, int x, int y, int r, int g, int b, int a)
		 {
			 return EA::Raster::SetPixelRGBA(pSurface, x, y, r, g, b, a);
		 }
		 int EARasterConcrete::SetPixelRGBANoClip(ISurface* pSurface, int x, int y, int r, int g, int b, int a)
		 {
			 return EA::Raster::SetPixelRGBANoClip(pSurface, x, y, r, g, b, a);
		 }

		// Rectangle functions
		 int EARasterConcrete::FillRectSolidColor(ISurface* pSurface, const Rect* pRect, const Color& color)
		 {
			 return EA::Raster::FillRectSolidColor(pSurface, pRect, color);
		 }
		 int EARasterConcrete::FillRectColor(ISurface* pSurface, const Rect* pRect, const Color& color)
		 {
			return EA::Raster::FillRectColor(pSurface, pRect, color);
		 }
		 int EARasterConcrete::RectangleColor(ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color)
		 {
			 return EA::Raster::RectangleColor(pSurface, x1, y1, x2, y2, color);
		 }
		 int EARasterConcrete::RectangleColor(ISurface* pSurface, const EA::Raster::Rect& rect, const Color& c)
		 {
			 return EA::Raster::RectangleColor(pSurface, rect, c);
		 }
		 int EARasterConcrete::RectangleRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a)
		 {
			 return EA::Raster::RectangleRGBA(pSurface, x1, y1, x2, y2, r, g, b, a);
		 }

		// int   BoxColor                (Surface* pSurface, int x1, int y1, int x2, int y2, const Color& color){}
		// int   BoxRGBA                 (Surface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a){}

		// Line functions
		 int EARasterConcrete::HLineSolidColor(ISurface* pSurface, int x1, int x2, int  y, const Color& color)
		 {
			 return EA::Raster::HLineSolidColor(pSurface,x1,x2,y,color);
		 }

		 int EARasterConcrete::HLineColor(ISurface* pSurface, int x1, int x2, int  y, const Color& color)
		 {
			 return EA::Raster::HLineColor(pSurface,  x1,  x2,   y,color);
		 }

		 int EARasterConcrete::VLineSolidColor(ISurface* pSurface, int  x, int y1, int y2, const Color& color)
		 {
			 return EA::Raster::VLineSolidColor(pSurface,   x,  y1,  y2, color);
		 }

		 int EARasterConcrete::VLineColor(ISurface* pSurface, int  x, int y1, int y2, const Color& color)
		 {
			 return EA::Raster::VLineColor(pSurface,   x,  y1,  y2, color);
		 }

		 int EARasterConcrete::LineColor(ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color)
		 {
			 return EA::Raster::LineColor(pSurface, x1, y1, x2, y2, color);
		 }
		 int EARasterConcrete::LineRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a)
		 {
			 return EA::Raster::LineRGBA(pSurface,  x1,  y1,  x2,  y2,  r,  g,  b,  a);
		 }
	
		 int EARasterConcrete::AALineRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a)
		 {
			 return EA::Raster::AALineRGBA(pSurface,  x1,  y1,  x2,  y2,  r,  g,  b,  a);
		 }

		// Circle / Ellipse
		 int EARasterConcrete::CircleColor(ISurface* pSurface, int x, int y, int radius, const Color& color)
		 {
			 return EA::Raster::CircleColor(pSurface,  x,  y,  radius, color);
		 }
		 int EARasterConcrete::CircleRGBA(ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a)
		 {
			 return EA::Raster::CircleRGBA(pSurface,  x,  y,  radius,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::EllipseColor(ISurface* pSurface, int x, int y, int rx, int ry, const Color& color)
		 {
			 return EA::Raster::EllipseColor(pSurface,  x,  y,  rx,  ry,  color);
		 }
		 int EARasterConcrete::EllipseRGBA(ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a)
		 {
			 return EA::Raster::EllipseRGBA(pSurface,  x,  y,  rx,  ry,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::AAEllipseColor(ISurface* pSurface, int xc, int yc, int rx, int ry, const Color& color)
		 {
			 return EA::Raster::AAEllipseColor(pSurface,  xc,  yc,  rx,  ry, color);
		 }
		 int EARasterConcrete::FilledEllipseColor(ISurface* pSurface, int x, int y, int rx, int ry, const Color& color)
		 {
			 return EA::Raster::FilledEllipseColor(pSurface,  x,  y,  rx,  ry, color);
		 }
		 int EARasterConcrete::FilledEllipseRGBA(ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a)
		 {
			 return EA::Raster::FilledEllipseRGBA(pSurface,  x,  y,  rx,  ry,  r,  g,  b,  a);
		 }

		// Polygon
		 int EARasterConcrete::SimpleTriangle(ISurface* pSurface, int  x, int  y, int size, Orientation o, const Color& color)
		 {
			 return EA::Raster::SimpleTriangle(pSurface,   x,   y,  size, o, color);
		 }
		 int EARasterConcrete::PolygonColor(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color)
		 {
			 return EA::Raster::PolygonColor(pSurface, vx,  vy,  n, color);
		 }
		 int EARasterConcrete::PolygonRGBA(ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a)
		 {
			 return EA::Raster::PolygonRGBA(pSurface, vx, vy,  n,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::AAPolygonColor(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color)
		 {
			 return EA::Raster::AAPolygonColor(pSurface, vx, vy,  n, color);
		 }
		 int EARasterConcrete::AAPolygonRGBA(ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a)
		 {
			 return EA::Raster::AAPolygonRGBA(pSurface, vx, vy,  n,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::FilledPolygonColor(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color)
		 {
			 return EA::Raster::FilledPolygonColor(pSurface, vx, vy,  n, color);
		 }
		 int EARasterConcrete::FilledPolygonRGBA(ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a)
		 {
			 return EA::Raster::FilledPolygonRGBA(pSurface, vx, vy,  n,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::FilledPolygonColorMT(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color, int** polyInts, int* polyAllocated)
		 {
			 return EA::Raster::FilledPolygonColorMT(pSurface, vx,  vy,  n, color, polyInts, polyAllocated);
		 }
		 int EARasterConcrete::FilledPolygonRGBAMT(ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a, int** polyInts, int* polyAllocated)
		 {
			 return EA::Raster::FilledPolygonRGBAMT(pSurface, vx, vy,  n,  r,  g,  b,  a, polyInts, polyAllocated);
		 }


#if UNUSED_IRASTER_CALLS_ENABLED

         int EARasterConcrete::HLineSolidRGBA(ISurface* pSurface, int x1, int x2, int  y, int r, int g, int b, int a)
		 {
			 return EA::Raster::HLineSolidRGBA(pSurface,  x1,  x2,   y,  r,  g,  b,  a);
		 }
         int EARasterConcrete::HLineRGBA(ISurface* pSurface, int x1, int x2, int  y, int r, int g, int b, int a)
		 {
			 return EA::Raster::HLineRGBA(pSurface,  x1,  x2,   y,  r,  g,  b,  a);
		 }
         int EARasterConcrete::VLineSolidRGBA(ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a)
		 {
			 return EA::Raster::VLineSolidRGBA(pSurface,   x,  y1,  y2,  r,  g,  b,  a);
		 }
         int EARasterConcrete::VLineRGBA(ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a)
		 {
			 return EA::Raster::VLineRGBA(pSurface, x, y1, y2, r, g, b, a);
		 }
         int EARasterConcrete::AALineColor(ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color, bool bDrawEndpoint)
		 {
			 return EA::Raster::AALineColor(pSurface,  x1,  y1,  x2,  y2,color,bDrawEndpoint);
		 }
		 int EARasterConcrete::AALineColor(ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color)
		 {
			 return EA::Raster::AALineColor(pSurface,  x1,  y1,  x2,  y2, color);
		 }
         int EARasterConcrete::ArcColor(ISurface* pSurface, int x, int y, int r, int start, int end, const Color& color)
		 {
			 return EA::Raster::ArcColor(pSurface,  x,  y,  r,  start,  end, color);
		 }
		 int EARasterConcrete::ArcRGBA(ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a)
		 {
			 return EA::Raster::ArcRGBA(pSurface,  x,  y,  radius,  start,  end,  r,  g,  b,  a);
		 }
         int EARasterConcrete::AACircleColor(ISurface* pSurface, int x, int y, int r, const Color& color)
		 {
			 return EA::Raster::AACircleColor(pSurface,  x,  y,  r, color);
		 }
		 int EARasterConcrete::AACircleRGBA(ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a)
		 {
			 return EA::Raster::AACircleRGBA(pSurface,  x,  y,  radius,  r,  g,  b,  a);
		 }
         int EARasterConcrete::FilledCircleColor(ISurface* pSurface, int x, int y, int r, const Color& color)
		 {
			 return EA::Raster::FilledCircleColor(pSurface,  x,  y,  r, color);
		 }
		 int EARasterConcrete::FilledCircleRGBA(ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a)
		 {
			 return EA::Raster::FilledCircleRGBA(pSurface,  x,  y,  radius,  r,  g,  b,  a);
		 }
         int EARasterConcrete::AAEllipseRGBA(ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a)
		 {
			 return EA::Raster::AAEllipseRGBA(pSurface,  x,  y,  rx,  ry,  r,  g,  b,  a);
		 }
         int EARasterConcrete::PieColor(ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color)
		 {
			 return EA::Raster::PieColor(pSurface,  x,  y,  radius,  start,  end, color);
		 }
		 int EARasterConcrete::PieRGBA(ISurface* pSurface, int x, int y, int radius,  int start, int end, int r, int g, int b, int a)
		 {
			 return EA::Raster::PieRGBA(pSurface,  x,  y,  radius,   start,  end,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::FilledPieColor(ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color)
		 {
			 return EA::Raster::FilledPieColor(pSurface,  x,  y,  radius,  start,  end, color);
		 }
		 int EARasterConcrete::FilledPieRGBA(ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a)
		 {
			 return EA::Raster::FilledPieRGBA(pSurface,  x,  y,  radius,  start,  end,  r,  g,  b,  a);
		 }
         int EARasterConcrete::TrigonColor(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color)
		 {
			 return EA::Raster::TrigonColor(pSurface,  x1,  y1,  x2,  y2,  x3,  y3, color);
		 }
		 int EARasterConcrete::TrigonRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a)
		 {
			 return EA::Raster::TrigonRGBA(pSurface,  x1,  y1,  x2,  y2,  x3,  y3,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::AATrigonColor(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color)
		 {
			 return EA::Raster::AATrigonColor(pSurface,  x1,  y1,  x2,  y2,  x3,  y3, color);
		 }
		 int EARasterConcrete::AATrigonRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a)
		 {
			 return EA::Raster::AATrigonRGBA(pSurface,  x1,  y1,  x2,  y2,  x3,  y3,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::FilledTrigonColor(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color)
		 {
			 return EA::Raster::FilledTrigonColor(pSurface,  x1,  y1,  x2,  y2,  x3,  y3, color);
		 }
		 int EARasterConcrete::FilledTrigonRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a)
		 {
			 return EA::Raster::FilledTrigonRGBA(pSurface,  x1,  y1,  x2,  y2,  x3,  y3,  r,  g,  b,  a);
		 }
		 int EARasterConcrete::TexturedPolygon(ISurface* pSurface, const int* vx, const int* vy, int n, ISurface* pTexture,int texture_dx,int texture_dy)
		 {
			 //notImplemented();
			 return -1;
			 //return EA::Raster::TexturedPolygon(pSurface, vx, vy,  n, pTexture, texture_dx, texture_dy);
		 }
		 int EARasterConcrete::TexturedPolygonMT(ISurface* pSurface, const int* vx, const int* vy, int n, ISurface* pTexture, int texture_dx, int texture_dy, int** polyInts, int* polyAllocated)
		 {
			 //notImplemented();
			 return -1;
			 //return EA::Raster::TexturedPolygonMT(pSurface, vx, vy,  n, pTexture,  texture_dx,  texture_dy, polyInts, polyAllocated);
		 }
#endif



		///////////////////////////////////////////////////////////////////////
		// Resampling
		///////////////////////////////////////////////////////////////////////

		 ISurface* EARasterConcrete::ZoomSurface(ISurface* pSurface, double zoomx, double zoomy, bool bSmooth)
		 {
			 return EA::Raster::ZoomSurface(pSurface, zoomx, zoomy, bSmooth);
		 }

		 void EARasterConcrete::ZoomSurfaceSize(int width, int height, double zoomx, double zoomy, int* dstwidth, int* dstheight)
		 {
			 return EA::Raster::ZoomSurfaceSize(width, height, zoomx, zoomy, dstwidth, dstheight);
		 }

		 ISurface* EARasterConcrete::ShrinkSurface(ISurface* pSurface, int factorX, int factorY)
		 {
			 return EA::Raster::ShrinkSurface(pSurface, factorX, factorY);
		 }

		 ISurface* EARasterConcrete::RotateSurface90Degrees(ISurface* pSurface, int nClockwiseTurns)
		 {
			 return EA::Raster::RotateSurface90Degrees(pSurface, nClockwiseTurns);
		 }

         ISurface* EARasterConcrete::TransformSurface(ISurface* pSurface, Rect& scrRect, Matrix2D& matrix)
		 {
			 return EA::Raster::TransformSurface(pSurface, scrRect, matrix);
		 }   

		 ISurface* EARasterConcrete::CreateTransparentSurface(ISurface* pSource, int surfaceAlpha)
		 {
			 return EA::Raster::CreateTransparentSurface(pSource, surfaceAlpha);
		 }


		///////////////////////////////////////////////////////////////////////
		// Blit functions
		///////////////////////////////////////////////////////////////////////

		 bool EARasterConcrete::ClipForBlit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, Rect& rectSourceResult, Rect& rectDestResult)
		 {
			 return EA::Raster::ClipForBlit(pSource, pRectSource, pDest, pRectDest, rectSourceResult, rectDestResult);
		 }
		 int EARasterConcrete::Blit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pDestClipRect)
		 {
			 return EA::Raster::Blit(pSource, pRectSource, pDest, pRectDest, pDestClipRect);
		 }

		 int EARasterConcrete::BlitNoClip(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest)
		 {
			 return EA::Raster::BlitNoClip(pSource, pRectSource, pDest, pRectDest);
		 }

		 int EARasterConcrete::BlitTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, int offsetX, int offsetY)
		 {
			 return EA::Raster::BlitTiled(pSource, pRectSource, pDest, pRectDest, offsetX, offsetY);
		 }

		 int EARasterConcrete::BlitEdgeTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pRectSourceCenter)
		 {
			 return EA::Raster::BlitEdgeTiled(pSource, pRectSource, pDest, pRectDest, pRectSourceCenter);
		 }

		 bool EARasterConcrete::SetupBlitFunction(ISurface* pSource, ISurface* pDest)
		 {
			 return EA::Raster::SetupBlitFunction(pSource, pDest);
		 }

		///////////////////////////////////////////////////////////////////////
		// Utility functions
		///////////////////////////////////////////////////////////////////////

		// The result parameter may refer to one of the source parameters.
		 bool EARasterConcrete::IntersectRect(const Rect& a, const Rect& b, Rect& result)
		 {
			 return EA::Raster::IntersectRect(a, b, result);
		 }

		// A PPM file is a simple bitmap format which many picture viewers can read.
		 bool EARasterConcrete::WritePPMFile(const char* pPath, ISurface* pSurface, bool bAlphaOnly)
		 {
			 return EA::Raster::WritePPMFile(pPath, pSurface, bAlphaOnly);
		 }

		 RGBA32 EARasterConcrete::makeRGB(int32_t r, int32_t g, int32_t b)
		 {
			 return EA::Raster::makeRGB( r,  g,  b);
		 }
		 RGBA32 EARasterConcrete::makeRGBA(int32_t r, int32_t g, int32_t b, int32_t a)
		 {
			 return EA::Raster::makeRGBA( r,  g,  b,  a);
		 }

	}
}

