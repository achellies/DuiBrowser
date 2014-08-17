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

#ifndef WebDatabaseManager_h
#define WebDatabaseManager_h

#if ENABLE(DATABASE)

/**
 *  @file  WebDatabaseManager.h
 *  WebDatabaseManager description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <DatabaseTrackerClient.h>
#include <DatabaseTrackerClient.h>
#include "Vector.h"

namespace WebCore {
    class SecurityOrigin;
    class String;
    class DatabaseDetails;
}

class WebSecurityOrigin;

#define WebDatabaseDisplayNameKey "WebDatabaseDisplayNameKey"
#define WebDatabaseExpectedSizeKey "WebDatabaseExpectedSizeKey"
#define WebDatabaseUsageKey "WebDatabaseUsageKey"

#define WebDatabaseDidModifyOriginNotification "WebDatabaseDidModifyOriginNotification"
#define WebDatabaseDidModifyDatabaseNotification "WebDatabaseDidModifyDatabaseNotification"
#define WebDatabaseNameKey "WebDatabaseNameKey"



class WebDatabaseManager : private WebCore::DatabaseTrackerClient {
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebDatabaseManager* createInstance();


    /**
     *  ~WebDatabaseManager description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebDatabaseManager();

    /**
     *  sharedWebDatabaseManager description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebDatabaseManager* sharedWebDatabaseManager();

    /**
     *  origins description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WTF::Vector<RefPtr<WebCore::SecurityOrigin> > origins();

    /**
     *  databasesWithOrigin description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WTF::Vector<WebCore::String> databasesWithOrigin(WebSecurityOrigin* origin);

    /**
     *  detailsForDatabase description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::DatabaseDetails detailsForDatabase(WebCore::String databaseName, WebSecurityOrigin* origin);

    /**
     *  deleteAllDatabases description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void deleteAllDatabases();

    /**
     *  deleteOrigin description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void deleteOrigin(WebSecurityOrigin* origin);

    /**
     *  deleteDatabase description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void deleteDatabase(WebCore::String databaseName, WebSecurityOrigin* origin);


    /**
     *  dispatchDidModifyOrigin description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void dispatchDidModifyOrigin(WebCore::SecurityOrigin*);

    /**
     *  dispatchDidModifyDatabase description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void dispatchDidModifyDatabase(WebCore::SecurityOrigin*, const WebCore::String& databaseName);

private:

    /**
     *  WebDatabaseManager description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebDatabaseManager();
};


    /**
     *  WebKitSetWebDatabasesPathIfNecessary description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
void WebKitSetWebDatabasesPathIfNecessary();
#endif
#endif
