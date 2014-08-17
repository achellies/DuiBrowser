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
// EARaster.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef EARASTER_EARASTER_H
#define EARASTER_EARASTER_H


#include <EABase/eabase.h>
#include <EARaster/EARasterColor.h>
#include <EARaster/EARasterConfig.h>

#ifndef UNUSED_IRASTER_CALLS_ENABLED
    #define UNUSED_IRASTER_CALLS_ENABLED 0    // Set to 1 to activate unused Raster calls.  These are disabled to simplify IRaster.
#endif

namespace WKAL  // a.k.a. namespace WebCore
{
    class IntRect;
}


namespace EA
{
    namespace Raster
    {
        // Forward declarations
        class ISurface;
        class Color;


        // Typedefs
        typedef uint32_t NativeColor;     // Refers to a color used by a surface in the surface's PixelFormatType.


        // Orientation
        // These can be used as values or as flags.
        enum Orientation
        {
            kOLeft  = 0x01,
            kOUp    = 0x02,     // a.k.a. Top
            kORight = 0x04,
            kODown  = 0x08      // a.k.a. Bottom
        };


        // Note: WebKit's WKAL::Color type declares itself to be RGBA, but it is wrong; 
        // it is using ARGB, in particular the 32 bit 0xAARRGGBB type as we have below.
        enum PixelFormatType
        {
            kPixelFormatTypeARGB,       // 32 bit 0xAARRGGBB, byte order in memory depends on endian-ness. For little endian it is B, G, R, A; for big endian it is A, R, G, B.
            kPixelFormatTypeRGBA,       // 32 bit 0xRRGGBBAA, byte order in memory depends on endian-ness. For little endian it is A, B, G, R; for big endian it is R, G, B, A. 
            kPixelFornatTypeXRGB,       // 32 bit 0xXXRRGGBB, byte order in memory depends on endian-ness. The X means the data is unused or can be considered to be always 0xff.
            kPixelFormatTypeRGBX,       // 32 bit 0xRRGGBBXX, byte order in memory depends on endian-ness. The X means the data is unused or can be considered to be always 0xff.
            kPixelFormatTypeRGB         // 24 bit 0xRRGGBB, byte order in memory is always R, G, B.
        };


        struct PixelFormat
        {
            PixelFormatType mPixelFormatType;   // e.g. kPixelFormatTypeARGB.
            uint8_t         mBytesPerPixel;     // Typically four, as in the case of ARGB.
            uint8_t         mSurfaceAlpha;      // Alpha that is applied to the entire surface and not just per pixel.
	        uint32_t        mRMask;
	        uint32_t        mGMask;
	        uint32_t        mBMask;
	        uint32_t        mAMask;
	        uint8_t         mRShift;
	        uint8_t         mGShift;
	        uint8_t         mBShift;
	        uint8_t         mAShift;
        };


        enum SurfaceFlags
        {
            kFlagRAMSurface     = 0x00,     // The pixel data resides in regular RAM.
            kFlagTextureSurface = 0x01,     // The pixel data resides in a hardware texture instead of RAM.
            kFlagOtherOwner     = 0x02,     // The Surface data is owned by somebody other than the Surface, and the Surface doesn't free it.
            kFlagDisableAlpha   = 0x04,     // Source alpha blending is used if the source has an alpha channel.
            kFlagIgnoreCompressRLE         = 0x08, // This image did not compress so ignore it for compression -CS Added 1/ 15/09
            kFlagIgnoreCompressYCOCGDXT5   = 0x10, // This image did not compress so ignore it for duplicate compression
            kFlagCompressedRLE             = 0x20, // Set when image was compressed using RLE
            kFlagCompressedYCOCGDXT5       = 0x40  // Set when image was compressed using RLE
        };

        
        // This provides some information on the general surface category that is being allocated
        enum SurfaceCategory
        {
            kSurfaceCategoryDefault,            // Default if no category is known
            kSurfaceCategoryMainView,           // The main view surface
            kSurfaceCategoryImage,              // Image or image buffer    
            kSurfaceCategoryImageCompression,   // This is a scratch surface used by the decompression to unpack to for the draw
            kSurfaceCategoryText,               // A text surface, probably a string of glyphs
            kSurfaceCategoryMovie,              // Movie
            kSurfaceCategoryZoom,               // Zoomed or shrunk surface - probably a scratch surface
            kSurfaceCategoryScratch,            // Temp surface
            kSurfaceCategorySelectDropDown,     // Select dropdown popup
            kSurfaceCategoryTooltip,            // Tooltip popup
            kSurfaceCategoryCanvas,             // For canvas surfaces (not yet actived in EAWebKit - canvas surfaces are set as image surfaces)
            kSurfaceCategoryExternal,           // Not inside EAWebKIt
        };


        struct BlitInfo
        {
            ISurface* mpSource;
            uint8_t* mpSPixels;     // Start of blit data.
            int      mnSWidth;      // Width of blit data.
            int      mnSHeight;     // Height of blit data.
            int      mnSSkip;       // Bytes from end of blit row to beginning of new blit row.

            ISurface* mpDest;
            uint8_t* mpDPixels;
            int      mnDWidth;
            int      mnDHeight;
            int      mnDSkip;
        };

        typedef void (*BlitFunctionType)(const BlitInfo& blitInfo);


        enum DrawFlags
        {
            kDrawFlagIdenticalFormats = 0x01    // The source and dest surfaces of a surface to surface operation are of identical format.
        };

        struct Point
        {
            int x;
            int y;

            Point() { } // By design, don't initialize members.
            Point(int px, int py) : x(px), y(py) { }

            void set(int px, int py) {x = px; y = py;}
        };


        // For now we use a x/y/w/h rect instead of a x0/y0/x1/y1 rect like often seen.
        // This is because WebKit's IntRect class works like that. The problem with 
        // the IntRect class is that it is tedious to use.
        struct Rect
        {
            int x;
            int y;
            int w;
            int h;

            Rect() { }  // By design, don't initialize members.

