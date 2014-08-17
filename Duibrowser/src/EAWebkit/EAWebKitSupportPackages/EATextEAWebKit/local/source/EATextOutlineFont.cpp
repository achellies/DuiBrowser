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
// EATextOutlineFont.cpp
//
// Copyright (c) 2004 Electronic Arts. All Rights Reserved.
// By Paul Pedriana
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// FreeType Settings
// 
//     Setting                                      Explanation
//     ------------------------------------------------------------------------
//     FT_CONFIG_OPTION_SUBPIXEL_RENDERING          Needed if you want to do LCD-specific smoothing. 
//     TT_CONFIG_OPTION_BYTECODE_INTERPRETER        Needed if you want to use hinting present in TrueType-based fonts.
//     TT_CONFIG_OPTION_UNPATENTED_HINTING          Needed if you want to draw certain unusual Asian fonts but you have TT_CONFIG_OPTION_BYTECODE_INTERPRETER disabled. 
//
//
///////////////////////////////////////////////////////////////////////////////

#include <EAText/EATextOutlineFont.h>
#include <EAText/EATextScript.h>
#include <EAText/EATextFontServer.h>
#include <EAText/internal/StdC.h>
#include <EAIO/EAStreamFixedMemory.h>
#include <coreallocator/icoreallocatormacros.h>
#include EA_ASSERT_HEADER

#if EATEXT_EFFECTS_ENABLED
    #include <EAText/EATextEffects.h>
#endif


#define FTFontUnitsToFloat(x) (FFFixed26ToFloat(FT_MulFix((x), mpFaceData->mFTFace->size->metrics.x_scale)))

#define FFFloatToFixed16(x) ((int32_t)((x) * 65536.f))
#define FFFloatToFixed26(x) ((int32_t)((x) * 64.f))

#define FFFixed16ToFloat(x) (((float)(x) * (1.f / 65536.f))) // 1/65536 optimizes to an exact floating point constant, as 65536 is a power of 2.
#define FFFixed26ToFloat(x) (((float)(x) * (1.f / 64.f)))    // 1/64 optimizes to an exact floating point constant, as 65536 is a power of 2.


namespace EA
{

namespace Text
{


#if EATEXT_USE_FREETYPE
    extern FT_Library gFTLibrary;

    static unsigned long FT_Stream_Io(FT_Stream stream, unsigned long offset, unsigned char* buffer, unsigned long count)
    {
        IO::IStream* const pStream = (IO::IStream*)stream->descriptor.pointer;

        if(pStream->SetPosition((IO::off_type)offset) && count)  // count is zero when FreeType wants to simply seek to a position.
        {
            const uint32_t nActualSize = (uint32_t)pStream->Read(buffer, (IO::size_type)count);

            if(nActualSize == (uint32_t)count)
                return nActualSize;
        }

        return 0;  // FreeType doesn't document what an error return value should be.
    }

    static void FT_Stream_Close(FT_Stream /*stream*/)
    {
        // We don't close the stream here, because we manage stream 
        // lifetime at a higher level. We do this because two fonts 
        // could be sharing the same stream and we don't want one
        // font to close the stream of another font.
    }

#endif



///////////////////////////////////////////////////////////////////////////////
// FaceData
///////////////////////////////////////////////////////////////////////////////

FaceData::FaceData(Allocator::ICoreAllocator* pCoreAllocator)
  :
    #if EATEXT_USE_FREETYPE
        mFTFace(NULL),
        mFTStreamRec(),
    #endif

    #if EATEXT_THREAD_SAFETY_ENABLED
        mMutex(),
    #endif

