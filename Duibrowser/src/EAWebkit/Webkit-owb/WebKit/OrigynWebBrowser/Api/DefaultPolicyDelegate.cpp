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
* This file was modified by Electronic Arts Inc Copyright ?2009-2010
*/

#include "config.h"
#include "DefaultPolicyDelegate.h"
#include "WebActionPropertyBag.h"
#include "WebFramePolicyListener.h"
#include "WebError.h"
#include "WebFrame.h"
#include "WebView.h"
#include "WebMutableURLRequest.h"

#include <PlatformString.h>
#include "CString.h"
#if defined(EA_PLATFORM_WINCE)
	#include <windows.h> 
#else
	#include <sys/types.h> 
	#include <sys/stat.h> 
#endif
#include "BAL/Includes/FakedDeepsee.h"

#ifndef _MSC_VER
    #include <unistd.h>
#endif

#ifndef S_ISDIR
    #define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif


using namespace WebCore;

// FIXME: move this enum to a separate header file when other code begins to use it.
typedef enum WebExtraNavigationType {
    WebNavigationTypePlugInRequest = WebNavigationTypeOther + 1
} WebExtraNavigationType;

DefaultPolicyDelegate::DefaultPolicyDelegate()
{
    DS_CONSTRUCT();
}

DefaultPolicyDelegate::~DefaultPolicyDelegate()
{
    DS_DESTRUCT();
}

DefaultPolicyDelegate* DefaultPolicyDelegate::sharedInstance()
{
    if (!m_st_pSharedinstance)
    {
        m_st_pSharedinstance = DefaultPolicyDelegate::createInstance();
    }
    return m_st_pSharedinstance;
}

DefaultPolicyDelegate* DefaultPolicyDelegate::createInstance()
{
    DefaultPolicyDelegate* instance = new DefaultPolicyDelegate();
    return instance;
}


void DefaultPolicyDelegate::decidePolicyForNavigationAction(WebView* webView, 
    /*[in]*/ WebActionPropertyBag* actionInformation, 
    /*[in]*/ WebMutableURLRequest* request, 
    /*[in]*/ WebFrame* /*frame*/, 
    /*[in]*/ WebFramePolicyListener* listener)
{
    int navType = 0;

    String ret = actionInformation->Read(WebActionNavigationTypeKey);
    navType = ret.toInt();

    bool canHandleRequest = webView->canHandleRequest(request);
    if (canHandleRequest)
        listener->use();
    else if (navType == WebNavigationTypePlugInRequest)
        listener->use();
    else {
        String url = request->URL();
        // A file URL shouldn't fall through to here, but if it did,
        // it would be a security risk to open it.
        if (!url.startsWith("file:")) {
            // FIXME: Open the URL not by means of a webframe, but by handing it over to the system and letting it determine how to open that particular URL scheme.  See documentation for [NSWorkspace openURL]
            ;
        }
        listener->ignore();
    }
}

void DefaultPolicyDelegate::decidePolicyForNewWindowAction(
    /*[in]*/ WebView* /*webView*/, 
    /*[in]*/ WebActionPropertyBag* /*actionInformation*/, 
    /*[in]*/ WebMutableURLRequest* /*request*/, 
    /*[in]*/ String /*frameName*/, 
    /*[in]*/ WebFramePolicyListener* listener)
{
    listener->use();
}

void DefaultPolicyDelegate::decidePolicyForMIMEType(
    /*[in]*/ WebView* webView, 
    /*[in]*/ String type, 
    /*[in]*/ WebMutableURLRequest* request, 
    /*[in]*/ WebFrame* /*frame*/, 
    /*[in]*/ WebFramePolicyListener* listener)
{
    bool canShowMIMEType = webView->canShowMIMEType(type);
    if (canShowMIMEType)
        canShowMIMEType = false;

    String url = request->URL();

    if (url.startsWith("file:")) {
        bool isDirectory = false;

		#if defined(EA_PLATFORM_WINCE)
			WIN32_FIND_DATA findData = {0};
			HANDLE fileHandle = FindFirstFileW(url.charactersWithNullTermination(), &findData); 
			if (fileHandle != INVALID_HANDLE_VALUE)
			{
				if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					isDirectory = true;

				FindClose(fileHandle);
			}
		#else
			struct stat st;
			if (stat(url.utf8().data(), &st))
				isDirectory = S_ISDIR(st.st_mode);
		#endif

        if (isDirectory)
            listener->ignore();
        else if(canShowMIMEType)
            listener->use();
        else
            listener->ignore();
    } else if (canShowMIMEType)
        listener->use();
    else
        listener->ignore();
}

void DefaultPolicyDelegate::unableToImplementPolicyWithError(
    /*[in]*/ WebView* /*webView*/, 
    /*[in]*/ WebError* error, 
    /*[in]*/ WebFrame* frame)
{
    String errorStr;
    errorStr = error->localizedDescription();

    String frameName;
    frameName = frame->name();

    //LOG_ERROR("called unableToImplementPolicyWithError:%S inFrame:%S", errorStr ? errorStr : TEXT(""), frameName ? frameName : TEXT(""));
}

//+daw ca 29/07/2008 static and global management
DefaultPolicyDelegate* DefaultPolicyDelegate::m_st_pSharedinstance = NULL;

void DefaultPolicyDelegate::staticFinalize()
{
    delete m_st_pSharedinstance;
    m_st_pSharedinstance = NULL;
}
//-daw ca
