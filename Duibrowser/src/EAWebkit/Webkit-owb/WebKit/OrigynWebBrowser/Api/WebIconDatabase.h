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

#ifndef WebIconDatabase_H
#define WebIconDatabase_H
#if ENABLE(ICON_DATABASE)


/**
 *  @file  WebIconDatabase.h
 *  WebIconDatabase description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <IconDatabaseClient.h>
#include <IconDatabaseClient.h>
#include <IntSize.h>
#include <IntSizeHash.h>

#include <wtf/Vector.h>
#include <wtf/Threading.h>

namespace WebCore
{
    class IconDatabase;
    class String;
    class Image;
    class IntSize;
}; //namespace WebCore

using namespace WTF;

#define WebIconDatabaseDidAddIconNotification "WebIconDatabaseDidAddIconNotification"
#define WebIconNotificationUserInfoURLKey "WebIconNotificationUserInfoURLKey"
#define WebIconDatabaseDidRemoveAllIconsNotification "WebIconDatabaseDidRemoveAllIconsNotification"

class WebIconDatabase : public WebCore::IconDatabaseClient
{
public:

    /**
     *  createInstance creates an instance
     * @param[in]: 
     * @param[out]: WebIconDatabase*
     */
    static WebIconDatabase* createInstance();

    /**
     *  sharedWebIconDatabase share reference to WebIconDatabase
     * @param[in]: 
     * @param[out]: WebIconDatabase* 
     * @code
     * @endcode
     */
    static WebIconDatabase* sharedWebIconDatabase();
private:

    /**
     *  WebIconDatabase constructor
     * @param[in]: 
     * @param[out]: 
     */
    WebIconDatabase();

    /**
     *  init initialised database for icons
     * @param[in]: 
     * @param[out]: 
     */
    void init();
public:

    /**
     *  ~WebIconDatabase destructor
     * @param[in]: 
     * @param[out]: 
     */
    virtual ~WebIconDatabase();

    /**
     *  *sharedIconDatabase returns an instance of the icon database
     * @param[in]: 
     * @param[out]: WebIconDatabase instance
     */
    virtual WebIconDatabase *sharedIconDatabase();

    /**
        @method iconURLForURL:withSize:cache:
        @discussion Returns an icon URL for a web site URL from memory or disk. nil if none is found.
        @param URL
     */
    virtual WebCore::Image *iconForURL(WebCore::String url, WebCore::IntSize size, bool cache);

    /**
     @method retainIconForURL:
        @abstract Increments the retain count of the icon.
        @param URL
     */
    virtual void retainIconForURL(WebCore::String url);

    /**
    @method releaseIconForURL:
        @abstract Decrements the retain count of the icon.
        @param URL
     */
    virtual void releaseIconForURL(WebCore::String url);

    /**
       @method removeAllIcons:
        @abstract EmpTies the Icon Database

     */
    virtual void removeAllIcons();

    /**
       @method delayDatabaseCleanup:
        @discussion Only effective if called before the database begins removing icons.
        delayDatabaseCleanUp increments an internal counter that when 0 begins the database clean-up.
        The counter equals 0 at initialization.

     */
    virtual void delayDatabaseCleanup();

    /**
        @method allowDatabaseCleanup:
        @discussion Informs the database that it now can begin removing icons.
        allowDatabaseCleanup decrements an internal counter that when 0 begins the database clean-up.
        The counter equals 0 at initialization.

     */
    virtual void allowDatabaseCleanup();

    /**
       @method iconURLForURL:withSize:cache:
        @discussion Returns an icon URL for a web site URL from memory or disk. nil if none is found.
        @param URL

     */
    virtual WebCore::String iconURLForURL(WebCore::String url);


    /**
     * dispatchDidRemoveAllIcons removes all icons
     * @param[in]: 
     * @param[out]: 
     */
    virtual void dispatchDidRemoveAllIcons();

    /**
     *  dispatchDidAddIconForPageURL add an icon for a page
     * @param[in]: page name
     * @param[out]: 
     */
    virtual void dispatchDidAddIconForPageURL(const WebCore::String&);


    /**
     *  iconDatabaseDidAddIconNotification notifies of Icon addition
     * @param[in]:  
     * @param[out]: WebCore::String for which icon has been added
     */
    static WebCore::String iconDatabaseDidAddIconNotification();

    /**
     *  iconDatabaseDidRemoveAllIconsNotification notify of icon removal
     * @param[in]: 
     * @param[out]: 
     */
    static WebCore::String iconDatabaseDidRemoveAllIconsNotification();

    /**
     *  iconDatabaseNotificationUserInfoURLKey defines a topic for observer 
     */
    static WebCore::String iconDatabaseNotificationUserInfoURLKey();
protected:
    static WebIconDatabase* m_sharedWebIconDatabase;

    // Keep a set of HBITMAPs around for the default icon, and another
    // to share amongst present site icons

    /**
     *  getOrCreateSharedBitmap creates WebCore::Image to store Bitmap
     * @param[in]: size
     * @param[out]: WebCore::Image*
     * @code
     * @endcode
     */
    WebCore::Image* getOrCreateSharedBitmap(IntSize size);

    /**
     *  getOrCreateDefaultIconBitmap : create (or get) a share bitmap of size to create image
     * @param[in]: size
     * @param[out]: WebCore::Image*
     */
    WebCore::Image* getOrCreateDefaultIconBitmap(IntSize size);
    HashMap<IntSize, WebCore::Image*> m_defaultIconMap;
    HashMap<IntSize, WebCore::Image*> m_sharedIconMap;

    Mutex m_notificationMutex;
    Vector<WebCore::String> m_notificationQueue;

    /**
     *  scheduleNotificationDelivery prepare notification
     */
    void scheduleNotificationDelivery();
    bool m_deliveryRequested;


    /**
     *  deliverNotifications deliver notifications
     */
    static void deliverNotifications(void*);
};
#endif
#endif
