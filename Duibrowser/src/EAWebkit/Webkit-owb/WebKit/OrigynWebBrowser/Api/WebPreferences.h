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

#ifndef WebPreferences_H
#define WebPreferences_H

#include <wtf/FastAllocBase.h>

#include "BALBase.h"
#include "PlatformString.h"


enum WebKitEditableLinkBehavior
{
    WebKitEditableLinkDefaultBehavior,
    WebKitEditableLinkAlwaysLive,
    WebKitEditableLinkOnlyLiveWithShiftKey,
    WebKitEditableLinkLiveWhenNotFocused,
    WebKitEditableLinkNeverLive
};


enum WebKitCookieStorageAcceptPolicy
{
    WebKitCookieStorageAcceptPolicyAlways = 0,
    WebKitCookieStorageAcceptPolicyNever,
    WebKitCookieStorageAcceptPolicyOnlyFromMainDocumentDomain
};

enum WebCacheModel
{
    WebCacheModelDocumentViewer = 0,
    WebCacheModelDocumentBrowser = 1,
    WebCacheModelPrimaryWebBrowser = 2
};

enum FontSmoothingType
{
    FontSmoothingTypeStandard=0,
    FontSmoothingTypeLight,
    FontSmoothingTypeMedium,
    FontSmoothingTypeStrong,
    FontSmoothingTypeWindows
};


class WebPreferences/*: public WTF::FastAllocBase*/
{
public:
    static WebPreferences* createInstance();

    //+daw ca 22/07 static management
    static void staticFinalize();
    //-daw ca

    virtual ~WebPreferences();

protected:
    WebPreferences();

public:
    // IWebPreferences

    virtual WebPreferences* standardPreferences();
    virtual WebPreferences* initWithIdentifier(WebCore::String anIdentifier);
    virtual WebCore::String identifier();

    virtual WebCore::String standardFontFamily();
    virtual void            setStandardFontFamily(WebCore::String family);
    virtual WebCore::String fixedFontFamily();
    virtual void            setFixedFontFamily(WebCore::String family);
    virtual WebCore::String serifFontFamily();
    virtual void            setSerifFontFamily(WebCore::String family);
    virtual WebCore::String sansSerifFontFamily();
    virtual void            setSansSerifFontFamily(WebCore::String family);
    virtual WebCore::String cursiveFontFamily();
    virtual void            setCursiveFontFamily(WebCore::String family);
    virtual WebCore::String fantasyFontFamily();
    virtual void            setFantasyFontFamily(WebCore::String family);

    virtual int  defaultFontSize();
    virtual void setDefaultFontSize(int fontSize);
    virtual int  defaultFixedFontSize();
    virtual void setDefaultFixedFontSize(int fontSize);
    virtual int  minimumFontSize();
    virtual void setMinimumFontSize(int fontSize);
    virtual int  minimumLogicalFontSize();
    virtual void setMinimumLogicalFontSize(int fontSize);

    virtual WebCore::String defaultTextEncodingName();
    virtual void            setDefaultTextEncodingName(WebCore::String name);

    virtual bool userStyleSheetEnabled();
    virtual void setUserStyleSheetEnabled(bool enabled);

    // URL The location of the user style sheet.
    virtual WebCore::String userStyleSheetLocation();
    virtual void            setUserStyleSheetLocation(WebCore::String location);

    virtual bool isJavaEnabled();
    virtual void setJavaEnabled(bool enabled);

    virtual bool isJavaScriptEnabled();
    virtual void setJavaScriptEnabled(bool enabled);

    virtual bool javaScriptCanOpenWindowsAutomatically();
    virtual void setJavaScriptCanOpenWindowsAutomatically(bool enabled);

	virtual unsigned int javaScriptStackSize();
	virtual void SetJavaScriptStackSize(unsigned int stackSize);

    virtual bool arePlugInsEnabled();
    virtual void setPlugInsEnabled(bool enabled);

    virtual bool allowsAnimatedImages();
    virtual void setAllowsAnimatedImages(bool enabled);

    virtual bool allowAnimatedImageLooping();
    virtual void setAllowAnimatedImageLooping(bool enabled);

    virtual void setLoadsImagesAutomatically(bool enabled);
    virtual bool loadsImagesAutomatically();

    // If autosave preferences is YES the settings represented by
    // WebPreferences will be stored in the user defaults database.
    virtual void setAutosaves(bool enabled);
    virtual bool autosaves();

    virtual void setShouldPrintBackgrounds(bool enabled);
    virtual bool shouldPrintBackgrounds();

