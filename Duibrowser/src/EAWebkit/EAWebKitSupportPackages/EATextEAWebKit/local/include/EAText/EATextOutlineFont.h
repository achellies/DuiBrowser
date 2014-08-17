/*
Copyright (C) 2004,2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EATextOutlineFont.h
//
// By Paul Pedriana - 2004
//
///////////////////////////////////////////////////////////////////////////////


#ifndef EATEXT_EATEXTOUTLINEFONT_H
#define EATEXT_EATEXTOUTLINEFONT_H


#include <EABase/EABase.h>
#include <EAText/EAText.h>
#include <EAText/EATextFont.h>
#include <EAText/internal/EATextOpenType.h>

#if EATEXT_USE_FREETYPE
    #include <ft2build.h>
    #include FT_FREETYPE_H  // #include <freetype/freetype.h>
    #include FT_SIZES_H     // #include <freetype/ftsizes.h>
#endif


namespace EA
{
    namespace Allocator
    {
        class ICoreAllocator;
    }

    namespace IO
    {
        class IStream;
    }

    namespace Text
    {
        class EffectsProcessor;


        /// FaceData
        ///
        /// All fonts of the same face share some data between themselves.
        /// Thus arial.ttf has some data that is shared between Arial 10, Arial 12, etc.
        /// This sharing of face data can save upwards of 200 K per font for larger fonts.
        ///
        class FaceData
        {
        public:
            FaceData(Allocator::ICoreAllocator* pCoreAllocator);
           ~FaceData();

            int AddRef();
            int Release();

        public:
            #if EATEXT_USE_FREETYPE
                FT_Face       mFTFace;              // Represents the font Face (e.g. Arial bold), which can be drawn at multiple sizes.
                FT_StreamRec_ mFTStreamRec;         // Used for providing custom IO for the Face.
             #endif

            #if EATEXT_THREAD_SAFETY_ENABLED
                EA::Thread::Futex mMutex;
            #endif

        protected:
            FaceData(const FaceData&);
            FaceData& operator=(const FaceData&);

            Allocator::ICoreAllocator* mpCoreAllocator;

            #if EATEXT_THREAD_SAFETY_ENABLED
                EA::Thread::AtomicInt32 mRefCount;
            #else
                int mRefCount;
            #endif
        };


        /// OutlineFont
        ///
        /// A font is an incarnation of a typeface. This Font class represents not just a typeface in the 
        /// conceptual sense but represents a font file itself. Thus Font is an interface to a (e.g. TrueType)
        /// font and has functionality needed to load that font and support the reading of glyphs from 
        /// that font. Given that third party libraries exist which do the work of navigating TrueType font
        /// file records and do the work of converting outlines to bitmaps, a TrueType implementation of the
        /// Font class is relatively simple and small. 
        ///
        /// It is intended that a user of this class would implement glyph bitmap caching. This class doesn't 
        /// implement such caching itself, as that is outside the scope of responsibility for this class. 
        /// The format of a bitmap cache is likely to be specific to the texture formats in use with the 
        /// rasterizing software and hardware. Also, caching algorithms may be specific to the given application
        /// at hand. Note also that what is said here about bitmap caching applies equally to outline caching.
        ///
        /// You will note that there is a distinction between characters and glyph ids that is made in the API.
        /// This is both proper and important. Fonts don't have characters, they have glyphs. In most cases, 
        /// there is a 1:1 mapping of characters to glyphs, but this is not always so. A font has one or more
        /// mappings from characters to glyphs, though some glyphs (e.g. ligatures) are not mapped from any
        /// character. It is also more efficient to work in terms of glyphs instead of characters, as any API
        /// call that works with a character would have to convert it to a glyph in order to do its task.
        ///
        /// The only supported character mapping is UCS Unicode. Thus all Chars must be UCS Unicode encoded and 
        /// multibyte encodings such as Chinese Big5, Windows MBCS, and UTF8 Unicode are not recognized. You can
        /// convert multibyte encodings to UCS Unicode with utility functions if needed.
        ///
        /// Example usage:
        ///    OutlineFont* pFont = new OutlineFont;
        ///    if(pFont->Open(ReadFunction, SeekFunction, pContext)){
        ///       pFont->SetTransform(16);
        ///       GlyphBitmap* pGlyphBitmap;
        ///       if(pFont->RenderGlyphBitmap(&pGlyphBitmap, 'a', kRFUnicodeId)){
        ///          // Do something with the bitmap.
        ///          pFont->DoneGlyphBitmap(pGlyphBitmap);
        ///       }
        ///       pFont->Close();
        ///    }
        ///    delete pFont;
        ///
        class OutlineFont : public Font
        {
        public:
            /// OutlineFont
            ///
            /// Constructor. The OutlineFont constructor merely initializes member data to zero. No significant
            /// operations are done. 
            ///
            OutlineFont(Allocator::ICoreAllocator* pCoreAllocator = NULL);
            OutlineFont(const OutlineFont&);
            OutlineFont& operator=(const OutlineFont&);

            EATEXT_VIRTUAL ~OutlineFont();

            EATEXT_VIRTUAL void         SetAllocator(Allocator::ICoreAllocator* pCoreAllocator);
            EATEXT_VIRTUAL BitmapFormat GetBitmapFormat() const;
            EATEXT_VIRTUAL FontType     GetFontType();
            EATEXT_VIRTUAL bool         GetFontDescription(FontDescription& fontDescription);
            EATEXT_VIRTUAL bool         GetFontMetrics(FontMetrics& fontMetrics);
            EATEXT_VIRTUAL bool         GetGlyphMetrics(GlyphId glyphId, GlyphMetrics& glyphMetrics);
            EATEXT_VIRTUAL uint32_t     GetGlyphIds(const Char* pCharArray, uint32_t nCharArrayCount, GlyphId* pGlyphIdArray = NULL, 
                                                    bool bUseReplacementGlyph = true, const uint32_t nGlyphIdStride = sizeof(GlyphId), bool bWriteInvalidGlyphs = false);
            EATEXT_VIRTUAL uint32_t     GetGlyphIds(const char8_t* pCharArray, uint32_t nCharArrayCount, GlyphId* pGlyphIdArray = NULL, 
                                                    bool bUseReplacementGlyph = true, const uint32_t nGlyphIdStride = sizeof(GlyphId), bool bWriteInvalidGlyphs = false);
            EATEXT_VIRTUAL bool  IsCharSupported(Char c, Script script);
            EATEXT_VIRTUAL bool  GetKerning(GlyphId g1, GlyphId g2, Kerning& kerning, int direction, bool bHorizontalLayout = true);
            EATEXT_VIRTUAL float GetSize() const;
            EATEXT_VIRTUAL bool  SetTransform(float f00, float f01, float f10, float f11);
            EATEXT_VIRTUAL bool  SetTransform(float fSize, float fAngle = 0.f, float fSkew = 0.f);
            EATEXT_VIRTUAL bool  RenderGlyphBitmap(const GlyphBitmap** pGlyphBitmap, GlyphId g, uint32_t renderFlags = kRFDefault, 
                                                    float fXFraction = 0, float fYFraction = 0);
            EATEXT_VIRTUAL void  DoneGlyphBitmap(const GlyphBitmap* pGlyphBitmap);
            EATEXT_VIRTUAL OTF*  GetOTF();

            /// CreateFaceData
            ///
            /// Utility function for setting up a shared FaceData struct.
            /// Either pStream or pSourceData must be non-NULL, and the non-NULL one
            /// of these will be used.
            /// The returned FaceData is AddRef'd for the caller.
            ///
            static FaceData* CreateFaceData(Allocator::ICoreAllocator* pAllocator, IO::IStream* pStream, 
                                                                const void* pSourceData, uint32_t nSourceSize, int nFaceIndex);

            /// SetFontFFaceData
            /// 
            /// Sets the FaceData to be used in a subsequent call to Open.
            /// Cannot be called when the font is open. If you don't call this function
            /// before Open, this class will create a private instance. Upon Close,  
            /// the existing FaceData will be set to NULL and you must call 
            /// SetFontFFaceData again before Open.
            /// This function AddRefs the FaceData, and the Close function
            /// Releases the FaceData.
            ///
            void SetFontFFaceData(FaceData* pFontFFaceData);


            /// Open
            /// 
            /// Initializes a OutlineFont for usage with data supplied by the stream functions.
            /// Returns true if the data source could be read and the OutlineFont could be initialized for usage.
            /// The Stream is AddRefd by the font for its usage and is Released upon completion of usage.
            ///
            /// The input nFaceIndex refers to the 0-based index of a particular font face within a font file.
            /// Some font formats (e.g. TrueType) allow for multiple fonts to be stored in a single TrueType
            /// font file. The face index allows you to refer to a specific font within the file. If the given
            /// index is not present, the return value is false. You can count the faces in a font file by 
            /// simply calling Open with progressively increasing face indexes until the return value is false.
            ///
            EATEXT_VIRTUAL bool Open(IO::IStream* pStream, int nFaceIndex = 0);


            /// Open
            ///
            /// Initializes a OutlineFont from a complete font memory image. The memory is read by the OutlineFont but
            /// is not written to. The source data is not freed by the OutlineFont when usage is complete.
            ///
            EATEXT_VIRTUAL bool Open(const void* pSourceData, uint32_t nSourceSize, int nFaceIndex = 0);


            /// Close
            ///
            /// Closes the font and frees any resources allocated by it after being opened.
            /// The OutlineFont is returned to the same essential state that it was before being Opened.
            /// The font must be opened or the return value will be false.
            /// If the font is not closed before it is destroyed, it will be closed upon its final Release.
            ///
            EATEXT_VIRTUAL bool Close();


            /// SetOption
            ///
            /// kOptionOpenTypeFeatures must be set before the font is opened.
            /// kOptionDPI must be set before the font is opened.
            /// See enum Font::Option for more details about available options.
            ///
            EATEXT_VIRTUAL void SetOption(int32_t option, int32_t value);


            /// SetSmoothing
            ///
            /// Should not be called after the OutlineFont is first used to render glyphs. Otherwise the glyph
            /// metrics for this font may become inaccurate and glyphs will draw imperfectly.
            /// The default value is kSmoothNone.
            ///
            EATEXT_VIRTUAL void SetSmoothing(Smooth smooth);


            /// SetEffect
            ///
            /// This function must be called before the font is opened.
            /// To set the effect outline, shadow, raised, depressed
            /// The value of ParamX and ParamY depends on which effect is desired.
            /// In case of outline, it is the thickness of the outline in pixels
            /// In case of shadow, it the offset value, and the ratio will be important
            /// All parameters other than the effect parameter are deprecated, as the 
            /// new effects system makes these obsolete. These parameters are ignored
            /// by the new effects system
            ///
            EATEXT_VIRTUAL void SetEffect(uint32_t effect, float fParamX = 0, float fParamY = 0, Color BaseColor = 0, Color EffectColor = 0, Color HighlightColor = 0);


            // Possibly add functions to control font stretching outside the user-specified size/transform.
            // The idea is that you can control the nominal font stretching without the user having to specify it.

        protected:
            friend class FontServer;
            friend class EffectsProcessor;

        protected:
            bool     OpenInternal(int nFaceIndex);
            void     GetCurrentGlyphMetrics(GlyphMetrics& glyphMetrics);
            void     InitEffectsProcessor();

        protected:
            FontDescription     mFontDescription;
            FontMetrics         mFontMetrics;
            GlyphMetricsMap     mGlyphMetricsMap;
            GlyphBitmap         mGlyphBitmap;
            uint16_t            mnRenderCount;
            int16_t             mDPI;
            bool                mbEffectsInitialized;
            bool                mbFontMetricsCalculated;
            bool                mbEnableHinting;
            bool                mbUseAutoHinting;
            bool                mbLCD;
            EffectsProcessor*   mpEffectsProcessor;
            FaceData*           mpFaceData;

            #if EATEXT_OPENTYPE_ENABLED
                bool      mbOTFEnabled;     // If true then we read OTF information when opening the font.
                OTF       mOTF;             // To consider: Make mOTF be allocated via our memory allocator instead of being a fixed member. Some fonts may not have nor need OTF information.
            #endif

            #if EATEXT_USE_FREETYPE
                FT_Size        mFTSize;
            #endif

        }; // class OutlineFont

    } // namespace Text

} // namespace EA



#endif // EATEXT_EATEXTOUTLINEFONT_H

















