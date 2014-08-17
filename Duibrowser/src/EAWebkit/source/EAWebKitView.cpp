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
// EAWebKitView.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitGraphics.h>
#include <EAWebKit/EAWebkitSTLWrapper.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>
#include <EAWebKit/internal/EAWebkitNodeListContainer.h>
#include <EAWebKit/internal/EAWebkitOverlaySurfaceArrayContainer.h>
#include <EAWebKit/EAWebKitFPUPrecision.h>
#include <EARaster/EARaster.h>
#ifdef  USE_EATHREAD_LIBRARY
    #include <EAThread/eathread_sync.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Api/WebView.h>
#include <FrameView.h>
#include <FrameTree.h>
#include <FrameLoader.h>
#include <DocumentLoader.h>
#include <SubstituteData.h>
#include <ScriptController.h>
#include <Cursor.h>
#include <Cache.h>
#include <PageCache.h>
#include <FontCache.h>
#include <ResourceHandleManager.h>
#include "MainThread.h"
#include "SharedTimer.h"
#include <KeyboardEvent.h>
#include <FocusDirection.h>
#include <page.h>
#include <FocusController.h>
#include <EASTL/vector.h>

#include <EAWebKit/internal/InputBinding/EAWebKitDocumentNavigator.h>
#include <EAWebKit/internal/InputBinding/EAWebKitEventListener.h>
#include <EAWebKit/internal/InputBinding/EAWebKitPolarRegion.h>
#include <EAWebKit/internal/InputBinding/EAWebKitUtils.h>
#include <EAWebKit/internal/InputBinding/EAWebKitDocumentNavigationDelegates.h>

#include "HTMLInputElement.h"
#include <NodeList.h>
#include <EventNames.h>
#include <EAWebKit/internal/InputBinding/EAWebKitDOMWalker.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <HTMLInputElement.h>
#include <HTMLNames.h>
#include <EAWebKit/internal/EAWebKitString.h>
#include "debugger.h"
#include "ExecState.h"
#include "DebuggerCallFrame.h"
#include "JSDOMWindow.h"
#include "c_runtime.h"
#include "BAL/OWBAL/Concretizations/Types/Common/BCbal_objectCommon.h"
#include "WebCore/bridge/bal/bal_class.h"
#include "HTMLTextAreaElement.h"
#include <EAWebKit/internal/EAWebkitJavascriptBinding.h>
#include "BAL/WKAL/Concretizations/Widgets/EA/BCPlatformScrollBarEA.h"
#include <EAWebKit/internal/EAWebKitViewHelper.h>

#include <float.h>

#ifdef _MSC_VER
	#define snprintf _snprintf
#endif

namespace EA
{

namespace WebKit
{

	View* AutoSetActiveView::spActiveView = NULL;

    LoadInfo::LoadInfo()
		: mpView(NULL),
		mLET(kLETNone),
		mbStarted(false),
		mbCompleted(false),
		mContentLength(-1),
		mProgressEstimation(0.0),
		mURI(),
		mResourceURI(),
		mPageTitle(),
        mLastChangedTime(0)
	{
	}


///////////////////////////////////////////////////////////////////////
// View Notification
///////////////////////////////////////////////////////////////////////


ViewNotification* gpViewNotification = NULL;


EAWEBKIT_API void SetViewNotification(ViewNotification* pViewNotification)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	gpViewNotification = pViewNotification;
}


EAWEBKIT_API ViewNotification* GetViewNotification()
{
    return gpViewNotification;
}

// Constructors for the metrics callback system
ViewProcessInfo::ViewProcessInfo()
 : mpView(0),
   mProcessType(kVProcessTypeNone),
   mProcessStatus(kVProcessStatusNone),
   mStartTime(0.0),
   mIntermediateTime(0.0),
   mURI(0),
   mSize(0),
   mJobId(0)
{
}

ViewProcessInfo::ViewProcessInfo(VProcessType type,VProcessStatus status, View* pView)
 : mpView(pView),
   mProcessType(type),
   mProcessStatus(status),
   mStartTime(0.0),
   mIntermediateTime(0.0),
   mURI(0),
   mSize(0),
   mJobId(0)
{
    // Set up a default active view if found   
    if(!pView)
        mpView = AutoSetActiveView::GetActiveView();
}

void ViewProcessInfo::ResetTime()
{
	mStartTime = 0.0;
	mIntermediateTime = 0.0;
}

//Global array for profile process.
ViewProcessInfo gProcessInfoArray[kVProcessTypeLast];

void NOTIFY_PROCESS_STATUS(VProcessType processType, VProcessStatus processStatus, EA::WebKit::View *pView)
{
	const Parameters& parameters = EA::WebKit::GetParameters();
	if(!parameters.mbEnableProfiling)
		return;

	EAW_ASSERT_MSG(processType<kVProcessTypeLast, "Size mismatch. The process enum is higher than the size of the array.");
	if(processType<kVProcessTypeLast)
	{
        if(pView)
            gProcessInfoArray[processType].mpView = pView;
        else
            gProcessInfoArray[processType].mpView = AutoSetActiveView::GetActiveView();

        gProcessInfoArray[processType].mProcessType = processType; //lame but works(since we don't initialize the global array with each process type.
		gProcessInfoArray[processType].mProcessStatus = processStatus;
		
		if(processStatus == kVProcessStatusStarted)
			gProcessInfoArray[processType].ResetTime();
		
		EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
		if(pViewNotification)
			pViewNotification->ViewProcessStatus(gProcessInfoArray[processType]);

	}
}

void NOTIFY_PROCESS_STATUS(ViewProcessInfo& process, VProcessStatus processStatus)
{
	const Parameters& parameters = EA::WebKit::GetParameters();
	if(!parameters.mbEnableProfiling)
		return;

	if(processStatus == kVProcessStatusStarted)
		process.ResetTime();

	process.mProcessStatus = processStatus;
	
	EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
	if(pViewNotification)
		pViewNotification->ViewProcessStatus(process);
}


///////////////////////////////////////////////////////////////////////
// Javascript value
///////////////////////////////////////////////////////////////////////
EAWEBKIT_API EA::WebKit::JavascriptValue* CreateJavaScriptValue() 
{
	return EAWEBKIT_NEW("JavaScriptValue") EA::WebKit::JavascriptValue();
}

EAWEBKIT_API void DestroyJavaScriptValue(EA::WebKit::JavascriptValue* pValue)                    
{
	EAWEBKIT_DELETE pValue;
}

EAWEBKIT_API EA::WebKit::JavascriptValue* CreateJavaScriptValueArray(int count)
{
    return EAWEBKIT_NEW("JavaScriptValue") EA::WebKit::JavascriptValue[count];
}

EAWEBKIT_API void DestroyJavaScriptValueArray(EA::WebKit::JavascriptValue* pValue)                    
{
    EAWEBKIT_DELETE[] pValue;
}

///////////////////////////////////////////////////////////////////////
// View
///////////////////////////////////////////////////////////////////////

ViewArray* ViewArray::mpViewArrayInstance = NULL;


EAWEBKIT_API int GetViewCount()
{
#ifdef USE_EATHREAD_LIBRARY
    // To do: Use a mutex instead of simply memory barriers.
    EAReadBarrier();
#endif

    return (int)ViewArray::GetArray().size();
}

EAWEBKIT_API View* GetView(int index)
{
#ifdef USE_EATHREAD_LIBRARY    
    // To do: Use a mutex instead of simply memory barriers.
    EAReadBarrier();
#endif

    if(index < (int)ViewArray::GetArray().size())
        return ViewArray::GetArray()[(eastl_size_t)index];

    return NULL;
}

EAWEBKIT_API bool IsViewValid(View* pView)
{
#ifdef USE_EATHREAD_LIBRARY    
    // To do: Use a mutex instead of simply memory barriers.
    EAReadBarrier();
#endif

    // This is a linear search, but the list size is usually tiny.
    for(eastl_size_t i = 0, iEnd = ViewArray::GetArray().size(); i < iEnd; ++i)
    {
        if(pView == ViewArray::GetArray()[i])
            return true;
    }

    return false;
}


EAWEBKIT_API View* GetView(::WebView* pWebView)
{
    // To do: Use a mutex instead of simply memory barriers.
    // Instead of doing a loop, we could possibly stuff a pointer into the given WebView directly or indirectly.
    for(eastl_size_t i = 0, iEnd = ViewArray::GetArray().size(); i < iEnd; ++i)
    {
        View*    const pViewCurrent    = ViewArray::GetArray()[i];
        WebView* const pWebViewCurrent = pViewCurrent->GetWebView();

        if(pWebViewCurrent == pWebView)
            return pViewCurrent;
    }

    return NULL;
}


EAWEBKIT_API View* GetView(::WebFrame* pWebFrame)
{
	if(pWebFrame)
		return GetView(pWebFrame->webView());

	return NULL;
}


EAWEBKIT_API View* GetView(WebCore::Frame* pFrame)
{
    if(pFrame)
    {
        WebCore::Page* pPage = pFrame->page();
        if(pPage)
        {
            WebView* pWebView = ::kit(pPage);  //From #include <WebView.h>
            return GetView(pWebView);
        }
    }
   return NULL;
}


EAWEBKIT_API View* GetView(WebCore::FrameView* pFrameView)
{
    if(pFrameView)
        return GetView(pFrameView->frame());

    return NULL;
}

EAWEBKIT_API View* CreateView() 
{
	return EAWEBKIT_NEW("View") View();//WTF::fastNew<View>();
}

EAWEBKIT_API void DestroyView(View* pView)
{
	EAWEBKIT_DELETE pView;//WTF::fastDelete<View>(pView);
}

const float PI_4 = 3.14159f / 4.0f;

View::View()
  : mpWebView(0),
    mpSurface(0),
    mViewParameters(),
    mLoadInfo(),
    mCursorPos(0, 0),
    mpModalInputClient(0),
	mOverlaySurfaceArrayContainer(0),
    mLinkHookManager(this),
    mTextInputStateInfo(),
	mDebugger(0),
	mNodeListContainer(0),
	mBestNodeFrame(0),
	mBestNodeX(0),
	mBestNodeY(0),
	mBestNodeWidth(0),
	mBestNodeHeight(0),
	mCachedNavigationUpId(),
	mCachedNavigationDownId(),
	mCachedNavigationLeftId(),
	mCachedNavigationRightId(),
	mURI(),
	mJavascriptBindingObject(0),
    mJavascriptBindingObjectName(),
    mOwnsViewSurface(true),
	mEmulatingConsoleOnPC(false)
{
    ViewArray::GetArray().push_back(this);
	mNodeListContainer = EAWEBKIT_NEW("NodeListContainer") NodeListContainer();//WTF::fastNew<NodeListContainer> ();
	mOverlaySurfaceArrayContainer = EAWEBKIT_NEW("OverlaySurfaceArrayContainer") OverlaySurfaceArrayContainer();//WTF::fastNew<OverlaySurfaceArrayContainer> ();
}

View::~View()
{
    ViewArray::GetArray().erase(eastl::find(ViewArray::GetArray().begin(), ViewArray::GetArray().end(), this));

	if (mDebugger)
	{
		delete mDebugger;
		mDebugger = 0;
	}

	if(mNodeListContainer)
	{
		EAWEBKIT_DELETE mNodeListContainer;//WTF::fastDelete<NodeListContainer>(mNodeListContainer);
		mNodeListContainer = 0;
	}
	if(mOverlaySurfaceArrayContainer)
	{
		EAWEBKIT_DELETE mOverlaySurfaceArrayContainer;//WTF::fastDelete<OverlaySurfaceArrayContainer>(mOverlaySurfaceArrayContainer);
		mOverlaySurfaceArrayContainer = 0;
	}

    View::ShutdownView();
}


bool View::InitView(const ViewParameters& vp)
{
	SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	EAW_ASSERT_MSG(!mpSurface, "Surface pointer not NULL when calling InitView\n");
	EAW_ASSERT_MSG(!mpWebView, "WebView pointer not NULL when calling InitView\n");

	if(mpSurface || mpWebView)//Don't allow reinit of View. Return false instead.
		return false;

	mViewParameters = vp;
    
    if(!mpSurface) 
    {
         bool isExternalSurfaceGood = false;

        // Check if we can use the optional user view surface 
        if(vp.mpViewSurface)
        {
            isExternalSurfaceGood = true;  

            // Inspect the provided external surface...
            // Verify that the size is correct and that we are in ARGB
            if(vp.mpViewSurface->GetPixelFormat().mPixelFormatType != EA::Raster::kPixelFormatTypeARGB)
            {
                isExternalSurfaceGood = false;
                EAW_ASSERT_MSG(isExternalSurfaceGood, "View::Provided surface needs to be in ARGB format. Using default instead.");
            }      
            else if ((vp.mpViewSurface->GetWidth() !=  vp.mWidth) || (vp.mpViewSurface->GetHeight() !=  vp.mHeight) )   
            {
                // Attempt to resize. 
                isExternalSurfaceGood = vp.mpViewSurface->Resize(vp.mWidth, vp.mHeight, false);  
                EAW_ASSERT_MSG(isExternalSurfaceGood, "View::Failed to resize external surface. Using default instead.");
            }
        }
        
        if(!isExternalSurfaceGood)
        {
            // The default: Create a surface using EARaster instance.
            mpSurface = GetEARasterInstance()->CreateSurface(vp.mWidth, vp.mHeight, EA::Raster::kPixelFormatTypeARGB, EA::Raster::kSurfaceCategoryMainView);
            mOwnsViewSurface = true;
        }
        else 
        {
            mpSurface = vp.mpViewSurface;   
            mOwnsViewSurface = false;    
        }   
        
        EAW_ASSERT(mpSurface);
   }

    if(mpSurface)
    {
        mpWebView = WebView::createInstance();

        if(mpWebView)
        {
            const Parameters& parameters = EA::WebKit::GetParameters();

            const WKAL::IntRect   viewRect(0, 0, vp.mWidth, vp.mHeight);
            const WebCore::String frameName(parameters.mpApplicationName ? parameters.mpApplicationName : "EAWebKit");
            const WebCore::String groupName;//10/19/10 - abaldeva - We should probably specify the group name in the same way we do for frameName above.

            mpWebView->parseConfigFile();
            mpWebView->initWithFrame(viewRect, frameName, groupName);
            mpWebView->setViewWindow(mpSurface);
            mpWebView->setProhibitsMainFrameScrolling(!vp.mbScrollingEnabled);//10/19/10 - abaldeva - This should probably stop scrolling for every frame including iframe etc? 
            mpWebView->setTabKeyCyclesThroughElements(vp.mbTabKeyFocusCycle);
            if(parameters.mpUserAgent)
			{
				WebCore::String userAgent(parameters.mpUserAgent);
				
				char eawebkitVersion[32];
				memset(eawebkitVersion,'\0',sizeof(eawebkitVersion));
				snprintf(eawebkitVersion,31," EAWebKit/%s",EAWEBKIT_VERSION);
				userAgent.append(WebCore::String(eawebkitVersion));//There isn't an .append(char*)
				
				mpWebView->setCustomUserAgent(userAgent);
			}

            if(parameters.mpApplicationName)
                mpWebView->setApplicationNameForUserAgent(WebCore::String(parameters.mpApplicationName));

            // We store a pointer to the view 'this' in the Surface.
            mpSurface->SetUserData(this);
            
            mpWebView->setTranparentBackground(vp.mbTransparentBackground); // 7/23/09 CSidhall - Added storing of transparent background
            // Apply the setting to the current frame view.
            if(vp.mbTransparentBackground)
            {
                WebCore::FrameView* pMainFrameView = GetFrameView();
                EAW_ASSERT(pMainFrameView);

                pMainFrameView->setTransparent(vp.mbTransparentBackground);
            }
        }
        else
            Shutdown();
    }

    return (mpSurface != NULL) && (mpWebView != NULL);
}


void View::ShutdownView()
{
	SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	SET_AUTO_COLLECTOR_STACK_BASE();   // 9/4/09 - CSidhall - Added to get approximate stack base

	if (mJavascriptBindingObject)
	{
		delete mJavascriptBindingObject;
        mJavascriptBindingObject = NULL;	
		//abaldeva:Temporarily moved to EAWebKit Shutdown to support multiple views
		//KJS::Bindings::BalClass::cleanup();
	}
	

    SetModalInput(NULL);

    if(mpWebView)
    {
        delete mpWebView;
        mpWebView = NULL;
    }

    if(mpSurface)
    {
        if(mOwnsViewSurface)
            GetEARasterInstance()->DestroySurface(mpSurface);
    
        mpSurface = NULL;
    }
}


void View::GetSize(int& w, int& h) const
{
    if(mpSurface)
    {
        w = mpSurface->GetWidth();
        h = mpSurface->GetHeight();
    }
    else
    {
        w = 0;
        h = 0;
    }
}


bool View::SetSize(int w, int h)
{
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	bool bResult = true;

    // Only recreate surfaces if there is a change.
    if(!mpSurface || (mpSurface->GetWidth() != w) || (mpSurface->GetHeight() != h))
    {
        if(mpSurface)
            bResult = mpSurface->Resize(w, h, false);
        else
        {
            mpSurface = GetEARasterInstance()->CreateSurface(w, h, EA::Raster::kPixelFormatTypeARGB, EA::Raster::kSurfaceCategoryMainView);
            mOwnsViewSurface = true;            
            bResult   = (mpSurface != NULL);
        }
    }

    EAW_ASSERT_MSG(mpWebView, "If this fails, are you calling SetSize before calling InitView?\n");

    if(mpWebView)
    {
        mpWebView->setViewWindow(mpSurface);

        BalResizeEvent resizeEvent = { w, h };
        mpWebView->onResize(resizeEvent);
    }

	return bResult;
}


bool View::SetURI(const char* pURI)
{
    // Note that by design we do not try to 'fix' the URI by doing things like prepending "http://" in front
    // of it. The reason for this is that the user may be using a custom URI scheme that we are not aware of.
    // It is the job of the higher layer to fix URIs if they want to.
	SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	EAW_ASSERT_MSG(mpWebView, "View::SetURI failure. Have you called View::InitView yet?");
	EAW_ASSERT_MSG(pURI && pURI[0], "View::SetURI failure. Incorrect URL");

	
	if(mpWebView && pURI && pURI[0])
    {
        // Notify of a possible focus change - this will shut down popups if active
        if(mpModalInputClient)
                mpModalInputClient->OnFocusChangeEvent(false);  
        
        ::WebFrame* pMainWebFrame = GetWebFrame();
        EAW_ASSERT(pMainWebFrame);

        double timeoutSeconds = (double)EA::WebKit::GetParameters().mPageTimeoutSeconds;
		
        pMainWebFrame->loadURL(pURI, timeoutSeconds);
		
		return true;
    }

	return false; 
}

void View::Refresh()
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
		
	
	if(mpWebView)
	{
		::WebFrame* pMainWebFrame = GetWebFrame();
		EAW_ASSERT(pMainWebFrame);
		pMainWebFrame->reload(); 
	}
}


