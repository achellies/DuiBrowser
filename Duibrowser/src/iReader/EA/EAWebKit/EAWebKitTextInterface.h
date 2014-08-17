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

// 5/20/09 CSidhall - Abstract Interface Wrapper for EAText package 
// Note: Enums and some struct have been copied from EAText which was written 
// by Paul Pedriana
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_TEXT_INTERFACE_H
#define EAWEBKIT_TEXT_INTERFACE_H

#include <EABase/EABase.h>
#include <CoreAllocator/icoreallocator_interface.h>
#include <string.h>     // For memset
#include <EAIO/EAFileStream.h>

namespace EA
{
    namespace WebKit
    {
        ///--- Exposed constants and Enums from EAText ---
        typedef char16_t Char;
        typedef uint16_t GlyphId;
        typedef uint32_t Color;
        
        const int kCharInvalid = 0xFFFF;  
        const GlyphId kGlyphIdInvalid = 0xffff;
        const uint32_t kFamilyNameCapacity      = 32;

        enum Pitch
        {
            kPitchVariable, /// Variable width of characters (e.g. Arial).
            kPitchFixed,    /// Fixed width of characters (e.g. Courier).
            kPitchDefault = kPitchVariable
        };
        
        enum Script
        {                                 // ISO 15924 code (four letter codes; see http://www.unicode.org/reports/tr24/)
            kScriptUnknown = -1,          //
            kScriptCommon = 0,            // Zyyy  // Special reserved code for characters that are used in multiple scripts.
            kScriptInherited,             // Qaai  // Special reserved code for script inherited by the environment.

            kScriptStart,                 //
            kScriptArabic = kScriptStart, // Arab
            kScriptArmenian,              // Armn
            kScriptBengali,               // Beng
            kScriptBraille,               // Brai
            kScriptBopomofo,              // Bopo                           // Mandarin Chinese
            kScriptBuhid,                 // Buhd
            kScriptCanadianAboriginal,    // Cans
            kScriptCherokee,              // Cher
            kScriptCoptic,                // Qaac
            kScriptCypriot,               // Cprt
            kScriptCyrillic,              // Cyrl (Cyrs)
            kScriptDeseret,               // Dsrt
            kScriptDevanagari,            // Deva
            kScriptEthiopic,              // Ethi
            kScriptGeorgian,              // Geor (Geon, Geoa)
            kScriptGothic,                // Goth
            kScriptGreek,                 // Grek
            kScriptGujarati,              // Gujr
            kScriptGurmukhi,              // Guru
            kScriptHan,                   // Hani                           // Chinese and Japanese symbols (Kanji)
            kScriptHangul,                // Hang                           // Korean symbols
            kScriptHanunoo,               // Hano
            kScriptHebrew,                // Hebr
            kScriptHiragana,              // Hira                           // Japanese syllabary
            kScriptKannada,               // Knda
            kScriptKatakana,              // Kana
            kScriptKhmer,                 // Khmr
            kScriptLao,                   // Laoo
            kScriptLatin,                 // Latn (Latf, Latg)              // English, Spanish, German French, etc.
            kScriptLimbu,                 // Limb
            kScriptLinearB,               // Linb
            kScriptMalayalam,             // Mlym
            kScriptMongolian,             // Mong
            kScriptMyanmar,               // Mymr
            kScriptOgham,                 // Ogam
            kScriptOldItalic,             // Ital
            kScriptOriya,                 // Orya
            kScriptOsmanya,               // Osma
            kScriptRunic,                 // Runr
            kScriptShavian,               // Shaw
            kScriptSinhala,               // Sinh
            kScriptSyriac,                // Syrc (Syrj, Syrn, Syre)
            kScriptTagalog,               // Tglg
            kScriptTagbanwa,              // Tagb
            kScriptTaiLe,                 // Tale
            kScriptTamil,                 // Taml
            kScriptTelugu,                // Telu
            kScriptThaana,                // Thaa
            kScriptThai,                  // Thai
            kScriptTibetan,               // Tibt
            kScriptUgaritic,              // Ugar
            kScriptYi,                    // Yiii
            kScriptEnd = kScriptYi
        };
       
