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

// #if ENABLE(DATABASE)
#if (defined( ENABLE_DATABASE ) && ENABLE_DATABASE)

#error

#include "config.h"
#include "WebDatabaseManager.h"

#include "WebSecurityOrigin.h"

#include <PlatformString.h>
#include <DatabaseTracker.h>
#include <FileSystem.h>
#include <SecurityOrigin.h>
#include "BCObserverService.h"
#include "BAL/Includes/FakedDeepsee.h"
using namespace WebCore;


static WebDatabaseManager* s_sharedWebDatabaseManager;

WebDatabaseManager* WebDatabaseManager::createInstance()
{
    WebDatabaseManager* manager = new WebDatabaseManager();
    return manager;
}

WebDatabaseManager::WebDatabaseManager()
{
    DS_CONSTRUCT();
}

WebDatabaseManager::~WebDatabaseManager()
{
    DS_DESTRUCT();
}

WebDatabaseManager* WebDatabaseManager::sharedWebDatabaseManager()
{
    if (!s_sharedWebDatabaseManager) {
        s_sharedWebDatabaseManager = WebDatabaseManager::createInstance();
        DatabaseTracker::tracker().setClient(s_sharedWebDatabaseManager);
    }

    return s_sharedWebDatabaseManager;
}

Vector<RefPtr<SecurityOrigin> > WebDatabaseManager::origins()
{
    Vector<RefPtr<SecurityOrigin> > origins;
    if (this != s_sharedWebDatabaseManager)
        return origins;

    DatabaseTracker::tracker().origins(origins);

    return origins;
}

Vector<String> WebDatabaseManager::databasesWithOrigin(WebSecurityOrigin* origin)
{
    Vector<String> databaseNames;
    if (this != s_sharedWebDatabaseManager)
        return databaseNames;

    WebSecurityOrigin *webSecurityOrigin = WebSecurityOrigin::createInstance(origin->securityOrigin());
    if (!webSecurityOrigin)
        return databaseNames;

    DatabaseTracker::tracker().databaseNamesForOrigin(webSecurityOrigin->securityOrigin(), databaseNames);

    delete webSecurityOrigin;

    return databaseNames;
}

DatabaseDetails WebDatabaseManager::detailsForDatabase(WebCore::String databaseName, WebSecurityOrigin* origin)
{
    if (this != s_sharedWebDatabaseManager)
        return DatabaseDetails();

    WebSecurityOrigin* webSecurityOrigin = WebSecurityOrigin::createInstance(origin->securityOrigin());
    if (!webSecurityOrigin)
        return DatabaseDetails();

    DatabaseDetails details = DatabaseTracker::tracker().detailsForNameAndOrigin(databaseName, webSecurityOrigin->securityOrigin());

    delete webSecurityOrigin;

    return details;
}

void WebDatabaseManager::deleteAllDatabases()
{
    DatabaseTracker::tracker().deleteAllDatabases();
}

void WebDatabaseManager::deleteOrigin(WebSecurityOrigin* origin)
{
    if (this != s_sharedWebDatabaseManager)
        return;

    WebSecurityOrigin* webSecurityOrigin = WebSecurityOrigin::createInstance(origin->securityOrigin());
    if (!webSecurityOrigin)
        return;

    DatabaseTracker::tracker().deleteOrigin(webSecurityOrigin->securityOrigin());

    delete webSecurityOrigin;
}

void WebDatabaseManager::deleteDatabase(WebCore::String databaseName, WebSecurityOrigin* origin)
{
    if (this != s_sharedWebDatabaseManager)
        return;

    WebSecurityOrigin* webSecurityOrigin = WebSecurityOrigin::createInstance(origin->securityOrigin());
    if (!webSecurityOrigin)
        return;

    DatabaseTracker::tracker().deleteDatabase(webSecurityOrigin->securityOrigin(), databaseName);

    delete webSecurityOrigin;
}

void WebDatabaseManager::dispatchDidModifyOrigin(SecurityOrigin* origin)
{
    WebSecurityOrigin* securityOrigin = WebSecurityOrigin::createInstance(origin);
    OWBAL::BCObserverService::createBCObserverService()->notifyObserver(WebDatabaseDidModifyOriginNotification, "", securityOrigin);

    delete securityOrigin;
}

void WebDatabaseManager::dispatchDidModifyDatabase(SecurityOrigin* origin, const String& databaseName)
{
    WebSecurityOrigin* securityOrigin = WebSecurityOrigin::createInstance(origin);

    OWBAL::BCObserverService::createBCObserverService()->notifyObserver(WebDatabaseDidModifyDatabaseNotification, "", securityOrigin);

    delete securityOrigin;
}

void WebKitSetWebDatabasesPathIfNecessary()
{
    static bool pathSet = false;
    if (pathSet)
        return;

    WebCore::String databasesDirectory = WebCore::pathByAppendingComponent(WebCore::homeDirectoryPath(), "Databases");
    WebCore::DatabaseTracker::tracker().setDatabaseDirectoryPath(databasesDirectory);

    pathSet = true;
}
#endif