bool View::LoadResourceRequest(const WebCore::ResourceRequest& resourceRequest)
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if(mpWebView)
    {
		::WebFrame* pMainWebFrame = GetWebFrame();
        EAW_ASSERT(pMainWebFrame);
        pMainWebFrame->loadRequest(resourceRequest);
        
		return true;
    }

    return false;
}
const char16_t* View::GetURI()
{
    WebCore::Frame* pMainFrame = GetFrame();

    if(pMainFrame)
    {
        const WebCore::KURL&   url  = pMainFrame->loader()->url();
        const WebCore::String& sURL = url.string();

        GetFixedString(mURI)->assign(sURL.characters(), sURL.length());
		return GetFixedString(mURI)->c_str();
    }

	return NULL;
  
}

bool View::SetHTML(const char* pHTML, size_t length, const char* pBaseURL)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	return SetContent(pHTML, length, "text/html", "utf-8", pBaseURL);
}


bool View::SetContent(const void* pData, size_t length, const char* pMimeType, const char* pEncoding, const char* pBaseURL)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	WebCore::Frame* pMainFrame = GetFrame();

    if(pMainFrame)
    {
		WebCore::KURL                      kurl(pBaseURL); // It handles the case of pBaseURL == NULL.
        WebCore::ResourceRequest           request(kurl);
        WTF::RefPtr<WebCore::SharedBuffer> buffer(WebCore::SharedBuffer::create((const char*)pData, length));
        const WebCore::String              sMimeType(pMimeType ? pMimeType : "text/html");
        const WebCore::String              sEncoding(pEncoding ? pEncoding : "");
        WebCore::SubstituteData            substituteData(buffer, sMimeType, sEncoding, kurl);

        pMainFrame->loader()->load(request, substituteData);

		return true;
    }

    return false;
}


void View::CancelLoad()
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	WebCore::Frame* pMainFrame = GetFrame();
	if(pMainFrame && pMainFrame->loader())
	{
		pMainFrame->loader()->stopAllLoaders();
	}
}

void View::QueueRegionToDrawUpdate(int x, int y, int width, int height)
{
	const WKAL::IntRect rect(x, y, width, height);
	mpWebView->addToDirtyRegion(rect);

	WebCore::FrameView* const pMainFrameView  = GetFrameView();
	if(pMainFrameView)
		pMainFrameView->setNeedsLayout();
}

bool View::GoBack()
{
	SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	SET_AUTO_COLLECTOR_STACK_BASE();   
    
	bool returnFlag = false;
    if(mpWebView)
    {
        returnFlag =mpWebView->goBack();
        if(returnFlag)
        {
			// Update 10/25/10 - Note by Arpit Baldeva - Fix the problem of page not refreshing correctly when navigated via history mechanism.            
			QueueRegionToDrawUpdate(0, 0, mpSurface->GetWidth(), mpSurface->GetHeight());

            // Notify of a possible focus change - this will shut down popups if active
            if(mpModalInputClient)
                mpModalInputClient->OnFocusChangeEvent(false);  
        }
    }
     return returnFlag;
}


