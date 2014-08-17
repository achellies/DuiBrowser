/*
 * Copyright (C) 2006, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Nicholas Shanks <webkit@nickshanks.com>
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
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
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

#include "config.h"
#include "FontCache.h"

#include "Font.h"
#include "FontFallbackList.h"
#include "FontPlatformData.h"
#include "FontSelector.h"
#include "StringHash.h"
#include <wtf/HashMap.h>
#include <wtf/ListHashSet.h>
#include <EAWebKit/EAWebKitTextInterface.h>
#include <EAWebKit/EAWebKitView.h>

using namespace WTF;

namespace WKAL {
#include <wtf/FastAllocBase.h>

struct FontPlatformDataCacheKey/*: public WTF::FastAllocBase*/ {
    FontPlatformDataCacheKey(const AtomicString& family = AtomicString(), unsigned size = 0, unsigned weight = 0, bool italic = false,
        bool isPrinterFont = false, FontRenderingMode renderingMode = NormalRenderingMode, const EA::WebKit::TextEffectData* pEffectData = 0  )
        : m_family(family)
        , m_size(size)
        , m_weight(weight)
        , m_italic(italic)
        , m_printerFont(isPrinterFont)
        , m_renderingMode(renderingMode)
        , m_effectData()
    {
        if(pEffectData)
        {
            m_effectData.type = pEffectData->type;
            m_effectData.x = pEffectData->x;
            m_effectData.y = pEffectData->y;
            m_effectData.blur = pEffectData->blur;
            m_effectData.c = pEffectData->c;
            m_effectData.cBase = pEffectData->cBase;
        }
    }

    FontPlatformDataCacheKey(HashTableDeletedValueType) : m_size(hashTableDeletedSize()) { }
    bool isHashTableDeletedValue() const { return m_size == hashTableDeletedSize(); }

    bool operator==(const FontPlatformDataCacheKey& other) const
    {
        return equalIgnoringCase(m_family, other.m_family) && m_size == other.m_size && 
               m_weight == other.m_weight && m_italic == other.m_italic && m_printerFont == other.m_printerFont &&
               m_renderingMode == other.m_renderingMode && m_effectData == other.m_effectData;
    }

    AtomicString m_family;
    unsigned m_size;
    unsigned m_weight;
    bool m_italic;
    bool m_printerFont;
    FontRenderingMode m_renderingMode;
    EA::WebKit::TextEffectData m_effectData;
private:
    static unsigned hashTableDeletedSize() { return 0xFFFFFFFFU; }
};

inline unsigned computeHash(const FontPlatformDataCacheKey& fontKey)
{
    unsigned hashCodes[] = {
        CaseFoldingHash::hash(fontKey.m_family),
        fontKey.m_size,
        fontKey.m_weight,
        static_cast<unsigned>(fontKey.m_italic) << 2 | static_cast<unsigned>(fontKey.m_printerFont) << 1 | static_cast<unsigned>(fontKey.m_renderingMode),
        static_cast<unsigned> (fontKey.m_effectData.type),
        static_cast<unsigned> (fontKey.m_effectData.x),
        static_cast<unsigned> (fontKey.m_effectData.y), 
        fontKey.m_effectData.c,                         // (We can ignore tracking the blur since not used currently)
        fontKey.m_effectData.cBase
    };
    return StringImpl::computeHash(reinterpret_cast<UChar*>(hashCodes), sizeof(hashCodes) / sizeof(UChar));
}

struct FontPlatformDataCacheKeyHash/*: public WTF::FastAllocBase*/ {
    static unsigned hash(const FontPlatformDataCacheKey& font)
    {
        return computeHash(font);
    }
         
    static bool equal(const FontPlatformDataCacheKey& a, const FontPlatformDataCacheKey& b)
    {
        return a == b;
    }

    static const bool safeToCompareToEmptyOrDeleted = true;
};

struct FontPlatformDataCacheKeyTraits : WTF::GenericHashTraits<FontPlatformDataCacheKey> {
    static const bool emptyValueIsZero = true;
    static const FontPlatformDataCacheKey& emptyValue()
    {
        static FontPlatformDataCacheKey key(nullAtom);
        return key;
    }
    static void constructDeletedValue(FontPlatformDataCacheKey* slot)
    {
        new (slot) FontPlatformDataCacheKey(HashTableDeletedValue);
    }
    static bool isDeletedValue(const FontPlatformDataCacheKey& value)
    {
        return value.isHashTableDeletedValue();
    }
};

typedef HashMap<FontPlatformDataCacheKey, FontPlatformData*, FontPlatformDataCacheKeyHash, FontPlatformDataCacheKeyTraits> FontPlatformDataCache;

