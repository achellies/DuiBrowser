/*
 * Copyright (C) 2007 Pleyo.  All rights reserved.
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

#ifndef BCOBSERVERSERVICE_H
#define BCOBSERVERSERVICE_H

#include <wtf/FastAllocBase.h>
#include "Observer.h"
#include "StringHash.h"
#include <wtf/HashMap.h>
#include <wtf/Vector.h>
#include "PlatformString.h"

using WebCore::String;

class BalObject;
namespace OWBAL {

    /**
     * @brief the BCObserverService
     *
     * The observer service implementation
     *
     */
    class BCObserverService/*: public WTF::FastAllocBase*/ {
        public:
	    static BCObserverService *createBCObserverService();

            virtual void registerObserver(const String& topic, BCObserver* observer);
            virtual void notifyObserver(const String &topic, BalObject *obj);
	    virtual void notifyObserver(const String &topic, const String &data, void *subject);
	    virtual void removeObserver(const String& topic, BCObserver* observer);
        private:
            BCObserverService();
            virtual ~BCObserverService() {};
            HashMap<String, Vector<BCObserver*> > m_topic;
        private:
	        static BCObserverService* m_pObserverSingleton;
        public:
	        static void staticFinalize();
    };
}

#endif //BCOBSERVERSERVICE_H