bool View::GoForward()
{
	SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	SET_AUTO_COLLECTOR_STACK_BASE();   
	
	
	bool returnFlag = false;
    if(mpWebView)
    {
        returnFlag = mpWebView->goForward();
        if(returnFlag)
        {
			// Update 10/25/10 - Note by Arpit Baldeva - Fix the problem of page not refreshing correctly when navigated via history mechanism.
			QueueRegionToDrawUpdate(0, 0, mpSurface->GetWidth(), mpSurface->GetHeight());
            
            // Notify of a possible focus change - this will shut down popups if active
            if(mpModalInputClient)
                mpModalInputClient->OnFocusChangeEvent(false);  
        }
    }
    return returnFlag;
}


bool View::CanGoBack(uint32_t count)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if(mpWebView)
        return mpWebView->canGoBack(count);
    return false;
}


bool View::CanGoForward(uint32_t count)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if(mpWebView)
        return mpWebView->canGoForward(count);
    return false;
}


LoadInfo& View::GetLoadInfo()
{
    return mLoadInfo;
}
                          
TextInputStateInfo& View::GetTextInputStateInfo()
{
    return mTextInputStateInfo;
}

static void TranslateJavaScriptValue(KJS::JSValue* pValue, EA::WebKit::JavascriptValue* pReturnValue, ExecState* exec)
{
    if (pValue->isBoolean())
    {
        pReturnValue->SetBooleanValue(pValue->getBoolean());
    }
    else if (pValue->isUndefinedOrNull())
    {
        pReturnValue->SetUndefined();
    }
    else if (pValue->isNumber())
    {
        pReturnValue->SetNumberValue(pValue->uncheckedGetNumber());
    }
    else if (pValue->isString())
    {
        // 5/11/10 - Fix proposed by Chin Yee Cheng for the fixed string expects a null terminated string.
        UString valueStr = pValue->getString();
        GetFixedString(pReturnValue->GetStringValue())->assign(valueStr.data(), valueStr.size());

        pReturnValue->SetStringType();
    }
    else if (pValue->isObject(&JSArray::info))
    {
        // Arrays, handle separately from objects
        const JSArray* pJSArray = static_cast<const JSArray*>(pValue->getObject());
        unsigned arrayLength = pJSArray->getLength();
        VectorJavaScriptValue* pJavaScriptValues = GetVectorJavaScriptValue(pReturnValue->GetArrayValue());
        pJavaScriptValues->resize(arrayLength);

        for (unsigned i = 0; i < arrayLength; ++i)
        {
            JSValue* pItemValue = pJSArray->get(exec, i);
            TranslateJavaScriptValue(pItemValue, &(pJavaScriptValues->at(i)), exec);
        }
        pReturnValue->SetArrayType();
    }
    else if (pValue->isObject())
    {
        JSObject* pJSObject = pValue->getObject();
        HashMapJavaScriptValue* hashMap = GetHashMap(pReturnValue->GetHashMapValue());

        PropertyNameArray propertyNames(exec);
        pJSObject->getPropertyNames(exec, propertyNames);

        PropertyNameArray::const_iterator start = propertyNames.begin();
        PropertyNameArray::const_iterator end = propertyNames.end();
        for (PropertyNameArray::const_iterator iProp = start; iProp != end; ++iProp) {
            JSValue *val = pJSObject->get(exec, *iProp);

            const char16_t *name = iProp->ustring().data();
            JavascriptValue *nativeValue = &(*hashMap)[name];
            TranslateJavaScriptValue(val, nativeValue, exec);
        }

        pReturnValue->SetObjectType();
    }
    else
    {
        pReturnValue->SetUndefined();
    }
    // Do we need to free pValue in some way?
}

bool View::EvaluateJavaScript(const char* pScriptSource, size_t length, EA::WebKit::JavascriptValue* pReturnValue)
{
	// For a more general solution, which we may want to look into, see:
	//     https://lists.webkit.org/pipermail/webkit-dev/2008-November/005686.html
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	SET_AUTO_COLLECTOR_STACK_BASE();   // Need to store collector stack base as this call can bypass the viewTick()

	//We don't do any utf-8 to 16 conversion and assume that the input is ascii-encoded. Otherwise, we may end up with a performance penalty.
	//The caller may choose to use the 16 bit version if non-ascii character support is required.
	const WebCore::String sScriptSource(pScriptSource, length);   
	return EvaluateJavaScript(sScriptSource.characters(),sScriptSource.length(), pReturnValue);
	
}


bool View::EvaluateJavaScript(const char16_t* pScriptSource, size_t length, EA::WebKit::JavascriptValue* pReturnValue)
{
	// For a more general solution, which we may want to look into, see:
	//     https://lists.webkit.org/pipermail/webkit-dev/2008-November/005686.html
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	SET_AUTO_COLLECTOR_STACK_BASE();   // Need to store collector stack base as this call can bypass the viewTick()
	
    // We default the return value to be undefined for now
	if(pReturnValue)
		pReturnValue->SetUndefined();
	
	bool returnFlag = false;
	WebCore::Frame* pMainFrame = GetFrame();//Note 10/19/10 - abaldeva - We need to investigate if we should get the focus Or MainFrame here.
	if(pMainFrame)
	{
		WebCore::ScriptController* pProxy = pMainFrame->script();
		if(pProxy)
		{
			const WebCore::String sFileName;
			const int             baseLine = 1;
			const WebCore::String sScriptSource(pScriptSource, length);

			KJS::JSValue* pValue = pProxy->evaluate(sFileName, baseLine, sScriptSource);

			if(pValue)
			{
				if(pReturnValue)
                {
                    // we need the exec only because JSArray::get requires the exec state. This could potentially be removed.
                    ExecState* exec = pProxy->windowShell()->window()->globalExec();
                    TranslateJavaScriptValue(pValue, pReturnValue, exec);
                }

                // 7/27/10 CSidhall So this is a small API change, we now return true if a non zero pValue was found versus previously where 
                // we only returned true if  pValue was non zero and a pReturnValue was set by the user.  This was suggested by Chin Yee Cheng.
                // So any return with a non NULL pValue is a success.  (It is unclear if there could be some special
                // exceptions to this in case of with "Break" or "Continue" internal JS completion codes but we consider those as fails for now).
                
                returnFlag = true;  
            }
            else
            {
                // No valid return so leave the returnValue to undefined
            }
		
            // 5/12/10 CSidhall - Changed this to just flag the update instead of doing a full layout update as EvaluateJavascript might be called x times in a same frame. 
	        // We could consider only updating the layout in case only of a pValue return
            WebCore::FrameView* const pMainFrameView  = GetFrameView();
	        if(pMainFrameView)
		        pMainFrameView->setNeedsLayout();
        }
	}
	return returnFlag;	
}


bool View::Tick()
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	SET_AUTO_COLLECTOR_STACK_BASE();  

    // Notify tick start callback (this is already known by the user but groups it with other profile calls)
    NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeViewTick, EA::WebKit::kVProcessStatusStarted, this);

	WTF::dispatchFunctionsFromMainThread();

	//Note by Arpit Baldeva: We manually trigger the jobs to be downloaded and requested here instead of
	//relying on the timer callback. As it turns out, this improves our page load by 2X to 3X (PC improvements are around ~3X).
	//One thing to note here is that ideally these calls should be moved to a separate EAWebKit Tick() as following
	//calls are not view specific. 
	WebCore::ResourceHandleManager* pRHM = WebCore::ResourceHandleManager::sharedInstance();
	EAW_ASSERT(pRHM);
	if(pRHM)
	{
		pRHM->TickDownload();
		NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeTransportTick, EA::WebKit::kVProcessStatusStarted, this);
		pRHM->TickTransportHandlers();
		NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeTransportTick, EA::WebKit::kVProcessStatusEnded);
	}

	WebCore::fireTimerIfNeeded();

	//Note by Arpit Baldeva: We can also conditionally decide to bail out instead of doing 2nd update below
	//if it turns out that we end up spending too much time in the Tick() in some case. Currenly, I have not
	//seen such a scenario so keeping it till someone complaints.
	if(pRHM)
	{
		pRHM->TickDownload();
		NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeTransportTick, EA::WebKit::kVProcessStatusStarted);
		pRHM->TickTransportHandlers();
		NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeTransportTick, EA::WebKit::kVProcessStatusEnded, this);
	}



	// Notify Draw start Process callback
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDraw, EA::WebKit::kVProcessStatusStarted, this);
    // Check for a dirty FrameView, and if so then trigger a repaint.
    // The repaint will result in any notifications being sent out.
    WebCore::FrameView*     const pMainFrameView  = GetFrameView();
    EA::WebKit::Parameters& parameters        = EA::WebKit::GetParameters();

    if(pMainFrameView && mpWebView && pMainFrameView->IsDirty()) // If there is anything to draw...
    {
        if(parameters.mbDrawIntermediatePages || !mpWebView->isLoading())  // If we are to draw intermediate pages (as well as completed pages) or if the view has already completed loading...
        {
            
            BalEventExpose exposeEvent;
            mpWebView->onExpose(exposeEvent);
        }
    }
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDraw, EA::WebKit::kVProcessStatusEnded, this);

	// Notify tick end callback (this is already known by the user but groups it with other profile calls)
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeViewTick, EA::WebKit::kVProcessStatusEnded, this);

	return true;
}


void View::RedrawArea(int x, int y, int w, int h)
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if(!mpWebView)
		return;
    
	// This function triggers a WebKit-level redraw the of view (i.e. HTML) in an area.
    if((w == 0) || (h == 0))
    {
        x = 0;
        y = 0;
        w = mpSurface->GetWidth();
        h = mpSurface->GetHeight();
    }

    const WKAL::IntRect rect(x, y, w, h);
    mpWebView->addToDirtyRegion(rect);

    BalEventExpose exposeEvent;
    mpWebView->onExpose(exposeEvent);
}


void View::ViewUpdated(int x, int y, int w, int h)
{
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	// Since the view surface has been updated, we will need to redraw any existing
    // overlay surfaces (e.g. overlaid popup modal menus).
    BlitOverlaySurfaces();

    // 12/10/10 CSidhall - Moved the view update/draw event notifications to onExpose() in WebViewPrivate.cpp to avoid multiple callbacks.
}


void View::Scroll(int x, int y)
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeScrollEvent, EA::WebKit::kVProcessStatusStarted, this);
	
	if(mpWebView)
    {
        mpWebView->scrollBy(WebCore::IntPoint(x, y));
        //abaldeva:TODO - Fix this. Modal Input means that WebKit does not get input events. So it is meant for exclusiveness.
		if(mpModalInputClient)
            mpModalInputClient->OnScrollViewEvent();
    }
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeScrollEvent, EA::WebKit::kVProcessStatusEnded, this);
}

void View::Scroll(bool xAxisScroll, bool yAxisScroll, int xScrollLines, int xScrollDelta,
				  int yScrollLines, int yScrollDelta)
{
	MouseWheelEvent mouseWheelEvent;	
	mouseWheelEvent.mX = mCursorPos.x;
	mouseWheelEvent.mY = mCursorPos.y;
	
	if(yAxisScroll)
	{
		mouseWheelEvent.mbShift = false;
		mouseWheelEvent.mLineDelta = yScrollLines;
		mouseWheelEvent.mZDelta = yScrollDelta;
		OnMouseWheelEvent(mouseWheelEvent);
	}

	if(xAxisScroll)
	{
		mouseWheelEvent.mbShift = true;
		mouseWheelEvent.mLineDelta = -xScrollLines;
		mouseWheelEvent.mZDelta = -xScrollDelta;
		OnMouseWheelEvent(mouseWheelEvent);
	}
	
}

