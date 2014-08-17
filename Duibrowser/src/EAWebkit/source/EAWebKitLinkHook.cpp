/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// EAWebKitLinkHook.cpp
// By Nicki Vankoughnett and Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include <EAWebKit/EAWebKitLinkHook.h>
#include <EAWebKit/EAWebKitView.h>
#include <EAWebKit/EAWebkitSTLWrapper.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>
#include "KURL.h"
#include "ResourceRequest.h"
#include "HTTPHeaderMap.h"


namespace EA
{
namespace WebKit
{


bool LinkHookManager::ModifyResourceRequest(WebCore::ResourceRequest& resourceRequest)
{
    bool bIntercepted = false;

    ViewNotification* pVN = GetViewNotification();

    if(pVN)
    {
        LinkNotificationInfo  lni;
        lni.mpView              = mpView;
        lni.mbURIIntercepted    = false;
		lni.mbURIInterceptedByDomainFiltering = false;//We don't make any decision here. It is done inside the resource manager.

		EA::WebKit::HeaderMap& originalHeaderMap = *GetHeaderMap(lni.mpOriginalHeaderMap);
		EA::WebKit::HeaderMap& newHeaderMap = *GetHeaderMap(lni.mpModifiedHeaderMap);
		
		// Setup mOriginalURI
		const WebCore::String& webCoreURI = resourceRequest.url().string();
		GetFixedString(lni.mOriginalURI)->assign(webCoreURI.characters(), webCoreURI.length());

		// Setup originalHeaderMap
		const WebCore::HTTPHeaderMap& webcoreHeaderMap = resourceRequest.httpHeaderFields();
		for(WebCore::HTTPHeaderMap::const_iterator it(webcoreHeaderMap.begin()); it != webcoreHeaderMap.end(); ++it)
		{
			const WebCore::String& webCoreKey   = it->first;
			const WebCore::String& webCoreValue = it->second;

			HeaderMap::value_type entry(HeaderMap::key_type(webCoreKey.characters(), webCoreKey.length()), 
				HeaderMap::mapped_type(webCoreValue.characters(), webCoreValue.length()));

			originalHeaderMap.insert(entry);
		}
	
	
		//Note by Arpit Baldeva: Changed the code so that it better reflects the main app behavior and is consistent with 
		//other ViewNotification API. This was discussed with Sims team over the email.
		//The interception has a confusing meaning here. This is how I interpret as possible scenarios.
		//1. Application does not do anything. Simply returns false. So nothing changes.
		//2. Application takes control of the URL(say for downloading stuff). In this case, application returns true to indicate this. Further, they
		//specify by setting the mbURIIntercepted to be true that the link does not need processing anymore.
		//3. Application is simply interested in doing something else with this link. Say it wants to redirect any access to www.url1.com to 
		//www.url2.com.

		if(!pVN->LinkSelected(lni)) // case 1: Application did not do anything with the notification and returned false
		{
			bIntercepted = false;//We too return false here so that WebKit does its own stuff with the Link.
		}
		else //application did something....l
		{
			//Check if application wanted WebKit to not handle URL anymore. For example, download stuff.
			//This is indicated by setting the lni.mbURIIntercepted
			if(lni.mbURIIntercepted) //case 2
			{
				bIntercepted = lni.mbURIIntercepted;// We are finished, WebKit does not do anything anymore.
			}
			else //case 3
			{
				//Check if the application simply changed the link location or headers. If yes, set them and let the WebKit do its work.
				if(!GetFixedString(lni.mModifiedURI)->empty())
				{
					OWBAL::KURL currentURL(GetFixedString(lni.mModifiedURI)->c_str());
					resourceRequest.setURL(currentURL);
				}

				if(!newHeaderMap.empty())
				{
					// We need to const_cast this, as ResourceRequest doesn't have a function to clear the headers.
					WebCore::HTTPHeaderMap& webCoreHeaderMap = const_cast<WebCore::HTTPHeaderMap&>(resourceRequest.httpHeaderFields());

					webCoreHeaderMap.clear(); // We will be completely re-writing the headers.

					for(HeaderMap::const_iterator it = newHeaderMap.begin(); it != newHeaderMap.end(); ++it)
					{
						const WebCore::String key(it->first.c_str(), it->first.length());
						const WebCore::String val(it->second.c_str(), it->second.length());

						resourceRequest.addHTTPHeaderField(key, val);
					}
				}	
				bIntercepted = false;//The WebKit should take this new data and do its job.
			}

		}
/*
        if(pVN->LinkSelected(lni))
        {
            bIntercepted = lni.mbURIIntercepted;

            if(!bIntercepted)
            {
                if(!GetFixedString(lni.mModifiedURI)->empty())
                {
                    OWBAL::KURL currentURL(GetFixedString(lni.mModifiedURI)->c_str());
                    resourceRequest.setURL(currentURL);
                }

                if(!newHeaderMap.empty())
                {
                    // We need to const_cast this, as ResourceRequest doesn't have a function to clear the headers.
                    WebCore::HTTPHeaderMap& webCoreHeaderMap = const_cast<WebCore::HTTPHeaderMap&>(resourceRequest.httpHeaderFields());

                    webCoreHeaderMap.clear(); // We will be completely re-writing the headers.

                    for(HeaderMap::const_iterator it = newHeaderMap.begin(); it != newHeaderMap.end(); ++it)
                    {
                        const WebCore::String key(it->first.c_str(), it->first.length());
                        const WebCore::String val(it->second.c_str(), it->second.length());

                        resourceRequest.addHTTPHeaderField(key, val);
                    }
                }
            }
        }
*/
    }

    return bIntercepted;
}


} //namespace WebKit
} //namespace EA