        enum Weight
        {
            kWeightLightest = 100,
            kWeightNormal   = 400,  /// The CSS standard specifies 400 as normal.
            kWeightBold     = 700,  /// The CSS standard specifies 700 as bold.
            kWeightHeaviest = 900,
            kWeightDefault  = kWeightNormal
        };
  
        struct GlyphMetrics
        {
            float mfSizeX;            /// The width of the glyph.
            float mfSizeY;            /// The height of the glyph.
            float mfHBearingX;        /// The distance from the origin ("pen position") to the left of the glyph. This usually is a value >= 0. May be negative for glyphs that are diacritics which modify the character before them.
            float mfHBearingY;        /// The distance from the origin to the top of the glyph. This is usually a value >= 0. May be negative for glyphs that are entirely below the baseline (horizontal text).
            float mfHAdvanceX;        /// The distance from the origin to the origin of the next glyph. This is usually a value > 0.

            #if EATEXT_VERTICAL_ENABLED
                float mfVBearingX;    /// X bearing for when text is layed out vertically.
                float mfVBearingY;    /// Y bearing for when text is layed out vertically.
                float mfVAdvanceY;    /// Advance for when text is layed out vertically.
            #endif
 
        };
        
        struct FontMetrics
        {
            float mfSize;                   /// The classic font size metric; what you see in the text editor 'size' box.
            Pitch mPitch;                   /// Specifies if the font is of variable or fixed pitch (i.e. monospaced).
            float mfHAdvanceXMax;           /// For fixed pitch fonts, this is the advance for all non-zero-advance glyphs.
            float mfVAdvanceYMax;           /// For fixed pitch fonts, this is the advance for all non-zero-advance glyphs. May be equal to mfLineHeight for fonts without vertical metrics.
            float mfAscent;                 /// Max ascent value for the font. This will normally be >= 0. Covers the distance from the baseline to the top of the highest lower-case glyph (usually 'b').
            float mfDescent;                /// Min descent value for the font. This will normally be <= 0. Covers the distance from the baseline to the bottom of the lowest glyph. Bottoms of characters such as j, p, and q would be in this area.
            float mfLeading;                /// The extra space between the bottom of the descent and the top of the next line of text below the current one. Leading is often zero.
            float mfBaseline;               /// The distance from the top of a line down to the baseline. Equal to mfAscent + mfExternalLeading.
            float mfLineHeight;             /// The height of a line of text. Equal to mfDescent + mfBaseline. Also equal to mfDescent + mfAscent + mfExternalLeading.
            float mfXHeight;                /// The height of lower-case Latin glyphs (usually 'x').
            float mfCapsHeight;             /// The height of top of upper-case Latin glyphs (usually 'H').
            float mfUnderlinePosition;      /// The position is relative to baseline; negative values mean below the baseline. Refers to the center of the line. For a thickness of two, position is the top pixel.
            float mfUnderlineThickness;     /// Thickness of the line. 
            float mfLinethroughPosition;    /// The position is relative to baseline. Refers to the center of the line. For a thickness of two, position is the top pixel.
            float mfLinethroughThickness;   /// Thickness of the line. 
            float mfOverlinePosition;       /// The position is relative to baseline. Refers to the center of the line. For a thickness of two, position is the top pixel.
            float mfOverlineThickness;      /// Thickness of the line.

            FontMetrics() { memset(this, 0, sizeof(*this)); }
        };
        
        enum FontType
        {
            kFontTypeUnknown,   /// The font type is not known.
            kFontTypeBitmap,    /// The glyphs are derived from pregenerated bitmaps.
            kFontTypeOutline,   /// The glyphs are derived from outlines (such as Bezier). TrueType, PostScript, and OpenType fonts are outline fonts, though they allow for embedding of bitmaps. 
            kFontTypeStroke,    /// The glyphs are derived from strokes. This is common for East Asian kanji.
            kFontTypePolygon,   /// The glyphs are derived from polygons.
        };