void View::GetScrollOffset(int& x, int& y)
{
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if(mpWebView)
	{
		const WebCore::IntPoint point = mpWebView->scrollOffset();
		x = point.x();
		y = point.y();
	}
	else
	{
		x = 0;
		y = 0;
	}
}


void View::OnKeyboardEvent(const KeyboardEvent& keyboardEvent)
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended); 
    SET_AUTO_COLLECTOR_STACK_BASE();  // Mark stack for Javascript collector
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeKeyBoardEvent, EA::WebKit::kVProcessStatusStarted, this);
	
	if(mpModalInputClient)
        mpModalInputClient->OnKeyboardEvent(keyboardEvent);
    else if(mpWebView)
    {
        if(keyboardEvent.mbDepressed)
            mpWebView->onKeyDown(keyboardEvent);
        else
            mpWebView->onKeyUp(keyboardEvent);
    }

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeKeyBoardEvent, EA::WebKit::kVProcessStatusEnded, this);
}


void View::OnMouseMoveEvent(const MouseMoveEvent& mouseMoveEvent)
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
    SET_AUTO_COLLECTOR_STACK_BASE();  // Mark stack for Javascript collector
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeMouseMoveEvent, EA::WebKit::kVProcessStatusStarted, this);
	
	//+09/15/10 - Note by Arpit Baldeva: Moved it (from few lines below)before firing the event. This is because during the mpWebView->onMouseMotion call, the Cursor::Set is called where we inform the 
	//application about the current cursor position. 
	mCursorPos.set(mouseMoveEvent.mX, mouseMoveEvent.mY);
	
	if(mpModalInputClient)
        mpModalInputClient->OnMouseMoveEvent(mouseMoveEvent);
    else if(mpWebView)
    {
        mpWebView->onMouseMotion(mouseMoveEvent);
    
        //+ 2/5/10 CSidhall - If this was already handled by onMouseMotion, the delta will be 0 so no action
        // will be taken.  If not handled, this will fix the problem of the auto mode of a scroll 
        // thumb staying active (=stuck).
        WKAL::PlatformScrollbar::updateAutoScrollThumbWithMouseMove(mouseMoveEvent);   
    
    }
	//-09/15/10 - Note by Arpit Baldeva: Disabled following with the explanation above.
    //mCursorPos.set(mouseMoveEvent.mX, mouseMoveEvent.mY);

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeMouseMoveEvent, EA::WebKit::kVProcessStatusEnded, this);
}


void View::OnMouseButtonEvent(const MouseButtonEvent& mouseButtonEvent)
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	SET_AUTO_COLLECTOR_STACK_BASE();  // Mark stack for Javascript collector
    NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeMouseButtonEvent, EA::WebKit::kVProcessStatusStarted, this);
	
	if(mpModalInputClient)
        mpModalInputClient->OnMouseButtonEvent(mouseButtonEvent);
    else if(mpWebView)
    {
        if(mouseButtonEvent.mbDepressed)
            mpWebView->onMouseButtonDown(mouseButtonEvent);
        else
        {
            mpWebView->onMouseButtonUp(mouseButtonEvent);

            //+ 2/5/10 CSidhall - If was already handled by onMouseButtonUp, should just return.  If not handled, this
            // will exit the auto mode of a scroll thumb.
            WKAL::PlatformScrollbar::updateAutoScrollThumbWithMouseRelease();   
        }
    }

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeMouseButtonEvent, EA::WebKit::kVProcessStatusEnded, this);

}


void View::OnMouseWheelEvent(const MouseWheelEvent& mouseWheelEvent)
{
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	SET_AUTO_COLLECTOR_STACK_BASE();  // Mark stack for Javascript collector
    NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeMouseWheelEvent, EA::WebKit::kVProcessStatusStarted, this);
		
	if(mpModalInputClient)
        mpModalInputClient->OnMouseWheelEvent(mouseWheelEvent);
    else if(mpWebView)
        mpWebView->onScroll(mouseWheelEvent);

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeMouseWheelEvent, EA::WebKit::kVProcessStatusEnded, this);
}


void View::GetCursorPosition(int& x, int& y) const
{
    x = mCursorPos.x;
    y = mCursorPos.y;
}


//const EA::Raster::Point& View::GetCursorPos() const
//{
//    return mCursorPos;
//}


void View::OnFocusChangeEvent(bool /*bHasFocus*/)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	// To do: We need to find a way to tell the WebView or its ScrollView to not draw highlighting around focus objects.
}


bool View::SetModalInput(ModalInputClient* pModalInputClient)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	if(mpModalInputClient != pModalInputClient)
    {
        if(mpModalInputClient)
            mpModalInputClient->ModalEnd();

        mpModalInputClient = pModalInputClient;

        if(mpModalInputClient)
            mpModalInputClient->ModalBegin();
    }

	return true;
}


ModalInputClient* View::GetModalInputClient() const
{
    return mpModalInputClient;
}


void View::SetOverlaySurface(EA::Raster::ISurface* pSurface, const EA::Raster::Rect& viewRect)
{
	SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	// This function is not re-entrant, as it modifies its data. You cannot safely call it while within a user callback.

    OverlaySurfaceArray::iterator it;

    for(it = mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.begin(); it != mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.end(); ++it)
    {
        if(it->mpSurface == pSurface) // If this surface already exists and is being moved...
        {
            EA::Raster::Rect& rectCurrent = it->mViewRect;

            if(memcmp(&rectCurrent, &viewRect, sizeof(viewRect))) // If there is an in-place change...
            {
                // RedrawArea triggers a WebKit-level redraw of an area.
                // To do: It would be better if we triggered a redraw of only the area
                // exposed by the move, as the new location may overlap the old one.
                RedrawArea(rectCurrent.x, rectCurrent.y, rectCurrent.w, rectCurrent.h);
                rectCurrent = viewRect;
            }

            break;
        }
    }

    if(it == mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.end()) // If it was not already present...
    {
        OverlaySurfaceInfo osi;

        osi.mpSurface = pSurface;
        osi.mViewRect = viewRect;

        mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.push_back(osi);
    }

    // Blit pSurface to viewRect. 
    // We don't need to care about what might be underneath this area, 
    // but we will care about it when the overlay surface is moved or destroyed.
    if(viewRect.w && viewRect.h)
    {
        const EA::Raster::Rect sourceRect(0, 0, viewRect.w, viewRect.h);

        // 2/8/09 CSidhall - Added clip rectangle reset for overlay draw to fix partial draw bug          
        const EA::Raster::Rect savedSurfaceRect(mpSurface->GetClipRect().x, mpSurface->GetClipRect().y,mpSurface->GetClipRect().width(),mpSurface->GetClipRect().height());
        WebCore::FrameView* pFV = GetFrameView();
        if(pFV)
        {    
            WebCore::IntSize scrollOffset = pFV->scrollOffset();
            const EA::Raster::Rect visRect(pFV->contentsX() - scrollOffset.width(), pFV->contentsY() - scrollOffset.height(), pFV->width(), pFV->height());
            mpSurface->SetClipRect(&visRect);
        }

        // Notify user of view draw start event.  
        EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();
        EA::WebKit::ViewUpdateInfo vui = { this, viewRect.x, viewRect.y, viewRect.w, viewRect.h, EA::WebKit::ViewUpdateInfo::kViewDrawStart };
        if(pVN)
        {
            pVN->DrawEvent(vui);
        }    

        GetEARasterInstance()->Blit(pSurface, &sourceRect, mpSurface, &viewRect, NULL);

        // 2/8/09 CSidhall Added rectangle restore (might not be needed but restores everything as it was)
        mpSurface->SetClipRect(&savedSurfaceRect);


        // Notify user of view draw end event.
        if(pVN)
        {
            vui.mDrawEvent = EA::WebKit::ViewUpdateInfo::kViewDrawEnd; 
            pVN->DrawEvent(vui);

            // TODO: Deprecate this view update (replaced with DrawEvent)
            pVN->ViewUpdate(vui);
        }
    }
}


void View::RemoveOverlaySurface(EA::Raster::ISurface* pSurface)
{
	// This could occur during the shutdown sequence with some dependency shutdown issues.
    if(!mOverlaySurfaceArrayContainer)
        return;
    
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	// This function is not re-entrant, as it modifies its data. You cannot safely call it while within a user callback.

    for(OverlaySurfaceArray::iterator it = mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.begin(); it != mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.end(); ++it)
    {
        if(it->mpSurface == pSurface)
        {
            EA::Raster::Rect rect = it->mViewRect;

            mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.erase(it); // Must erase before triggering the redraw.

            // RedrawArea triggers a WebKit-level redraw of an area.
            // To do: It would be better if we triggered a redraw of only the area
            // exposed by the move, as the new location may overlap the old one.
            RedrawArea(rect.x, rect.y, rect.w, rect.h);

            break;
        }
    }

}


// This function (re-)blits any existing overlay surfaces over the view.
// If the underlying page is changing while an overlay surface is present
// then the drawing of the page onto the view will necessitate redrawing 
// the overlay in that area.
void View::BlitOverlaySurfaces()
{
    if(mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.size() <= 0)
        return;
    
    SET_AUTO_ACTIVE_VIEW(this);
    SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
   // 6/20/10 CSidhall - Added clip rectangle reset for overlay draw to fix partial draw bug          
   const EA::Raster::Rect savedSurfaceRect(mpSurface->GetClipRect().x, mpSurface->GetClipRect().y,mpSurface->GetClipRect().width(),mpSurface->GetClipRect().height());
    WebCore::FrameView* pFV = GetFrameView();
    if(pFV)
    {    
        WebCore::IntSize scrollOffset = pFV->scrollOffset();
        const EA::Raster::Rect visRect(pFV->contentsX() - scrollOffset.width(), pFV->contentsY() - scrollOffset.height(), pFV->width(), pFV->height());
        mpSurface->SetClipRect(&visRect);
    } 

    for(OverlaySurfaceArray::iterator it = mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.begin(); it != mOverlaySurfaceArrayContainer->mOverlaySurfaceArray.end(); ++it)
    {
        const OverlaySurfaceInfo& osi = *it;
        const EA::Raster::Rect sourceRect(0, 0, osi.mViewRect.w, osi.mViewRect.h);
        GetEARasterInstance()->Blit(osi.mpSurface, &sourceRect, mpSurface, &osi.mViewRect, NULL);
    }

    // Restore clip
     mpSurface->SetClipRect(&savedSurfaceRect);

}


WebView* View::GetWebView() const
{
    return mpWebView;
}


::WebFrame* View::GetWebFrame(bool focusFrame /* = false */) const
{
	if(mpWebView)
	{
		WebCore::Page* pPage = mpWebView->page();
		EAW_ASSERT(pPage);

		WebCore::FocusController* pFocusController = (pPage ? pPage->focusController() : NULL);
		EAW_ASSERT(pFocusController);

		if(pFocusController)
		{
			WebCore::Frame* pFrame = (focusFrame ? pFocusController->focusedFrame() : pFocusController->getMainFrame());

			::WebFrame* pWebFrame  = (pFrame ? kit(pFrame) : NULL);

			return pWebFrame;
		}
	}

	return NULL;
}

