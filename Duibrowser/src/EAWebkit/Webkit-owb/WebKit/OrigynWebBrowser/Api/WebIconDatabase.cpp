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

// #if ENABLE(ICON_DATABASE)
#if (defined( ICON_DATABASE ) && ICON_DATABASE)

#error

#include "config.h"
#include "WebIconDatabase.h"

#include "WebPreferences.h"

#include <PlatformString.h>
#include <FileSystem.h>
#include <IconDatabase.h>
#include <Image.h>
#include <PlatformString.h>
#include <wtf/MainThread.h>
#include "BCObserverService.h"
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;
using namespace WTF;

WebIconDatabase* WebIconDatabase::m_sharedWebIconDatabase = 0;

WebIconDatabase::WebIconDatabase()
: m_deliveryRequested(false)
{
    DS_CONSTRUCT();
}

WebIconDatabase::~WebIconDatabase()
{
    DS_DESTRUCT();
}

void WebIconDatabase::init()
{
    WebPreferences* standardPrefs = WebPreferences::sharedStandardPreferences();
    bool enabled = standardPrefs->iconDatabaseEnabled();
//     if (!enabled) {
//         LOG_ERROR("Unable to get icon database enabled preference");
//     }
    iconDatabase()->setEnabled(!!enabled);

    iconDatabase()->setClient(this);

    String databasePath = standardPrefs->iconDatabaseLocation();
//     if (prefDatabasePath == String())
//         LOG_ERROR("Unable to get icon database location preference");

    if (databasePath.isEmpty()) {
        databasePath = homeDirectoryPath();
        /*if (prefDatabasePath.isEmpty())
            LOG_ERROR("Failed to construct default icon database path");*/
    }

    bool ret = iconDatabase()->open(databasePath);
    /*if (!ret)
            LOG_ERROR("Failed to open icon database path");*/
}

WebIconDatabase* WebIconDatabase::createInstance()
{
    WebIconDatabase* instance = new WebIconDatabase();
    return instance;
}

WebIconDatabase* WebIconDatabase::sharedWebIconDatabase()
{
    if (m_sharedWebIconDatabase) {
        return m_sharedWebIconDatabase;
    }
    m_sharedWebIconDatabase = createInstance();
    m_sharedWebIconDatabase->init();
    return m_sharedWebIconDatabase;
}

WebIconDatabase *WebIconDatabase::sharedIconDatabase()
{
    return sharedWebIconDatabase();
}

WebCore::Image *WebIconDatabase::iconForURL(WebCore::String url, IntSize size, bool /*cache*/)
{
    IntSize intSize(size);

    Image* icon = iconDatabase()->iconForPageURL(url, intSize);
    return icon;
}

void WebIconDatabase::retainIconForURL(WebCore::String url)
{
    iconDatabase()->retainIconForPageURL(url);
}

void WebIconDatabase::releaseIconForURL(WebCore::String url)
{
    iconDatabase()->releaseIconForPageURL(url);
}

void WebIconDatabase::removeAllIcons()
{
    iconDatabase()->removeAllIcons();
}

void WebIconDatabase::delayDatabaseCleanup()
{
    IconDatabase::delayDatabaseCleanup();
}

void WebIconDatabase::allowDatabaseCleanup()
{
    IconDatabase::allowDatabaseCleanup();
}

WebCore::String WebIconDatabase::iconURLForURL(WebCore::String url)
{
    return iconDatabase()->iconURLForPageURL(url);
}


WebCore::Image* WebIconDatabase::getOrCreateSharedBitmap(IntSize size)
{
    WebCore::Image* result = m_sharedIconMap.get(size);
    if (result)
        return result;
//     result = new Image();
//     result->setContainerSize(size);
//     m_sharedIconMap.set(size, result);
    return result;
}

WebCore::Image* WebIconDatabase::getOrCreateDefaultIconBitmap(IntSize size)
{
    WebCore::Image* result = m_defaultIconMap.get(size);
    if (result)
        return result;

//     result = new Image();
//     result->setContainerSize(size);
    static Image* defaultIconImage = 0;
    if (!defaultIconImage) {
        defaultIconImage = Image::loadPlatformResource("urlIcon");
    }
    m_defaultIconMap.set(size, defaultIconImage);

    return result;
}

void WebIconDatabase::dispatchDidRemoveAllIcons()
{
    // Queueing the empty string is a special way of saying "this queued notification is the didRemoveAllIcons notification"
    MutexLocker locker(m_notificationMutex);
    m_notificationQueue.append(String());
    scheduleNotificationDelivery();
}

void WebIconDatabase::dispatchDidAddIconForPageURL(const String& pageURL)
{   
    MutexLocker locker(m_notificationMutex);
    m_notificationQueue.append(pageURL.copy());
    scheduleNotificationDelivery();
}

void WebIconDatabase::scheduleNotificationDelivery()
{
    // Caller of this method must hold the m_notificationQueue lock
    ASSERT(!m_notificationMutex.tryLock());

    if (!m_deliveryRequested) {
        m_deliveryRequested = true;
        callOnMainThread(deliverNotifications, 0);
    }
}

WebCore::String WebIconDatabase::iconDatabaseDidAddIconNotification()
{
    static WebCore::String didAddIconName = WebIconDatabaseDidAddIconNotification;
    return didAddIconName;
}

WebCore::String WebIconDatabase::iconDatabaseNotificationUserInfoURLKey()
{
    static WebCore::String iconUserInfoURLKey = WebIconNotificationUserInfoURLKey;
    return iconUserInfoURLKey;
}

String WebIconDatabase::iconDatabaseDidRemoveAllIconsNotification()
{
    static WebCore::String didRemoveAllIconsName = WebIconDatabaseDidRemoveAllIconsNotification;
    return didRemoveAllIconsName;
}

static void postDidRemoveAllIconsNotification(WebIconDatabase* iconDB)
{
    OWBAL::BCObserverService::createBCObserverService()->notifyObserver(WebIconDatabase::iconDatabaseDidRemoveAllIconsNotification(), "", iconDB);
}

static void postDidAddIconNotification(const String& pageURL, WebIconDatabase* iconDB)
{
    OWBAL::BCObserverService::createBCObserverService()->notifyObserver(WebIconDatabase::iconDatabaseDidAddIconNotification(), pageURL, iconDB);
}

void WebIconDatabase::deliverNotifications(void*)
{
    ASSERT(m_sharedWebIconDatabase);
    if (!m_sharedWebIconDatabase)
        return;

    ASSERT(m_sharedWebIconDatabase->m_deliveryRequested);

    Vector<String> queue;
    {
        MutexLocker locker(m_sharedWebIconDatabase->m_notificationMutex);
        queue.swap(m_sharedWebIconDatabase->m_notificationQueue);
        m_sharedWebIconDatabase->m_deliveryRequested = false;
    }

    for (unsigned i = 0; i < queue.size(); ++i) {
        if (queue[i].isNull())
            postDidRemoveAllIconsNotification(m_sharedWebIconDatabase);
        else
            postDidAddIconNotification(queue[i], m_sharedWebIconDatabase);
    }
}
#endif