        enum CharCategory
        {
            kCCUnassigned             =  0, /// Non-category for unassigned and non-character code points.
            kCCGeneralOtherTypes      =  0, /// Cn other, not assigned. Same as kCCUnassigned. Note that no characters in UnicodeData.txt have this property. 
            kCCControlChar            =  1, /// Cc
            kCCFormatChar             =  2, /// Cf
            kCCPrivateUseChar         =  3, /// Co
            kCCSurrogate              =  4, /// Cs
            kCCUppercaseLetter        =  5, /// Lu
            kCCLowercaseLetter        =  6, /// Ll
            kCCTitlecaseLetter        =  7, /// Lt
            kCCModifierLetter         =  8, /// Lm
            kCCOtherLetter            =  9, /// Lo
            kCCNonSpacingMark         = 10, /// Mn
            kCCEnclosingMark          = 11, /// Me
            kCCCombiningSpacingMark   = 12, /// Mc
            kCCDecimalDigitNumber     = 13, /// Nd
            kCCLetterNumber           = 14, /// Nl
            kCCOtherNumber            = 15, /// No
            kCCSpaceSeparator         = 16, /// Zs
            kCCLineSeparator          = 17, /// Zl
            kCCParagraphSeparator     = 18, /// Zp
            kCCDashPunctuation        = 19, /// Pd
            kCCStartPunctuation       = 20, /// Ps
            kCCEndPunctuation         = 21, /// Pe
            kCCConnectorPunctuation   = 22, /// Pc
            kCCOtherPunctuation       = 23, /// Po
            kCCInitialPunctuation     = 24, /// Pi
            kCCFinalPunctuation       = 25, /// Pf
            kCCMathSuymbol            = 26, /// Sm
            kCCCurrencySymbol         = 27, /// Sc
            kCCModifierSymbol         = 28, /// Sk
            kCCOtherSymbol            = 29  /// So
        };

        enum BidiClass
        {
            kBidiClassON = 0,   /// Other Neutrals. Includes all other characters, including OBJECT REPLACEMENT CHARACTER.
            kBidiClassL,        /// Left-to-Right. Includes LRM, Most alphabetic, syllabic, Han ideographic characters, digits that are neither European nor Arabic, ...
            kBidiClassR,        /// Right-to-Left. Includes RLM, Hebrew alphabet, most punctuation specific to that script, ...
            kBidiClassAN,       /// Arabic Number. Includes Arabic-Indic digits, Arabic decimal & thousands separators, ...
            kBidiClassEN,       /// European Number. Includes European digits, Eastern Arabic-Indic digits, ...
            kBidiClassAL,       /// Right-to-Left Arabic. Includes Arabic, Thaana, and Syriac alphabets, most punctuation specific to those scripts, ...
            kBidiClassNSM,      /// Non-Spacing Mark. Includes characters marked Mn (Non-Spacing Mark) and Me (Enclosing Mark) in the Unicode Character Database.
            kBidiClassCS,       /// Common Number Separator. Includes colon, comma, full stop (period), non-breaking space, ...
            kBidiClassES,       /// European Number Separator. Includes plus sign, minus sign.
            kBidiClassET,       /// European Number Terminator. Includes degree, currency symbols, ...
            kBidiClassBN,       /// Boundary Neutral. Includes most formatting and control characters, other than those explicitly given types above.
            kBidiClassS,        /// Segment Separator. Includes tab.
            kBidiClassWS,       /// Whitespace. Includes space, figure space, line separator, form feed, general punctuation spaces, ...
            kBidiClassB,        /// Paragraph Separator. Includes paragraph separator, appropriate newline functions, higher-protocol paragraph determination.
            kBidiClassRLO,      /// Right-to-Left Override.
            kBidiClassRLE,      /// Right-to-Left Embedding.
            kBidiClassLRO,      /// Left-to-Right Override.
            kBidiClassLRE,      /// Left-to-Right Embedding.
            kBidiClassPDF,      /// Pop Directional Format.
        };