static FontPlatformDataCache* gFontPlatformDataCache = 0;

//+daw ca 29/07/2008 static and global management
AtomicString* FontCache::m_st_pCourier = NULL;
AtomicString* FontCache::m_st_pCourierNew = NULL;
AtomicString* FontCache::m_st_pTimes = NULL;
AtomicString* FontCache::m_st_pTimesNewRoman = NULL;
AtomicString* FontCache::m_st_pArial = NULL;
AtomicString* FontCache::m_st_pHelvetica = NULL;
const WebCore::AtomicString* FontCache::alternateFamilyName(const AtomicString& familyName)
{
    if (!m_st_pCourier) {
        m_st_pCourier = new AtomicString("Courier");
        m_st_pCourierNew = new AtomicString("Courier New");
        m_st_pTimes = new AtomicString("Times");
        m_st_pTimesNewRoman = new AtomicString("Times New Roman");
        m_st_pArial = new AtomicString("Arial");
        m_st_pHelvetica = new AtomicString("Helvetica");
    }


    // Alias Courier <-> Courier New
    if (equalIgnoringCase(familyName, *m_st_pCourier))
        return m_st_pCourierNew;
    if (equalIgnoringCase(familyName, *m_st_pCourierNew))
        return m_st_pCourier;

    // Alias Times and Times New Roman.
    if (equalIgnoringCase(familyName, *m_st_pTimes))
        return m_st_pTimesNewRoman;
    if (equalIgnoringCase(familyName, *m_st_pTimesNewRoman))
        return m_st_pTimes;

    // Alias Arial and Helvetica
    if (equalIgnoringCase(familyName, *m_st_pArial))
        return m_st_pHelvetica;
    if (equalIgnoringCase(familyName, *m_st_pHelvetica))
        return m_st_pArial;

    return &emptyAtom;
//-daw ca
}

FontPlatformData* FontCache::getCachedFontPlatformData(const FontDescription& fontDescription, 
                                                       const AtomicString& familyName,
                                                       bool checkingAlternateName)
{
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeFontLoading, EA::WebKit::kVProcessStatusStarted);
	    
    if (!gFontPlatformDataCache) {
        gFontPlatformDataCache = new FontPlatformDataCache;
        platformInit();
    }

    FontPlatformDataCacheKey key(familyName, fontDescription.computedPixelSize(), fontDescription.weight(), fontDescription.italic(),
                                 fontDescription.usePrinterFont(), fontDescription.renderingMode(), &fontDescription.getTextEffectData());
    FontPlatformData* result = 0;
    bool foundResult;
    FontPlatformDataCache::iterator it = gFontPlatformDataCache->find(key);
    if (it == gFontPlatformDataCache->end()) {
        result = createFontPlatformData(fontDescription, familyName);
        if(result) 
            gFontPlatformDataCache->set(key, result);
        foundResult = result;
    } else {
        result = it->second;
        foundResult = true;
    }

    if (!foundResult && !checkingAlternateName) {
        // We were unable to find a font.  We have a small set of fonts that we alias to other names, 
        // e.g., Arial/Helvetica, Courier/Courier New, etc.  Try looking up the font under the aliased name.
        const AtomicString* pAlternateName = alternateFamilyName(familyName);
        if (!pAlternateName->isEmpty())
            result = getCachedFontPlatformData(fontDescription, *pAlternateName, true);
        if (result) { 
            gFontPlatformDataCache->set(key, new FontPlatformData(*result)); // Cache the result under the old name.
        }
    }

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeFontLoading, EA::WebKit::kVProcessStatusEnded);
    return result;
}

struct FontDataCacheKeyHash/*: public WTF::FastAllocBase*/ {
    static unsigned hash(const FontPlatformData& platformData)
    {
        return platformData.hash();
    }
         
    static bool equal(const FontPlatformData& a, const FontPlatformData& b)
    {
        return a == b;
    }

    static const bool safeToCompareToEmptyOrDeleted = true;
};

struct FontDataCacheKeyTraits : WTF::GenericHashTraits<FontPlatformData> {
    static const bool emptyValueIsZero = true;
    static const bool needsDestruction = false;
    static const FontPlatformData& emptyValue()
    {
        static FontPlatformData key;
        return key;
    }
    static void constructDeletedValue(FontPlatformData* slot)
    {
        new (slot) FontPlatformData(HashTableDeletedValue);
    }
    static bool isDeletedValue(const FontPlatformData& value)
    {
        return value.isHashTableDeletedValue();
    }
};

typedef HashMap<FontPlatformData, pair<SimpleFontData*, unsigned>, FontDataCacheKeyHash, FontDataCacheKeyTraits> FontDataCache;

