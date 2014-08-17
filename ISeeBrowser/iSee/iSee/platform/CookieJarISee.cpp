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

#include "config.h"
#include "CookieJar.h"

#include "KURL.h"
#include "PlatformString.h"
#include "StringHash.h"
#include "StringBuilder.h"

#include <wtf/HashMap.h>

namespace WebCore {

typedef HashMap<String, String, CaseFoldingHash> CookieHash;

static HashMap<String, CookieHash*, CaseFoldingHash> cookieJar;

void setCookies(Document* /*document*/, const KURL& url, const KURL& /*policyURL*/, const String& value)
{
	if (value.isEmpty())
		return;
	
	String urlString;
	String path = url.path();
	if (path.isEmpty() || path == "/")
	{
		urlString = url.host();
		if (urlString.isEmpty())
			urlString = url.string();
	}
	else
		urlString = url.string();
	CookieHash* cookieHash = cookieJar.get(urlString);
	if (0 == cookieHash)
		cookieHash = new CookieHash();
	if (0 == cookieHash)
		return;

	Vector<String> cookie_list;
	value.split(';', true, cookie_list);

	for (int i = 0; i < cookie_list.size(); i++)
	{
		if (cookie_list[i].isEmpty())
			continue;
		Vector<String> cookie;
		cookie_list[i].split('=', true, cookie);
		cookie[0].stripWhiteSpace();
		if (cookie.size() > 1)
			cookieHash->set(cookie[0], cookie[1]);
		else
			cookieHash->set(cookie[0], String());
	}

	if (cookieHash->isEmpty())
		return;

	cookieJar.set(urlString, cookieHash);
}

String cookies(const Document* /*document*/, const KURL& url)
{
	if (url.isEmpty())
		return String();

	StringBuilder strBuilder;
	CookieHash* cookieHash = cookieJar.get(url.string());
	if (cookieHash)
	{
		CookieHash::const_iterator e = cookieHash->end();
		for (CookieHash::const_iterator i = cookieHash->begin(); i != e; ++i)
		{
			CookieHash::ValueType v = *i;
			strBuilder.append(v.first + "=" + v.second + "; ");
		}
	}
	
	String urlString = url.host();
	if (!urlString.isEmpty())
	{
		cookieHash = cookieJar.get(urlString);
		if (cookieHash)
		{
			CookieHash::const_iterator e = cookieHash->end();
			for (CookieHash::const_iterator i = cookieHash->begin(); i != e; ++i)
			{
				CookieHash::ValueType v = *i;
				strBuilder.append(v.first + "=" + v.second + "; ");
			}
		}
	}

	String &valueString = strBuilder.toString();
	if (valueString.length() >= 2)
		valueString.remove(valueString.length() - 2, 2);

	return valueString;
}

bool cookiesEnabled(const Document* /*document*/)
{
	return true;
}

}
