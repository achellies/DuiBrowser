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
* This file was modified by Electronic Arts Inc Copyright © 2008-2009
*/

///////////////////////////////////////////////////////////////////////////////
// BCLoggingEA.cpp
// Modified by Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "Logging.h"
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <EAWebKit/EAWebKit.h>

namespace OWBAL {


void InitializeLoggingChannelsIfNecessary()
{
    // Make sure these structs are in sync.
    EAW_ASSERT(EA::WebKit::kLogChannelEditing == LogEditing.mask); // Arbitrary entry

    EA::WebKit::Parameters& parameters = EA::WebKit::GetParameters();

    // To consider: Change WebKit's LogXXXX to be an array instead of a set of variables.
    //              That would allow this code to be simpler.

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelNotYetImplemented)
        LogNotYetImplemented.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelFrames)
        LogFrames.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelLoading)
        LogLoading.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelPopupBlocking)
        LogPopupBlocking.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelEvents)
        LogEvents.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelEditing)
        LogEditing.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelTextConversion)
        LogTextConversion.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelIconDatabase)
        LogIconDatabase.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelSQLDatabase)
        LogSQLDatabase.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelSpellingAndGrammar)
        LogSpellingAndGrammar.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelBackForward)
        LogBackForward.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelHistory)
        LogHistory.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelPageCache)
        LogPageCache.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelPlatformLeaks)
        LogPlatformLeaks.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelNetwork)
        LogNetwork.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelFTP)
        LogFTP.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelThreading)
        LogThreading.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelStorageAPI)
        LogStorageAPI.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelMedia)
        LogMedia.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelPlugin)
        LogPlugin.state = WTFLogChannelOn;

    if(parameters.mLogChannelFlags & EA::WebKit::kLogChannelArchives)
        LogArchives.state = WTFLogChannelOn;
}


} // namespace WebCore