    mpCoreAllocator(pCoreAllocator),
    mRefCount(0)
{
}


FaceData::~FaceData()
{
    #if EATEXT_USE_FREETYPE
        if(mFTFace)
            FT_Done_Face(mFTFace);
     #endif
}


int FaceData::AddRef()
{
    EA_ASSERT(mRefCount < 5000); // Sanity check.
    return ++mRefCount;
}


int FaceData::Release()
{
    #if EATEXT_THREAD_SAFETY_ENABLED
        int32_t rc = mRefCount.Decrement();
        if(rc)
            return rc;
    #else
        if(mRefCount > 1)
            return --mRefCount;
    #endif

    this->~FaceData();
    mpCoreAllocator->Free(this);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// OutlineFont
///////////////////////////////////////////////////////////////////////////////

OutlineFont::OutlineFont(Allocator::ICoreAllocator* pCoreAllocator)
  : Font(pCoreAllocator),
    mFontDescription(),
    mFontMetrics(),
    mGlyphMetricsMap(EA::Allocator::EASTLICoreAllocator(EATEXT_ALLOC_PREFIX "OutlineFont/GlyphMetricsMap", mpCoreAllocator)),
    mGlyphBitmap(),
    mnRenderCount(0),
    mDPI(EATEXT_DPI),
    mbEffectsInitialized(false),
    mbFontMetricsCalculated(false),
    mbEnableHinting(true),
    mbUseAutoHinting(false),
    mbLCD(false),
    mpEffectsProcessor(NULL),
    mpFaceData(NULL)
{
    #if EATEXT_OPENTYPE_ENABLED
        mbOTFEnabled = false;
        mOTF.SetAllocator(mpCoreAllocator);
    #endif

    #if EATEXT_USE_FREETYPE
        mFTSize = NULL;
    #endif
}


OutlineFont::~OutlineFont()
{
    OutlineFont::Close(); // Call OutlineFont::Close instead of just Close because Close may be a virtual function, and this is a destructor and vtables don't work in destructors.
}


void OutlineFont::SetAllocator(Allocator::ICoreAllocator* pCoreAllocator)
{
    Font::SetAllocator(pCoreAllocator);

    mGlyphMetricsMap.get_allocator().set_allocator(pCoreAllocator);

    #if EATEXT_OPENTYPE_ENABLED
        mOTF.SetAllocator(pCoreAllocator);
    #endif
}


void OutlineFont::SetFontFFaceData(FaceData* pFaceData)
{
    if(mpFaceData != pFaceData)
    {
        if(pFaceData)
            pFaceData->AddRef();

        if(mpFaceData)
            mpFaceData->Release();

        mpFaceData = pFaceData;
    }
}


FaceData* OutlineFont::CreateFaceData(Allocator::ICoreAllocator* pAllocator, IO::IStream* pStream, 
                                                 const void* pSourceData, uint32_t nSourceSize, int nFaceIndex)
{
    FaceData* pFaceData = (FaceData*)pAllocator->Alloc(sizeof(FaceData), EATEXT_ALLOC_PREFIX "FaceData", 0);

    if(pFaceData)
    {
        new(pFaceData) FaceData(pAllocator);
        pFaceData->AddRef();

        #if EATEXT_USE_FREETYPE
            FT_Error nFTError = 0;

            if(pStream)
            {
                pFaceData->mFTStreamRec.base               = NULL;
                pFaceData->mFTStreamRec.size               = (unsigned long)pStream->GetSize();
                pFaceData->mFTStreamRec.pos                = 0;
                pFaceData->mFTStreamRec.descriptor.pointer = pStream;
                pFaceData->mFTStreamRec.pathname.pointer   = NULL; // It could be useful to set this to the file name void the IStream if it's a stream.
                pFaceData->mFTStreamRec.read               = FT_Stream_Io;
                pFaceData->mFTStreamRec.close              = FT_Stream_Close;
              //pFaceData->mFTStreamRec.memory; // We shouldn't need to set these, as FreeType initializes them in FT_Open_Face().
              //pFaceData->mFTStreamRec.cursor;
              //pFaceData->mFTStreamRec.limit;

                FT_Open_Args openArgs;

                openArgs.flags       = FT_OPEN_STREAM;
                openArgs.memory_base = NULL;
                openArgs.memory_size = 0;
                openArgs.pathname    = NULL;
                openArgs.stream      = &pFaceData->mFTStreamRec;
                openArgs.driver      = NULL;    // Let FreeType find what kind of font file this is, though it ought to be TrueType.
                openArgs.num_params  = 0;
                openArgs.params      = NULL;

                nFTError = FT_Open_Face(gFTLibrary, &openArgs, nFaceIndex, &pFaceData->mFTFace);
            }
            else
            {
                #ifdef EA_DEBUG // We won't be using mFTStreamRec, so zero it in debug builds so it doesn't confuse us while looking at it in a debugger.
                    memset(&pFaceData->mFTStreamRec, 0, sizeof(pFaceData->mFTStreamRec));
                #endif

                nFTError = FT_New_Memory_Face(gFTLibrary, (const FT_Byte*)pSourceData, nSourceSize, nFaceIndex, &pFaceData->mFTFace);
            }

            // If this fails, did you forget to call EA::Text::Init() on startup?
            EA_ASSERT((nFTError == 0) && pFaceData->mFTFace);
            if(nFTError)
            {
                pFaceData->Release();
                pFaceData = NULL;
            }

        #endif
    }

    return pFaceData;
}


bool OutlineFont::Open(IO::IStream* pStream, int nFaceIndex)
{
    // We don't lock any mutex here because we assume that the owner of the font has not given 
    // an unopened font to anybody else and thus is handling any thread safety.

    bool bReturnValue;

    if(mpStream) // If already opened...
        bReturnValue = true;
    else
    {
        bReturnValue = false;

        EA_ASSERT(pStream && pStream->GetAccessFlags());
        if(pStream && pStream->GetAccessFlags()) // If there is a stream to open and if the stream is open...
        {
            // mpFaceData will be NULL only if the user is creating a private or otherwise 
            // standalone font as opposed to creating a font managed by the FontServer. Using the 
            // FontServer is more efficient, as it allows for sharing between multiple representations
            // of a given face.

            if(!mpFaceData)
            {
                mpFaceData = CreateFaceData(mpCoreAllocator, pStream, NULL, 0, nFaceIndex); // This will AddRef the return value for the caller.
                EA_ASSERT(mpFaceData != NULL);
            }

            if(mpFaceData)
            {
                #if EATEXT_THREAD_SAFETY_ENABLED
                    mpFaceData->mMutex.Lock(); // Lock the mutex for the remainder of this function.
                #endif

                bReturnValue = OpenInternal(nFaceIndex);
                EA_ASSERT(bReturnValue);

                if(bReturnValue)
                {
                    mpStream = pStream;
                    mpStream->AddRef(); // AddRef it for our own uses.
                }

                #if EATEXT_OPENTYPE_ENABLED
                    // Currently we unilaterally load the OTF (OpenType) information for the Font.
                    // We ignore the mOTF.Load() return value, as a failed load doesn't mean we 
                    // are inoperable; it may simply mean that the font doesn't have OpenType information.
                    // OpenType is really needed only for Arabic and Hindi (Devenagari). OpenType is 
                    // also useful for Thai, as it allows Thai decoration glyphs to be positioned 
                    // precisely. However, EAText has basic Thai decoration positioning functionality
                    // in the absence of OpenType and which is good enough in practice for most uses.
                    if(bReturnValue && mbOTFEnabled)
                        mOTF.Load(mpStream);
                #endif

                #if EATEXT_THREAD_SAFETY_ENABLED
                    if(mpFaceData)
                        mpFaceData->mMutex.Unlock(); // Lock the mutex for the remainder of this function.
                #endif
            }
        }

        if(!bReturnValue)
            Close();
    }

    return bReturnValue;
}


bool OutlineFont::Open(const void* pSourceData, uint32_t nSourceSize, int nFaceIndex)
{
    // We don't lock any mutex here because we assume that the owner of the font has not given 
    // an unopened font to anybody else and thus is handling any thread safety.

    bool bReturnValue;

    if(mpStream) // If already opened...
        bReturnValue = true;
    else
    {
        bReturnValue = false;

        EA_ASSERT(pSourceData != NULL);
        if(pSourceData) // If there is source to open...
        {
            // mpFaceData will be NULL only if the user is creating a private or otherwise 
            // standalone font as opposed to creating a font managed by the FontServer. Using the 
            // FontServer is more efficient, as it allows for sharing between multiple representations
            // of a given face.

            if(!mpFaceData)
            {
                mpFaceData = CreateFaceData(mpCoreAllocator, NULL, pSourceData, nSourceSize, nFaceIndex); // This will AddRef the return value for the caller.
                EA_ASSERT(mpFaceData != NULL);
            }

            if(mpFaceData)
            {
                #if EATEXT_THREAD_SAFETY_ENABLED
                    mpFaceData->mMutex.Lock(); // Lock the mutex for the remainder of this function.
                #endif

                bReturnValue = OpenInternal(nFaceIndex);
                EA_ASSERT(bReturnValue);

                // Do don't do anything additional with pSourceData. 
                // The user is expected to keep it around for the duration of the font.

                // To consider: Enable this code. Doing so will enable OpenType support for memory-based fonts.
                //#if EATEXT_OPENTYPE_ENABLED
                //    if(bReturnValue && mbOTFEnabled)
                //    {
                //        EA::IO::FixedMemoryStream stream(pSourceData, nSourceSize);
                //        stream.AddRef();
                //        mOTF.Load(&stream);
                //    }
                //#endif

                #if EATEXT_THREAD_SAFETY_ENABLED
                    if(mpFaceData)
                        mpFaceData->mMutex.Unlock(); // Lock the mutex for the remainder of this function.
                #endif
            }
        }

        if(!bReturnValue)
            Close();
    }

    return bReturnValue;
}


void OutlineFont::SetOption(int32_t option, int32_t value)
{
    // We don't lock any mutex because these options must be used before the font is opened.

    switch (option)
    {
        #if EATEXT_OPENTYPE_ENABLED
        case kOptionOpenTypeFeatures:
            mbOTFEnabled = (value != 0);
            break;
        #endif

        case kOptionDPI:
            EA_ASSERT(value > 0);
            mDPI = (int16_t)value;
            break;

        case kOptionEnableHinting:
            mbEnableHinting = (value != 0);
            break;

        case kOptionUseAutoHinting:
            mbUseAutoHinting = (value != 0);
            break;

        case kOptionLCD:
            mbLCD = (value != 0);
            break;
    }
}


bool OutlineFont::OpenInternal(int nFaceIndex)
{
    // This is an internal function, so we assume the mutex is locked.
    #if EATEXT_THREAD_SAFETY_ENABLED
        EA_ASSERT(mpFaceData && mpFaceData->mMutex.HasLock());
    #endif

    #if EATEXT_USE_FREETYPE
        if(mpFaceData->mFTFace)
        {
            // mFontDescription.mFamily
            if(mpFaceData->mFTFace->family_name)
                Strlcpy(mFontDescription.mFamily, mpFaceData->mFTFace->family_name, kFamilyNameCapacity);
            else
                Strlcpy(mFontDescription.mFamily, "UnknownFamily", kFamilyNameCapacity);

            // mFontDescription.mfSize
            mFontDescription.mfSize = 0.f;  // Don't set this here. User sets it separately.

            // mFontDescription.mStyle
            if(mpFaceData->mFTFace->style_flags & FT_STYLE_FLAG_ITALIC)
                mFontDescription.mStyle = kStyleItalic;
            else if(mpFaceData->mFTFace->family_name && Stristr(mpFaceData->mFTFace->family_name, " italic"))
                mFontDescription.mStyle = kStyleItalic;
            else if(mpFaceData->mFTFace->style_name && Stristr(mpFaceData->mFTFace->style_name, "italic"))
                mFontDescription.mStyle = kStyleItalic;
            else if(mpFaceData->mFTFace->style_name && Stristr(mpFaceData->mFTFace->style_name, "oblique"))
                mFontDescription.mStyle = kStyleOblique;
            else
                mFontDescription.mStyle = kStyleNormal;

            // mFontDescription.mWeight
            if(mpFaceData->mFTFace->style_flags & FT_STYLE_FLAG_BOLD)
                mFontDescription.mfWeight = kWeightBold;
            else if(mpFaceData->mFTFace->family_name && Stristr(mpFaceData->mFTFace->family_name, " bold"))
                mFontDescription.mfWeight = kWeightBold;
            else if(mpFaceData->mFTFace->style_name && Stristr(mpFaceData->mFTFace->style_name, "bold"))
                mFontDescription.mfWeight = kWeightBold;
            else
                mFontDescription.mfWeight = kWeightNormal;

            // mFontDescription.mfStretch
            mFontDescription.mfStretch = 1.f;

            // mFontDescription.mPitch
            if(mpFaceData->mFTFace->face_flags & FT_FACE_FLAG_FIXED_WIDTH)
                mFontDescription.mPitch = kPitchFixed;
            else
                mFontDescription.mPitch = kPitchVariable;

            // mFontDescription.mVariant
            // It appears that we need to read lower level font information to tell if the face is kVariantSmallCaps.
            mFontDescription.mVariant = kVariantNormal;

            // mFontDescription.mSmooth
            mFontDescription.mSmooth = kSmoothNone;  // Don't need to set this here. User sets normally it separately.


            // Misc Setup
            //////////////////////////////////////////////////

            // Set up mSupportedScriptSet.
            SetupSupportedScriptSet();

            // Set the replacement glyph for the font.
            Char c = (Char)EA_TEXT_REPLACEMENT_CHAR;
            mReplacementGlyphId = (GlyphId)FT_Get_Char_Index(mpFaceData->mFTFace, c); // FreeType returns 0 for missing glyph. Which is a problem since it doesn't tell you if there was an error.

            // Add a kGlyphIdZeroWidth glyph.
            GlyphMetrics gmZeroWidth;
            memset(&gmZeroWidth, 0, sizeof(gmZeroWidth));
            mGlyphMetricsMap.insert(GlyphMetricsMap::value_type(kGlyphIdZeroWidth, gmZeroWidth));

            return true;
        }
    #endif

    return false;
}


bool OutlineFont::Close()
{
    // Should we attempt to lock the mutex here? Normally we shouldn't be here
    // unless the font is no longer being used. We probably don't want to support
    // fonts that can be arbitrarily closed by one thread while possibly used 
    // by another thread.

    #if EATEXT_EFFECTS_ENABLED
        if(mpEffectsProcessor)
        {
            CORE_DELETE(mpCoreAllocator, mpEffectsProcessor);
            mpEffectsProcessor = NULL;
        }
    #endif

    #if EATEXT_USE_FREETYPE
        if(mFTSize)
        {
            FT_Done_Size(mFTSize); // This will remove it from the FTFace if it is active.
            mFTSize = NULL;
        }
    #endif

    if(mpFaceData)
    {
        mpFaceData->Release();
        mpFaceData = NULL;
    }

    if(mpStream)
    {
        mpStream->Release();
        mpStream = NULL;
    }

    mnRenderCount = 0;

    return true;
}


FontType OutlineFont::GetFontType()
{
    return (mpStream ? kFontTypeOutline : kFontTypeUnknown);
}


OutlineFont::BitmapFormat OutlineFont::GetBitmapFormat() const
{
    if(mFontDescription.mEffect == kEffectNone)
        return (mFontDescription.mSmooth == kSmoothEnabled) ? kBFGrayscale : kBFMonochrome;

    // As of this writing, effects are always done as ARGB and not RGBA or something else.
    return kBFARGB;
}


bool OutlineFont::GetFontDescription(FontDescription& fontDescription)
{
    // Disabled because this class' data is considered const at runtime:
    //#if EATEXT_THREAD_SAFETY_ENABLED
    //    EA_ASSERT(mpFaceData);
    //    EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
    //#endif

    if(mpStream)
    {
        fontDescription = mFontDescription;
        return true;
    }

    return false;
}


bool OutlineFont::GetFontMetrics(FontMetrics& fontMetrics)
{
    if(mpStream)
    {
        #if EATEXT_EFFECTS_ENABLED
            if(mFontDescription.mEffect && !mbFontMetricsCalculated)
            {
                #if EATEXT_THREAD_SAFETY_ENABLED
                    EA_ASSERT(mpFaceData);
                    EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
                #endif

                if(!mbEffectsInitialized)
                    InitEffectsProcessor();

                mpEffectsProcessor->AdjustFontMetrics(mFontMetrics);
                mbFontMetricsCalculated = true;
            }
        #endif

        fontMetrics = mFontMetrics;
        return true;
    }
    return false;
}


bool OutlineFont::GetGlyphMetrics(GlyphId glyphId, GlyphMetrics& glyphMetrics)
{
    bool bReturnValue = true;

    #if EATEXT_THREAD_SAFETY_ENABLED
        EA_ASSERT(mpFaceData);
        EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
    #endif

    const GlyphMetricsMap::const_iterator it = mGlyphMetricsMap.find(glyphId);

    if(it != mGlyphMetricsMap.end()) // If found...
    {
        const GlyphMetrics& glyphMetricsTemp = (*it).second;
        glyphMetrics = glyphMetricsTemp;
    }
    else
    {
        // If the following assert fails then Open wasn't successfully executed or SetTransform wasn't successfully executed after Open.
        #if EATEXT_USE_FREETYPE
            EA_ASSERT(mpFaceData->mFTFace && (mFontMetrics.mfSize > 0));
        #endif

        mnRenderCount++;

        if(mFontDescription.mEffect)
        {
            #if EATEXT_EFFECTS_ENABLED
                // The first time we try to render an effect, we initialize the EffectsProcessor.
                if(!mbEffectsInitialized)
                    InitEffectsProcessor();

                mpEffectsProcessor->Execute(this, glyphId);

                const GlyphMetrics& glyphMetricsTemp = mpEffectsProcessor->GetEffectsState()->mGlyphMetrics;
                const GlyphMetricsMap::value_type mapEntry(glyphId, glyphMetricsTemp);
                mGlyphMetricsMap.insert(mapEntry);
                glyphMetrics = glyphMetricsTemp;
            #else
                EA_FAIL_MESSAGE("OutlineFont::GetGlyphMetrics: EAText Effects are requested but disabled.");
                bReturnValue = false;
            #endif
        }
        else
        {
            #if EATEXT_USE_FREETYPE
                FT_Error nFTError = FT_Activate_Size(mFTSize);
                EA_ASSERT(nFTError == 0); (void)nFTError;

                // FreeType is a little confusing about load flags, load target flags and render flags. 
                // You pass FT_LOAD and FT_LOAD_TARGET flags to FT_Load_Glyph, and you pass 
                // FT_RENDER_MODE flags to FT_Render_Glyph. Its a little confusing because their 
                // names overlap in ways that make it easy to get confused about the effect of each.
                //
                // For FreeType, we take 'smoothing enabled' to mean hinting disabled. 

                int32_t loadFlags       = 0;        // Any of FT_LOAD_DEFAULT, FT_LOAD_NO_HINTING, FT_LOAD_NO_BITMAP, FT_LOAD_FORCE_AUTOHINT, FT_LOAD_PEDANTIC.
                int32_t loadTargetFlags = 0;        // One of FT_LOAD_TARGET_NORMAL, FT_LOAD_TARGET_LCD or FT_LOAD_TARGET_LIGHT. Don't want FT_LOAD_TARGET_MONO, as that targets the hinting algorithm at a true monochrome video screen and is not the same as rendering in mono.

                if(mbUseAutoHinting)
                    loadFlags |= FT_LOAD_FORCE_AUTOHINT;
                else if(!mbEnableHinting)
                    loadFlags |= FT_LOAD_NO_HINTING;

                if(mbLCD)
                    loadTargetFlags = FT_LOAD_TARGET_LCD;
                else
                    loadTargetFlags = FT_LOAD_TARGET_NORMAL;

                nFTError = FT_Load_Glyph(mpFaceData->mFTFace, glyphId, loadFlags | loadTargetFlags);
                EA_ASSERT(nFTError == 0); (void)nFTError;

                if(nFTError == 0)
                {
                    // const FT_Render_Mode renderMode = (mbLCD ? FT_RENDER_MODE_LCD : FT_RENDER_MODE_NORMAL); // FT_RENDER_MODE_NORMAL, FT_RENDER_MODE_LIGHT, FT_RENDER_MODE_MONO, FT_RENDER_MODE_LCD
                    const FT_Render_Mode renderMode = ((mFontDescription.mSmooth == kSmoothEnabled) || (mFontDescription.mEffect != kEffectNone)) ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

                    nFTError = FT_Render_Glyph(mpFaceData->mFTFace->glyph, renderMode);
                    EA_ASSERT(nFTError == 0); (void)nFTError;

                    if(nFTError == 0)
                    {
                        GetCurrentGlyphMetrics(glyphMetrics); // Get glyph metrics which we will return to the user.

                        const GlyphMetricsMap::value_type mapEntry(glyphId, glyphMetrics); // Cache the glyph metrics.
                        mGlyphMetricsMap.insert(mapEntry);
                    }
                }

                bReturnValue = (nFTError == 0);
 
            #endif
        }
    }

    return bReturnValue;
}


void OutlineFont::GetCurrentGlyphMetrics(GlyphMetrics& glyphMetrics)
{
    // This is an internal function, so we assume the mutex is locked.
    #if EATEXT_THREAD_SAFETY_ENABLED
        EA_ASSERT(mpFaceData && mpFaceData->mMutex.HasLock());
    #endif

    #if EATEXT_USE_FREETYPE
        EA_ASSERT(mpFaceData->mFTFace && mpFaceData->mFTFace->glyph);

        FT_GlyphSlot pGlyphSlot = mpFaceData->mFTFace->glyph;

        glyphMetrics.mfSizeX     = (float)pGlyphSlot->bitmap.width;
        glyphMetrics.mfSizeY     = (float)pGlyphSlot->bitmap.rows;
        glyphMetrics.mfHBearingX = (float)pGlyphSlot->bitmap_left;
        glyphMetrics.mfHBearingY = (float)pGlyphSlot->bitmap_top;
        glyphMetrics.mfHAdvanceX = FFFixed26ToFloat(pGlyphSlot->advance.x); // + 0.5f;  // Add 0.5f in order to cause integer rounding to work right. 

        #if EATEXT_VERTICAL_ENABLED
            // To consider: Implement this. In practice nobody is needing this, as vertical text is all but dead on computers.
        #endif
     #endif
}


uint32_t OutlineFont::GetGlyphIds(const Char* pCharArray, uint32_t nCharArrayCount, GlyphId* pGlyphIdArray, 
                                    bool bUseReplacementGlyph, const uint32_t nGlyphIdStride, bool bWriteInvalidGlyphs)
{
    // Disabled because this class' data is considered const at runtime:
    //#if EATEXT_THREAD_SAFETY_ENABLED
    //    EA_ASSERT(mpFaceData);
    //    EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
    //#endif

    #if EATEXT_USE_FREETYPE
        EA_ASSERT(mpFaceData->mFTFace);  // If this fails then Open wasn't successfully executed.
     #endif

    GlyphId     glyphId;    // Scratch memory in case pGlyphIdArray is NULL.
    const Char* pChar       = pCharArray;
    const Char* pCharEnd    = pCharArray + nCharArrayCount;
    GlyphId*    pGlyphId    = pGlyphIdArray ? pGlyphIdArray : &glyphId;
    uint32_t    glyphCount  = 0;

    while(pChar < pCharEnd)
    {
        #if EATEXT_USE_FREETYPE
             // FreeType returns 0 for a non-existing glyph. Which is a problem since it doesn't tell you if there was an error.
            const GlyphId g = (GlyphId)FT_Get_Char_Index(mpFaceData->mFTFace, *pChar);

            // Currently there is no known way to tell if FT_Get_Char_Index has failed and is returning the 0 glyph index due to an error or because it's the first glyph.
            bool bGlyphOK = (g != 0); // We can only hope that there isn't some real character at position zero.
         #endif

        if(bGlyphOK) // The large majority of the time this will be true.
            *pGlyphId = g;
        else
        {
            bGlyphOK = true;

            if(IsCharZeroWidth(*pChar))
                *pGlyphId = kGlyphIdZeroWidth;
            else if(bUseReplacementGlyph)
                *pGlyphId = mReplacementGlyphId;
            else if(bWriteInvalidGlyphs)
                *pGlyphId = kGlyphIdInvalid;
            else
                bGlyphOK = false;
        }

        if(bGlyphOK)
        {
            glyphCount++;

            if(pGlyphIdArray)
                pGlyphId = (GlyphId*)((char*)pGlyphId + nGlyphIdStride);
        }

        pChar++;
    }

    return glyphCount;
}


// Problem: We don't have a means whereby 
uint32_t OutlineFont::GetGlyphIds(const char8_t* pCharArray, uint32_t nCharArrayCount, GlyphId* pGlyphIdArray, 
                                    bool bUseReplacementGlyph, const uint32_t nGlyphIdStride, bool bWriteInvalidGlyphs)
{
    // Disabled because this class' data is considered const at runtime:
    //#if EATEXT_THREAD_SAFETY_ENABLED
    //    EA_ASSERT(mpFaceData);
    //    EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
    //#endif

    #if EATEXT_USE_FREETYPE
        EA_ASSERT(mpFaceData->mFTFace);  // If this fails then Open wasn't successfully executed.
    #endif

    GlyphId        glyphId;    // Scratch memory in case pGlyphIdArray is NULL.
    const char8_t* pChar       = pCharArray;
    const char8_t* pCharEnd    = pCharArray + nCharArrayCount;
    GlyphId*       pGlyphId    = pGlyphIdArray ? pGlyphIdArray : &glyphId;
    uint32_t       glyphCount  = 0;

    while(pChar < pCharEnd)
    {
        #if EATEXT_USE_FREETYPE
             // FreeType returns 0 for a non-existing glyph. Which is a problem since it doesn't tell you if there was an error.
            const GlyphId g = (GlyphId)FT_Get_Char_Index(mpFaceData->mFTFace, (uint8_t)*pChar);

            // Currently there is no known way to tell if FT_Get_Char_Index has failed and is returning the 0 glyph index due to an error or because it's the first glyph.
            bool bGlyphOK = (g != 0); // We can only hope that there isn't some real character at position zero.
       #endif

        if(bGlyphOK) // The large majority of the time this will be true.
            *pGlyphId = g;
        else
        {
            bGlyphOK = true;

            if(IsCharZeroWidth((uint8_t)*pChar))
                *pGlyphId = kGlyphIdZeroWidth;
            else if(bUseReplacementGlyph)
                *pGlyphId = mReplacementGlyphId;
            else if(bWriteInvalidGlyphs)
                *pGlyphId = kGlyphIdInvalid;
            else
                bGlyphOK = false;
        }

        if(bGlyphOK)
        {
            glyphCount++;

            if(pGlyphIdArray)
                pGlyphId = (GlyphId*)((char*)pGlyphId + nGlyphIdStride);
        }

        pChar++;
    }

    return glyphCount;
}


bool OutlineFont::IsCharSupported(Char c, Script script)
{
    // Disabled because this class' data is considered const at runtime:
    //#if EATEXT_THREAD_SAFETY_ENABLED
    //    EA_ASSERT(mpFaceData);
    //    EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
    //#endif

    // To consider: If script is kScriptUnknown then we could speed things
    //              up by getting the script for the Char and testing it
    //              against our script set. But that is only a win if the 
    //              script is not Latin, but very often in fact it will 
    //              be Latin.

    // We make the assumption here that all fonts support all ASCII characters.
    // This is an assumption that we can likely make for a font engine like this
    // which is used in conditions controlled by us and doesn't need to be 
    // fully arbitrary.
    if(c < 0x0080)
        return true;

    // Some scripts are directly tied to a specific language, and if a font
    // supports that script then it almost certainly supports all characters
    // of interest for both that script and the language associated with that
    // script. Examples of such scripts are Hebrew, Thai, Greek.
    // Some scripts work for multiple languages and thus if a font supports
    // such a script, it may not support all the languages for that script.
    // Examples of such scripts are Latin (English, French, Spanish, etc.) and 
    // Han (Japanese, Traditional Chinese, Simplified Chinese).
    switch (script)
    {
        default:
            return mSupportedScriptSet.test((size_t)script);

        case kScriptUnknown:
            if(c == kCharInvalid)
                return true; // In this case the user doesn't care if the char or script is supported.
            break;

        case kScriptHan:
        case kScriptLatin:
        case kScriptCommon:
            // With these scripts, we cannot possibly tell if a given 
            // character is supported based on the script alone.
            break;
    }

    #if EATEXT_USE_FREETYPE
        // FreeType returns 0 for a non-existing glyph. Which is a problem since it doesn't tell you if there was an error.
        return FT_Get_Char_Index(mpFaceData->mFTFace, c) != 0;
    #endif
}


bool OutlineFont::GetKerning(GlyphId g1, GlyphId g2, Kerning& kerning, int direction, bool /*bHorizontalLayout*/)
{
    bool bReturnValue = false;

    #if EATEXT_THREAD_SAFETY_ENABLED
        EA_ASSERT(mpFaceData);
        EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
    #endif

    #if EATEXT_USE_FREETYPE
        EA_ASSERT(mpFaceData->mFTFace);  // If this fails then Open wasn't successfully executed.
    #endif

    #if (EATEXT_KERNING_SUPPORT > 0)
        #if EATEXT_USE_FREETYPE
            if((mpFaceData->mFTFace->face_flags & FT_FACE_FLAG_KERNING) && (mFontMetrics.mPitch != kPitchFixed))
            {
                FT_Error nFTError = FT_Activate_Size(mFTSize);
                EA_ASSERT(nFTError == 0); (void)nFTError;

                FT_Vector ftVector;
                nFTError = FT_Get_Kerning(mpFaceData->mFTFace, g1, g2, FT_KERNING_DEFAULT, &ftVector);

                if(nFTError == 0) // If the kerning exists...
                {
                    // http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
                    // The default value is FT_KERNING_DEFAULT which has value 0. It corresponds to kerning distances expressed in 26.6 grid-fitted pixels 
                    // (which means that the values are multiples of 64). For scalable formats, this means that the design kerning distance is scaled, then rounded.
 
                    kerning.mfKernX = FFFixed26ToFloat(ftVector.x);
                    kerning.mfKernY = FFFixed26ToFloat(ftVector.y);
                    bReturnValue = true;
                }
            }
  
           
        #endif
    #endif

    if(!bReturnValue)
    {
        kerning.mfKernX = 0.f;
        kerning.mfKernY = 0.f;
    }

    return bReturnValue;
}


float OutlineFont::GetSize() const
{
    return mFontDescription.mfSize;
}


bool OutlineFont::SetTransform(float f00, float f01, float f10, float f11)
{
    // It's hard to say if this function should be thread-safe. If two threads call this function
    // at the same time, putting a mutex here doesn't really help the problem of differing transforms.
    #if EATEXT_THREAD_SAFETY_ENABLED
        EA_ASSERT(mpFaceData);
        EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
    #endif

    // We'll need to read the scale out of the transformation matrix.
    // We don't clear the glyph cache on transform change. Probably we should do this.
    EA_ASSERT_MESSAGE(mnRenderCount == 0, "OutlineFont::SetTransform: Font size shouldn't be changed. Doing so is bad because it forces a clearing of the glyph cache.");

    #if EATEXT_USE_FREETYPE
        EA_ASSERT(mpFaceData->mFTFace);

        if(mpFaceData->mFTFace)
        {
            FT_Error nFTError = FT_Activate_Size(mFTSize);
            EA_ASSERT(nFTError == 0); (void)nFTError;

            FT_Matrix ftMatrix;

            ftMatrix.xx = FFFloatToFixed16(f00);
            ftMatrix.xy = FFFloatToFixed16(f01);
            ftMatrix.yx = FFFloatToFixed16(f10);
            ftMatrix.yy = FFFloatToFixed16(f11);

            FT_Set_Transform(mpFaceData->mFTFace, &ftMatrix, NULL);

            // With FreeType, the concept size and transform are 
            // different things. The font transform doesn't specify the font size, it modifies it. 
            // So we don't calculate mFontMetrics here; rather we
            // calculate it in the OutlineFont::SetTransform(float fSize, float fAngle, float fSkew) function.

            return true;
        }
     #endif

    return false;
}


bool OutlineFont::SetTransform(float fSize, float /*fAngle*/, float /*fSkew*/)
{
    #if EATEXT_THREAD_SAFETY_ENABLED
        EA_ASSERT(mpFaceData);
        EA::Thread::AutoFutex autoMutex(mpFaceData->mMutex);
    #endif

    bool bResult = false;

    #if EATEXT_USE_FREETYPE
        FT_Error nFTError;

        if(!mFTSize)
        {
            nFTError = FT_New_Size(mpFaceData->mFTFace, &mFTSize);
            EA_ASSERT(nFTError == 0); (void)nFTError;
        }

        nFTError = FT_Activate_Size(mFTSize);
        EA_ASSERT(nFTError == 0); (void)nFTError;

        nFTError = FT_Set_Char_Size(mpFaceData->mFTFace, 0, FFFloatToFixed26(fSize), mDPI, mDPI);
        EA_ASSERT(nFTError == 0); (void)nFTError;

        // With FreeType, the concept size and transform are different things. 
        // The font transform doesn't specify the font size, it modifies it. 

        // Read FontMetrics
        mFontMetrics.mfSize                   = 0.f; // This is to be set below.
        mFontMetrics.mPitch                   = mFontDescription.mPitch;
        mFontMetrics.mfHAdvanceXMax           = FTFontUnitsToFloat(mpFaceData->mFTFace->max_advance_width);
        mFontMetrics.mfAscent                 = FTFontUnitsToFloat(mpFaceData->mFTFace->ascender);
        mFontMetrics.mfDescent                = FTFontUnitsToFloat(mpFaceData->mFTFace->descender);
        mFontMetrics.mfLeading                = FTFontUnitsToFloat(mpFaceData->mFTFace->height - mpFaceData->mFTFace->ascender + mpFaceData->mFTFace->descender);
        mFontMetrics.mfLineHeight             = FTFontUnitsToFloat(mpFaceData->mFTFace->height);
        mFontMetrics.mfBaseline               = mFontMetrics.mfLineHeight - mFontMetrics.mfLeading + mFontMetrics.mfDescent;
        mFontMetrics.mfXHeight                = mFontMetrics.mfAscent / 2;  // This is incorrect. To do: Find a better way to get the x-height. This information should be stored within the TrueType font.
        mFontMetrics.mfCapsHeight             = mFontMetrics.mfAscent;      // This is not strictly correct. 
        mFontMetrics.mfUnderlinePosition      = FTFontUnitsToFloat(mpFaceData->mFTFace->underline_position);
        mFontMetrics.mfUnderlineThickness     = FTFontUnitsToFloat(mpFaceData->mFTFace->underline_thickness);
        mFontMetrics.mfLinethroughPosition    = mFontMetrics.mfXHeight;
        mFontMetrics.mfLinethroughThickness   = mFontMetrics.mfUnderlineThickness;
        mFontMetrics.mfOverlinePosition       = mFontMetrics.mfCapsHeight + 1 + (mFontMetrics.mfUnderlineThickness / 2);
        mFontMetrics.mfOverlineThickness      = mFontMetrics.mfUnderlineThickness;

        // Assert that the underline position is a somewhat sane value.
        EA_ASSERT((mFontMetrics.mfUnderlinePosition > -mFontMetrics.mfLineHeight) && (mFontMetrics.mfUnderlinePosition <= mFontMetrics.mfLineHeight));

        if(mFontMetrics.mfUnderlineThickness < 1.f)
            mFontMetrics.mfUnderlineThickness = 1.f;

        if(mFontMetrics.mfLinethroughThickness < 1.f)
            mFontMetrics.mfLinethroughThickness = 1.f;

        if(mFontMetrics.mfOverlineThickness < 1.f)
            mFontMetrics.mfOverlineThickness = 1.f;

        if(mpFaceData->mFTFace->max_advance_height > 0)
            mFontMetrics.mfVAdvanceYMax = FTFontUnitsToFloat(mpFaceData->mFTFace->max_advance_height);
        else
            mFontMetrics.mfVAdvanceYMax = mFontMetrics.mfLineHeight;

        // If using custom effects, we delay the metrics calculation until needed,
        // as we need to run the custom effect program in order to know the metrics.
        mbFontMetricsCalculated = (mFontDescription.mEffect == 0);
 
        bResult = true;
    #endif

    if(bResult)
    {
        mFontDescription.mfSize = fSize;
        mFontMetrics.mfSize     = fSize;
    }

    return bResult;
}


void OutlineFont::SetSmoothing(Smooth smooth)
{
    EA_ASSERT_MESSAGE(mnRenderCount == 0, "OutlineFont::SetSmoothing: Should not set smoothing after glyphs have been rendered. Use a different font.");

    // Find a way to assert that we haven't started rendering characters yet.
    // If we have started rendering characters, the result is that glyph metrics might become inaccurate.
    mFontDescription.mSmooth = smooth;
}


void OutlineFont::SetEffect(uint32_t effect, float fParamX, float fParamY, Color BaseColor, Color EffectColor, Color HighlightColor)
{
    mFontDescription.mEffect          = effect;
    mFontDescription.mfEffectX        = fParamX;
    mFontDescription.mfEffectY        = fParamY;
    mFontDescription.mEffectBaseColor = BaseColor;
    mFontDescription.mEffectColor     = EffectColor;
    mFontDescription.mHighLightColor  = HighlightColor;

    #if EATEXT_EFFECTS_ENABLED
        if(effect)
        {
            if(!mpEffectsProcessor)
            {
                mpEffectsProcessor = CORE_NEW(mpCoreAllocator, EATEXT_ALLOC_PREFIX "EffectsProcessor", 0) EffectsProcessor(mpCoreAllocator);
                EA_ASSERT(mpEffectsProcessor);
                mbFontMetricsCalculated = false;
            }
        }
    #endif
}


void OutlineFont::InitEffectsProcessor()
{
    #if EATEXT_EFFECTS_ENABLED
        EA_ASSERT(mpEffectsProcessor);

        mbEffectsInitialized = true;

        // Possibly create an effects processor.
        if(!mpEffectsProcessor)
        {
            mpEffectsProcessor = CORE_NEW(mpCoreAllocator, EATEXT_ALLOC_PREFIX "EffectsProcessor", 0) EffectsProcessor(mpCoreAllocator);
            EA_ASSERT(mpEffectsProcessor);
            mbFontMetricsCalculated = false;
        }

        const EIWord* pInstructionList = NULL;
        uint32_t      instructionCount = 0;
        bool          bResult = false;

        // Provide emulation support for the deprecated enumerated effects types. 
        // It is expected that the new EATextEffects be used instead, as it is more flexible and powerful.
        switch(mFontDescription.mEffect)
        {
            case kEffectOutline:
            {
                EffectsState es;
                es.SetGlyphColor(mFontDescription.mEffectColor);
                es.SetGlyphBrush(kBSRound, 1 + ((int32_t)mFontDescription.mfEffectX * 2), .5, 1.f);
                es.DrawGlyphSmearOutline();
                es.Merge();
                es.Clear();
                es.SetGlyphColor(mFontDescription.mEffectBaseColor);
                es.DrawGlyph();
                es.Merge();
                es.SetGlyphHSpace(1, 0);
                pInstructionList = es.mInstructionList;
                instructionCount = es.mInstructionListSize;
                bResult = true;
                break;
            }

            case kEffectShadow:
            { 
                EffectsState es;
                es.SetGlyphColor(mFontDescription.mEffectColor);
                es.SetGlyphOffset((int32_t)mFontDescription.mfEffectX, (int32_t)mFontDescription.mfEffectY);
                es.DrawGlyph();
                es.ApplyBoxBlur(2, 1.1f, 2);
                es.Merge();
                es.Clear();
                es.SetGlyphColor(mFontDescription.mEffectBaseColor);
                es.SetGlyphOffset(0, 0);
                es.DrawGlyph();
                es.Merge();
                es.SetGlyphHSpace(1, 0);
                pInstructionList = es.mInstructionList;
                instructionCount = es.mInstructionListSize;
                bResult = true;
                break;
            }

            case kEffectRaised:
            case kEffectDepressed:
                // The previous versiosn of these were so lame that nobody used them.
                // So we just ignore them here.
                break;

            default:
                // Effects are identified by id and are registered with the FontServer.
                // Here we retrieve the effect definition from the FontServer and give 
                // it to the EffectProcessor.
                FontServer* const pFontServer = EA::Text::GetFontServer(false);
                EA_ASSERT(pFontServer); // If this fails then most likely you forgot to call SetFontServer with your FontServer.

                bResult = pFontServer->GetEffect(mFontDescription.mEffect, pInstructionList, instructionCount);
                EA_ASSERT(bResult); // If this fails then most likely you forgot to add the effect to the FontServer or you forgot to call SetFontServer with your FontServer.
                break;
        }


        if(bResult)
        {
            EffectsState* const pEffectsState = mpEffectsProcessor->GetEffectsState();
            pEffectsState->AppendInstructionList(pInstructionList, instructionCount);
        }
    #endif
}


bool OutlineFont::RenderGlyphBitmap(const GlyphBitmap** pGlyphBitmap, GlyphId glyphId, uint32_t /*renderFlags*/, float fXFraction, float fYFraction)
{
    #if EATEXT_THREAD_SAFETY_ENABLED
        // We have to hold this lock while the bitmap is outstanding to prevent
        // conflicts on the shared glyph buffer.
        EA_ASSERT(mpFaceData);
        mpFaceData->mMutex.Lock();
    #endif

    #if EATEXT_USE_FREETYPE
        EA_ASSERT(mpFaceData->mFTFace && pGlyphBitmap && (mFontMetrics.mfSize > 0));  // If this fails then Open wasn't successfully executed or SetTransform wasn't successfully executed after Open.
    #endif

   #if EATEXT_USE_FREETYPE
    if(mpFaceData->mFTFace)
   #endif
    {
        if(EA_UNLIKELY(glyphId == kGlyphIdZeroWidth))
        {
            mGlyphBitmap.mnWidth       = 0;
            mGlyphBitmap.mnHeight      = 0;
            mGlyphBitmap.mnStride      = 0;
            #if EATEXT_USE_FREETYPE
            //mGlyphBitmap.mBitmapFormat = kBFGrayscale;
            mGlyphBitmap.mBitmapFormat = (mFontDescription.mSmooth == kSmoothEnabled) ? kBFGrayscale : kBFMonochrome;
            #endif
            mGlyphBitmap.mpData        = 0;
            mGlyphBitmap.mGlyphMetrics = mGlyphMetricsMap[kGlyphIdZeroWidth];

           *pGlyphBitmap = &mGlyphBitmap;

            // Note that we are leaving here with the mutex still locked. 
            // It will be unlocked in DoneGlyphBitmap().
            return true;
        }
        else
        {
            if(mFontDescription.mEffect)
            {
                #if EATEXT_EFFECTS_ENABLED
                    // The first time we try to render an effect, we initialize the EffectsProcessor.
                    if(!mbEffectsInitialized)
                        InitEffectsProcessor();

                    mpEffectsProcessor->Execute(this, glyphId);

                    EffectsState* const pEffectsState = mpEffectsProcessor->GetEffectsState();
                    const GlyphMetrics& glyphMetrics = pEffectsState->mGlyphMetrics;

                    mGlyphBitmap.mGlyphMetrics = glyphMetrics;
                    mGlyphBitmap.mnWidth       = (uint32_t)pEffectsState->mActualGlyphRight  - pEffectsState->mActualGlyphLeft;
                    mGlyphBitmap.mnHeight      = (uint32_t)pEffectsState->mActualGlyphBottom - pEffectsState->mActualGlyphTop;
                    mGlyphBitmap.mnStride      = (uint32_t)pEffectsState->mBaseImage.mnStride;
                    mGlyphBitmap.mpData        = pEffectsState->mBaseImage.GetPixelPtr(pEffectsState->mActualGlyphLeft, pEffectsState->mActualGlyphTop);
                    mGlyphBitmap.mBitmapFormat = kBFARGB; // As of this writing, effects are always done as ARGB and not RGBA or something else.

                    if(mGlyphMetricsMap.find(glyphId) == mGlyphMetricsMap.end()) // If the glyph metrics aren't already cached...
                    {
                        const GlyphMetricsMap::value_type mapEntry(glyphId, glyphMetrics);
                        mGlyphMetricsMap.insert(mapEntry);
                    }

                    *pGlyphBitmap = &mGlyphBitmap;

                    return true;
                #endif
            }
            else
            {
                int8_t xFraction, yFraction; // These are values from -64 to +63; +16 means a quarter pixel adjustment rightward or downward.

                if(fXFraction)
                    xFraction = (int8_t)(fXFraction * 64.f);
                else
                    xFraction = 0;

                if(fYFraction)
                    yFraction = (int8_t)(fYFraction * 64.f);
                else
                    yFraction = 0;

                #if EATEXT_USE_FREETYPE
                    FT_Error nFTError = FT_Activate_Size(mFTSize);
                    EA_ASSERT(nFTError == 0); (void)nFTError;

                    // FreeType is a little confusing about load flags, load target flags and render flags. 
                    // You pass FT_LOAD and FT_LOAD_TARGET flags to FT_Load_Glyph, and you pass 
                    // FT_RENDER_MODE flags to FT_Render_Glyph. Its a little confusing because their 
                    // names overlap in ways that make it easy to get confused about the effect of each.
                    //
                    // For FreeType, we take 'smoothing enabled' to mean hinting disabled. 

                    int32_t loadFlags       = 0;        // Any of FT_LOAD_DEFAULT, FT_LOAD_NO_HINTING, FT_LOAD_NO_BITMAP, FT_LOAD_FORCE_AUTOHINT, FT_LOAD_PEDANTIC.
                    int32_t loadTargetFlags = 0;        // One of FT_LOAD_TARGET_NORMAL, FT_LOAD_TARGET_LCD or FT_LOAD_TARGET_LIGHT. Don't want FT_LOAD_TARGET_MONO, as that targets the hinting algorithm at a true monochrome video screen and is not the same as rendering in mono.

                    if(mbUseAutoHinting)
                        loadFlags |= FT_LOAD_FORCE_AUTOHINT;
                    else if(!mbEnableHinting)
                        loadFlags |= FT_LOAD_NO_HINTING;

                    if(mbLCD)
                        loadTargetFlags = FT_LOAD_TARGET_LCD;
                    else
                        loadTargetFlags = FT_LOAD_TARGET_NORMAL;

                    nFTError = FT_Load_Glyph(mpFaceData->mFTFace, glyphId, loadFlags | loadTargetFlags);
                    EA_ASSERT(nFTError == 0); (void)nFTError;

                    if(nFTError == 0)
                    {
                        // const FT_Render_Mode renderMode = (mbLCD ? FT_RENDER_MODE_LCD : FT_RENDER_MODE_NORMAL); // FT_RENDER_MODE_NORMAL, FT_RENDER_MODE_LIGHT, FT_RENDER_MODE_MONO, FT_RENDER_MODE_LCD
                        const FT_Render_Mode renderMode = ((mFontDescription.mSmooth == kSmoothEnabled) || (mFontDescription.mEffect != kEffectNone)) ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

                        nFTError = FT_Render_Glyph(mpFaceData->mFTFace->glyph, renderMode);
                        EA_ASSERT(nFTError == 0); (void)nFTError;
                    }

                    if(nFTError == 0) // If the render succeeded...
                    {
                        ++mnRenderCount;

                        // Since we only ever have one outstanding render at a time, we store the results
                        // in a member variable. Then we simply pass the address of that member variable 
                        // back to the user.
                        FT_GlyphSlot pGlyphSlot = mpFaceData->mFTFace->glyph;

                        mGlyphBitmap.mnWidth        = (uint32_t)pGlyphSlot->bitmap.width;
                        mGlyphBitmap.mnHeight       = (uint32_t)pGlyphSlot->bitmap.rows;
                        mGlyphBitmap.mnStride       = (uint32_t)pGlyphSlot->bitmap.pitch;
                        //mGlyphBitmap.mBitmapFormat  = kBFGrayscale;
                        mGlyphBitmap.mBitmapFormat  = (mFontDescription.mSmooth == kSmoothEnabled) ? kBFGrayscale : kBFMonochrome;
						switch (static_cast<FT_Pixel_Mode>(pGlyphSlot->bitmap.pixel_mode))
						{
						case FT_PIXEL_MODE_MONO:
							mGlyphBitmap.mBitmapFormat = kBFMonochrome;
							break;
						case FT_PIXEL_MODE_GRAY:
							mGlyphBitmap.mBitmapFormat = kBFGrayscale;
							break;
						case FT_PIXEL_MODE_LCD:
						case FT_PIXEL_MODE_LCD_V:
							mGlyphBitmap.mBitmapFormat = kBFGrayscale;
							break;
						}
                        mGlyphBitmap.mpData         = pGlyphSlot->bitmap.buffer;
                    }
        
                #endif

                GetCurrentGlyphMetrics(mGlyphBitmap.mGlyphMetrics);

                if(mGlyphMetricsMap.find(glyphId) == mGlyphMetricsMap.end()) // If the glyph metrics aren't already cached...
                {
                    const GlyphMetricsMap::value_type mapEntry(glyphId, mGlyphBitmap.mGlyphMetrics);
                    mGlyphMetricsMap.insert(mapEntry);
                }

                *pGlyphBitmap = &mGlyphBitmap;

                #if EATEXT_GLYPH_PIXEL_FIXING_ENABLED
                    // The following letters of the following fonts have a pixel that is 1 when it should 
                    // instead be 0, in kBFMonochrome mode only.
                    //    Arial 8: C K 
                    //    Arial 11: y & 
                    //    Verdana 8: 0 2
                    //    Verdana 9: X 6 8
                    //    Tahoma 8: 0
                    //    Tahoma 9: e 0
                    //    Tahoma 10: e 0
                    //    Microsoft Sans Serif 10: e q 8 p d 
                    //    Trebuchet MS 9: 8 
                    //    Trebuchet MS 11: 8 

                    // if(mGlyphBitmap.mBitmapFormat == kBFMonochrome)
                    // {
                    //     // To do:
                    //     // If this glyph refers to one of the font/char combimations above then 
                    //     // manually fix the glyph by turning off the bad pixel. It would be fastest
                    //     // if this was done via a small pre-calculated data stucture that was a 
                    //     // hash_map of font name + style + char to the pixel index which is broken.
                    // }
                #endif

                // Note that we are leaving here with the mutex still locked. 
                // It will be unlocked in DoneGlyphBitmap().
                return true;
            }
        }
    }

    #if EATEXT_THREAD_SAFETY_ENABLED
        mpFaceData->mMutex.Unlock();
    #endif

    return false;
}


void OutlineFont::DoneGlyphBitmap(const GlyphBitmap* pGlyphBitmap)
{
    #if EATEXT_THREAD_SAFETY_ENABLED
        // Note that the mutex is locked on entry to this function from a previous
        // call to RenderGlyphBitmap().
        EA_ASSERT(mpFaceData && mpFaceData->mMutex.HasLock());
    #endif

    #if EATEXT_USE_FREETYPE
        // FreeType has an FT_Done_Glyph() function, but we don't need to use it unless
        // we are working with a standalone glyph that isn't attached to the face in
        // what FreeType calls the face's "glyph slot." The face automatically manages
        // the memory for this glyph. It might be useful to have a way to force the face
        // to free that memory anyway, such as with the rendering of huge-sized glyphs.

        // Note: if we provide the option to force-free the glyph, we need to make sure 
        // we have thread-safety code here for this function.
    #endif

    #if EATEXT_THREAD_SAFETY_ENABLED
        mpFaceData->mMutex.Unlock();
    #endif
}


OTF* OutlineFont::GetOTF()
{
    #if EATEXT_OPENTYPE_ENABLED
        return &mOTF;
    #else
        return NULL;
    #endif
}




// Undefine values defined earlier.
#undef FFFloatToFixed16
#undef FFFloatToFixed26
#undef FFFixed16ToFloat
#undef FFFixed26ToFloat


} // namespace Text

} // namespace EA