        enum Style
        {
            kStyleNormal,  /// Normal characters, no slanting.
            kStyleOblique, /// Normal characters that are merely slanted. 
            kStyleItalic,  /// Modified characters (e.g. calligraphically) with slanting.
            kStyleDefault = kStyleNormal
        };
        
        enum Variant
        {
            kVariantNormal,
            kVariantSmallCaps,
            kVariantDefault = kVariantNormal
        };

        enum Smooth
        {
            kSmoothNone,                    /// Monochrome font with no anti-aliasing. Works fine for well-hinted fonts.
            kSmoothEnabled,                 /// Full anti-aliasing. This is slower because it requires destination reads and concomitant alpha blends.
            kSmoothDefault = kSmoothNone    /// Whatever the default is.
        };

        enum Effect
        {
            kEffectNone,
            kEffectOutline,
            kEffectShadow,
            kEffectRaised,
            kEffectDepressed,
            kEffectDefault = kEffectNone,
            kEffectUser = 256
        };

        // For storing text effect attributes
        struct TextEffectData
        {
            Effect  type;
            int   x;            // Multi use. Can bue used as an x offset param
            int   y;            // Multi use.  Can be used for a y offset param
            int   blur;         // Passed but not used
            Color c;            // Effect color
            Color cBase;        // Pen color
            
            TextEffectData() : type(kEffectNone), x(0), y(0), blur(0), c(0), cBase(0) {}

            bool operator==(const TextEffectData& other) const
            {
               return type == other.type && x == other.x && y == other.y
                  && blur == other.blur && c == other.c && cBase == other.cBase;
            }
        };
        
        
        enum RenderFlags
        {
            kRFNone       = 0,     /// No flag in particular.
            kRFUnicodeId  = 1,     /// The glyphId is to be interpreted as a Unicode character instead of a glyphId. This is slower but OK for testing purposes.
            kRFDefault    = kRFNone
        };

        enum BitmapFormat
        {
            kBFNone       =  0,     /// No format in particular.
            kBFMonochrome =  1,     /// 1 bit per pixel, no compression aside from the bits packed in the bytes.
            kBFGrayscale  =  8,     /// 8 bits per pixel, no compression. Each pixel represents opacity.
            kBFARGB       = 32,     /// 32 bits per pixel, no compression. Format is 32 bit 0xaarrggbb.
            kBFRGBA       = 33      /// 32 bits per pixel, no compression. Format is 32 bit 0xrrggbbaa.
        };

         ///--- Exposed structures from EAText ---
        struct IFontDescription
        {
            Char      mFamily[kFamilyNameCapacity]; /// Family name to which the font belongs. For example, Arial.
            float     mfSize;                       /// Font size. If size is not applicable, the value is 0.
            Style     mStyle;                       /// CSS Style type. For example, kStyleOblique.
            float     mfWeight;                     /// CSS Weight type. For example, kWeightBold.
            float     mfStretch;                    /// Stretch scaling value. 1.0 means neither stretched nor condensed.
            Pitch     mPitch;                       /// Pitch type. Tells if font is variable or fixed pitch. ** Perhaps this should be part of the family instead of the font.
            Variant   mVariant;                     /// CSS Variant type. For example, kVariantSmallCaps.
            Smooth    mSmooth;                      /// CSS Smooth type. For example, kSmoothEnabled.
            uint32_t  mEffect;                      /// To support outline, shadow, raised and depressed 
            float     mfEffectX;
            float     mfEffectY;
            Color     mEffectBaseColor;             /// Color of the text, this needs be here because we are rendering color glyphs
            Color     mEffectColor;                 /// If mEffect is outline, this is outline color, if effect is shadow, this shadow color
            Color     mHighLightColor;              /// used for shadow, rasied and depressed effect

            IFontDescription()
              : mfSize(0.f), mStyle(kStyleNormal), mfWeight(kWeightNormal), mfStretch(1.f), 
                mPitch(kPitchVariable), mVariant(kVariantNormal), mSmooth(kSmoothNone), mEffect(kEffectNone),
                mfEffectX(1.f), mfEffectY(1.f), mEffectBaseColor(0xffffffff), mEffectColor(0xff000000), mHighLightColor(0xffffffff) { mFamily[0] = 0; }