            Rect(int xNew, int yNew, int wNew, int hNew) 
                : x(xNew), y(yNew), w(wNew), h(hNew) { }

            int width()  const { return w; }
            int height() const { return h; }

            bool isPointInside(const Point& p) const { return (p.x >= x) && (p.x <= x + w) && (p.y >= y) && (p.y <= y + h); }

            void constrainRect(Rect& r) const
            {
                //if r is outside of this rect, set width or height to zero
                if(r.x > x+w || r.x + r.w < x){ r.w = 0;  return;}
                if(r.y > y+h || r.y + r.h < y){ r.h = 0;  return;}
                if(r.x < x){ r.w -= x - r.x; r.x = x;}
                if(r.x + r.w > x + w){r.w -= (r.x + r.w) - (x + w);}
                if(r.y < y){r.h -= y - r.y; r.y = y;}
                if(r.y + r.h > y + h){r.h -= (r.y + r.h) - (y + h);}
            }
        };

        EARASTER_API void IntRectToEARect(const WKAL::IntRect& in, EA::Raster::Rect& out);
        EARASTER_API void EARectToIntRect(const EA::Raster::Rect& in, WKAL::IntRect& out);

        struct Matrix2D
        {
            double m_m11;
            double m_m12;
            double m_m21;
            double m_m22;
            double m_dx;
            double m_dy;
        
            Matrix2D(double a, double b, double c, double d, double e, double f) :
             m_m11(a),m_m12(b),m_m21(c),m_m22(d),m_dx(e),m_dy(f){}
        };


        // Abstract ISurface class that is exportable.  
        class EARASTER_API ISurface
        {
        public:
            virtual ~ISurface(){}

            virtual int  AddRef()=0;
            virtual int  Release()=0;

            virtual void SetPixelFormat(PixelFormatType pft)=0;
            virtual bool Set(void* pData, int width, int height, int stride, PixelFormatType pft, bool bCopyData, bool bTakeOwnership, SurfaceCategory category)=0;
            virtual bool Set(ISurface* pSource)=0;
            virtual bool Resize(int width, int height, bool bCopyData)=0;
            virtual bool FreeData()=0;
            virtual void SetClipRect(const Rect* pRect)=0;
            virtual void SetSurfaceFlags(int flags) =0;
            virtual void SetBlitFunction(BlitFunctionType p) =0;
            virtual void SetDrawFlags(int flags) =0;
            virtual void SetBlitDest(ISurface* p) =0;
            virtual void SetUserData(void* p) =0; 
            virtual void SetClipRect(const Rect& r) =0;
            virtual void SetCompressedSize(int size) =0;
            virtual void SetCategory(SurfaceCategory category) =0;
            
            virtual int GetSurfaceFlags() const = 0;
            virtual int GetWidth() const =0;
            virtual int GetHeight() const =0;
            virtual int GetStride() const =0;
            virtual void* GetData() const =0;
            virtual Rect& GetClipRect() =0;
            virtual PixelFormat& GetPixelFormat() =0;
            virtual ISurface* GetBlitDest() const =0;
            virtual BlitFunctionType GetBlitFunction() const =0;
            virtual int GetDrawFlags() const =0;
            virtual void* GetUserData() const =0; 
            virtual int GetCompressedSize() =0;
            virtual SurfaceCategory GetCategory() =0;

            virtual void Lock() =0;                     // Surface is drawn to
            virtual void Unlock() =0;                   
        protected:
            virtual void InitMembers()=0 ;

  };


        // Surface
        //
        // This class implements a single rectangular 2D pixel surface. 
        // Uses main memory to allocate the buffer
        //
        class EARASTER_API Surface : public ISurface
        {
        public:
            Surface();
            Surface(int width, int height, PixelFormatType pft);
            virtual ~Surface();

            virtual int  AddRef();
            virtual int  Release();

            virtual void SetPixelFormat(PixelFormatType pft);
            virtual bool Set(void* pData, int width, int height, int stride, PixelFormatType pft, bool bCopyData, bool bTakeOwnership, SurfaceCategory category);
            virtual bool Set(ISurface* pSource);
            virtual bool Resize(int width, int height, bool bCopyData);
            virtual bool FreeData();
            virtual void SetClipRect(const Rect* pRect);
        
            virtual void Lock();
            virtual void Unlock();

            virtual int GetSurfaceFlags() const { return mSurfaceFlags; }
            virtual int GetWidth()  const { return mWidth; }
            virtual int GetHeight() const { return mHeight; }
            virtual int GetStride() const { return mStride; }
            virtual void* GetData() const { return mpData; }
            virtual Rect& GetClipRect() { return mClipRect; }
            virtual PixelFormat& GetPixelFormat() { return mPixelFormat; }
            virtual ISurface* GetBlitDest() const { return mpBlitDest; }
            virtual BlitFunctionType GetBlitFunction() const { return mpBlitFunction; }
            virtual int GetDrawFlags() const { return mDrawFlags; }
            virtual void* GetUserData() const { return mpUserData; }     
            virtual int GetCompressedSize() { return mCompressedSize; }
            virtual SurfaceCategory GetCategory() { return mCategory; };

            virtual void SetSurfaceFlags(int flags) { mSurfaceFlags = flags; }
            virtual void SetBlitFunction(BlitFunctionType p) { mpBlitFunction = p; }
            virtual void SetDrawFlags(int flags) { mDrawFlags = flags; }
            virtual void SetBlitDest(ISurface* p) { mpBlitDest = p; }
            virtual void SetUserData(void* p) { mpUserData = p; } 
            virtual void SetClipRect(const Rect& r) { mClipRect = r; }
            virtual void SetCompressedSize(int size) { mCompressedSize = size; }
            virtual void SetCategory(SurfaceCategory category) { mCategory = category; }

        protected:
            virtual void InitMembers();