WebCore::Frame* View::GetFrame(bool focusFrame /* = false */) const
{
	::WebFrame* pWebFrame  = GetWebFrame(focusFrame);

	WebCore::Frame* pFrame = (pWebFrame ? pWebFrame->impl() : NULL);

	return pFrame;
}


WebCore::FrameView* View::GetFrameView(bool focusFrame /* = false */) const
{
    if(mpWebView)
    {
        WebCore::Frame* pFrame = GetFrame(focusFrame);

		WebCore::FrameView* pFrameView = (pFrame ? pFrame->view() : NULL);

        return pFrameView;
    }

    return NULL;
}


WebCore::Page* View::GetPage() const
{
    if(mpWebView)
    {
        WebCore::Page* pPage = mpWebView->page();
        EAW_ASSERT(pPage);

        return pPage;
    }

    return NULL;
}


WebCore::Document* View::GetDocument(bool focusFrame /* = false */) const
{
    if(mpWebView)
    {
        WebCore::Frame* pFrame = GetFrame(focusFrame);

		WebCore::Document* pDocument = (pFrame ? pFrame->document() : NULL);

        return pDocument;
    }

    return NULL;
}


EA::Raster::ISurface* View::GetSurface() const
{
    // We could also get this from our FrameView, but we happen to keep our
    // own pointer to this.
    return mpSurface;
}


double View::GetEstimatedProgress() const
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if(mpWebView)
        return mpWebView->estimatedProgress(); // Returns a value in the range of [0.0, 1.0]
    return 0.0;
}

void View::ResetForNewLoad()
{
    mLoadInfo = LoadInfo();
}


// APIs for Navigation

//+ 6/11/09 Note: these following procedures were contributed by Chris Stott and the Skate Team

bool View::IsAlreadyOverNavigableElement()
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if (mpWebView)
	{
		// This works for multiple frames.
		WebCore::Element* element = mpWebView->elementAtPoint(mCursorPos.x, mCursorPos.y);

		if (element)
		{
			IsNodeNavigableDelegate delegate(this);

			ReverseDOMWalker<IsNodeNavigableDelegate> walker(element, delegate);

			return delegate.FoundNode();
		}

	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////
//
bool View::JumpToFirstLink(const char* jumpToClass, bool skipJumpIfAlreadyOverElement)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	WebCore::Frame* pFrame = GetFrame();
	// Search for the desired element in all the frames
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		if (document)
		{
			if (!skipJumpIfAlreadyOverElement)
			{
				JumpToElementWithClassDelegate delegate1(this, jumpToClass);

				DOMWalker<JumpToElementWithClassDelegate> walker(document, delegate1);		
				WebCore::Element* element1 = delegate1.FoundElement();
				if (element1)
				{
					UpdateCachedHints(element1);
					return true;
				}
			}		
		}

		pFrame = pFrame->tree()->traverseNext();
	}

	// We did not find the element, so we jump to the first jumpable link in all the frames
	pFrame = GetFrame();
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);
		
		if(document)
		{
			JumpToFirstLinkDelegate delegate2(this);
			DOMWalker<JumpToFirstLinkDelegate> walker(document, delegate2);		
			WebCore::Element* element2 = delegate2.FoundElement();
			if (element2)
			{
				UpdateCachedHints(element2);
				return true;
			}
		}

		pFrame = pFrame->tree()->traverseNext();
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////
//
bool View::JumpToId(const char* jumpToId)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	WebCore::Frame* pFrame = GetFrame();
	// Search for the desired element in all the frames
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		if (document)
		{
			JumpToElementWithIdDelegate delegate1(this, jumpToId);

			DOMWalker<JumpToElementWithIdDelegate> walker(document, delegate1);		
			WebCore::Element* element = delegate1.FoundElement();
			if (element)
			{
				UpdateCachedHints(element);
				return true;
			}
		}
		pFrame = pFrame->tree()->traverseNext();
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
void View::UpdateCachedHints(WebCore::Node* node)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if (node && node->hasAttributes())
	{
		WebCore::NamedAttrMap* attributeMap = node->attributes();

		WebCore::Attribute* upAttribute = attributeMap->getAttributeItem("navigationup");
		WebCore::Attribute* downAttribute = attributeMap->getAttributeItem("navigationdown");
		WebCore::Attribute* leftAttribute = attributeMap->getAttributeItem("navigationleft");
		WebCore::Attribute* rightAttribute = attributeMap->getAttributeItem("navigationright");

		const size_t kMaxAttribValLength = 256;
		char buffer[kMaxAttribValLength];
		wchar_t uBuffer[kMaxAttribValLength];
		if ( upAttribute )
		{
			unsigned int length = upAttribute->value().length();
			EAW_ASSERT_MSG(length < kMaxAttribValLength-1,"Attribute value buffer not big enough, truncating the string\n"); 
			wcsncpy(uBuffer, upAttribute->value().string().characters(), length);
			uBuffer[length] = L'\0';
			sprintf(buffer,"%S",uBuffer);
			GetFixedString(mCachedNavigationUpId)->assign(buffer);
		}
		else
		{
			GetFixedString(mCachedNavigationUpId)->assign("");
		}
		
		if ( downAttribute )
		{
			unsigned int length = downAttribute->value().length();
			EAW_ASSERT_MSG(length < kMaxAttribValLength-1,"Attribute value buffer not big enough, truncating the string\n"); 
			wcsncpy(uBuffer, downAttribute->value().string().characters(), length);
			uBuffer[length] = L'\0';
			sprintf(buffer,"%S",uBuffer);
			GetFixedString(mCachedNavigationDownId)->assign(buffer);;
		}
		else
		{
			GetFixedString(mCachedNavigationDownId)->assign("");
		}

		if ( leftAttribute )
		{
			unsigned int length = leftAttribute->value().length();
			EAW_ASSERT_MSG(length < kMaxAttribValLength-1,"Attribute value buffer not big enough, truncating the string\n"); 
			wcsncpy(uBuffer, leftAttribute->value().string().characters(), length);
			uBuffer[length] = L'\0';
			sprintf(buffer,"%S",uBuffer);
			GetFixedString(mCachedNavigationLeftId)->assign(buffer);;
		}
		else
		{
			GetFixedString(mCachedNavigationLeftId)->assign("");
		}

		if ( rightAttribute )
		{
			unsigned int length = rightAttribute->value().length();
			EAW_ASSERT_MSG(length < kMaxAttribValLength-1,"Attribute value buffer not big enough, truncating the string\n"); 
			wcsncpy(uBuffer, rightAttribute->value().string().characters(), length);
			uBuffer[length] = L'\0';
			sprintf(buffer,"%S",uBuffer);
			GetFixedString(mCachedNavigationRightId)->assign(buffer);
		}
		else
		{
			GetFixedString(mCachedNavigationRightId)->assign("");
		}
	}
	else
	{
		GetFixedString(mCachedNavigationUpId)->assign("");
		GetFixedString(mCachedNavigationDownId)->assign("");
		GetFixedString(mCachedNavigationLeftId)->assign("");
		GetFixedString(mCachedNavigationRightId)->assign("");
	}
}

//////////////////////////////////////////////////////////////////////////
//
void View::SetJumpNavigationParams(const JumpNavigationParams& jumpNavigationParams)
{
	mJumpNavigationParams = jumpNavigationParams;
}

//////////////////////////////////////////////////////////////////////////
//
bool View::JumpToNearestElement(EA::WebKit::JumpDirection direction)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	//We try to find an element in the current visiable area. If not found, we scroll the view in the intended direction.
	bool foundElement = JumpToNearestElement(direction, true);
	
	// If not found, we try one more time since newer elements may be visible now. In our case, it is necessary to do so(unlike some other implementation where only
	// scroll may happen). Imagine a player navigating through the music track listing and on the edge. If the user presses down, not only he is expecting the new
	// item to become visible, he is also expecting to be able to jump to it.
	if(!foundElement)
		foundElement = JumpToNearestElement(direction, false);

	return foundElement;

	
}


