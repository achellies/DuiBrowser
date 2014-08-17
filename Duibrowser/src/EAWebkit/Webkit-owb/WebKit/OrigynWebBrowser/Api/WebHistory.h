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

#ifndef WebHistory_H
#define WebHistory_H

#include <wtf/FastAllocBase.h>

namespace WebCore {
    class KURL;
    class PageGroup;
    class String;
}

class WebPreferences;
class WebHistoryItem;
class WebError;

class WebHistory/*: public WTF::FastAllocBase*/ {
public:

    /**
     *  createInstance creates an Instance of WebHistory
     * @param[in]: 
     * @param[out]: WebHistory
     */
    static WebHistory* createInstance();
private:

    /**
     *  WebHistory construtor
     * @param[in]: 
     * @param[out]: 
     */
    WebHistory();

public:

    /**
     *  ~WebHistory destructor
     * @param[in]: 
     * @param[out]: 
     */
    virtual ~WebHistory();

    /**
       @method optionalSharedHistory
       @abstract Returns a shared WebHistory instance initialized with the default history file.
       @result A WebHistory object.
     */
    virtual WebHistory* optionalSharedHistory();

    /**
       @method loadFromURL:error:
        @param URL The URL to use to initialize the WebHistory.
        @param error Set to nil or an NSError instance if an error occurred.
        @abstract The designated initializer for WebHistory.
        @result Returns YES if successful, NO otherwise.
     */
    virtual void setOptionalSharedHistory(WebHistory* history);

    /**
        @method saveToURL:error:
        @discussion Save history to URL. It is the client's responsibility to call this at appropriate times.
        @param URL The URL to use to save the WebHistory.
        @param error Set to nil or an NSError instance if an error occurred.
        @result Returns YES if successful, NO otherwise.

     */
    virtual WebError* loadFromURL(WebCore::String url);

    /**
        @method saveToURL:error:
        @discussion Save history to URL. It is the client's responsibility to call this at appropriate times.
        @param URL The URL to use to save the WebHistory.
        @param error Set to nil or an NSError instance if an error occurred.
        @result Returns YES if successful, NO otherwise.

     */
    virtual WebError* saveToURL(WebCore::String url);

    /**
     @method addItems:
     @param newItems An array of WebHistoryItems to add to the WebHistory.
     */
    virtual void addItems(int itemCount, WebHistoryItem** items);

    /**
        @method removeItems:
        @param items An array of WebHistoryItems to remove from the WebHistory.

     */
    virtual void removeItems(int itemCount, WebHistoryItem** items);

    /**
        @method orderedLastVisitedDays
        @discussion Get an array of NSCalendarDates, each one representing a unique day that contains one
        or more history items, ordered from most recent to oldest.
        @result Returns an array of NSCalendarDates for which history items exist in the WebHistory.
        
        ADVISORY NOTE:  This method may change for the 1.0 SDK.

     */
    virtual void removeAllItems();


    /**
        @method itemForURL:
        @abstract Get an item for a specific URL
        @param URL The URL of the history item to search for
        @result Returns an item matching the URL

     */
    virtual WebHistoryItem* itemForURL(WebCore::String url);

    /**
      @method setHistoryItemLimit:
        @discussion Limits the number of items that will be stored by the WebHistory.
        @param limit The maximum number of items that will be stored by the WebHistory.

     */
    virtual void setHistoryItemLimit(int limit);

    /**
        @method setHistoryAgeInDaysLimit:
        @discussion setHistoryAgeInDaysLimit: sets the maximum number of days to be read from
        stored history.
        @param limit The maximum number of days to be read from stored history.

     */
    virtual int historyItemLimit();

    /**
        @method setHistoryAgeInDaysLimit:
        @discussion setHistoryAgeInDaysLimit: sets the maximum number of days to be read from
        stored history.
        @param limit The maximum number of days to be read from stored history.
     */
    virtual void setHistoryAgeInDaysLimit(int limit);

    /**
        @method historyAgeInDaysLimit
        @return Returns the maximum number of days to be read from stored history.

     */
    virtual int historyAgeInDaysLimit();


    /**
     *  sharedHistory return a reference to WebHistory
     * @param[in]: 
     * @param[out]:  WebHistory*
     */
    static WebHistory* sharedHistory();

    /**
       @method addItem adds an Item to History
       @param[in]: URL and Name to save
     */
    void addItem(const WebCore::KURL&, const WebCore::String&);

    /**
     * @method adds a PageGroup to history
     */
    void addVisitedLinksToPageGroup(WebCore::PageGroup&);

private:
    enum NotificationType
    {
        kWebHistoryItemsAddedNotification = 0,
        kWebHistoryItemsRemovedNotification = 1,
        kWebHistoryAllItemsRemovedNotification = 2,
        kWebHistoryLoadedNotification = 3,
        kWebHistoryItemsDiscardedWhileLoadingNotification = 4,
        kWebHistorySavedNotification = 5
    };

    WebPreferences *m_preferences;
//+daw ca 29/07/2008 static and global management
private:
    static WebHistory* m_st_pSharedHistory;
public:
    static void staticFinalize();
//-daw ca
};

#endif
