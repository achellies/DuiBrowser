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

/////////////////////////////////////////////////////////////////////////////
// BCCookieJarEA.cpp
// Written by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "CookieJar.h"
#include "ResourceHandleManager.h"
#include "KURL.h"

#include "BCCookieManagerEA.h"

namespace WKAL {

 
///////////////////////////////////////////////////////////////////////////////
// Note by Paul Pedriana:
//
// See http://webmaster.info.aol.com/aboutcookies.html to understand a little about cookies in HTTP.
// Also http://msdn.microsoft.com/en-us/library/aa384321(VS.85).aspx
// Also http://www.faqs.org/rfcs/rfc2965.html
//
// Web pages that test cookie support can be found at:
//     http://www.tempesttech.com/cookies/cookietest1.asp  (very simple)
//     http://www.pageout.net/page.dyn/cookie_tester
//
// Set-Cookie HTTP header format (server to client):
//     Set-Cookie: <name>=<value>[; <name>=<value>]...[; expires=<date>][; domain=<domain_name>][; path=<some_path>][; secure][; httponly]
// Example:
//     Set-Cookie:Test=test_value; expires=Sat, 01-Jan-2000 00:00:00 GMT; path=/
//
// Cookie HTTP header format (client to server):
//     Cookie: <name>=<value>[; <name>=<value>]...
// Example:
//     Cookie: Test=test_value; color=green
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// WebKit expects cookie support to be handled by the HTTP transport system
// and WebKit doesn't do any work to parse or send cookies, with two exceptions:
//
//    - The HTML of a page has an http header directive in a <meta> tag within the <head> tag, as with:
//          <html><head><meta http-equiv="Set-Cookie" content="SomeCookie=SomeValue; expires=Sat, 01-Jan-2000 00:00:00 GMT; path=/"/></head></html>
//
//    - The JavaScript of a page directly sets a cookie, as with the following JavaScript:
//          document.cookie = "SomeCookie=SomeValue; expires=Sat, 01-Jan-2000 00:00:00 GMT; path=/"
//
// In either case we have a cookie being specified in the page instead of the 
// HTTP headers and the transport system doesn't see it. So WebKit calls the 
// setCookies function here and expects that somehow this is relayed to the transport system.
//
// Cookies that come via HTTP headers need to be handled by the transport system
// or some external entity that gets to see the headers upon receiving them.
// Similarly, sending of cookies is always done via HTTP headers and need to be sent by 
// the transport system or some external cookie management system that can set
// header entries before they are sent. 
//
// So what we need to do is implement cookie support for our transport system
// and have the functions here tie into it.
///////////////////////////////////////////////////////////////////////////////


// This cookieJar is a stand-in and should be removed. What we need to do is to relay
// setCookies() and cookies() to the transport system's support for cookies. Granted, 
// an option may be to have the transport system get cookies from something we write 
// here. But in any case we will want something more proper and persistable than a simple
// HashMap which has no concept of cookie expirations or paths.
//static HashMap<String, String> cookieJar;

// This function is called by the Document or JSDocument when the page itself specifies
// a cookie. We want to relay this to our central cookie manager.
void setCookies(Document* /*document*/, const KURL& url, const KURL& /*policyURL*/, const String& value)
{
    EA::WebKit::CookieManager* cookieManager = WKAL::ResourceHandleManager::sharedInstance()->GetCookieManager();
    
    if(cookieManager)
    {
        EA::WebKit::FixedString16_256 urlStr16;
        urlStr16.assign(url.string().characters(),url.string().length());
        EA::WebKit::FixedString8_256 url8; 
        EA::WebKit::ConvertToString8(urlStr16,url8);

        EA::WebKit::FixedString16_256 valueStr16;
        valueStr16.assign(value.characters(),value.length());
        EA::WebKit::FixedString8_256 valueStr8; 
        EA::WebKit::ConvertToString8(valueStr16, valueStr8);
        cookieManager->ProcessCookieHeader(valueStr8.c_str(),url8.c_str());
    }
}

// This function would be called by the application if it wants to put up a dialog box to 
// display the currently recognized cookies for a URI. This is not currently called by 
// the transport system prior to sending an HTTP page request. As of this writing, our
// application doesn't call this function.
String cookies(const Document* /*document*/, const KURL& url)
{
    EA::WebKit::CookieManager* cookieManager = WKAL::ResourceHandleManager::sharedInstance()->GetCookieManager();
    if(cookieManager)
    {
        EA::WebKit::FixedString8_256 cookieStr8 = cookieManager->GetCookieTextForURL(url,true); 
        //EA::WebKit::FixedString16_256 cookieStr16;
        //EA::WebKit::ConvertToString16(cookieStr8, cookieStr16);
        //String cookieStr(cookieStr16.c_str());
        return String(cookieStr8.c_str());
    }
    else
    {
        return String();
    }
}

// This function would be called by the application if it wants to display the current setting.
// As of this writing, our application doesn't call this function. It could also be called if 
// the page references the 'navigator.cookieEnabled' DOM property.
bool cookiesEnabled(const Document* /*document*/)
{
    EA::WebKit::CookieManager* cookieManager = WKAL::ResourceHandleManager::sharedInstance()->GetCookieManager();

    const EA::WebKit::CookieManagerParameters& params = cookieManager->GetParameters();

    return (params.mMaxCookieCount > 0);
}

// This is called on application shutdown to clear the runtime cookies. 
// This could be used to shutdown whatever application-level cookie support
// exists if it is not being managed by the transport system, which probably
// can take care of its own cookie shutdown if it happens to be managing 
// cookies itself.
void releaseCookies()
{
    // We don't need to do anything, as our ResourceHandleManager manages this.
}

}