        public:
            PixelFormat         mPixelFormat;   // The format of the pixel data.
            int                 mSurfaceFlags;  // See enum SurfaceFlags.
            void*               mpData;         // The pixel data itself.
            int                 mWidth;         // In pixels.
            int                 mHeight;        // In pixels.
            int                 mStride;        // In bytes.
            int                 mLockCount;     // Used if the surface is somewhere other than conventional RAM.
            int                 mRefCount;      // Factory functions return a Surface with a mRefCount of 1.
            void*               mpUserData;     // Arbitrary data the user can associate with this surface. Used to store compressed images if image compression is active. Also used to store the view for scroll bars.
            int                 mCompressedSize; // Size of buffer if compression was used - CS 1/15/09 Added
            // Draw info.
            Rect                mClipRect;      // Drawing is restricted to within this rect.
            ISurface*            mpBlitDest;     // The last surface blitted to. Allows us to cache blit calculations.
            int                 mDrawFlags;     // See enum DrawFlags.
            BlitFunctionType    mpBlitFunction; // The blitting function currently used to blit to mpBlitDest.
            SurfaceCategory     mCategory;      // Surface main category like a main view surface or an image or text...

        };


        ///////////////////////////////////////////////////////////////////////
        // Primitive functions
        //
        // Functions with int return values use 0 for OK and -1 for error.
        //
        // Coordinates are end-inclusive. Thus a line drawn from x1 to x2 includes
        // a point at x2.
        //
        // Note the use of NativeColor vs. Color. NativeColor is a uint32_t
        // which is always in the pixel format of the surface that is using
        // it. This is as opposed to the Color type which is a generic 
        // ARGB color type which may need to be converted to a surface's
        // native type before written to the surface.
        ///////////////////////////////////////////////////////////////////////

        // Surface management
        EARASTER_API ISurface*   CreateSurface();
        EARASTER_API ISurface*   CreateSurface(int width, int height, PixelFormatType pft,SurfaceCategory category);
        EARASTER_API ISurface*   CreateSurface(ISurface* pSurface);
        EARASTER_API ISurface*   CreateSurface(void* pData, int width, int height, int stride, PixelFormatType pft, bool bCopyData, bool bTakeOwnership,SurfaceCategory category);
        EARASTER_API void        DestroySurface(ISurface* pSurface);

        // Color conversion
        EARASTER_API void        ConvertColor(NativeColor c, PixelFormatType cFormat, Color& result);
        EARASTER_API void        ConvertColor(int r, int g, int b, int a, Color& result);
        EARASTER_API NativeColor ConvertColor(const Color& c, PixelFormatType resultFormat);
        EARASTER_API void        ConvertColor(const Color& c, int& r, int& g, int& b, int& a);
        EARASTER_API void        ConvertColor(NativeColor c, const PixelFormat& pf, int& r, int& g, int& b, int& a);
        EARASTER_API void        ConvertColor(NativeColor c, const PixelFormat& pf, int& r, int& g, int& b);

        // Pixel functions
        EARASTER_API void  GetPixel                (ISurface* pSurface, int x, int y, Color& color);
        EARASTER_API int   SetPixelSolidColor      (ISurface* pSurface, int x, int y, const Color& color);
        EARASTER_API int   SetPixelSolidColorNoClip(ISurface* pSurface, int x, int y, const Color& color);
        EARASTER_API int   SetPixelColor           (ISurface* pSurface, int x, int y, const Color& color);
        EARASTER_API int   SetPixelColorNoClip     (ISurface* pSurface, int x, int y, const Color& color);
        EARASTER_API int   SetPixelRGBA            (ISurface* pSurface, int x, int y, int r, int g, int b, int a);
        EARASTER_API int   SetPixelRGBANoClip      (ISurface* pSurface, int x, int y, int r, int g, int b, int a);

        // Rectangle functions
        EARASTER_API int   FillRectSolidColor      (ISurface* pSurface, const Rect* pRect, const Color& color);
        EARASTER_API int   FillRectColor           (ISurface* pSurface, const Rect* pRect, const Color& color);
        EARASTER_API int   RectangleColor          (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color);
        EARASTER_API int   RectangleColor          (ISurface* pSurface, const EA::Raster::Rect& rect, const Color& c);
        EARASTER_API int   RectangleRGBA           (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a);

      //EARASTER_API int   BoxColor                (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color);
      //EARASTER_API int   BoxRGBA                 (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a);

        // Line functions
        EARASTER_API int   HLineSolidColor(ISurface* pSurface, int x1, int x2, int  y, const Color& color);
        EARASTER_API int   HLineColor     (ISurface* pSurface, int x1, int x2, int  y, const Color& color);
        EARASTER_API int   VLineSolidColor(ISurface* pSurface, int  x, int y1, int y2, const Color& color);
        EARASTER_API int   VLineColor     (ISurface* pSurface, int  x, int y1, int y2, const Color& color);
        EARASTER_API int   LineColor      (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color);
        EARASTER_API int   LineRGBA       (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a);
        EARASTER_API int   AALineRGBA     (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a);

        // Circle / Ellipse
        EARASTER_API int   CircleColor       (ISurface* pSurface, int x, int y, int radius, const Color& color);
        EARASTER_API int   CircleRGBA        (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a);
        EARASTER_API int   EllipseColor      (ISurface* pSurface, int x, int y, int rx, int ry, const Color& color);
        EARASTER_API int   EllipseRGBA       (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a);
        EARASTER_API int   AAEllipseColor    (ISurface* pSurface, int xc, int yc, int rx, int ry, const Color& color);
        EARASTER_API int   FilledEllipseColor(ISurface* pSurface, int x, int y, int rx, int ry, const Color& color);
        EARASTER_API int   FilledEllipseRGBA (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a);
      