static FontDataCache* gFontDataCache = 0;

const int cMaxInactiveFontData = 120;  // Pretty Low Threshold
const float cInactiveFontDataPurgeRatio = 0.2f;

class InactiveFontDataHashSet : public ListHashSet<const SimpleFontData*>
{
private:
    typedef ListHashSet<const SimpleFontData*> BaseClass;
public:
    virtual ~InactiveFontDataHashSet();
    void clear();
};

InactiveFontDataHashSet::~InactiveFontDataHashSet()
{
    clear();
}

void InactiveFontDataHashSet::clear()
{
    BaseClass::iterator iter;
    for (iter = begin(); iter != end(); ++iter) {
        const SimpleFontData* fontData = *iter.get();
        delete fontData;
    }

    BaseClass::clear();
}


static InactiveFontDataHashSet* gInactiveFontData = 0;

SimpleFontData* FontCache::getCachedFontData(const FontPlatformData* platformData)
{
    if (!platformData)
        return 0;

    if (!gFontDataCache) {
        gFontDataCache = new FontDataCache;
        gInactiveFontData = new InactiveFontDataHashSet;
    }
    
    FontDataCache::iterator result = gFontDataCache->find(*platformData);
    if (result == gFontDataCache->end()) {
        if (gInactiveFontData->size() > cMaxInactiveFontData)
            purgeInactiveFontData((int)(cMaxInactiveFontData * cInactiveFontDataPurgeRatio));

        pair<SimpleFontData*, unsigned> newValue(new SimpleFontData(*platformData), 1);

        // 6/15/09 CSidhall  -Moved the add ref into the constructor of SimpleFontData 

        gFontDataCache->set(*platformData, newValue);
        return newValue.first;
    }
    if (!result.get()->second.second++) {
        if(gInactiveFontData->contains(result.get()->second.first))
            gInactiveFontData->remove(result.get()->second.first);
    }

    return result.get()->second.first;
}

void FontCache::releaseFontData(const SimpleFontData* fontData)
{
    ASSERT(gFontDataCache);
    ASSERT(!fontData->isCustomFont());

    if(!gFontDataCache)
        return;
    FontDataCache::iterator it = gFontDataCache->find(static_cast<const SimpleFontData*>(fontData)->platformData());
    ASSERT(it != gFontDataCache->end());

    if (!--it->second.second)
        gInactiveFontData->add(static_cast<const SimpleFontData*>(fontData));
}

void FontCache::purgeInactiveFontData(int count)
{
    if (!gInactiveFontData)
        return;

    ListHashSet<const SimpleFontData*>::iterator end = gInactiveFontData->end();
    ListHashSet<const SimpleFontData*>::iterator it = gInactiveFontData->begin();
    for (int i = 0; i < count && it != end; ++it, ++i) {
        const SimpleFontData* fontData = *it.get();
        gFontDataCache->remove(fontData->platformData());
        delete fontData;
    }

    if (it == end) {
        // Removed everything
        gInactiveFontData->clear();
    } else {
        for (int i = 0; i < count; ++i)
            gInactiveFontData->remove(gInactiveFontData->begin());
    }
}

size_t FontCache::fontDataCount()
{
    if (gFontDataCache)
        return gFontDataCache->size();
    return 0;
}

size_t FontCache::inactiveFontDataCount()
{
    if (gInactiveFontData)
        return gInactiveFontData->size();
    return 0;
}