            bool operator==(const IFontDescription& fd) const;
        };

        struct GlyphBitmap
        {
            uint32_t      mnWidth;          /// Width of the physical glyph, uint32_t version of mGlyphMetrics.mfSizeX. Not the same as advance width. 
            uint32_t      mnHeight;         /// Height of the physical glypyh, uint32_t version of mGlyphMetrics.mfSizeY.
            GlyphMetrics  mGlyphMetrics;    /// GlyphMetrics; includes glyph size, bearing, advance.
            const void*   mpData;           /// The physical glyph representation. The data format depends on the Font type and glyph format. For OutlineFont and BmpFont, this is a bitmap. For PolygonFont, this is a PolygonGlyph (or custom user representation).
            BitmapFormat  mBitmapFormat;    /// Glyph representation format.
            uint32_t      mnStride;         /// For bitmapped glyphs, this is the bitmap stride in bytes.
        };

         struct Kerning
        {
            float mfKernX;
            float mfKernY;
        };

        ///--- Abstract bases classes and structures ---
         
        class IFont;
        struct IFontStyle;
                
        /// FontServer Interface base
        class IFontServer
        {
        public:
            virtual ~IFontServer() {}
 
            virtual IFont* CreateNewFont(EA::WebKit::FontType fontType) = 0;

            virtual IFont* GetFont(IFontStyle* pTextStyle, IFont* pFontArray[] = NULL, uint32_t nFontArrayCapacity = 0, 
                    const TextEffectData* pEffect = 0, Char c = kCharInvalid, Script script = kScriptUnknown, bool bManaged = true) = 0;

            virtual uint32_t EnumerateFonts(IFontDescription* pFontDescriptionArray, uint32_t nCount) = 0;
			virtual uint32_t AddDirectory(const char16_t* pFaceDirectory, const char16_t* pFilter = NULL) = 0;
			virtual bool AddSubstitution(const char16_t* pFamily, const char16_t* pFamilySubstitution) = 0;
            virtual uint32_t AddFace(IO::IStream* pStream, FontType fontType) = 0;

            
            // We also use the font server pointer as a way to access other Text functions.
            // Heare are various font related interfaces
            virtual int32_t GetCombiningClass(Char c) = 0;
            virtual Char GetMirrorChar(Char c) = 0;
            virtual CharCategory GetCharCategory(Char c) = 0;
            virtual BidiClass GetBidiClass(Char c) = 0;
            virtual uint32_t GetFamilyNameArrayCapacity() = 0;
            virtual uint32_t GetFamilyNameCapacity() = 0;
            
            virtual IFontStyle* CreateTextStyle() = 0;
 
            // Text break iterator
            virtual void* CharacterBreakIterator(Char* pText, int length) = 0;
            virtual void* WordBreakIterator(Char* pText, int length) = 0;
            virtual void* LineBreakIterator(Char* pText, int length) = 0;
            virtual void* SentenceBreakIterator(Char* pText, int length) = 0;
            virtual int TextBreakFirst(void* pIterator) = 0;
            virtual int TextBreakNext(void* pIterator) = 0;
            virtual int TextBreakCurrent(void* pIterator) = 0;
            virtual int TextBreakPreceding(void* pIterator, int position) = 0;
            virtual int TextBreakFollowing(void* pIterator, int position) = 0;
            virtual bool IsTextBreak(void* pIterator, int position) = 0;
            virtual int FindNextWordFromIndex(Char* chars, int len, int position, bool forward) = 0;
            virtual void FindWordBoundary(Char* chars, int len, int position, int* start, int* end) =0;
        }; 
        
        /// Font abstract base class
        class IGlyphCache;
        struct IGlyphTextureInfo;
        class IFont
        {
        public:
            virtual ~IFont() {}