bool View::JumpToNearestElement(EA::WebKit::JumpDirection direction, bool scrollIfElementNotFound)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	// Note by Arpit Baldeva:
	// We have a problem here. mpModalInputClient object is supposed to be used for Modal input only however the only class using this object 
	// is html SELECT element(implemented as a popup). But in reality, html SELECT element is NOT modal. So it is little ill-conceived. 
	// For example, in all the browsers, if you scroll the mouse wheel on the frame, the SELECT element disappears and the actual frame scrolls.

	// For any modal input needs on a web page, the users are advised to use the Z-layer technique with Javascript/CSS - http://jqueryui.com/demos/dialog/#modal-confirmation.

	// The problem we want to solve here is have the SELECT element respond to the controller input correctly(select element one by one).
	// But the button event information is lost by the time we are in the EA::WebKit::View. For the foreseeable future, there is no candidate
	// other than html SELECT element which is implemented as a modal popup inside EAWebKit. So inside EA::WebKit::View, we create a dummy
	// button event from the Jump direction and make SELECT respond to it. If any other object starts using the modal input, this would need to be
	// revisited. But then, we'll need to solve a plethora of issues. So we do minimum work here to not break other things.

	bool handledByModalInputClient = false;
	if(mpModalInputClient)
	{
		EA::WebKit::ButtonEvent btnEvent;
		switch(direction)
		{
			/*
			case EA::WebKit::JumpLeft:
			{
			btnEvent.mID = EA::WebKit::kButton0;
			handledByModalInputClient = mpModalInputClient->OnButtonEvent(btnEvent);
			}
			*/
		case EA::WebKit::JumpUp:
			{
				btnEvent.mID = EA::WebKit::kButton1;
				handledByModalInputClient =  mpModalInputClient->OnButtonEvent(btnEvent);
				break;
			}
			/*
			case EA::WebKit::JumpRight:
			{
			btnEvent.mID = EA::WebKit::kButton2;
			handledByModalInputClient =  mpModalInputClient->OnButtonEvent(btnEvent);
			}
			*/
		case EA::WebKit::JumpDown:
			{
				btnEvent.mID = EA::WebKit::kButton3;
				handledByModalInputClient =  mpModalInputClient->OnButtonEvent(btnEvent);
				break;
			}
		default:
			// We don't return and allow any other button press to go to the main View. At the same time, we make the SELECT element lose focus.
			{
				mpModalInputClient->OnFocusChangeEvent(false);
				break;
			}
		}
	}

	if(handledByModalInputClient)
		return true;

	if(!mpWebView)
	{
		return false;
	}

	int lastX		= mCursorPos.x;
	int lastY		= mCursorPos.y;

	// Following is a shortcut to drive navigation from a page.
	switch (direction)
	{
	case EA::WebKit::JumpRight:
		if (GetFixedString(mCachedNavigationRightId)->compare(""))
		{
			if (!GetFixedString(mCachedNavigationRightId)->compare("ignore"))
			{
				return false;
			}

			if (JumpToId(GetFixedString(mCachedNavigationRightId)->c_str()))
			{
				return true;
			}
		}
		break;

	case EA::WebKit::JumpDown:
		if (GetFixedString(mCachedNavigationDownId)->compare(""))
		{
			if (!GetFixedString(mCachedNavigationDownId)->compare("ignore"))
			{
				return false;
			}

			if (JumpToId(GetFixedString(mCachedNavigationDownId)->c_str()))
			{
				return true;
			}
		}
		break;

	case EA::WebKit::JumpLeft:
		if (GetFixedString(mCachedNavigationLeftId)->compare(""))
		{
			if (!GetFixedString(mCachedNavigationLeftId)->compare("ignore"))
			{
				return false;
			}

			if (JumpToId(GetFixedString(mCachedNavigationLeftId)->c_str()))
			{
				return true;
			}
		}
		break;

	case EA::WebKit::JumpUp:
		if (GetFixedString(mCachedNavigationUpId)->compare(""))
		{
			if (!GetFixedString(mCachedNavigationUpId)->compare("ignore"))
			{
				return false;
			}

			if (JumpToId(GetFixedString(mCachedNavigationUpId)->c_str()))
			{
				return true;
			}
		}
		break;

	default:
		EAW_FAIL_MSG("Should not have got here\n");
	}


	// Iterate over all the frames and find the closest element in any of all the frames.
	WebCore::Frame* pFrame		= GetFrame();
	float currentRadialDistance = FLT_MAX; // A high value to start with so that the max distance between any two elements in the surface is under it.
	WebCore::Node* currentBestNode = NULL;
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		if(document)
		{
			WebCore::FrameView* pFrameView = document->view();
			WebCore::IntPoint scrollOffset;
			if(pFrameView)
			{
				scrollOffset.setX(pFrameView->scrollOffset().width());
				scrollOffset.setY(pFrameView->scrollOffset().height());
			}

			// We figure out the start position(It is center of the currently hovered element almost all the time but can be slightly different 
			// due to scroll sometimes).
			mCentreX = lastX + scrollOffset.x();
			mCentreY = lastY + scrollOffset.y();

			DocumentNavigator navigator(this, document, direction, WebCore::IntPoint(mCentreX, mCentreY), mBestNodeX, mBestNodeY, mBestNodeWidth, mBestNodeHeight, mJumpNavigationParams.mNavigationTheta, mJumpNavigationParams.mStrictAxesCheck, currentRadialDistance);
			navigator.FindBestNode(document);

			if(navigator.GetBestNode())
			{
				currentBestNode			= navigator.GetBestNode();
				currentRadialDistance	= navigator.GetBestNodeRadialDistance();
			}

		}

		pFrame = pFrame->tree()->traverseNext();
	}

	bool foundSomething = false;
	if (currentBestNode) //We found the node to navigate. Move the cursor and we are done.
	{
		foundSomething = true;
		MoveMouseCursorToNode(currentBestNode, false);
		UpdateCachedHints(currentBestNode);
	}
	else if(scrollIfElementNotFound)// Node is not found. 
	{
		// Based on the intended direction of movement, scroll so that some newer elements are visible.
		
		int cursorPosBeforeScrollX, cursorPosBeforeScrollY;
		cursorPosBeforeScrollX = mCursorPos.x;
		cursorPosBeforeScrollY = mCursorPos.y;

		switch(direction)
		{
		case EA::WebKit::JumpDown:
			{
				ScrollOnJump(true, -mJumpNavigationParams.mNumLinesToAutoScroll);
				break;
			}

		case EA::WebKit::JumpUp:
			{
				ScrollOnJump(true, mJumpNavigationParams.mNumLinesToAutoScroll);
				break;
			}
		case EA::WebKit::JumpRight:
			{
				ScrollOnJump(false, -mJumpNavigationParams.mNumLinesToAutoScroll);
				break;
			}
		case EA::WebKit::JumpLeft:
			{
				ScrollOnJump(false, mJumpNavigationParams.mNumLinesToAutoScroll);
				break;
			}
		default:
			{
				EAW_ASSERT_MSG(false, "Should not reach here\n");
			}
		}

		// We move the mouse cursor back to the location where the last best node was found. This is so that we don't end up with the cursor being in no man's land. While that may work 
		// for ordinary sites, it may not work well with customized pages that leverage CSS to visually indicate current position rather than a cursor graphic.
		// We don't call MoveMouseCursorToNode() with last cached node as there are edge cases where we may be holding an invalid node. Using a cached frame and checking against the
		// current valid frames safeguards against that.

		WebCore::IntSize scrollOffset;
		WebCore::Frame* pFrame1	= GetFrame();
		while(pFrame1)
		{
			if(pFrame1 == mBestNodeFrame)//Find the frame where last best node existed.
			{
				if(pFrame1->view())
				{
					scrollOffset = pFrame1->view()->scrollOffset();//We read scroll offset here as it could have changed in the switch statement above.
					break;
				}
			}
			pFrame1 = pFrame1->tree()->traverseNext();
		}
		
		int targetcursorPosAfterScrollX, targetcursorPosAfterScrollY;
		targetcursorPosAfterScrollX = mBestNodeX + mBestNodeWidth / 2 - scrollOffset.width();
		targetcursorPosAfterScrollY = mBestNodeY + mBestNodeHeight/ 2 - scrollOffset.height();

		EA::WebKit::MouseMoveEvent moveEvent;
		memset( &moveEvent, 0, sizeof(moveEvent) );
		
		const int cursorInset = 5;// Make cursor stay inside 5 pixels from boundaries. No known issues but added this as a safety measure so that we do not lose cursor ever.
		
		moveEvent.mX	= Clamp( cursorInset, targetcursorPosAfterScrollX, GetSurface()->GetWidth() - cursorInset );
		moveEvent.mY	= Clamp( cursorInset, targetcursorPosAfterScrollY, GetSurface()->GetHeight() - cursorInset );

		moveEvent.mDX	= moveEvent.mX - cursorPosBeforeScrollX;
		moveEvent.mDY	= moveEvent.mY - cursorPosBeforeScrollY;

		OnMouseMoveEvent(moveEvent);
		// We intentionally don't call JumpToNearestElement(direction, false) here to avoid recursion. We do it in the overloaded function above.
	}
		

#if EAWEBKIT_ENABLE_JUMP_NAVIGATION_DEBUGGING
	mNodeListContainer->mFoundNodes = navigator.mNodeListContainer->mFoundNodes;
	mNodeListContainer->mRejectedByAngleNodes = navigator.mNodeListContainer->mRejectedByAngleNodes;
	mNodeListContainer->mRejectedByRadiusNodes = navigator.mNodeListContainer->mRejectedByRadiusNodes;
	mNodeListContainer->mRejectedWouldBeTrappedNodes = navigator.mNodeListContainer->mRejectedWouldBeTrappedNodes;

	mAxesX = navigator.mAxesX;
	mAxesY = navigator.mAxesY;

	// this is fairly arbitrary
	const uint32_t axisLength = 500;

	switch (direction)
	{
	case EA::WebKit::JumpRight:
		mMinX = mCentreX + (int)(axisLength*cos(mJumpNavigationParams.mNavigationTheta));
		mMinY = mCentreY - (int)(axisLength*sin(mJumpNavigationParams.mNavigationTheta));

		mMaxX = mCentreX + (int)(axisLength*cos(mJumpNavigationParams.mNavigationTheta));
		mMaxY = mCentreY + (int)(axisLength*sin(mJumpNavigationParams.mNavigationTheta));
		break;

	case EA::WebKit::JumpDown:
		mMinX = mCentreX + (int)(axisLength*sin(mJumpNavigationParams.mNavigationTheta));
		mMinY = mCentreY + (int)(axisLength*cos(mJumpNavigationParams.mNavigationTheta));

		mMaxX = mCentreX - (int)(axisLength*sin(mJumpNavigationParams.mNavigationTheta));
		mMaxY = mCentreY + (int)(axisLength*cos(mJumpNavigationParams.mNavigationTheta));
		break;

	case EA::WebKit::JumpLeft:
		mMinX = mCentreX - (int)(axisLength*cos(mJumpNavigationParams.mNavigationTheta));
		mMinY = mCentreY + (int)(axisLength*sin(mJumpNavigationParams.mNavigationTheta));

		mMaxX = mCentreX - (int)(axisLength*cos(mJumpNavigationParams.mNavigationTheta));
		mMaxY = mCentreY - (int)(axisLength*sin(mJumpNavigationParams.mNavigationTheta));
		break;

	case EA::WebKit::JumpUp:
		mMinX = mCentreX - (int)(axisLength*sin(mJumpNavigationParams.mNavigationTheta));
		mMinY = mCentreY - (int)(axisLength*cos(mJumpNavigationParams.mNavigationTheta));

		mMaxX = mCentreX + (int)(axisLength*sin(mJumpNavigationParams.mNavigationTheta));
		mMaxY = mCentreY - (int)(axisLength*cos(mJumpNavigationParams.mNavigationTheta));

		break;

	default:
		EAW_FAIL_MSG("Should not have got here\n");
	}

#endif	

	return foundSomething;


}
#if EAWEBKIT_ENABLE_JUMP_NAVIGATION_DEBUGGING		

//////////////////////////////////////////////////////////////////////////
//
void View::DrawBestNode(DrawNodeCallback callback)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if (mBestNodeWidth > 0 && mBestNodeHeight > 0)
	{
		callback(mBestNodeX,mBestNodeY,mBestNodeWidth,mBestNodeHeight, mCentreX, mCentreY);
	}
}