const FontData* FontCache::getFontData(const Font& font, int& familyIndex, FontSelector* fontSelector)
{
    FontPlatformData* result = 0;

    int startIndex = familyIndex;
    const FontFamily* startFamily = &font.fontDescription().family();
    for (int i = 0; startFamily && i < startIndex; i++)
        startFamily = startFamily->next();
    const FontFamily* currFamily = startFamily;
    while (currFamily && !result) {
        familyIndex++;
        if (currFamily->family().length()) {
            if (fontSelector) {
                FontData* data = fontSelector->getFontData(font.fontDescription(), currFamily->family());
                if (data)
                    return data;
            }
            result = getCachedFontPlatformData(font.fontDescription(), currFamily->family());
        }
        currFamily = currFamily->next();
    }

    if (!currFamily)
        familyIndex = cAllFamiliesScanned;

    if (!result) {
        // We didn't find a font. Try to find a similar font using our own specific knowledge about our platform.
        // For example on OS X, we know to map any families containing the words Arabic, Pashto, or Urdu to the
        // Geeza Pro font.

        //+ 4/27/09 CSidhall - The old code created a new fontPlatfromData here but left the pointer to leak.
        // We will use the exising font cache to store it in so that we can collect it.
        // There might be some risk if fontPlatformData gets added from another location with the same
        // key value.  This would overwrite the mapped value, causing a leak.  Have not seen this to occur in
        // however while testing several sites.
        // Original code:
        // result = getSimilarFontPlatformData(font);
        
        // New code:
        // Verify that we don't have one already in the cache (the familyIndex might have skipped this step)
        const FontDescription& fontDescription = font.fontDescription(); 
        // Use first font family as default name for the key
        const AtomicString& familyName = font.fontDescription().family().family();  
       
        result = getCachedFontPlatformData(fontDescription, familyName);
        if(!result) {
            // No default font was found so should be safe to add to key location
            result = getSimilarFontPlatformData(font);

            FontPlatformDataCacheKey key(familyName, fontDescription.computedPixelSize(), fontDescription.weight(), fontDescription.italic(),
                                 fontDescription.usePrinterFont(), fontDescription.renderingMode(),&fontDescription.getTextEffectData());
            gFontPlatformDataCache->set(key,result);
        }
        //- CS 
    }

    if (!result && startIndex == 0) {
        // If it's the primary font that we couldn't find, we try the following. In all other cases, we will
        // just use per-character system fallback.

        if (fontSelector) {
            // Try the user's preferred standard font.
            if (FontData* data = fontSelector->getFontData(font.fontDescription(), "-webkit-standard"))
                return data;
        }

        // Still no result.  Hand back our last resort fallback font.
        result = getLastResortFallbackFont(font.fontDescription());
    }

    // Now that we have a result, we need to go from FontPlatformData -> FontData.
    return getCachedFontData(result);
}




//+daw ca 24/07 static and global management
void FontCache::staticFinalize()
{
    if(gFontDataCache) {
        // Disabled because gInactiveFontData is dependent on this leaking.
        // for(FontDataCache::iterator it(gFontDataCache->begin()), itEnd = gFontDataCache->end(); it != itEnd; ++it)
        // {
        //     FontDataCache::ValueType& entry = *it;
        // 
        //     if(entry.second.first)
        //     {
        //         delete entry.second.first;
        //         entry.second.first = NULL;
        //     }
        // }

        // Disabled because gInactiveFontData is dependent on this leaking.
        // purgeInactiveFontData(1000000); // Purge all possible entries.

        // 5/13/10 Chris Sidhall - Added to fix a memory leak with a CSS FontFace source if the font is not already cached and needs to create one.
        // (Probably only occurs if you exit before the font makes it to the inactive list...).
        for(FontDataCache::iterator it(gFontDataCache->begin()), itEnd = gFontDataCache->end(); it != itEnd; ++it) {
            FontDataCache::ValueType& entry = *it;
            if(entry.second.first) {
               SimpleFontData* pSimpleFontData = entry.second.first;
               // Only delete if inactive is not using it for inactive will clean up it's own.
               if(!gInactiveFontData->contains(pSimpleFontData)) {                    
                   delete pSimpleFontData;
                   entry.second.first = NULL;
               }
            }
         }

	    delete gFontDataCache;
	    gFontDataCache = NULL;
    }

	if (gInactiveFontData) {
        // Note by Paul Pedriana (1/2009): gInactiveFontData is a container which is smart 
        // enough to auto-free its member data in its destructor.
		delete gInactiveFontData;
		gInactiveFontData = NULL;
	}

	if(gFontPlatformDataCache) {
        for(FontPlatformDataCache::iterator it(gFontPlatformDataCache->begin()), itEnd = gFontPlatformDataCache->end(); it != itEnd; ++it) {
            FontPlatformDataCache::ValueType& entry = *it;
        
            if(entry.second) {
                delete entry.second;
                entry.second = NULL;
            }
        }

        delete gFontPlatformDataCache;
	    gFontPlatformDataCache = NULL;
    }

	if (m_st_pCourier) {
		m_st_pCourier->externalDeref();
		delete m_st_pCourier;
		m_st_pCourier = NULL;

		m_st_pCourierNew->externalDeref();
		delete m_st_pCourierNew;
		m_st_pCourierNew = NULL;

		m_st_pTimes->externalDeref();
		delete m_st_pTimes;
		m_st_pTimes = NULL;

		m_st_pTimesNewRoman->externalDeref();
		delete m_st_pTimesNewRoman;
		m_st_pTimesNewRoman = NULL;

		m_st_pArial->externalDeref();
		delete m_st_pArial;
		m_st_pArial = NULL;

		m_st_pHelvetica->externalDeref();
		delete m_st_pHelvetica;
		m_st_pHelvetica = NULL;
	}
}
//-daw ca


} // namespace WebCore
