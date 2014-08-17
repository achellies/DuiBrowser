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


#ifndef BitmapImage_h
#define BitmapImage_h

#include <wtf/FastAllocBase.h>
#include "Image.h"
#include "Color.h"
#include "IntSize.h"

namespace WKAL {
    struct FrameData;
}

// This complicated-looking declaration tells the FrameData Vector that it should copy without
// invoking our constructor or destructor. This allows us to have a vector even for a struct
// that's not copyable.
namespace WTF {
    template<> class VectorTraits<WebCore::FrameData> : public SimpleClassVectorTraits {};
}

namespace WKAL {

template <typename T> class Timer;

// ================================================
// FrameData Class
// ================================================

struct FrameData : Noncopyable {
public:
#if NO_MACRO_NEW
	// Placement operator new.
        void* operator new(size_t, void* p) { return p; }
        void* operator new[](size_t, void* p) { return p; }

        void* operator new(size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNew);
            return p;
        }

        void operator delete(void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNew);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }

        void* operator new[](size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNewArray);
            return p;
        }

        void operator delete[](void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNewArray);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }
#endif //NO_MACRO_NEW
    FrameData()
        : m_frame(0)
        , m_duration(0)
        , m_hasAlpha(true)
    {
    }

    ~FrameData()
    { 
        clear();
    }

    void clear();

    NativeImagePtr m_frame;
    float m_duration;
    bool m_hasAlpha;
};

// =================================================
// BitmapImage Class
// =================================================

class BitmapImage : public Image {
    friend class GeneratedImage;
    friend class GraphicsContext;
public:
    BitmapImage(BalSurface*, ImageObserver* = 0);
    BitmapImage(ImageObserver* = 0);
    ~BitmapImage();

    virtual bool isBitmapImage() const { return true; } // 7/14/09 CSidhall - Added

    virtual IntSize size() const;

    virtual bool dataChanged(bool allDataReceived);

    // It may look unusual that there is no start animation call as public API.  This is because
    // we start and stop animating lazily.  Animation begins whenever someone draws the image.  It will
    // automatically pause once all observers no longer want to render the image anywhere.
    virtual void stopAnimation();
    virtual void resetAnimation();
    
    virtual unsigned decodedSize() const { return m_decodedSize; }

    virtual NativeImagePtr nativeImageForCurrentFrame() { return frameAtIndex(currentFrame()); }

    bool imagePruneLockStatus() const;  // 7/14/09 CSidhall - Added 
protected:
    virtual void draw(GraphicsContext*, const FloatRect& dstRect, const FloatRect& srcRect, CompositeOperator);
    size_t currentFrame() const { return m_currentFrame; }
    size_t frameCount();
    NativeImagePtr frameAtIndex(size_t);
    float frameDurationAtIndex(size_t);
    bool frameHasAlphaAtIndex(size_t); 

    // Decodes and caches a frame. Never accessed except internally.
    void cacheFrame(size_t index);

    // Called to invalidate all our cached data.  If an image is loading incrementally, we only
    // invalidate the last cached frame.
    virtual void destroyDecodedData(bool incremental = false);

    // Whether or not size is available yet.    
    bool isSizeAvailable();

    // Animation.
    bool shouldAnimate();
    virtual void startAnimation();
    void advanceAnimation(Timer<BitmapImage>*);
    
    // Handle platform-specific data
    void initPlatformData();
    void invalidatePlatformData();
    
    // Checks to see if the image is a 1x1 solid color.  We optimize these images and just do a fill rect instead.
    void checkForSolidColor();
    
    virtual bool mayFillWithSolidColor() const { return m_isSolidColor && m_currentFrame == 0; }
    virtual Color solidColor() const { return m_solidColor; }

    ImageSource m_source;
    mutable IntSize m_size; // The size to use for the overall image (will just be the size of the first image).
    
    size_t m_currentFrame; // The index of the current frame of animation.
    Vector<FrameData> m_frames; // An array of the cached frames of the animation. We have to ref frames to pin them in the cache.
    
    Timer<BitmapImage>* m_frameTimer;
    int m_repetitionCount; // How many total animation loops we should do.
    int m_repetitionsComplete;  // How many repetitions we've finished.

    Color m_solidColor;  // If we're a 1x1 solid color, this is the color to use to fill.
    bool m_isSolidColor;  // Whether or not we are a 1x1 solid image.

    bool m_animatingImageType;  // Whether or not we're an image type that is capable of animating (GIF).
    bool m_animationFinished;  // Whether or not we've completed the entire animation.

    bool m_allDataReceived;  // Whether or not we've received all our data.

    mutable bool m_haveSize; // Whether or not our |m_size| member variable has the final overall image size yet.
    bool m_sizeAvailable; // Whether or not we can obtain the size of the first image frame yet from ImageIO.
    unsigned m_decodedSize; // The current size of all decoded frames.

    mutable bool m_haveFrameCount;
    size_t m_frameCount;
};

}

#endif
