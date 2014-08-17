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
// EAWebKitTextWrapper.h
// 6/16/09 CSidhall - Wrapper example for EAText package 
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_UTIL_TEXT_WRAPPER_H
#define EAWEBKIT_UTIL_TEXT_WRAPPER_H

#include <EAWebKit/EAWebKitConfig.h>
#include <EABase/EABase.h>
#include <EAWebKit/EAWebKitTextInterface.h>

// EA Text dependencies
#include <EAText/EATextFontServer.h>   
#include <EAText/EATextFont.h>   
#include <EAText/EATextOutlineFont.h>
#include <EAText/EATextCache.h>
#include <EAText/EATextBreak.h> 

#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif
namespace EA
{
    namespace TextWrapper
    {
        void InitFontSystem();
        void ShutDownFontSystem();

        // Wrapper class for the IFont base class. 
        class FontProxy : public EA::WebKit::IFont
        {
        public:
        
           FontProxy();
           FontProxy(EA::Text::Font* pFont);
           ~FontProxy();

            void SetAllocator(Allocator::ICoreAllocator* pCoreAllocator);
            int AddRef();
            int Release();
            bool GetFontMetrics(EA::WebKit::FontMetrics& fontMetricsInternal);
            bool GetGlyphMetrics(EA::WebKit::GlyphId glyphId, EA::WebKit::GlyphMetrics& glyphMetricsInternal);
            uint32_t GetGlyphIds(const EA::WebKit::Char* pCharArray, uint32_t nCharArrayCount, EA::WebKit::GlyphId* pGlyphIdArray = NULL, 
                bool bUseReplacementGlyph = true, const uint32_t nGlyphIdStride = sizeof(EA::WebKit::GlyphId), bool bWriteInvalidGlyphs = false);

            bool IsCharSupported(EA::WebKit::Char c, EA::WebKit::Script script = EA::WebKit::kScriptUnknown);
            float GetSize() const;
            bool GetKerning(EA::WebKit::GlyphId g1, EA::WebKit::GlyphId g2, EA::WebKit::Kerning& kerning, int direction, bool bHorizontalLayout = true);

            // This is for the casted outline font class
            bool OpenOutline(const void* pSourceData, uint32_t nSourceSize, int nFaceIndex = 0);

            bool DrawGlyphBitmap(EA::WebKit::IGlyphCache* pGlyphCache, EA::WebKit::GlyphId g, EA::WebKit::IGlyphTextureInfo& glyphTextureInfo);

            // Wrapper utilities
            void DestroyWrapper();
            void SetFontPointer(void* pFont);  // Basically the EA::Text::Font when using EAText
            void* GetFontPointer();                     // Uses void so EAText does not need to be included in EAWebKit text interface

        private:
            EA::Text::Font* mpFont;   // Basically the EA::Text::Font proxy
            int mRefCount;  // Needs its own refcount for can be different than the Font ref count because set up outside of EAWebKit
        };

        // Wrapper structure for IFontStyle
        struct FontStyleProxy : EA::WebKit::IFontStyle
        {
            EA::WebKit::Char* GetFamilyNameArray(int index);
            void SetSize(float value);
            void SetStyle(EA::WebKit::Style value);
            void SetWeight(float value);
            void SetVariant(EA::WebKit::Variant value);
            void SetSmooth(EA::WebKit::Smooth value);
            void SetFontStyle(void* pFontStyle);
            void* GetFontStylePointer();
            void Destroy();

        private:
            EA::Text::FontStyle* mpFontStyle;   // = EA::Text::FontStyle proxy when using EAText
        };

        // Wrapper for the IFontServer
        class FontServerProxy : public EA::WebKit::IFontServer
        {
        public:
            FontServerProxy(EA::Text::FontServer* pFontServer = NULL); //If a NULL is passed, if will try to create a default FontServer
            ~FontServerProxy();
			FontProxy* CreateFontProxyWrapper(EA::Text::Font* pFont);

            EA::WebKit::IFont* CreateNewFont(EA::WebKit::FontType fontType);
            EA::WebKit::IFont* GetFont(EA::WebKit::IFontStyle* pTextStyle, EA::WebKit::IFont* pFontArray[] = NULL, uint32_t nFontArrayCapacity = 0, 
                const EA::WebKit::TextEffectData* pEffect = 0, EA::WebKit::Char c = EA::WebKit::kCharInvalid, EA::WebKit::Script script = EA::WebKit::kScriptUnknown, bool bManaged = true);

