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
#include "WebHistoryItem.h"

#include <PlatformString.h>
#include <HistoryItem.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

static HashMap<HistoryItem*, WebHistoryItem*>& historyItemWrappers()
{
    static HashMap<HistoryItem*, WebHistoryItem*> staticHistoryItemWrappers;
    return staticHistoryItemWrappers;
}

WebHistoryItem::WebHistoryItem(PassRefPtr<HistoryItem> historyItem)
: m_historyItem(historyItem)
{
    ASSERT(!historyItemWrappers().contains(m_historyItem.get()));
    historyItemWrappers().set(m_historyItem.get(), this);
    DS_CONSTRUCT();
}

WebHistoryItem::~WebHistoryItem()
{
    ASSERT(historyItemWrappers().contains(m_historyItem.get()));
    historyItemWrappers().remove(m_historyItem.get());
    DS_DESTRUCT();
}

WebHistoryItem* WebHistoryItem::createInstance()
{
    WebHistoryItem* instance = new WebHistoryItem(HistoryItem::create());
    return instance;
}

WebHistoryItem* WebHistoryItem::createInstance(PassRefPtr<HistoryItem> historyItem)
{
    WebHistoryItem* instance;

    instance = historyItemWrappers().get(historyItem.get());

    if (!instance)
        instance = new WebHistoryItem(historyItem);

    return instance;
}

bool WebHistoryItem::hasURLString()
{
    return m_historyItem->urlString().isEmpty() ? false : true;
}

int WebHistoryItem::visitCount()
{
    return m_historyItem->visitCount();
}

void WebHistoryItem::setVisitCount(int count)
{
    m_historyItem->setVisitCount(count);
}

void WebHistoryItem::mergeAutoCompleteHints(WebHistoryItem* otherItem)
{
    if (!otherItem)
        return;

    m_historyItem->mergeAutoCompleteHints(otherItem->historyItem());
}

void WebHistoryItem::setLastVisitedTimeInterval(double time)
{
    m_historyItem->setLastVisitedTime(time);
}

void WebHistoryItem::setTitle(WebCore::String title)
{
    m_historyItem->setTitle(title);
}

WebCore::String WebHistoryItem::RSSFeedReferrer()
{
    return m_historyItem->rssFeedReferrer();
}

void WebHistoryItem::setRSSFeedReferrer(WebCore::String url)
{
    m_historyItem->setRSSFeedReferrer(url);
}

bool WebHistoryItem::hasPageCache()
{
    // FIXME - TODO
    return false;
}

void WebHistoryItem::setHasPageCache(bool /*hasCache*/)
{
    // FIXME - TODO
}

WebCore::String WebHistoryItem::target()
{
    return m_historyItem->target();
}

bool WebHistoryItem::isTargetItem()
{
    return m_historyItem->isTargetItem() ? true : false;
}

Vector<WebHistoryItem*> WebHistoryItem::children()
{
    Vector<WebHistoryItem*> child;
    const HistoryItemVector& coreChildren = m_historyItem->children();
    if (coreChildren.isEmpty())
        return child;
    size_t childCount = coreChildren.size();

    for (unsigned i = 0; i < childCount; ++i) {
        WebHistoryItem* item = WebHistoryItem::createInstance(coreChildren[i]);
        child.append(item);
    }

    return child;
}

void WebHistoryItem::initWithURLString(WebCore::String urlString, WebCore::String title, double lastVisited)
{
    historyItemWrappers().remove(m_historyItem.get());
    m_historyItem = HistoryItem::create(urlString, title, lastVisited);
    historyItemWrappers().set(m_historyItem.get(), this);
}

WebCore::String WebHistoryItem::originalURLString()
{
    return m_historyItem->originalURLString();
}

WebCore::String WebHistoryItem::URLString()
{
    return m_historyItem->urlString();
}

WebCore::String WebHistoryItem::title()
{
    return m_historyItem->title();
}

double WebHistoryItem::lastVisitedTimeInterval()
{
    return m_historyItem->lastVisitedTime();
}

void WebHistoryItem::setAlternateTitle(WebCore::String title)
{
    m_alternateTitle = title;
}

WebCore::String WebHistoryItem::alternateTitle()
{
    return m_alternateTitle;
}

Image* WebHistoryItem::icon()
{
    return 0;
}

HistoryItem* WebHistoryItem::historyItem() const
{
    return m_historyItem.get();
}


//+daw ca 29/07/2008 static and global management
void WebHistoryItem::staticFinalize()
{
    historyItemWrappers().clear();
}
//-daw ca