            virtual void SetAllocator(EA::Allocator::ICoreAllocator* pCoreAllocator) = 0;

            virtual int AddRef() = 0;

            virtual int Release() = 0;
       
            virtual bool GetFontMetrics(FontMetrics& fontMetrics) = 0;
 
            virtual bool GetGlyphMetrics(GlyphId glyphId, GlyphMetrics& glyphMetrics) = 0;

            virtual uint32_t GetGlyphIds(const Char* pCharArray, uint32_t nCharArrayCount, GlyphId* pGlyphIdArray = NULL, 
                                                bool bUseReplacementGlyph = true, const uint32_t nGlyphIdStride = sizeof(GlyphId), bool bWriteInvalidGlyphs = false) = 0;

            virtual bool IsCharSupported(Char c, Script script = kScriptUnknown) = 0;

            virtual float GetSize() const = 0;

            virtual bool GetKerning(GlyphId g1, GlyphId g2, Kerning& kerning, int direction, bool bHorizontalLayout = true) = 0;

            virtual void* GetFontPointer() =0;

            virtual void DestroyWrapper() = 0;

            // Special for outline font to avoid extra wrapper class
            virtual bool OpenOutline(const void* pSourceData, uint32_t nSourceSize, int nFaceIndex = 0) = 0;

            // Extracted part of the BCFontEA.cpp draw 
            virtual bool DrawGlyphBitmap(IGlyphCache* pGlyphCache, GlyphId g, IGlyphTextureInfo& glyphTextureInfo) = 0;

        }; // Font

        struct IFontStyle
        {
            virtual~IFontStyle() {}
            virtual Char* GetFamilyNameArray(int index) = 0;
            virtual void SetSize(float value) = 0;
            virtual void SetStyle(Style value) = 0;
            virtual void SetWeight(float value) = 0;
            virtual void SetVariant(Variant value) = 0;
            virtual void SetSmooth(Smooth value) = 0;
            virtual void Destroy() = 0;
            virtual void* GetFontStylePointer() = 0;
        };
        
	    struct ITextureInfo
	    {
            virtual ~ITextureInfo() {}
            virtual int AddRef() = 0;
            virtual int Release() = 0;	
            virtual uint32_t GetSize() = 0;
            virtual intptr_t GetStride() = 0;
            virtual uint8_t* GetData() = 0;
            virtual uint32_t GetFormat() =0;            // Get the bitmap format (32 or 8)
            virtual void* GetTextureInfoPointer() = 0;
            virtual void SetTextureInfoPointer(void* p) = 0;
            virtual void DestroyWrapper() = 0;
        };

        struct IGlyphTextureInfo
        {
            ITextureInfo* mpTextureInfo; /// The texture this glyph belongs to.
            float        mX1;           /// Position of glyph on texture. These are UV positions and so are between 0 and 1.
            float        mY1;           /// Position of glyph on texture.
            float        mX2;           /// Position of glyph on texture.
            float        mY2;           /// Position of glyph on texture.
        
            IGlyphTextureInfo() {mpTextureInfo = 0; mX1=mY1=mX2=mY2=0.0f;}
        };	

        /// Glyph base class       
        class IGlyphCache
        {
        public:
    		virtual ~IGlyphCache(){}				
		
	    	virtual ITextureInfo* GetTextureInfo(uint32_t nTextureIndex) = 0;

            virtual bool GetGlyphTextureInfo(IFont* pFont, GlyphId glyphId,IGlyphTextureInfo& glyphTextureInfo) = 0;

            virtual bool AddGlyphTexture(IFont* pFont, GlyphId glyphId, const void* pSourceData, uint32_t nSourceSizeX, 
                                         uint32_t nSourceSizeY, uint32_t nSourceStride, uint32_t nSourceFormat,
                                         IGlyphTextureInfo& glyphTextureInfo) = 0;

		    virtual bool EndUpdate(ITextureInfo* pTextureInfo) = 0;

            virtual void* GetGlyphCachePointer() =0;
        };
    } // Namespace Internal
} // Namespace EA
#endif // Header include guard