        // Polygon
        EARASTER_API int   SimpleTriangle      (ISurface* pSurface, int  x, int  y, int size, Orientation o, const Color& color);
        EARASTER_API int   PolygonColor        (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color);
        EARASTER_API int   PolygonRGBA         (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a);
        EARASTER_API int   AAPolygonColor      (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color);
        EARASTER_API int   AAPolygonRGBA       (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a);
        EARASTER_API int   FilledPolygonColor  (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color);
        EARASTER_API int   FilledPolygonColorMT(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color, int** polyInts, int* polyAllocated);
        EARASTER_API int   FilledPolygonRGBAMT (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a, int** polyInts, int* polyAllocated);
        EARASTER_API int   FilledPolygonRGBA   (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a);


#if UNUSED_IRASTER_CALLS_ENABLED 
        // Line
        EARASTER_API int   HLineSolidRGBA (ISurface* pSurface, int x1, int x2, int  y, int r, int g, int b, int a);
        EARASTER_API int   HLineRGBA      (ISurface* pSurface, int x1, int x2, int  y, int r, int g, int b, int a);
        EARASTER_API int   VLineSolidRGBA (ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a);
        EARASTER_API int   VLineRGBA      (ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a);
        EARASTER_API int   AALineColor    (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color, bool bDrawEndpoint);
        EARASTER_API int   AALineColor    (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color);

        // Circle / Ellipse
        EARASTER_API int   ArcColor          (ISurface* pSurface, int x, int y, int r, int start, int end, const Color& color);
        EARASTER_API int   ArcRGBA           (ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a);
        EARASTER_API int   AACircleColor     (ISurface* pSurface, int x, int y, int r, const Color& color);
        EARASTER_API int   AACircleRGBA      (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a);
        EARASTER_API int   FilledCircleRGBA  (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a);
        EARASTER_API int   FilledCircleColor (ISurface* pSurface, int x, int y, int r, const Color& color);
        EARASTER_API int   AAEllipseRGBA     (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a);
        EARASTER_API int   PieColor          (ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color);
        EARASTER_API int   PieRGBA           (ISurface* pSurface, int x, int y, int radius,  int start, int end, int r, int g, int b, int a);
        EARASTER_API int   FilledPieColor    (ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color);
        EARASTER_API int   FilledPieRGBA     (ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a);
        // Polygon
        EARASTER_API int   TrigonColor         (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
        EARASTER_API int   TrigonRGBA          (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);
        EARASTER_API int   AATrigonColor       (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
        EARASTER_API int   AATrigonRGBA        (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);
        EARASTER_API int   FilledTrigonColor   (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
        EARASTER_API int   FilledTrigonRGBA    (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);
        EARASTER_API int   TexturedPolygon     (ISurface* pSurface, const int* vx, const int* vy, int n, ISurface* pTexture,int texture_dx,int texture_dy);
        EARASTER_API int   TexturedPolygonMT   (ISurface* pSurface, const int* vx, const int* vy, int n, ISurface* pTexture, int texture_dx, int texture_dy, int** polyInts, int* polyAllocated);
#endif // UNUSED_IRASTER_CALLS_ENABLED

        ///////////////////////////////////////////////////////////////////////
        // Resampling
        ///////////////////////////////////////////////////////////////////////

        // Zoom
        // Zooms a 32bit or 8bit 'src' surface to newly created 'dst' surface.
        // 'zoomx' and 'zoomy' are scaling factors for width and height. If 'smooth' is 1
        // then the destination 32bit surface is anti-aliased. If the surface is not 8bit
        // or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.
        // zoomX/zoomY can be less than 1.0 for shrinking.
        EARASTER_API ISurface* ZoomSurface(ISurface* pSurface, double zoomx, double zoomy, bool bSmooth);

        // Returns the size of the target surface for a zoomSurface() call
        EARASTER_API void ZoomSurfaceSize(int width, int height, double zoomx, double zoomy, int* dstwidth, int* dstheight);

        // Shrinks a 32bit or 8bit surface to a newly created surface.
        // 'factorX' and 'factorY' are the shrinking ratios (i.e. 2 = 1/2 the size,
        // 3 = 1/3 the size, etc.) The destination surface is antialiased by averaging
        // the source box RGBA or Y information. If the surface is not 8bit
        // or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.
        EARASTER_API ISurface* ShrinkSurface(ISurface* pSurface, int factorX, int factorY);

        // Returns a rotated surface by 90, 180, or 270 degrees.
        EARASTER_API ISurface* RotateSurface90Degrees(ISurface* pSurface, int nClockwiseTurns);

        // Apply a transform to surface .
        EARASTER_API ISurface* TransformSurface(ISurface* pSurface, Rect& scrRect, Matrix2D& matrix);


        // Creates a surface that is the same as pSource but with surfaceAlpha multiplied into pSource.
        EARASTER_API ISurface* CreateTransparentSurface(ISurface* pSource, int surfaceAlpha);


        ///////////////////////////////////////////////////////////////////////
        // Blit functions
        ///////////////////////////////////////////////////////////////////////

        // Generates rectSourceResult and rectDestResult from source and dest
        // surfaces and unclipped rectangles.
        EARASTER_API bool ClipForBlit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, Rect& rectSourceResult, Rect& rectDestResult);

        // Does a 1:1 blit from pRectSource in pSource to pRectDest in pDest. 
        // Handles the case whereby pRectSource and pRectDest may refer to out of bounds of 
        // pSource and pDest, respectively.
        // If pDestClipRect is non-NULL, the output is further clipped to pDestClipRect.
        // pDestClipRect is not quite the same as pRectDest, as it's sometimes 
        // useful to blit a source rect to a dest rect but have it clip to another rect.
        // Returns 0 if OK or a negative error code.
        EARASTER_API int Blit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pDestClipRect = NULL);

        // Does a 1:1 blit from pSource to pDest with the assumption that pRectSource and 
        // pRectDest are already clipped to pSource and pDest, respectively.
        // Returns 0 if OK or a negative error code.
        EARASTER_API int BlitNoClip(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest);

        //////////////////////////////////////////////////////////////////////////
        /// Blit a repeating pattern.
        /// The offsetX/Y position is the location within pRectDest that 
        /// the source origin will be. The blit is clipped to within pRectDest.
        /// If pRectSource is NULL then the entire Source is used, else the part
        /// of pSource that is tiled into pDest is the defined by pRectSource.
        EARASTER_API int BlitTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, int offsetX, int offsetY);

