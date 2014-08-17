/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"
#include "PageCache.h"

#include "Cache.h"
#include "CachedPage.h"
#include "FrameLoader.h"
#include "HistoryItem.h"
#include "Logging.h"
#include "SystemTime.h"

using namespace std;

namespace WebCore {

static const double autoreleaseInterval = 3;

static PageCache* stPageCache = NULL;

PageCache::~PageCache()
{
}

void PageCache::staticFinalize()
{
	if(stPageCache)
	{
    	delete stPageCache;
	    stPageCache=NULL;
    }
}

PageCache* pageCache()
{
    //daw ca warning
	if(!stPageCache)
		stPageCache=new PageCache;
	return stPageCache;
}

PageCache::PageCache()
    : m_capacity(0)
    , m_size(0)
    , m_head(0)
    , m_tail(0)
    , m_autoreleaseTimer(this, &PageCache::releaseAutoreleasedPagesNowOrReschedule)
{
}

void PageCache::setCapacity(int capacity)
{
    ASSERT(capacity >= 0);
    m_capacity = max(capacity, 0);

    prune();
}

void PageCache::add(PassRefPtr<HistoryItem> prpItem, PassRefPtr<CachedPage> cachedPage)
{
    ASSERT(prpItem);
    ASSERT(cachedPage);
    
    HistoryItem* item = prpItem.releaseRef(); // Balanced in remove().

    // Remove stale cache entry if necessary.
    if (item->m_cachedPage)
        remove(item);

    item->m_cachedPage = cachedPage;
    addToLRUList(item);
    ++m_size;
    
    prune();
}

void PageCache::remove(HistoryItem* item)
{
    // Safely ignore attempts to remove items not in the cache.
    if (!item || !item->m_cachedPage)
        return;

    autorelease(item->m_cachedPage.release());
    removeFromLRUList(item);
    --m_size;

    item->deref(); // Balanced in add().
}

void PageCache::prune()
{
    while (m_size > m_capacity) {
        ASSERT(m_tail && m_tail->m_cachedPage);
        remove(m_tail);
    }
}

void PageCache::addToLRUList(HistoryItem* item)
{
    item->m_next = m_head;
    item->m_prev = 0;

    if (m_head) {
        ASSERT(m_tail);
        m_head->m_prev = item;
    } else {
        ASSERT(!m_tail);
        m_tail = item;
    }

    m_head = item;
}

void PageCache::removeFromLRUList(HistoryItem* item)
{
    if (!item->m_next) {
        ASSERT(item == m_tail);
        m_tail = item->m_prev;
    } else {
        ASSERT(item != m_tail);
        item->m_next->m_prev = item->m_prev;
    }

    if (!item->m_prev) {
        ASSERT(item == m_head);
        m_head = item->m_next;
    } else {
        ASSERT(item != m_head);
        item->m_prev->m_next = item->m_next;
    }
}

void PageCache::releaseAutoreleasedPagesNowOrReschedule(Timer<PageCache>* timer)
{

//+ CSidhall 9/11/09 - Changed this code for it could cause a memory overflow on systems with limited memory.
// The user idle timer is not implemented so it was always 0.0 and this would load the list until we get 42 
// entries.    
// Old Code:
/*
    double loadDelta = currentTime() - FrameLoader::timeOfLastCompletedLoad();
    float userDelta = userIdleTime();
    
    // FIXME: <rdar://problem/5211190> This limit of 42 risks growing the page cache far beyond its nominal capacity.
    if ((userDelta < 0.5 || loadDelta < 1.25) && m_autoreleaseSet.size() < 42) {
        LOG(PageCache, "WebCorePageCache: Postponing releaseAutoreleasedPagesNowOrReschedule() - %f since last load, %f since last input, %i objects pending release", loadDelta, userDelta, m_autoreleaseSet.size());
        timer->startOneShot(autoreleaseInterval);
        return;
    }

    LOG(PageCache, "WebCorePageCache: Releasing page caches - %f seconds since last load, %f since last input, %i objects pending release", loadDelta, userDelta, m_autoreleaseSet.size());
*/
// New Code:
   
    // We will use the capacity set by the user app as a guide for how much we can overflow.
    // If the user has limited memory, the m_capacity will likely be a low value so we don't want to have too
    // many pages waiting on the list for it could crash the memory system.
    const static int MIN_PAGE_OVERFLOW_CAPACITY = 2; // We allow at least one so the load timer can trigger
    const static int MAX_PAGE_OVERFLOW_CAPACITY = 42; // No idea why 42 but this was the original limit.
    
    int overflowCapacity = m_capacity;
    if(overflowCapacity < MIN_PAGE_OVERFLOW_CAPACITY)
    {
        overflowCapacity = MIN_PAGE_OVERFLOW_CAPACITY;
    }
    else if (overflowCapacity > MAX_PAGE_OVERFLOW_CAPACITY)
    {
        overflowCapacity = MAX_PAGE_OVERFLOW_CAPACITY;
    }

    double loadDelta = currentTime() - FrameLoader::timeOfLastCompletedLoad();

    // This can be triggered too soon if a page is already on the list and a new one gets placed while the timer
    // is about to fire, then it won't get the 3 sec delay safety from the timer interval but would have the 
    // load time delta as delay safety mesure unless it overflows which then could be a risk...
    // Removed user idle time for now as not implemented and it forced the overflow to trigger.
    const static double MIN_PAGE_LOAD_DELAY = 1.25;      

    if ( (loadDelta < MIN_PAGE_LOAD_DELAY) && (m_autoreleaseSet.size() < overflowCapacity) ) {
        LOG(PageCache, "WebCorePageCache: Postponing releaseAutoreleasedPagesNowOrReschedule() - %f since last load, %i objects pending release", loadDelta, m_autoreleaseSet.size());
        timer->startOneShot(autoreleaseInterval);
        return;
    }

    LOG(PageCache, "WebCorePageCache: Releasing page caches - %f seconds since last load, %i objects pending release", loadDelta,  m_autoreleaseSet.size());
//-CS

    releaseAutoreleasedPagesNow();
}

void PageCache::releaseAutoreleasedPagesNow()
{
    m_autoreleaseTimer.stop();

    // Postpone dead pruning until all our resources have gone dead.
    cache()->setPruneEnabled(false);

    CachedPageSet tmp;
    tmp.swap(m_autoreleaseSet);

    CachedPageSet::iterator end = tmp.end();
    for (CachedPageSet::iterator it = tmp.begin(); it != end; ++it)
        (*it)->clear();

    // Now do the prune.
    cache()->setPruneEnabled(true);
    cache()->prune();
}

void PageCache::autorelease(PassRefPtr<CachedPage> page)
{
    ASSERT(page);
    ASSERT(!m_autoreleaseSet.contains(page.get()));
    m_autoreleaseSet.add(page);
    if (!m_autoreleaseTimer.isActive())
        m_autoreleaseTimer.startOneShot(autoreleaseInterval);
}

} // namespace WebCore