//////////////////////////////////////////////////////////////////////////
//
void View::DrawFoundNodes(DrawNodeCallback callback)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	for (WebCoreNodeListIterator i = mNodeListContainer->mFoundNodes.begin(); i != mNodeListContainer->mFoundNodes.end(); ++i)
	{
		WebCore::Node* node = *i;

		if (node && node->isHTMLElement())
		{
			WebCore::HTMLElement* element = (WebCore::HTMLElement*)node;

			WebCore::IntRect rect = element->getRect();
			
			WebCore::FrameView* pFrameView = element->document()->view(); //Can be NULL
			if(pFrameView)
			{
				rect.setX(rect.x() + pFrameView->x() - pFrameView->scrollOffset().width());
				rect.setY(rect.y() + pFrameView->y() - pFrameView->scrollOffset().height());
			}
	
			callback(rect.x(), rect.y(),rect.width(),rect.height(), mCentreX, mCentreY);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
void View::DrawRejectedByRadiusNodes(DrawNodeCallback callback)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	for (WebCoreNodeListIterator i = mNodeListContainer->mRejectedByRadiusNodes.begin(); i != mNodeListContainer->mRejectedByRadiusNodes.end(); ++i)
	{
		WebCore::Node* node = *i;

		if (node && node->isHTMLElement())
		{
			WebCore::HTMLElement* element = (WebCore::HTMLElement*)node;

			WebCore::IntRect rect = element->getRect();
			
			WebCore::FrameView* pFrameView = element->document()->view(); //Can be NULL
			if(pFrameView)
			{
				rect.setX(rect.x() + pFrameView->x() - pFrameView->scrollOffset().width());
				rect.setY(rect.y() + pFrameView->y() - pFrameView->scrollOffset().height());
			}

			callback(rect.x(), rect.y(),rect.width(),rect.height(), mCentreX, mCentreY);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
void View::DrawRejectedByAngleNodes(DrawNodeCallback callback)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	for (WebCoreNodeListIterator i = mNodeListContainer->mRejectedByAngleNodes.begin(); i != mNodeListContainer->mRejectedByAngleNodes.end(); ++i)
	{
		WebCore::Node* node = *i;

		if (node && node->isHTMLElement())
		{
			WebCore::HTMLElement* element = (WebCore::HTMLElement*)node;

			WebCore::IntRect rect = element->getRect();
			
			WebCore::FrameView* pFrameView = element->document()->view(); //Can be NULL
			if(pFrameView)
			{
				rect.setX(rect.x() + pFrameView->x() - pFrameView->scrollOffset().width());
				rect.setY(rect.y() + pFrameView->y() - pFrameView->scrollOffset().height());
			}

			callback(rect.x(), rect.y(),rect.width(),rect.height(), mCentreX, mCentreY);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
void View::DrawRejectedWouldBeTrappedNodes(DrawNodeCallback callback)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	for (WebCoreNodeListIterator::iterator i = mNodeListContainer->mRejectedWouldBeTrappedNodes.begin(); i != mNodeListContainer->mRejectedWouldBeTrappedNodes.end(); ++i)
	{
		WebCore::Node* node = *i;

		if (node && node->isHTMLElement())
		{
			WebCore::HTMLElement* element = (WebCore::HTMLElement*)node;

			WebCore::IntRect rect = element->getRect();
			
			WebCore::FrameView* pFrameView = element->document()->view(); //Can be NULL
			if(pFrameView)
			{
				rect.setX(rect.x() + pFrameView->x() - pFrameView->scrollOffset().width());
				rect.setY(rect.y() + pFrameView->y() - pFrameView->scrollOffset().height());
			}

			callback(rect.x(), rect.y(),rect.width(),rect.height(), mCentreX, mCentreY);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
void View::DrawSearchAxes(DrawAxesCallback callback)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	callback(mAxesX, mAxesY, mMinX, mMinY, mMaxX, mMaxY);
}

//////////////////////////////////////////////////////////////////////////
//
#endif
void View::EnterTextIntoSelectedInput(const char16_t* text, bool textWasAccepted)
{
	SET_AUTO_ACTIVE_VIEW(this);
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	WebCore::Frame* pFrame = GetFrame();

	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		WebCore::Node* node = document->focusedNode();
		if (node && node->isHTMLElement())
		{
			//+ 8/12/09 CSidhall - Activated text input             
			if(node->hasTagName(WebCore::HTMLNames::inputTag)) 
			{
				WebCore::HTMLInputElement* pInputElement = static_cast<WebCore::HTMLInputElement*> (node);

				if(textWasAccepted && pInputElement->isTextField())
				{
					pInputElement->setValue(text);                   
				}
				pInputElement->blur();//We want to blur even if the text was not accepted and only if the element was an editable field.
			}
			else if(node->hasTagName(WebCore::HTMLNames::textareaTag))
			{
				WebCore::HTMLTextAreaElement* pTextAreaElement = static_cast<WebCore::HTMLTextAreaElement*> (node);

				if(textWasAccepted)
				{
					pTextAreaElement->setValue(text);                   
				}
				pTextAreaElement->blur();//We want to blur even if the text was not accepted and only if the element was an editable field.
			}
			//-CS

			break;
		}		
		
		pFrame = pFrame->tree()->traverseNext();
	}
}


void View::EnterTextIntoSelectedInput(const char8_t* text, bool textWasAccepted)
{
	EAW_ASSERT_MSG(false, "This is deprecated and should not be used");
	return;

	//We don't do any utf-8 to 16 conversion and assume that the input is ascii-encoded. Otherwise, we may end up with a performance penalty.
	//The caller may choose to use the 16 bit version if non-ascii character support is required.
	WebCore::String textStr(text);   
	return EnterTextIntoSelectedInput(textStr.charactersWithNullTermination(),textWasAccepted);

}

WebCore::String GetTextFromSelectedInputAsString(EA::WebKit::View* view, bool contentText)
{
	WebCore::Frame* pFrame = view->GetFrame();
	
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		if(document)
		{
			WebCore::Node* node = document->focusedNode();
			if( node && node->isHTMLElement()) 
			{
				if(node->hasTagName( WebCore::HTMLNames::inputTag)) 
				{
					WebCore::HTMLInputElement* pInputElement = static_cast<WebCore::HTMLInputElement*> (node);
					if(pInputElement->isTextField())
					{
						if(contentText)
						{
							return pInputElement->value();                
						}
						else
						{
							return pInputElement->getAttribute("title").string();
						}
					}
				}
				else if(node->hasTagName(WebCore::HTMLNames::textareaTag))
				{
					WebCore::HTMLTextAreaElement* pTextAreaElement = static_cast<WebCore::HTMLTextAreaElement*> (node);
					if(contentText)
					{
						return pTextAreaElement->value();
					}
					else
					{
						return pTextAreaElement->getAttribute("title").string();
					}
				}
				
				break;
			}
		}


		pFrame = pFrame->tree()->traverseNext();
	}
	
	return WebCore::String();
}

//+ 8/12/09 CSidhall - Added to get the input text from a field
uint32_t View::GetTextFromSelectedInput(char8_t* contentTextBuffer, const uint32_t maxcontentTextBufferLength, char8_t* titleTextBuffer, const uint32_t maxTitleTextBufferLength)
{
	EAW_ASSERT_MSG(false, "This is deprecated and should not be used.");
	return 0;

	SET_AUTO_ACTIVE_VIEW(this);
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	uint32_t size = 0;
	if(contentTextBuffer && maxcontentTextBufferLength)
	{
		WebCore::String val = GetTextFromSelectedInputAsString(this, true);
		if(!val.isEmpty())                
		{
			size = val.length() + 1;    // + 1 For terminator 
			const UChar* pSource = val.charactersWithNullTermination();
			EAW_ASSERT_MSG(size <=maxcontentTextBufferLength, "Truncating the string as buffer length is not sufficient\n");
			EA::Internal::Strlcpy(contentTextBuffer, pSource, maxcontentTextBufferLength, size);
			contentTextBuffer[maxcontentTextBufferLength-1] = 0;
		}
	}
	return size;    
}
//- CS

uint32_t View::GetTextFromSelectedInput(char16_t* contentTextBuffer, const uint32_t maxcontentTextBufferLength, char16_t* titleTextBuffer, const uint32_t maxTitleTextBufferLength)
{
	SET_AUTO_ACTIVE_VIEW(this);
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	uint32_t size = 0;
	if(titleTextBuffer && maxTitleTextBufferLength)
	{
		WebCore::String val = GetTextFromSelectedInputAsString(this,false);
		if(!val.isEmpty())                
		{
			size = val.length() + 1;    // + 1 For terminator 
			const UChar* pSource = val.charactersWithNullTermination();
			EAW_ASSERT_MSG(size <=maxTitleTextBufferLength, "Truncating the string as buffer length is not sufficient\n");
			EA::Internal::Strlcpy(titleTextBuffer, pSource, maxTitleTextBufferLength);
			titleTextBuffer[maxTitleTextBufferLength-1] = 0;
		}
		else
		{
			titleTextBuffer[0] = 0;
		}
	}
	
	size = 0;
	if(contentTextBuffer && maxcontentTextBufferLength)
	{
		WebCore::String val = GetTextFromSelectedInputAsString(this,true);
		if(!val.isEmpty())                
		{
			size = val.length() + 1; 
			const UChar* pSource = val.charactersWithNullTermination();
			EAW_ASSERT_MSG(size <=maxcontentTextBufferLength, "Truncating the string as buffer length is not sufficient\n");
			EA::Internal::Strlcpy(contentTextBuffer, pSource, maxcontentTextBufferLength);
			contentTextBuffer[maxcontentTextBufferLength-1] = 0;
		}
		else
		{
			contentTextBuffer[0] = 0;
		}
	}

	//Returning size is not so useful as we don't allow embedded NULL and now we have two buffers passed in. For now, we return the size of the content to be backward compatible.
	return size;    
}

//////////////////////////////////////////////////////////////////////////
//
void View::AdvanceFocus(EA::WebKit::FocusDirection direction, const EA::WebKit::KeyboardEvent& event)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	EAW_ASSERT(GetFrame());
	EAW_ASSERT(GetFrame()->page());
	EAW_ASSERT(GetFrame()->page()->focusController());

	GetFrame()->page()->focusController()->advanceFocus((WebCore::FocusDirection)direction, 0);//Note 10/19/10 - abaldeva - We need to investigate if we should get the focus Or MainFrame here.

	MoveMouseCursorToFocusElement();
}



//////////////////////////////////////////////////////////////////////////
//
// 
void View::MoveMouseCursorToFocusElement()
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	WebCore::Frame* pFrame = GetFrame();

	while (pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);
		if (document)
		{
			if (WebCore::Node* node = document->focusedNode())
			{
				MoveMouseCursorToNode(node);
				break;
			}
		}

		pFrame = pFrame->tree()->traverseNext();
	}
}

void View::ScrollOnJump(bool vertical, float numLinesDelta)
{
	EA::WebKit::MouseWheelEvent mouseWheelEvent;
	GetCursorPosition( mouseWheelEvent.mX, mouseWheelEvent.mY );
	
	if(vertical)
		mouseWheelEvent.mbShift = false;
	else
		mouseWheelEvent.mbShift = true;

	mouseWheelEvent.mZDelta = 1; //This is not being used on the (EA)WebKit side.
	mouseWheelEvent.mLineDelta = numLinesDelta;
	OnMouseWheelEvent(mouseWheelEvent);
}
//////////////////////////////////////////////////////////////////////////
//
void View::MoveMouseCursorToNode(WebCore::Node* node, bool scrollIfNecessary)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if (mpWebView && node)
	{
		WebCore::HTMLElement* element = (WebCore::HTMLElement*)node;

		WebCore::IntRect rect = element->getRect();
		WebCore::IntPoint frameOffset;
		WebCore::IntPoint scrollOffset;
		
		WebCore::FrameView* pFrameView = element->document()->view(); //Can be NULL
		if(pFrameView)
		{
			//Use move here instead of setX/setY as it results in 1 call instead of two and takes advantage that ctor sets x,y to 0.
			frameOffset.move(pFrameView->x(), pFrameView->y());
			scrollOffset.move(pFrameView->scrollOffset().width(), pFrameView->scrollOffset().height());
		}
		
		
		// This will be true if this function is called from anywhere except JumpToNearestElement(). This enables us to not lose the cursor during
		// arbitrary jumping of elements from either code or webpage using the Navigation APIs.
		if(scrollIfNecessary)
		{
			WebCore::IntPoint targetCursorLocation(frameOffset.x()+rect.x()+rect.width()/2  - scrollOffset.x(), frameOffset.y()+rect.y()+rect.height()/2  - scrollOffset.y());
			// Added 1 in all the line delta below resulting in a better visual behavior when the element happens to be at the edge.
			if(targetCursorLocation.y() > GetSurface()->GetHeight())
			{
				float numLinesDelta = -(((targetCursorLocation.y() - mCursorPos.y)/LINE_STEP)+1);
				ScrollOnJump(true, numLinesDelta);
			}
			if(targetCursorLocation.y()< 0.0f)
			{
				float numLinesDelta = (((mCursorPos.y - targetCursorLocation.y())/LINE_STEP)+1);
				ScrollOnJump(true, numLinesDelta);
			}
			if(targetCursorLocation.x() > GetSurface()->GetWidth())
			{
				float numLinesDelta = -(((targetCursorLocation.x() - mCursorPos.x)/LINE_STEP)+1);
				ScrollOnJump(false, numLinesDelta);
			}
			if(targetCursorLocation.x()< 0.0f)
			{
				float numLinesDelta = (((mCursorPos.x - targetCursorLocation.x())/LINE_STEP)+1);
				ScrollOnJump(false, numLinesDelta);
			}
													
			// Read the scroll offset again as it may have changed.
			if(pFrameView) 
			{
				scrollOffset.setX(pFrameView->scrollOffset().width());
				scrollOffset.setY(pFrameView->scrollOffset().height());
			}
		}

		mBestNodeFrame	= element->document()->frame(); //Can be NULL
		mBestNodeX		= frameOffset.x()  + rect.x();
		mBestNodeY		= frameOffset.y()  + rect.y();
		mBestNodeWidth	= rect.width();
		mBestNodeHeight = rect.height();
		
		int lastX		= mCursorPos.x;
		int lastY		= mCursorPos.y;
		
		int newX		= mBestNodeX + rect.width()/2  - scrollOffset.x();
		int newY		= mBestNodeY + rect.height()/2 - scrollOffset.y();

		EA::WebKit::MouseMoveEvent moveEvent;
		memset( &moveEvent, 0, sizeof(moveEvent) );
		moveEvent.mDX	= newX-lastX;
		moveEvent.mDY	= newY-lastY;
		const int cursorInset = 5;// Make cursor stay inside 5 pixels from boundaries. No known issues but added this as a safety measure so that we do not lose cursor ever.
		moveEvent.mX	= Clamp( cursorInset, newX, GetSurface()->GetWidth() - cursorInset );
		moveEvent.mY	= Clamp( cursorInset, newY, GetSurface()->GetHeight() - cursorInset );

		this->OnMouseMoveEvent( moveEvent );
	}
}


void View::SetCursorPosition(int x, int y)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	{
		int lastX		= 0;
		int lastY		= 0;
		this->GetCursorPosition( lastX, lastY );

		int newX		= x;
		int newY		= y;

		EA::WebKit::MouseMoveEvent moveEvent;
		memset( &moveEvent, 0, sizeof(moveEvent) );
		moveEvent.mDX	= newX-lastX;
		moveEvent.mDY	= newY-lastY;
		moveEvent.mX	= Clamp( 0, newX, this->GetSurface()->GetWidth() );
		moveEvent.mY	= Clamp( 0, newY, this->GetSurface()->GetHeight() );
		this->OnMouseMoveEvent( moveEvent );
	}
}