        //////////////////////////////////////////////////////////////////////////
        /// This function stretches an image over a rectangular region
        /// by repeating (tiling) the center portion of the image.
        /// This is useful for drawing arbitrarily sized GUI buttons and
        /// scrollbar parts.
        ///
        /// The image is divided like so:
        /// +-----------+-----------+-----------+
        /// | TL Corner |   Top     | TR Corner |
        /// |           |   Edge    |           |
        /// +-----------+-----------+-----------+
        /// | Left      |   Center  | Right     |
        /// | Edge      |           | Edge      |
        /// +-----------+-----------+-----------+
        /// | BL Corner |   Bottom  | BR Corner |
        /// |           |   Edge    |           |
        /// +-----------+-----------+-----------+
        /// The center portions will be tiled to cover the destination
        /// rectangle, where the edge portions will only be blitted
        /// once.
        ///
        /// Parameters:
        ///   pDest - the target drawing context.
        ///   pRectDest - the destination rectangle to fill
        ///   pImage - the source image to render.
        ///   pRectSource - the source rectangle which has nine parts.
        ///   pRectSourceCenter - Defines where the dividing lines between center
        ///       and edges are (For example, mLeft controls the position
        ///       of the dividing line between left edge and center.)
        EARASTER_API int BlitEdgeTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pRectSourceCenter);

        // Sets up the blit function needed to blit pSource to pDest.
        // Normally you don't need to call this function, as the Surface class and Blit 
        // functions will do it automatically.
        EARASTER_API bool SetupBlitFunction(ISurface* pSource, ISurface* pDest);



        ///////////////////////////////////////////////////////////////////////
        // Utility functions
        ///////////////////////////////////////////////////////////////////////

        // The result parameter may refer to one of the source parameters.
        EARASTER_API bool IntersectRect(const Rect& a, const Rect& b, Rect& result);

        // A PPM file is a simple bitmap format which many picture viewers can read.
        EARASTER_API bool WritePPMFile(const char* pPath, ISurface* pSurface, bool bAlphaOnly);

		class IEARaster
		{
		public:
			virtual ~IEARaster()
			{

			}
			virtual void IntRectToEARect(const WKAL::IntRect& in, EA::Raster::Rect& out) = 0;
			virtual void EARectToIntRect(const EA::Raster::Rect& in, WKAL::IntRect& out) = 0;
			// Surface management
			virtual ISurface*    CreateSurface() = 0;
			virtual ISurface*    CreateSurface(int width, int height, PixelFormatType pft, SurfaceCategory category) = 0;
			virtual ISurface*    CreateSurface(ISurface* pSurface) = 0;
			virtual ISurface*    CreateSurface(void* pData, int width, int height, int stride, PixelFormatType pft, bool bCopyData, bool bTakeOwnership,SurfaceCategory category) = 0;
			virtual void        DestroySurface(ISurface* pSurface) = 0;

			// Color conversion
			virtual void        ConvertColor(NativeColor c, PixelFormatType cFormat, Color& result) = 0;
			virtual void        ConvertColor(int r, int g, int b, int a, Color& result) = 0;
			virtual NativeColor ConvertColor(const Color& c, PixelFormatType resultFormat) = 0;
			virtual void        ConvertColor(const Color& c, int& r, int& g, int& b, int& a) = 0;
			virtual void        ConvertColor(NativeColor c, const PixelFormat& pf, int& r, int& g, int& b, int& a) = 0;
			virtual void        ConvertColor(NativeColor c, const PixelFormat& pf, int& r, int& g, int& b) = 0;

			// Pixel functions
			virtual void  GetPixel                (ISurface* pSurface, int x, int y, Color& color) = 0;
			virtual int   SetPixelSolidColor      (ISurface* pSurface, int x, int y, const Color& color) = 0;
			virtual int   SetPixelSolidColorNoClip(ISurface* pSurface, int x, int y, const Color& color) = 0;
			virtual int   SetPixelColor           (ISurface* pSurface, int x, int y, const Color& color) = 0;
			virtual int   SetPixelColorNoClip     (ISurface* pSurface, int x, int y, const Color& color) = 0;
			virtual int   SetPixelRGBA            (ISurface* pSurface, int x, int y, int r, int g, int b, int a) = 0;
			virtual int   SetPixelRGBANoClip      (ISurface* pSurface, int x, int y, int r, int g, int b, int a) = 0;

			// Rectangle functions
			virtual int   FillRectSolidColor      (ISurface* pSurface, const Rect* pRect, const Color& color) = 0;
			virtual int   FillRectColor           (ISurface* pSurface, const Rect* pRect, const Color& color) = 0;
			virtual int   RectangleColor          (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color) = 0;
			virtual int   RectangleColor          (ISurface* pSurface, const EA::Raster::Rect& rect, const Color& c) = 0;
			virtual int   RectangleRGBA           (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a) = 0;

			// Line functions
            virtual int   HLineColor     (ISurface* pSurface, int x1, int x2, int  y, const Color& color) = 0;
            virtual int   VLineColor     (ISurface* pSurface, int  x, int y1, int y2, const Color& color) = 0;
            virtual int   LineRGBA       (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a) = 0;

			// Circle / Ellipse
			virtual int   CircleColor       (ISurface* pSurface, int x, int y, int radius, const Color& color) = 0;
			virtual int   EllipseColor      (ISurface* pSurface, int x, int y, int rx, int ry, const Color& color) = 0;
            virtual int   EllipseRGBA       (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a) = 0;
            virtual int   FilledEllipseColor(ISurface* pSurface, int x, int y, int rx, int ry, const Color& color) = 0;
            virtual int   FilledEllipseRGBA (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a) = 0;