    // If private browsing is enabled, WebKit will not store information
    // about sites the user visits.
    virtual void setPrivateBrowsingEnabled(bool enabled);
    virtual bool privateBrowsingEnabled();

    // If tabsToLinks is enabled, the tab key moves to links in addition
    // to form inputs and buttons. If disabled then the tab key just cycles 
    // through form inputs and buttons. 
    // Safari preference dialog: "Press Tab to highlight each item on a web page"
    virtual void setTabsToLinks(bool enabled);
    virtual bool tabsToLinks();

    virtual bool textAreasAreResizable();
    virtual void setTextAreasAreResizable(bool enabled);

    // Sets whether the receiver's associated WebViews use the shared page cache.
    // Pages are cached as they are added to a WebBackForwardList, and
    // removed from the cache as they are removed from a WebBackForwardList.  
    // Because the page cache is global, caching a page in one WebBackForwardList 
    // may cause a page in another WebBackForwardList to be evicted from the cache.
    virtual bool usesPageCache();
    virtual void setUsesPageCache(bool usesPageCache);

    virtual WebCore::String iconDatabaseLocation();
    virtual void            setIconDatabaseLocation(WebCore::String location);

    virtual bool iconDatabaseEnabled();
    virtual void setIconDatabaseEnabled(bool enabled);

    virtual FontSmoothingType fontSmoothing();
    virtual void              setFontSmoothing(FontSmoothingType smoothingType);

    virtual WebKitEditableLinkBehavior editableLinkBehavior();
    virtual void                       setEditableLinkBehavior(WebKitEditableLinkBehavior behavior);

    virtual WebKitCookieStorageAcceptPolicy cookieStorageAcceptPolicy();
    virtual void                            setCookieStorageAcceptPolicy(WebKitCookieStorageAcceptPolicy acceptPolicy);

    virtual bool continuousSpellCheckingEnabled();
    virtual void setContinuousSpellCheckingEnabled(bool enabled);

    virtual bool grammarCheckingEnabled();
    virtual void setGrammarCheckingEnabled(bool enabled);

    virtual bool allowContinuousSpellChecking();
    virtual void setAllowContinuousSpellChecking(bool enabled);

    virtual bool isDOMPasteAllowed();
    virtual void setDOMPasteAllowed(bool enabled);

    virtual WebCacheModel cacheModel();
    virtual void          setCacheModel(WebCacheModel cacheModel);
	
	
    // IWebPreferencesPrivate

    virtual void setDeveloperExtrasEnabled(bool);
    virtual bool developerExtrasEnabled();

    virtual void setAutomaticallyDetectsCacheModel(bool automaticallyDetectsCacheModel);
    virtual bool automaticallyDetectsCacheModel();

    virtual void setAuthorAndUserStylesEnabled(bool enabled);
    virtual bool authorAndUserStylesEnabled();

    // WebPreferences

    // This method accesses a different preference key than developerExtrasEnabled.
    // See <rdar://5343767> for the justification.
    bool developerExtrasDisabledByOverride();

    static WebCore::String webPreferencesChangedNotification();
    static WebCore::String webPreferencesRemovedNotification();

    static void setInstance(WebPreferences* instance, WebCore::String identifier);
    static void removeReferenceForIdentifier(WebCore::String identifier);

    static WebPreferences* sharedStandardPreferences();

    // From WebHistory.h

    int  historyItemLimit();
    void setHistoryItemLimit(int limit);
    int  historyAgeInDaysLimit();
    void setHistoryAgeInDaysLimit(int limit);

    void willAddToWebView();
    void didRemoveFromWebView();

    void postPreferencesChangesNotification();

protected:
    void            setValueForKey(WebCore::String key, WebCore::String value);
    WebCore::String valueForKey(WebCore::String key);
    WebCore::String stringValueForKey(WebCore::String key);
    int             integerValueForKey(WebCore::String key);
    bool            boolValueForKey(WebCore::String key);
    float           floatValueForKey(WebCore::String key);
    unsigned int    longlongValueForKey(WebCore::String key);
    void            setStringValue(WebCore::String key, WebCore::String value);
    void            setIntegerValue(WebCore::String key, int value);
    void            setBoolValue(WebCore::String key, bool value);
    void            setLongLongValue(WebCore::String key, unsigned int value);

    static WebPreferences* getInstanceForIdentifier(WebCore::String identifier);

    static void initializeDefaultSettings();

    void save();
    void load();

protected:
    WebCore::String m_identifier;
    bool            m_autoSaves;
    bool            m_automaticallyDetectsCacheModel;
    unsigned        m_numWebViews;
};

#endif
