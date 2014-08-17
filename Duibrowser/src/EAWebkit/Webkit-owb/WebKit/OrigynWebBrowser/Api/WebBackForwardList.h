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

#ifndef WebBackForwardList_H
#define WebBackForwardList_H

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebBackForwardList.h
 *  WebBackForwardList description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include "WebHistoryItem.h"
#include "WebHistoryItem.h"

#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {
    class BackForwardList;
}

class WebBackForwardList/*: public WTF::FastAllocBase*/
{
public:
    
    void close();
    
    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebBackForwardList* createInstance(PassRefPtr<WebCore::BackForwardList>);
protected:

    /**
     *  WebBackForwardList description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebBackForwardList(PassRefPtr<WebCore::BackForwardList>);

public:

    /**
     *  ~WebBackForwardList description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebBackForwardList();

    /**
     *  addItem description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void addItem(WebHistoryItem *item);

    /**
     *  goBack description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void goBack();

    /**
     *  goForward description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void goForward();

    /**
     *  goToItem description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void goToItem(WebHistoryItem *item);

    /**
     *  *backItem description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebHistoryItem *backItem();

    /**
     *  *currentItem description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebHistoryItem *currentItem();

    /**
     *  *forwardItem description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebHistoryItem *forwardItem();

    /**
     *  backListWithLimit description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual int backListWithLimit(int limit, WebHistoryItem **list);

    /**
     *  forwardListWithLimit description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual int forwardListWithLimit(int limit, WebHistoryItem **list);

    /**
     *  capacity description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual int capacity();

    /**
     *  setCapacity description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setCapacity(int size);

    /**
     *  backListCount description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual int backListCount();

    /**
     *  forwardListCount description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual int forwardListCount();

    /**
     *  containsItem description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool containsItem(WebHistoryItem *item);

    /**
     *  *itemAtIndex description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebHistoryItem *itemAtIndex(int index);


    /**
     *  removeItem description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void removeItem(WebHistoryItem* item);

protected:
    RefPtr<WebCore::BackForwardList> m_backForwardList;

//+daw ca 29/07/2008 static and global management
public:
    static void staticFinalize();
//-daw ca

};

#endif