			// Polygon
			virtual int   SimpleTriangle      (ISurface* pSurface, int  x, int  y, int size, Orientation o, const Color& color) = 0;
            virtual int   PolygonRGBA         (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a) = 0;
            virtual int   FilledPolygonRGBA   (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a) = 0;

            //--- Start of draw functions used only within EARaster ---
            // Note: These seem only used within EARaster and not directly by EAWebKit.  Also some might be using each other some so might actually be unused).
			// Line functions
            virtual int   HLineSolidColor(ISurface* pSurface, int x1, int x2, int  y, const Color& color) = 0;
			virtual int   VLineSolidColor(ISurface* pSurface, int  x, int y1, int y2, const Color& color) = 0;
            virtual int   LineColor      (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color) = 0;
            
            // Circle / Ellipse
			virtual int   CircleRGBA        (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a) = 0;
   			virtual int   AAEllipseColor    (ISurface* pSurface, int xc, int yc, int rx, int ry, const Color& color) = 0;

            // Polygon
			virtual int   PolygonColor        (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color) = 0;
			virtual int   AAPolygonColor      (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color) = 0;
			virtual int   AAPolygonRGBA       (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a) = 0;
			virtual int   FilledPolygonColor  (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color) = 0;
			virtual int   FilledPolygonRGBAMT (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a, int** polyInts, int* polyAllocated) = 0;
			virtual int   FilledPolygonColorMT(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color, int** polyInts, int* polyAllocated) = 0;
            //--- End of draw functions used only within EARaster ---

#if UNUSED_IRASTER_CALLS_ENABLED 
            //--- Start of unused draw functions inside EAWebKit ---
            // These seem unused by EAWebKit.  We might consider removing them as they just complicate the IRaster interface and take code space          
            // Line functions
            virtual int   HLineSolidRGBA (ISurface* pSurface, int x1, int x2, int  y, int r, int g, int b, int a) = 0;
			virtual int   HLineRGBA      (ISurface* pSurface, int x1, int x2, int  y, int r, int g, int b, int a) = 0;
			virtual int   VLineSolidRGBA (ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a) = 0;
			virtual int   VLineRGBA      (ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a) = 0;
			virtual int   AALineColor    (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color, bool bDrawEndpoint) = 0;
   			virtual int   AALineColor    (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color) = 0;
            
            // Circle / Ellipse
			virtual int   ArcColor          (ISurface* pSurface, int x, int y, int r, int start, int end, const Color& color) = 0;
			virtual int   ArcRGBA           (ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a) = 0;
			virtual int   AACircleColor     (ISurface* pSurface, int x, int y, int r, const Color& color) = 0;
			virtual int   AACircleRGBA      (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a) = 0;
			virtual int   FilledCircleColor (ISurface* pSurface, int x, int y, int r, const Color& color) = 0;
			virtual int   FilledCircleRGBA  (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a) = 0;
   			virtual int   AAEllipseRGBA     (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a) = 0;
            virtual int   PieColor          (ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color) = 0;
			virtual int   PieRGBA           (ISurface* pSurface, int x, int y, int radius,  int start, int end, int r, int g, int b, int a) = 0;
			virtual int   FilledPieColor    (ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color) = 0;
   			virtual int   FilledPieRGBA     (ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a) = 0;
            // Polygon
			virtual int   TrigonColor         (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color) = 0;
			virtual int   TrigonRGBA          (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a) = 0;
			virtual int   AATrigonColor       (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color) = 0;
			virtual int   AATrigonRGBA        (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a) = 0;
			virtual int   FilledTrigonColor   (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color) = 0;
			virtual int   FilledTrigonRGBA    (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a) = 0;
			virtual int   TexturedPolygon     (ISurface* pSurface, const int* vx, const int* vy, int n, ISurface* pTexture,int texture_dx,int texture_dy) = 0;
            virtual int   TexturedPolygonMT   (ISurface* pSurface, const int* vx, const int* vy, int n, ISurface* pTexture, int texture_dx, int texture_dy, int** polyInts, int* polyAllocated) = 0;
            //--- End of unused functions inside EAWebKit ---
#endif // UNUSED_IRASTER_CALLS_ENABLED 


			///////////////////////////////////////////////////////////////////////
			// Resampling
			///////////////////////////////////////////////////////////////////////

			// Zoom
			// Zooms a 32bit or 8bit 'src' surface to newly created 'dst' surface.
			// 'zoomx' and 'zoomy' are scaling factors for width and height. If 'smooth' is 1
			// then the destination 32bit surface is anti-aliased. If the surface is not 8bit
			// or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.
			// zoomX/zoomY can be less than 1.0 for shrinking.
			virtual ISurface* ZoomSurface(ISurface* pSurface, double zoomx, double zoomy, bool bSmooth) = 0;

			// Returns the size of the target surface for a zoomSurface() = 0 call
			virtual void ZoomSurfaceSize(int width, int height, double zoomx, double zoomy, int* dstwidth, int* dstheight) = 0;

			// Shrinks a 32bit or 8bit surface to a newly created surface.
			// 'factorX' and 'factorY' are the shrinking ratios (i.e. 2 = 1/2 the size,
			// 3 = 1/3 the size, etc.) The destination surface is antialiased by averaging
			// the source box RGBA or Y information. If the surface is not 8bit
			// or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.
			virtual ISurface* ShrinkSurface(ISurface* pSurface, int factorX, int factorY) = 0;

			// Returns a rotated surface by 90, 180, or 270 degrees.
			virtual ISurface* RotateSurface90Degrees(ISurface* pSurface, int nClockwiseTurns) = 0;

            // Apply a transform to a surface.
            virtual ISurface* TransformSurface(ISurface* pSurface, Rect& scrRect, Matrix2D& matrix) = 0;

			// Creates a surface that is the same as pSource but with surfaceAlpha multiplied into pSource.
			virtual ISurface* CreateTransparentSurface(ISurface* pSource, int surfaceAlpha) = 0;


			///////////////////////////////////////////////////////////////////////
			// Blit functions
			///////////////////////////////////////////////////////////////////////

