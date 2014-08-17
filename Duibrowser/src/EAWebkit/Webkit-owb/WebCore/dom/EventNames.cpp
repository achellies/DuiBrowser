/*
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 2005 Apple Computer, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"

#ifdef AVOID_STATIC_CONSTRUCTORS
#define DOM_EVENT_NAMES_HIDE_GLOBALS 1
#endif

#include "EventNames.h"
#include "StaticConstructors.h"

namespace WebCore { 

static EventNames* staticEventNames;

EventNames& eventNames()
{
    return *staticEventNames;
}

#define INITIALIZE_EVENT_NAME(name)   , name##Event(#name)

EventNames::EventNames() : dummy(0)
DOM_EVENT_NAMES_FOR_EACH(INITIALIZE_EVENT_NAME)
{
}

EventNames::~EventNames()
{
    #define DEREF_EVENT_NAMES(name) name##Event.externalDeref();
    DOM_EVENT_NAMES_FOR_EACH(DEREF_EVENT_NAMES);
}

void EventNames::init()
{
    if (!staticEventNames) 
    {
        // Initialization is not thread safe, so this function must be called from the main thread first.
        //ASSERT(isMainThread());
        AtomicString::init();
        staticEventNames = new EventNames;
    }
}

void EventNames::unInit()
{
	delete staticEventNames;
	staticEventNames  = NULL;
}


} //namespace WebCore 