bool View::IsEmulatingConsoleOnPC() const
{
	return mEmulatingConsoleOnPC;
}

void View::SetEmulatingConsoleOnPC(bool emulatingConsoleOnPC)
{
	mEmulatingConsoleOnPC = emulatingConsoleOnPC;
}

//////////////////////////////////////////////////////////////////////////
//
void View::AttachEventsToInputs(KeyboardCallback callback)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	WebCore::Frame* pFrame = GetFrame();

	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		if (document)
		{
			InputDelegate id(callback);
			TextAreaDelegate tad(callback);

			DOMWalker<InputDelegate> walker(document, id);		
			DOMWalker<TextAreaDelegate> walker2(document, tad);		
		}

		pFrame = pFrame->tree()->traverseNext();
	}
	
	
}

void View::AttachJavascriptDebugger()
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

}




//////////////////////////////////////////////////////////////////////////
//
void View::AttachEventToElementBtId(const char* id, KeyboardCallback callback)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	EAW_ASSERT(id);

	WebCore::Frame* pFrame = GetFrame();

	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		if (document)
		{
			WebCore::Element* element = document->getElementById(id);
			if (element)
			{
				KeyboardEventListener* listener = new KeyboardEventListener(callback, (WebCore::HTMLElement*)element);
				element->setHTMLEventListener(WebCore::eventNames().clickEvent, WTF::adoptRef(listener) );

				break;
			}
		}
		pFrame = pFrame->tree()->traverseNext();
	}


	
}

//////////////////////////////////////////////////////////////////////////
//
void View::SetElementText(WebCore::HTMLElement* htmlElement, const char* text)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	EAW_ASSERT(htmlElement);
	EAW_ASSERT(text);

	WebCore::ExceptionCode error = 0;
	htmlElement->setInnerHTML(text, error);
	
}

//////////////////////////////////////////////////////////////////////////
//
void View::SetInputElementValue(WebCore::HTMLElement* htmlElement, char16_t* text)
{	
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	EAW_ASSERT(htmlElement);
	EAW_ASSERT(text);

	if (htmlElement->tagName() == "INPUT")
	{
		WebCore::HTMLInputElement* inputElement = (WebCore::HTMLInputElement*)htmlElement;

		inputElement->setValue(text);
		inputElement->blur();
	}
	else if (htmlElement->tagName() == "TEXTAREA")
	{
		WebCore::HTMLTextAreaElement* inputElement = (WebCore::HTMLTextAreaElement*)htmlElement;

		inputElement->setValue(text);
		inputElement->blur();
	}

	
}

//////////////////////////////////////////////////////////////////////////
//
void View::SetElementTextById(const char* id, const char* text)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	EAW_ASSERT(id);
	EAW_ASSERT(text);

	WebCore::Frame* pFrame = GetFrame();
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);
	
		if (document)
		{
			WebCore::Element* element = document->getElementById(id);
			if (element && element->isHTMLElement())
			{
				WebCore::HTMLElement* htmlElement = (WebCore::HTMLElement*)element;

				WebCore::ExceptionCode error = 0;
				htmlElement->setInnerHTML(text, error);

				break;
			}
		}

		pFrame = pFrame->tree()->traverseNext();
	}
}

//////////////////////////////////////////////////////////////////////////
//
bool View::ClickElementById(const char* id)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   

	EAW_ASSERT(id);
	if(!id || !id[0])
		return false;

	if(mpModalInputClient)
		mpModalInputClient->OnFocusChangeEvent(false);

	
	bool elementClicked = false;

	WebCore::Frame* pFrame = GetFrame();
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		if (document)
		{
			WebCore::Element* element = document->getElementById(id);

			if (element && element->isHTMLElement())
			{    
				WebCore::HTMLElement* htmlElement = (WebCore::HTMLElement*)element;
				htmlElement->click();
				elementClicked = true;
			}
		}

		if(elementClicked)
			break;

		pFrame = pFrame->tree()->traverseNext();
	}
	
    return false;
}

//////////////////////////////////////////////////////////////////////////
//

//////////////////////////////////////////////////////////////////////////
//
bool View::ClickElementsByClass(const char* className)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	EAW_ASSERT(className);
	if(!className || !className[0])
		return false;

	if(mpModalInputClient)
		mpModalInputClient->OnFocusChangeEvent(false);

	
	bool elementClicked = false;
	
	WebCore::Frame* pFrame = GetFrame();
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		ClickElementsByIdOrClassDelegate delegate(className, false);
		if (document)
		{
			DOMWalker<ClickElementsByIdOrClassDelegate> walker(document, delegate);	
		}

		elementClicked = delegate.GetReturnValue();

		if(elementClicked)
			break;

		pFrame = pFrame->tree()->traverseNext();
	}

	return elementClicked;
}

//////////////////////////////////////////////////////////////////////////
//
bool View::ClickElementsByIdOrClass(const char* idOrClassName)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	EAW_ASSERT(idOrClassName);
	if(!idOrClassName || !idOrClassName[0])
		return false;
	
	if(mpModalInputClient)
		mpModalInputClient->OnFocusChangeEvent(false);

	bool elementClicked = false;

	WebCore::Frame* pFrame = GetFrame();
	while(pFrame)
	{
		WebCore::Document* document = pFrame->document();
		EAW_ASSERT(document);

		ClickElementsByIdOrClassDelegate delegate(idOrClassName, true);
		DOMWalker<ClickElementsByIdOrClassDelegate> walker(document, delegate);	
		elementClicked = delegate.GetReturnValue();

		if(elementClicked)
			break;

		pFrame = pFrame->tree()->traverseNext();
	}

	return elementClicked;
}

bool View::Click()
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	//Do not deal with modal input here since OnMouseButtonEvent deals with it.
	EA::WebKit::MouseButtonEvent event;
	memset(&event, 0, sizeof(event));
	event.mId = EA::WebKit::kMouseLeft;
	event.mX  = mCursorPos.x;
	event.mY  = mCursorPos.y;

	event.mbDepressed = true;
	OnMouseButtonEvent(event);
	event.mbDepressed = false;
	OnMouseButtonEvent(event);

	return true;
}


//- Contributed by Chris Stott and team





//////////////////////////////////////////////////////////////////////////
//
void View::CreateJavascriptBindings(const char* bindingObjectName)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	GetFixedString(mJavascriptBindingObjectName)->assign(bindingObjectName);

	mJavascriptBindingObject = 	new JavascriptBindingObject(this);
	if(mJavascriptBindingObject)
        this->mpWebView->mainFrame()->addToJSWindowObject(GetFixedString(mJavascriptBindingObjectName)->c_str(), mJavascriptBindingObject);
}

//////////////////////////////////////////////////////////////////////////
//
void View::RebindJavascript()
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	if(mJavascriptBindingObject)
        this->mpWebView->mainFrame()->addToJSWindowObject(GetFixedString(mJavascriptBindingObjectName)->c_str(), mJavascriptBindingObject);
}

//////////////////////////////////////////////////////////////////////////
//
void View::RegisterJavascriptMethod(const char* name)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	
	if (mJavascriptBindingObject)
	{
		mJavascriptBindingObject->addMethod(name);
	}
	else
	{
		DebugLogInfo info;
        info.mpView = this;
        info.mpLogText = "No binding object created yet";
		info.mType = kDebugLogJavascript;
		EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
		if(pViewNotification)
			pViewNotification->DebugLog(info);
	}

	
}

//////////////////////////////////////////////////////////////////////////
//
void View::RegisterJavascriptProperty(const char* name)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if (mJavascriptBindingObject)
	{
		mJavascriptBindingObject->addProperty(name);
	}
}

//////////////////////////////////////////////////////////////////////////
//
void View::UnregisterJavascriptMethod(const char* name)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if (mJavascriptBindingObject)
	{
		mJavascriptBindingObject->removeMethod(name);
	}
}

//////////////////////////////////////////////////////////////////////////
//
void View::UnregisterJavascriptProperty(const char* name)
{
	SET_AUTOFPUPRECISION(kFPUPrecisionExtended);   
	if (mJavascriptBindingObject)
	{
		mJavascriptBindingObject->removeProperty(name);
	}
}


} // namespace WebKit

} // namespace EA