			// Generates rectSourceResult and rectDestResult from source and dest
			// surfaces and unclipped rectangles.
			virtual bool ClipForBlit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, Rect& rectSourceResult, Rect& rectDestResult) = 0;

			// Does a 1:1 blit from pRectSource in pSource to pRectDest in pDest. 
			// Handles the case whereby pRectSource and pRectDest may refer to out of bounds of 
			// pSource and pDest, respectively.
			// If pDestClipRect is non-NULL, the output is further clipped to pDestClipRect.
			// pDestClipRect is not quite the same as pRectDest, as it's sometimes 
			// useful to blit a source rect to a dest rect but have it clip to another rect.
			// Returns 0 if OK or a negative error code.
			virtual int Blit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pDestClipRect = NULL) = 0;

			// Does a 1:1 blit from pSource to pDest with the assumption that pRectSource and 
			// pRectDest are already clipped to pSource and pDest, respectively.
			// Returns 0 if OK or a negative error code.
			virtual int BlitNoClip(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDestz) = 0;

			//////////////////////////////////////////////////////////////////////////
			/// Blit a repeating pattern.
			/// The offsetX/Y position is the location within pRectDest that 
			/// the source origin will be. The blit is clipped to within pRectDest.
			/// If pRectSource is NULL then the entire Source is used, else the part
			/// of pSource that is tiled into pDest is the defined by pRectSource.
			virtual int BlitTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, int offsetX, int offsetY) = 0;

			//////////////////////////////////////////////////////////////////////////
			/// This function stretches an image over a rectangular region
			/// by repeating (tiling) the center portion of the image.
			/// This is useful for drawing arbitrarily sized GUI buttons and
			/// scrollbar parts.
			///
			/// The image is divided like so:
			/// +-----------+-----------+-----------+
			/// | TL Corner |   Top     | TR Corner |
			/// |           |   Edge    |           |
			/// +-----------+-----------+-----------+
			/// | Left      |   Center  | Right     |
			/// | Edge      |           | Edge      |
			/// +-----------+-----------+-----------+
			/// | BL Corner |   Bottom  | BR Corner |
			/// |           |   Edge    |           |
			/// +-----------+-----------+-----------+
			/// The center portions will be tiled to cover the destination
			/// rectangle, where the edge portions will only be blitted
			/// once.
			///
			/// Parameters:
			///   pDest - the target drawing context.
			///   pRectDest - the destination rectangle to fill
			///   pImage - the source image to render.
			///   pRectSource - the source rectangle which has nine parts.
			///   pRectSourceCenter - Defines where the dividing lines between center
			///       and edges are (For example, mLeft controls the position
			///       of the dividing line between left edge and center.) = 0
			virtual int BlitEdgeTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pRectSourceCenter) = 0;

			// Sets up the blit function needed to blit pSource to pDest.
			// Normally you don't need to call this function, as the Surface class and Blit 
			// functions will do it automatically.
			virtual bool SetupBlitFunction(ISurface* pSource, ISurface* pDest) = 0;



			///////////////////////////////////////////////////////////////////////
			// Utility functions
			///////////////////////////////////////////////////////////////////////

			// The result parameter may refer to one of the source parameters.
			virtual bool IntersectRect(const Rect& a, const Rect& b, Rect& result) = 0;

			// A PPM file is a simple bitmap format which many picture viewers can read.
			virtual bool WritePPMFile(const char* pPath, ISurface* pSurface, bool bAlphaOnly) = 0;

			virtual RGBA32 makeRGB(int32_t r, int32_t g, int32_t b) = 0;
			virtual RGBA32 makeRGBA(int32_t r, int32_t g, int32_t b, int32_t a) = 0;

		};



		class EARasterConcrete: public IEARaster
		{
		public:
			virtual ~EARasterConcrete()
			{

			}
			virtual void IntRectToEARect(const WKAL::IntRect& in, EA::Raster::Rect& out);
			virtual void EARectToIntRect(const EA::Raster::Rect& in, WKAL::IntRect& out);
			// Surface management
			virtual ISurface*    CreateSurface();
			virtual ISurface*    CreateSurface(int width, int height, PixelFormatType pft,SurfaceCategory category=kSurfaceCategoryDefault);
			virtual ISurface*    CreateSurface(ISurface* pSurface);
			virtual ISurface*    CreateSurface(void* pData, int width, int height, int stride, PixelFormatType pft, bool bCopyData, bool bTakeOwnership,SurfaceCategory category=kSurfaceCategoryDefault);
			virtual void        DestroySurface(ISurface* pSurface);

			// Color conversion
			virtual void        ConvertColor(NativeColor c, PixelFormatType cFormat, Color& result);
			virtual void        ConvertColor(int r, int g, int b, int a, Color& result);
			virtual NativeColor ConvertColor(const Color& c, PixelFormatType resultFormat);
			virtual void        ConvertColor(const Color& c, int& r, int& g, int& b, int& a);
			virtual void        ConvertColor(NativeColor c, const PixelFormat& pf, int& r, int& g, int& b, int& a);
			virtual void        ConvertColor(NativeColor c, const PixelFormat& pf, int& r, int& g, int& b);

			// Pixel functions
			virtual void  GetPixel                (ISurface* pSurface, int x, int y, Color& color);
			virtual int   SetPixelSolidColor      (ISurface* pSurface, int x, int y, const Color& color);
			virtual int   SetPixelSolidColorNoClip(ISurface* pSurface, int x, int y, const Color& color);
			virtual int   SetPixelColor           (ISurface* pSurface, int x, int y, const Color& color);
			virtual int   SetPixelColorNoClip     (ISurface* pSurface, int x, int y, const Color& color);
			virtual int   SetPixelRGBA            (ISurface* pSurface, int x, int y, int r, int g, int b, int a);
			virtual int   SetPixelRGBANoClip      (ISurface* pSurface, int x, int y, int r, int g, int b, int a);

			// Rectangle functions
			virtual int   FillRectSolidColor      (ISurface* pSurface, const Rect* pRect, const Color& color);
			virtual int   FillRectColor           (ISurface* pSurface, const Rect* pRect, const Color& color);
			virtual int   RectangleColor          (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color);
			virtual int   RectangleColor          (ISurface* pSurface, const EA::Raster::Rect& rect, const Color& c);
			virtual int   RectangleRGBA           (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a);

			//virtual int   BoxColor                (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color);
			//virtual int   BoxRGBA                 (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a);

			// Line functions
			virtual int   HLineSolidColor(ISurface* pSurface, int x1, int x2, int  y, const Color& color);
			virtual int   HLineColor     (ISurface* pSurface, int x1, int x2, int  y, const Color& color);
			virtual int   VLineSolidColor(ISurface* pSurface, int  x, int y1, int y2, const Color& color);
			virtual int   VLineColor     (ISurface* pSurface, int  x, int y1, int y2, const Color& color);
			virtual int   LineColor      (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color);
			virtual int   LineRGBA       (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a);
			virtual int   AALineRGBA     (ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a);

			// Circle / Ellipse
			virtual int   CircleColor       (ISurface* pSurface, int x, int y, int radius, const Color& color);
			virtual int   CircleRGBA        (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a);
			virtual int   EllipseColor      (ISurface* pSurface, int x, int y, int rx, int ry, const Color& color);
			virtual int   EllipseRGBA       (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a);
			virtual int   AAEllipseColor    (ISurface* pSurface, int xc, int yc, int rx, int ry, const Color& color);
			virtual int   FilledEllipseColor(ISurface* pSurface, int x, int y, int rx, int ry, const Color& color);
			virtual int   FilledEllipseRGBA (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a);


			// Polygon
			virtual int   SimpleTriangle      (ISurface* pSurface, int  x, int  y, int size, Orientation o, const Color& color);
			virtual int   PolygonColor        (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color);
			virtual int   PolygonRGBA         (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a);
			virtual int   AAPolygonColor      (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color);
			virtual int   AAPolygonRGBA       (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a);
			virtual int   FilledPolygonColor  (ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color);
			virtual int   FilledPolygonColorMT(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color, int** polyInts, int* polyAllocated);
			virtual int   FilledPolygonRGBAMT (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a, int** polyInts, int* polyAllocated);
   			virtual int   FilledPolygonRGBA   (ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a);

#if UNUSED_IRASTER_CALLS_ENABLED 
            // Line
			virtual int   HLineSolidRGBA (ISurface* pSurface, int x1, int x2, int  y, int r, int g, int b, int a);
			virtual int   HLineRGBA      (ISurface* pSurface, int x1, int x2, int  y, int r, int g, int b, int a);
			virtual int   VLineSolidRGBA (ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a);
			virtual int   VLineRGBA      (ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a);
			virtual int   AALineColor    (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color, bool bDrawEndpoint);
			virtual int   AALineColor    (ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color);

            // Circle / Ellipse
			virtual int   ArcColor          (ISurface* pSurface, int x, int y, int r, int start, int end, const Color& color);
			virtual int   ArcRGBA           (ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a);
			virtual int   AACircleColor     (ISurface* pSurface, int x, int y, int r, const Color& color);
			virtual int   AACircleRGBA      (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a);
			virtual int   FilledCircleColor (ISurface* pSurface, int x, int y, int r, const Color& color);
			virtual int   FilledCircleRGBA  (ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a);
			virtual int   AAEllipseRGBA     (ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a);
			virtual int   PieColor          (ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color);
			virtual int   PieRGBA           (ISurface* pSurface, int x, int y, int radius,  int start, int end, int r, int g, int b, int a);
			virtual int   FilledPieColor    (ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color);
			virtual int   FilledPieRGBA     (ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a);
            
            // Polygon
			virtual int   TrigonColor         (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
			virtual int   TrigonRGBA          (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);
			virtual int   AATrigonColor       (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
			virtual int   AATrigonRGBA        (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);
			virtual int   FilledTrigonColor   (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
			virtual int   FilledTrigonRGBA    (ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);
			virtual int   TexturedPolygon     (ISurface* pSurface, const int* vx, const int* vy, int n, ISurface* pTexture,int texture_dx,int texture_dy);
			virtual int   TexturedPolygonMT   (ISurface* pSurface, const int* vx, const int* vy, int n, ISurface* pTexture, int texture_dx, int texture_dy, int** polyInts, int* polyAllocated);
#endif // UNUSED_IRASTER_CALLS_ENABLED

			virtual ISurface* ZoomSurface(ISurface* pSurface, double zoomx, double zoomy, bool bSmooth);
			virtual void ZoomSurfaceSize(int width, int height, double zoomx, double zoomy, int* dstwidth, int* dstheight);
			virtual ISurface* ShrinkSurface(ISurface* pSurface, int factorX, int factorY);
			virtual ISurface* RotateSurface90Degrees(ISurface* pSurface, int nClockwiseTurns);
            virtual ISurface* TransformSurface(ISurface* pSurface, Rect& scrRect, Matrix2D& matrix);
            virtual ISurface* CreateTransparentSurface(ISurface* pSource, int surfaceAlpha);
			virtual bool ClipForBlit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, Rect& rectSourceResult, Rect& rectDestResult);
			virtual int Blit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pDestClipRect = NULL);
			virtual int BlitNoClip(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest);
			virtual int BlitTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, int offsetX, int offsetY);
			virtual int BlitEdgeTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pRectSourceCenter);
			virtual bool SetupBlitFunction(ISurface* pSource, ISurface* pDest);
			virtual bool IntersectRect(const Rect& a, const Rect& b, Rect& result);
			virtual bool WritePPMFile(const char* pPath, ISurface* pSurface, bool bAlphaOnly);
			virtual RGBA32 makeRGB(int32_t r, int32_t g, int32_t b);
			virtual RGBA32 makeRGBA(int32_t r, int32_t g, int32_t b, int32_t a);

		};


    } // namespace Raster

} // namespace EA



#endif // Header include guard
