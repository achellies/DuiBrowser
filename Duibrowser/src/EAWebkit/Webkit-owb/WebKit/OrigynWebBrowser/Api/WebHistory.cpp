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

#include "config.h"
#include "WebHistory.h"

#include "WebHistoryItem.h"
#include "WebPreferences.h"
#include <wtf/Vector.h>
#include <KURL.h>
#include <PageGroup.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;


WebHistory::WebHistory()
: m_preferences(0)
{
    m_preferences = WebPreferences::sharedStandardPreferences();
    DS_CONSTRUCT();
}

WebHistory::~WebHistory()
{
    m_preferences = 0;
    DS_DESTRUCT();
}

WebHistory* WebHistory::createInstance()
{
    WebHistory* instance = new WebHistory();
    return instance;
}

WebHistory* WebHistory::sharedHistory()
{
//+daw ca 29/07/2008 static and global management
    if (!m_st_pSharedHistory )
    {
        m_st_pSharedHistory = WebHistory::createInstance();
    }
    return m_st_pSharedHistory;
//-daw ca
}

WebHistory* WebHistory::optionalSharedHistory()
{
    return sharedHistory();
}

void WebHistory::setOptionalSharedHistory(WebHistory* history)
{
    if (sharedHistory() == history)
        return;
    *sharedHistory() = *history;
    PageGroup::setShouldTrackVisitedLinks(sharedHistory());
    PageGroup::removeAllVisitedLinks();
}

WebError* WebHistory::loadFromURL(String url)
{
    return 0;
}


WebError* WebHistory::saveToURL(String url)
{
    return 0;
}



void WebHistory::addItems(int itemCount, WebHistoryItem** items)
{
}

void WebHistory::removeItems(int itemCount, WebHistoryItem** items)
{
}

void WebHistory::removeAllItems()
{
}

WebHistoryItem* WebHistory::itemForURL(WebCore::String url)
{
    return 0;
}


void WebHistory::setHistoryItemLimit(int limit)
{
    if (!m_preferences)
        return;
    m_preferences->setHistoryItemLimit(limit);
}

int WebHistory::historyItemLimit()
{
    if (!m_preferences)
        return 0;
    return m_preferences->historyItemLimit();
}

void WebHistory::setHistoryAgeInDaysLimit(int limit)
{
    if (!m_preferences)
        return ;
    m_preferences->setHistoryAgeInDaysLimit(limit);
}

int WebHistory::historyAgeInDaysLimit()
{
    if (!m_preferences)
        return 0;
    return m_preferences->historyAgeInDaysLimit();
}


void WebHistory::addItem(const KURL& url, const String& title)
{
}

void WebHistory::addVisitedLinksToPageGroup(PageGroup& group)
{
}

//+daw ca 29/07/2008 static and global management
WebHistory* WebHistory::m_st_pSharedHistory = NULL;
void WebHistory::staticFinalize()
{
    delete m_st_pSharedHistory;
    m_st_pSharedHistory = NULL;
}
//-daw ca
