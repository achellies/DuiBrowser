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
#include "WebBackForwardList.h"

#include "WebFrame.h"
#include "WebPreferences.h"

#include <BackForwardList.h>
#include <HistoryItem.h>
#include "BAL/Includes/FakedDeepsee.h"

using std::min;
using namespace WebCore;

static HashMap<BackForwardList*, WebBackForwardList*>& backForwardListWrappers()
{
    static HashMap<BackForwardList*, WebBackForwardList*> staticBackForwardListWrappers;
    return staticBackForwardListWrappers;
}

WebBackForwardList::WebBackForwardList(PassRefPtr<BackForwardList> backForwardList)
    : m_backForwardList(backForwardList)
{
    ASSERT(!backForwardListWrappers().contains(m_backForwardList.get()));
    backForwardListWrappers().set(m_backForwardList.get(), this);
    DS_CONSTRUCT();
}

WebBackForwardList::~WebBackForwardList()
{
    if (m_backForwardList->closed())
	{
		if (backForwardListWrappers().contains(m_backForwardList.get()))
		{
		    backForwardListWrappers().remove(m_backForwardList.get());
		}
	}
    DS_DESTRUCT();
}

void WebBackForwardList::close()
{
	backForwardListWrappers().remove(m_backForwardList.get());
	m_backForwardList->close();
}


WebBackForwardList* WebBackForwardList::createInstance(PassRefPtr<BackForwardList> backForwardList)
{
    WebBackForwardList* instance;

    instance = backForwardListWrappers().get(backForwardList.get());

    if (!instance)
        instance = new WebBackForwardList(backForwardList);

    return instance;
}

void WebBackForwardList::addItem(WebHistoryItem* item)
{
    m_backForwardList->addItem(item->historyItem());
}

void WebBackForwardList::goBack()
{
    m_backForwardList->goBack();
}

void WebBackForwardList::goForward()
{
    m_backForwardList->goForward();
}

void WebBackForwardList::goToItem(WebHistoryItem* item)
{
    m_backForwardList->goToItem(item->historyItem());
}

WebHistoryItem* WebBackForwardList::backItem()
{
    HistoryItem* historyItem = m_backForwardList->backItem();

    if (!historyItem)
        return 0;

    return WebHistoryItem::createInstance(historyItem);
}

WebHistoryItem* WebBackForwardList::currentItem()
{
    HistoryItem* historyItem = m_backForwardList->currentItem();

    if (!historyItem)
        return 0;

    return WebHistoryItem::createInstance(historyItem);
}

WebHistoryItem* WebBackForwardList::forwardItem()
{
    HistoryItem* historyItem = m_backForwardList->forwardItem();

    if (!historyItem)
        return 0;

    return WebHistoryItem::createInstance(historyItem);
}

int WebBackForwardList::backListWithLimit(int limit, WebHistoryItem** list)
{
    HistoryItemVector historyItemVector;
    m_backForwardList->backListWithLimit(limit, historyItemVector);

    if (list)
        for (unsigned i = 0; i < historyItemVector.size(); i++)
            list[i] = WebHistoryItem::createInstance(historyItemVector[i].get());

    return static_cast<int>(historyItemVector.size());
}

int WebBackForwardList::forwardListWithLimit(int limit, WebHistoryItem** list)
{
    HistoryItemVector historyItemVector;
    m_backForwardList->forwardListWithLimit(limit, historyItemVector);

    if (list)
        for (unsigned i = 0; i < historyItemVector.size(); i++)
            list[i] = WebHistoryItem::createInstance(historyItemVector[i].get());

    return static_cast<int>(historyItemVector.size());
}

int WebBackForwardList::capacity()
{
    return (int)m_backForwardList->capacity();
}

void WebBackForwardList::setCapacity(int size)
{
    if (size < 0)
        return;
    
    m_backForwardList->setCapacity(size);
}

int WebBackForwardList::backListCount()
{
    return m_backForwardList->backListCount();
}

int WebBackForwardList::forwardListCount()
{
    return m_backForwardList->forwardListCount();
}

bool WebBackForwardList::containsItem(WebHistoryItem* item)
{
    if (!item)
        return false;

    return m_backForwardList->containsItem(item->historyItem());
}

WebHistoryItem* WebBackForwardList::itemAtIndex(int index)
{
    HistoryItem* historyItem = m_backForwardList->itemAtIndex(index);

    if (!historyItem)
        return 0;

    return WebHistoryItem::createInstance(historyItem);
}

void WebBackForwardList::removeItem(WebHistoryItem* item)
{
    if (!item)
        return;

    m_backForwardList->removeItem(item->historyItem());
}

//+daw ca 29/07/2008 static and global management
void WebBackForwardList::staticFinalize()
{
	backForwardListWrappers().clear();
}
//-daw ca