            uint32_t EnumerateFonts(EA::WebKit::IFontDescription* pFontDescriptionArray, uint32_t nCount);
			uint32_t AddDirectory(const char16_t* pFaceDirectory, const char16_t* pFilter = NULL);
			virtual bool AddSubstitution(const char16_t* pFamily, const char16_t* pFamilySubstitution);
            virtual uint32_t AddFace(IO::IStream* pStream, EA::WebKit::FontType fontType);



            // Font Style interface wrapper
            EA::WebKit::IFontStyle* CreateTextStyle();

            // Access to some general unicode interface functions
            int32_t GetCombiningClass(EA::WebKit::Char c);
            EA::WebKit::Char GetMirrorChar(EA::WebKit::Char c);
            EA::WebKit::CharCategory GetCharCategory(EA::WebKit::Char c);
            EA::WebKit::BidiClass GetBidiClass(EA::WebKit::Char c);
            uint32_t GetFamilyNameArrayCapacity();
            uint32_t GetFamilyNameCapacity();
          
            // Text break iterator access functions
            void* CharacterBreakIterator(EA::WebKit::Char* pText, int length);
            void* WordBreakIterator(EA::WebKit::Char* pText, int length);
            void* LineBreakIterator(EA::WebKit::Char* pText, int length);
            void* SentenceBreakIterator(EA::WebKit::Char* pText, int length);
            int TextBreakFirst(void* pIter);
            int TextBreakNext(void* pIter);
            int TextBreakCurrent(void* pIter);
            int TextBreakPreceding(void* pIter, int position);
            int TextBreakFollowing(void* pIter, int position);
            bool IsTextBreak(void* pIter, int position);
            int FindNextWordFromIndex(EA::WebKit::Char* chars, int len, int position, bool forward);
            void FindWordBoundary(EA::WebKit::Char* chars, int len, int position, int* start, int* end);

        private:
            EA::Text::FontServer* mpFontServer;   // = EA::Text::FontServer proxy when using EAText
			bool mbOwnFontServer;           //true if we create ours
        };       

        // Wrapper for the ITextureInfo
        struct TextureInfoProxy : EA::WebKit::ITextureInfo  
	    {
            int AddRef();
            int Release();
            uint32_t GetSize();
            intptr_t GetStride();
            uint8_t* GetData();
            uint32_t GetFormat();
            void DestroyWrapper();
            void* GetTextureInfoPointer();
            void SetTextureInfoPointer(void* pInfo);

        private:
            EA::Text::TextureInfo* mpInfo;   // = EA::Text::TextureInfo proxy when using EAText
        };

        // Wrapper for the IGlyphCache base class
        class GlyphCacheProxy : public EA::WebKit::IGlyphCache
        {
			friend class FontServerProxy;
        public:
			GlyphCacheProxy(EA::Text::GlyphCache* pGlypheCache = NULL);//If a NULL is passes, we'll use EAText to create a GlyphCache
            ~GlyphCacheProxy();
            EA::WebKit::ITextureInfo* GetTextureInfo(uint32_t nTextureIndex);
            bool GetGlyphTextureInfo(EA::WebKit::IFont* pFont, EA::WebKit::GlyphId glyphId,EA::WebKit::IGlyphTextureInfo& glyphTextureInfo); 
            bool AddGlyphTexture(EA::WebKit::IFont* pFont, EA::WebKit::GlyphId glyphId, const void* pSourceData, uint32_t nSourceSizeX, 
                                         uint32_t nSourceSizeY, uint32_t nSourceStride, uint32_t nSourceFormat,
                                         EA::WebKit::IGlyphTextureInfo& glyphTextureInfo);

            bool EndUpdate(EA::WebKit::ITextureInfo* pTextureInfo);
            static TextureInfoProxy* CreateTextureInfoProxy(EA::Text::TextureInfo* pInfo);
            void* GetGlyphCachePointer() {return mpGlyphCache;}    

        private:
            EA::Text::GlyphCache* mpGlyphCache; // = EA::Text::GlyphCache proxy when using EAText
			bool mbOwnGlyphCache;//true if we create ours

       };

    } // Namespace TextWrapper

} // Namespace EA
#endif // Header include guard

