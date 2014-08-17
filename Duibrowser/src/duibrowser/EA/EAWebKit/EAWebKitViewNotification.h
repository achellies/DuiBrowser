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
// EAWebKitViewNotification.h
// By Arpit Baldeva (Refactored Paul Pedriana's code in EAWebKitView.h to a new header)
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_EAWEBKITVIEWNOTIFICATION_H
#define EAWEBKIT_EAWEBKITVIEWNOTIFICATION_H
#include <EAWebKit/EAWebKitConfig.h>
#include <EAWebKit/EAWebkitSTLWrapper.h>
#include <EAWebKit/EAWebKitJavascriptValue.h>
#include <EARaster/EARaster.h>
#include <EARaster/EARasterColor.h>

namespace EA
{
	namespace WebKit
	{
		// Required both in this header and EAWebKitView.h
		// Default Viewport size in which the View is rendered
		enum ViewDefault
		{
			kViewWidthDefault  = 800,
			kViewHeightDefault = 600
		};
	}
}
namespace EA
{
	namespace WebKit
	{
		class View;
		
		///////////////////////////////////////////////////////////////////////
		// View Notification
		// Create a single instance of a class derived from this class for all the EA:WebKit::View instances that you may need. This class acts like a 
		// notification/delegate object which lets the user of the EAWebKit library override some default operations (for example, drawing a custom focus
		// ring or handling errors).
		///////////////////////////////////////////////////////////////////////





		///////////////////////////////////////////////////////////////////////
		//
		//
		// struct(s) used to communicate with the application
		//
		

		// CursorChangeInfo
		// The user is required to respond to this message if the user wants 
		// cursors to change appropriately within the view.
		// The implementation of a cursor/mouse must be handled by the application
		// and is not implemented by EAWebKit. However, the Windows platform has built-in
		// support for cursors and so EAWebKit does have default cursor handling
		// for Windows, though this handling may not always work in full-screen
		// DirectX applications.
		struct CursorChangeInfo
		{
			View*	mpView;         // WebKit doesn't provide a WebView context for cursor management. However, we use our AutoSetActiveView for this functionality. 
			int     mNewCursorId;   // See enum CursorId, currently located in EAWebKitGraphics.h
			int		mX;				// Cursor location X
			int		mY;				// Cursor location Y

		};



		// ScrollbarDrawInfo
		// Used to notify the user of the need to draw a scrollbar.
		// The user can return false to allow default drawing to occur.
		struct ScrollbarDrawInfo
		{
			enum ScrollbarPart { None, BackBtn, ThumbBtn, ForwardBtn };

			View*                mpView;                // The associated view.
			EA::Raster::ISurface* mpSurface;             // 
			EA::Raster::Rect     mDirtyRect;            // The View/Surface dirty rectangle that needs to be updated.
			bool                 mIsVertical;           // Vertical vs. horizontal scroll bar.
			ScrollbarPart        mHoverPart;            // If other than None, the cursor is over this part and it is thus typically drawn in a highlighted state.
			EA::Raster::Rect     mRectFwdBtn;           // Rectangle that the forward/down button encompasses.
			EA::Raster::Rect     mRectBackBtn;          // Rectangle that the back/up button encompasses.
			EA::Raster::Rect     mRectThumb;            // Rectangle that the thumb encompasses.
			EA::Raster::Rect     mRectTrack;            // Rectangle that the scrollbar background encompasses.
		};

		// CustomFocusRingInfo
		// Used to notify the user of the need to draw a focus ring effect.
		// The user can return false to allow default drawing to occur.
		// Note that a "focus ring" is the highlight around an element (e.g. link) 
		// which indicates that it has focus in the view. For example, you can see such a ring in Google's Chrome when tabbing with the Keyboard. If a 
		// link has the focus and you hit enter, it'll take you to the destination page.
		struct FocusRingDrawInfo
		{
			View*                mpView;
			EA::Raster::ISurface* mpSurface;
			EA::Raster::Rect     mFocusRect;
			EA::Raster::Color    mSuggestedColor;
		};
		
		// ButtonDrawInfo
		// Used for push buttons, check boxes, radio buttons, dropdown arrows.
		// Provides the means to allow the EAWebKit user to override how buttons are drawn.  
		// The user can return false to allow default drawing to occur.
		struct ButtonDrawInfo
		{
			enum ButtonType { None, Button, Checkbox, Radio, DropDown };

			View*                mpView;                // The associated view.
			EA::Raster::ISurface* mpSurface;             // 
			EA::Raster::Rect     mDirtyRect;            // The View/Surface dirty rectangle that needs to be updated.
			EA::Raster::Rect     mButtonRect;           // Rectangle that the button encompasses.
			ButtonType           mButtonType;           // The type of button that needs to be drawn.
			bool                 mIsHovered;            // If the cursor is hovered over it and it is thus typically drawn in a highlighted state.
			bool                 mIsChecked;            // Applies to radio and checkbox only.
			bool                 mIsFocused;            // If it has the tab focus (not necessarily drawn the same as with hover). Typically you draw a dotted line around it or draw it in a highlighted state.
			bool				 mIsDisabled;			// Check for if the button is disabled.
		};

		// TextFieldDrawInfo
		struct TextFieldDrawInfo
		{
			View*                mpView;                // The associated view.
			EA::Raster::ISurface* mpSurface;             // 
			EA::Raster::Rect     mDirtyRect;            // The View/Surface dirty rectangle that needs to be updated.
			EA::Raster::Rect     mTextRect;             // Rectangle that the text editing area encompasses.
		};

		// PopupMenuDrawInfo		
		// Typically this involves a text area on the left and a arrow-down button on the right.
		struct PopupMenuDrawInfo
		{
			View*                mpView;
			EA::Raster::ISurface* mpSurface;             // 
			EA::Raster::Rect     mDirtyRect;            // The View/Surface dirty rectangle that needs to be updated.
			EA::Raster::Rect     mMenuRect;             // Rectangle of the top line of the popup menu, which is always visible whether it's activated or not.
			bool                 mIsHovered;            // If the cursor is hovered over it and it is thus typically drawn in a highlighted state.
			bool                 mIsFocused;            // If it has the tab focus (not necessarily drawn the same as with hover). Typically you draw a dotted line around it or draw it in a highlighted state.
			bool                 mIsActivated;          // True if the menu has been clicked and a multi-line selection menu has appeared.
			int                  mActivatedCount;       // Number of items in the popped up mutli-line menu.
			int                  mActivatedLineHeight;  // Should be ~equal to mActivatedRect.height() / mActivatedCount.
			int                  mSelectedIndex;        // 0-based index of the currently selected item.
			EA::Raster::Rect     mActivatedRect;        // Rectangle of the text area if mIsActivated is true. This is the rect of the multi-line selection below mMenuRect.
			bool				 mIsDisabled;			// Check for if the button is disabled.
		};


		// ViewUpdateInfo
		// Used to notify the user that the web view image has been updated.
		// The coordinates are x,y,w,h, with x/y referring to the upper-left corner of the view.
        struct ViewUpdateInfo
		{
            enum ViewDrawEvents { kViewDrawNone, kViewDrawStart, kViewDrawEnd };
            View* mpView;
			int   mX;
			int   mY;
			int   mW;
			int   mH;
            ViewDrawEvents  mDrawEvent;     // Note: The values in mX,mY,mW and mH are only set on kViewDrawEnded    
		};

		// ErrorInfo
		// This is for user-level errors, which should be reported to the end user
		// graphically in some way. 
		struct ErrorInfo
		{
			View*           mpView;     // 
			int             mErrorId;   // Currently this refers to the internal errors in /WebKit/OrigynWebBrowser/Api/WebError.h. This needs to be exposed.
			const char16_t* mpURI;      // The relevant URI.
			const char16_t* mpContext1; // Some possibly related context string. Dependent on the error.
		};


		// AssertionFailureInfo
		// This is for code assertion failures, which are typically generated
		// via EAW_ASSERT. This failure is not something to show an end-user and
		// is typically enabled in debug builds only. See the documentation for
		// EAW_ASSERT for more information.
		struct AssertionFailureInfo
		{
			View*		mpView;   
			const char* mpFailureText;
		};

		// DebugLogInfo
		// This is for code traces, which are typically generated by EAW_TRACE or 
		// some underlying equivalent within WebKit. This trace is not something to 
		// show an end-user and is typically enabled in debug builds only. 
		// See the documentation for EAW_TRACE for more information.

		// Type flags for the log info so it can be filtered if needed
		enum DebugLogType
		{
			kDebugLogGeneral,
			kDebugLogJavascript,
			kDebugLogAssertion,
			kDebugLogNetwork,
			kDebugLogGraphics,
			kDebugLogMemory
		};

		struct DebugLogInfo
		{
			View*		mpView;                  
			const char* mpLogText;
			DebugLogType mType;     // Type of debugLog Info.  This can be used to filter or redirect certain logs.
		};


		// LinkNotificationInfo
		// This allows user to intercept the execution of a hyperlink and modify it or intercede in its execution entirely.
		struct LinkNotificationInfo
		{
			View*							mpView;                 // 
			bool							mbURIIntercepted;       // If the user sets it  to true in the notification callback, then EAWebKit does nothing more and doesn't attempt to follow the link. The application is assumed to take ownership of the event.
			bool							mbURIInterceptedByDomainFiltering;// If EAWebKit sets it to true, EAWebKit auto-intercepted this link and would not navigate. The application may want to do something with this link (For example, open in an external browser if available).
			EASTLFixedString16Wrapper		mOriginalURI;           // 
			EASTLFixedString16Wrapper		mModifiedURI;           // If this is set to anything but empty, then it is used instead of mOriginalURI.
			EASTLHeaderMapWrapper			mpOriginalHeaderMap;    // This will always point to a valid non-empty header map.
			EASTLHeaderMapWrapper			mpModifiedHeaderMap;    // This will always point to a valid but initially empty header map. If this is set to anything but empty, then it is used intead of mpOriginalHeaderMap.
		};

		// LoadInfo
		enum LoadEventType
		{
			kLETNone,
			kLETPageRequest,            // new URL request
			kLETPageRedirect,           // new URL for the original due to a redirect. Typically you want to change the URL bar to reflect this new URL.
			kLETResponseReceived,       // The server has responded to the request.
			kLETContentLengthReceived,  // The page content length is now available.
			kLETLoadCompleted,          // The page load completed successfully.
			kLETLoadFailed,             // The page load failed.
			kLETWillClose,              // The page is about to close.
			kLETLoadStarted,            // 
			kLETTitleReceived,          // The title of the page is now available.
			kLETLoadCommitted,          // The old page will be cleared as the new page seems to be available.
			kLETLayoutCompleted,        // 
			kLETWillShow,               // The window is to be shown for the first time.
			kLETLoadProgressUpdate,     // Estimated fraction [0.0 .. 1.0] of page load completion.
		};

		struct EAWEBKIT_API LoadInfo
		{
			View*						mpView;               	// The associated View.
			LoadEventType				mLET;                 	// The LoadEventType that triggered this update.
			bool						mbStarted;            	// True if the load has started.
			bool						mbCompleted;          	// True if the load has completed.
			int64_t						mContentLength;       	// -1 means unknown.
			double						mProgressEstimation;  	// [0.0 .. 1.0]
			EASTLFixedString16Wrapper   mURI;                 	// The main document URI associated with current page load event. It should be called mPageURI but keeping it mURI for backward compatibility reasons.
			EASTLFixedString16Wrapper   mResourceURI;           // The URI associated with this current load event. This will change for each sub-resource on the page.
			EASTLFixedString16Wrapper   mPageTitle;           	// This gets set by the documentLoader during the load.
			uint64_t					mLastChangedTime;     	// Time (in milliseconds since View creation) of last update of this struct.
			int							mStatusCode;			// HTTP Status Code
			LoadInfo();
		};

		enum XmlHttpRequestLoadEventType
		{
			kXHRLETAbort,
			kXHRLETError,
			kXHRLETLoad,
			kXHRLETLoadStart,
			kXHRLETProgress
		};

		// This allows the user to receive notifications of XMLHttpRequest events outside of Javascript( i.e. application code).
		struct XMLHttpRequestEventInfo
		{
			XmlHttpRequestLoadEventType		mEventType; 
			bool							mLengthComputable;
			unsigned int					mLoaded;
			unsigned int					mTotal;
			const char16_t*					mURI;                 // The URI associated with this current load event.
		};

		// FileChooserInfo
		// This is used to tell the application to display a file load or save dialog box.
		struct FileChooserInfo
		{
			View*		mpView;            
			bool		mbIsLoad;          // If true, the dialog is for loading an existing file on disk. If false it is for saving to a new or existing file on disk.
			char16_t	mFilePath[256];    // The user writes to mFilePath and sets mbSucces to true or false.
			bool		mbSuccess;         // The user should set this to true or false, depending on the outcome.
		};

		// StringInfo
		// This allows the user to provide localized strings to the view.
		// See WebCore::LocalizedStringType for an enumeration of all string ids.
		struct StringInfo
		{
			View*		mpView;            // WebKit doesn't provide a WebView context for localized strings but we can set it if the active view is known.
			int			mStringId;         // EAWebKit sets the mStringId value, and the app fills in either the 
			char8_t		mString8[256];     // mString8 or mString16 string with the localized version of that string.
			char16_t	mString16[256];
		};

		// AuthenticationInfo
		// Used to request HTTP authentication (name/password) from a user.
		// This is seen in web browsers when they pop up a user/password dialog 
		// box while trying to navigate to a page. This is not the same as 
		// a user/password HTML input form seen on some pages, as this is 
		// at an HTTP level as opposed to an HTML level.
		// The ViewNotification::Authenticate function is called once with
		// mBegin = true, then it is repeatedly called with mbBegin = false
		// until the user sets mResult to a value other than zero to indicate
		// that the user has specified mName/mPassword or that the user 
		// has canceled.
		// It is important to note that multiple authentication requests 
		// can occur at the same time, as a page may have multiple protected
		// elements that need to be loaded at the same time. The mId parameter 
		// will be different for each request globally, so you can use that 
		// to distinguish between requests. Also, each request will only have 
		// its mbBegin parameter set to true once, the first time this message 
		// is sent.
		enum AIPersist
		{
			kAIPersistNone,         // Don't save user name, password.
			kAIPersistSession,      // Save for this session (i.e. app exection).
			kAIPersistIndefinitely  // Save across sessions (i.e. app executions).
		};

		enum AIResult
		{
			kAIResultNone,          // No result yet
			kAIResultOK,            // OK, use mName/mPassword.
			kAIResultCancel         // Cancel.
		};

		struct AuthenticationInfo
		{
			View*           mpView;             // The associated View.
			uintptr_t       mId;                // Input. Unique id assigned by authentication manager for each unique authentication request. You can use this to distinguish between requests.
			uintptr_t       mUserContext;       // Input. The receiver of this message can store data here for later retrieval.
			bool            mbBegin;            // Input. True the first time Authenticate is called, false for  
			const char16_t* mpURL;              // Input.
			const char16_t* mpRealm;            // Input.
			const char16_t* mpType;             // Input. Authentication type. We only support "Basic". Digest or NTLM are not supported.
			char16_t        mName[64];          // Output.
			char16_t        mPassword[64];      // Output.
			AIPersist       mPersistLevel;      // Output.
			AIResult        mResult;            // Output.
		};

		// TextInputStateInfo
		// Used to indicate that the TextInput state has changed, such as when a 
		// TextInput form control is gains or loses focus activation.

		enum KeyboardType
		{
			kDefaultKeyBoard = 0,			//eaw-kb-default
			kNumericKeyBoard,				//eaw-kb-numeric
			kJapaneseFullKeyBoard,			//eaw-kb-japanese-full
			kJapaneseHiraganaKeyBoard,		//eaw-kb-japanese-hiragana
			kJapaneseKatakanaKeyBoard,		//eaw-kb-japanese-katakana
			kRussianFullKeyBoard,			//eaw-kb-russian
			kKoreanFullKeyBoard,			//eaw-kb-korean
			kTraditionalChineseFullKeyBoard,//eaw-kb-traditional-chinese
			kSimplifiedChineseKeyBoard,		//eaw-kb-simplified-chinese
			kLatinFullKeyBoard,				//eaw-kb-latin-full
			kURLKeyBoard,					//eaw-kb-url
			kPasswordKeyBoard,				//eaw-kb-password
			kEmailKeyBoard,					//eaw-kb-email
			kGamerTagKeyBoard,				//eaw-kb-gamertag
			kPhoneKeyBoard,					//eaw-kb-phone
			kIPAddressKeyBoard,				//eaw-kb-ipaddress



			//Fill in more keyboard types as needed above this.
			kCountKeyBoardTypes
		};

		struct TextInputStateInfo
		{
			View*           mpView;             // The associated View.
			bool            mIsActivated;       // If keyboard input is active = true
			bool            mIsPasswordField;   // If a text password input field = true
			bool            mIsSearchField;     // If is a text search field
			KeyboardType	mKeyboardType;		// Keyboard type associated with the input field

			TextInputStateInfo() : mpView(0), mIsActivated(false), mIsPasswordField(false), mIsSearchField(false), mKeyboardType(kDefaultKeyBoard) {}
		};


		// ClipboardEventInfo
		// Used to allow an EAWebKit view to interact with the system clipboard.
		// Return true from the ViewNotification function to indicate success.
		struct ClipboardEventInfo
		{
			View*                        mpView;               // The associated View when found for there is no view context here normally.
			bool				         mReadFromClipboard;   // If true, then this is a request to read text from the system clipboard into mText. If false then this is a request to write mText to the system clipboard. 
			EASTLFixedString16Wrapper	 mText;                // This is to be written if mReadFromClipboard is true, and read if mReadFromClipboard is false.
		};

		// Used to pass arguments and notify the game when custom registered javascript methods are called.
		struct JavascriptMethodInvokedInfo
		{
			static const unsigned			MAX_ARGUMENTS = 10;
			View*                           mpView;    
			const char*						mMethodName;
			unsigned						mArgumentCount;
			JavascriptValue					mArguments[MAX_ARGUMENTS];
			JavascriptValue					mReturn;
		};

		// Used to carry the state of a custom javascript property.
		struct JavascriptPropertyInfo
		{
			View*                           mpView;    
			const char*						mPropertyName;
			JavascriptValue					mValue;		
		};

		///////////////////////////////////////////////////////////////////////
		// Javascript Value
		///////////////////////////////////////////////////////////////////////
		// Use these when you want a return value from View::EvaluateJavaScript().  Make sure to destroy the pointer when no longer needed.
		EAWEBKIT_API EA::WebKit::JavascriptValue* CreateJavaScriptValue(); 
		EAWEBKIT_API void DestroyJavaScriptValue(EA::WebKit::JavascriptValue* pValue);  
        EAWEBKIT_API EA::WebKit::JavascriptValue* CreateJavaScriptValueArray(int count); 
        EAWEBKIT_API void DestroyJavaScriptValueArray(EA::WebKit::JavascriptValue* pValues);  


		// Used to notify the application of any error in the Network layer(DirtySDK). Note that this has nothing to do with HTTP return codes
		// and their interpretation. For example, HTTP 404 does not mean a network layer error. An example of network layer error is a possible 
		// timeout(May be the internet is not connected or may be the server itself is down).
		enum NetworkErrorType
		{
			kNetworkErrorUnknown = 0,
			kNetworkErrorTimeOut,
			kNetworkErrorSSLCert
		};

		struct NetworkErrorInfo
		{
			const char*			mErrorMessage; //It is not kept around so copy it if you need it.
			NetworkErrorType	mNetworkErrorType;
			int32_t				mNetworkErrorCode;//This is the code that we get from DirtySDK. This would depend on the NetworkErrorType. We just pass it along.
			NetworkErrorInfo(NetworkErrorType networkErrorType = kNetworkErrorUnknown)
				: mErrorMessage(0)
				, mNetworkErrorType(networkErrorType)
				, mNetworkErrorCode(0)
			{

			}
		};

		// UriHistoryChanged notification info       
		// To notify when an item has been added/removed to the back/forward history. EAWebKit keeps the history internally for navigation and it
		// can be accessed using the View member functions. However, following may be useful to an application.
		struct UriHistoryChangedInfo
		{
			View*							mpView;                 // View context
			EASTLFixedString16Wrapper		mURI;                   // URI that is being added 
			bool                            mIsAdded;               // true if added and false if removed
		};


		// Tooltip notification info
		// This is for getting information about the tooltip string.  Applications can take this over(by returning true) and display the string
		// on their own or leave it up to EAWebKit (default) to handle the tooltip display (by returning false).
		struct ToolTipInfo
		{
			View*							mpView;                 // View context
			EASTLFixedString16Wrapper		mName;                  // Name giving by the tooltip. (For example the title property)
			EA::Raster::Point               mLocation;              // Current screen x,y location 
			int                             mHandleID;              // Node identifier to flag a tool tip change (like to change location).
		};

        // Movie notification info
		// This is for getting information from a movie embed tag.  Applications can optionally draw an image in the movie window if desired.
		// Example: <embed type="movie" src="file:///C:/update.avi" width="350" height="250"/>  
        // Required is using the embed tag and passing the scr. If the width or height attributes are 0, no local surface will be created. 
        // If the "movie" type is not passed, the src file extension will be examined for a movie MIME type (e.g. .avi or .flv).  
        // If you return true from this notification callback, the draw will update the view with the movie surface for it will assumes that the surface
        // was updated. If false, the view will only be updated with a regular screen refresh.
        // Setting the mTerminateCallback flag to true, will shut down the movie update callback which is pulsed based on the TriggerDelay.  
        // If the movie is destroyed, it will also notify one last time with the mTerminateCallback set to true.
        // Setting the mRemoveMovieSurfaceFlag to true will cause the internal draw surface to be remove and it will not longer draw to the view.
        // (The Loop, Pause, Auto and preload flags are not fully functional yet but are there for future compatibitlity e.g. HTML5).
        struct MovieUpdateInfo
		{
			View*							mpView;                 // View context
			const char16_t*                 mSrcURI;                // This is the field that was in the scr attribute tag
		    EA::Raster::ISurface*           mpMovieSurface;         // The movie texture (currently the full surface so no clip is needed)
			EA::Raster::Rect                mMovieRect;             // The movie rectangle size given by the embed attributes (width and height).
            EA::Raster::Rect                mWindowRect;            // Scroll adjusted offset of movie window rectangle (can be used to layer a movie on top for example)
            double                          mTriggerDelay;          // This sets in seconds when the next API call should be triggered (30HZ normally)
            bool                            mTerminateCallbackFlag; // This will stop the API from being called again (unless the page is reloaded) however the last surface will still be cached and paint when needed.
            bool                            mRemoveMovieSurfaceFlag; // This will remove the allocated movie surface 
            bool                            mLoopFlag;              // Loop request     
            bool                            mPauseFlag;             // Pause request  
            bool                            mAutoPlayFlag;          // AutoPlay request
            bool                            mPreloadFlag;           // Preload request 
        };


		// Profiling support
		// VProcessType - Various types of processes/functions to profile
		enum VProcessType
		{
			kVProcessTypeNone = -1,                 // 
			kVProcessTypeUser1,						// Generic for internal EAWebkit debugging
			kVProcessTypeUser2,						// Generic for internal EAWebkit debugging
			kVProcessTypeUser3,						// Generic for internal EAWebkit debugging

			kVProcessTypeViewTick,					// Main view update tick
			kVProcessTypeTransportTick,				// Tick the network update
			kVProcessTypeKeyBoardEvent,				// Keyboard Event
			kVProcessTypeMouseMoveEvent,			// Mouse Event
			kVProcessTypeMouseButtonEvent,			// Mouse Button
			kVProcessTypeMouseWheelEvent,			// Mouse Wheel
			kVProcessTypeScrollEvent,				// Scroll Event
			kVProcessTypeScript,                    // Main JavaScript (could include other calls like canvas draw)
			kVProcessTypeDraw,                      // Main Draw (might not include some special canvas draw calls)
			kVProcessTypeTHJobs,                    // Main job loop for resource handler
			kVProcessTypeTransportJob,              // Single job loop tracking using the transport system
			kVProcessTypeFileCacheJob,              // Single job loop tracking using the file cache system
			kVProcessTypeDrawImage,                 // Single image draw (includes most decoding, resize, compression render)
			kVProcessTypeDrawImagePattern,          // Tiling image draw (includes most decoding, resize, compression render)
			kVProcessTypeDrawGlyph,                 // Font draw (includes render)
			kVProcessTypeDrawRaster,                // Low level raster draw for font and images
			kVProcessTypeImageDecoder,              // Image decoder (JPEG, GIF, PNG)
			kVProcessTypeImageCompressionPack,      // Image compression packing
			kVProcessTypeImageCompressionUnPack,    // Image compression unpacking
			kVProcessTypeJavaScriptParser,          // JavaScript parser
			kVProcessTypeJavaScriptExecute,         // JavaScript execute
			kVProcessTypeCSSParseSheet,             // CSS Sheet parse
			kVProcessTypeFontLoading,               // Font loading




			//****************************************************************//
			//Add any new process types above this line//
			//****************************************************************//
			kVProcessTypeLast						// Keep this at the end for array size
		};
		
		// VProcessStatus - Various states a process can go through. For most processes, it would be start and end.
		// However, jobs can have more states. To keep it less complicated, we include those states here.
		// The queued states tell exactly how long it took for the previous state to finish. However, this does not mean that all the time was spent 
		// inside the state machine. This is because a state change can cause some other code to execute and delay the exact change of state. This actually
		// works well because this is the type of behavior we are interested in investigating anyway (Which states are taking longer to change and why). 
		enum VProcessStatus
		{
			//Common proces states
			kVProcessStatusNone,
			kVProcessStatusStarted,					// Start of a process or function
			kVProcessStatusEnded,					// End of a process or function

			//Uncommon process states(These are actually specific to Transport jobs)
			kVProcessStatusQueuedToInit,			// This is when the job is queued for Init state in the code.
			kVProcessStatusQueuedToConnection,		// This is when the Init state is finished and the job is queued for the Connection state.
			kVProcessStatusQueuedToTransfer,		// This is when the Connection state is finished and the job is queued for the Transfer state. 
			kVProcessStatusQueuedToDisconnect,      // This is when the Transfer state is finished and the job is queued for the Disconnect state. 
			kVProcessStatusQueuedToShutdown,		// This is when the Disconnect state is finished and the job is queued for the Shutdown state. 
			kVProcessStatusQueuedToRemove			// This is when the Shutdown state is finished and the job is queued for the Remove state. 
		};

		// ViewProcessInfo
		// This is mostly to give user insight of when certain key processes are started and stopped.
		// It can be used for profiling for example by timing the start and end status.
		// Things like URI and job information are kept in this structure to make things less complicated.
		struct ViewProcessInfo
		{
			View*                               mpView;             // The associated View.

			// Variables
			VProcessType						mProcessType;
			VProcessStatus						mProcessStatus;
			double								mStartTime;			// This is a user controlled workspace clock for timing
			double								mIntermediateTime;	// This is a user controlled workspace clock for timing
			const EASTLFixedString16Wrapper*	mURI;				// The URL associated with the process, if any.
			int									mSize;				// Various usage but mostly return size info
			int									mJobId;				// Job Id of this process, if any.						
			// Constructors
			ViewProcessInfo();
			ViewProcessInfo(VProcessType,VProcessStatus, View* pView = NULL);
			void ResetTime();
		};

		// Note by Arpit Baldeva: Use this function to notify an event change in state of a process found in the global array that keeps track of the 
		// predefined processes. This is what you would want to use most of the times.
		void NOTIFY_PROCESS_STATUS(VProcessType processType, VProcessStatus processStatus, EA::WebKit::View *pView = NULL);
		// Note by Arpit Baldeva: Use this function to notify an event change associated with jobs. Each job has a ViewProcessInfo attached with it.
		void NOTIFY_PROCESS_STATUS(ViewProcessInfo& process,  VProcessStatus processStatus);

		struct CreateViewInfo
		{
			View*		mpView;			// The associated View.
			View*		mpCreatedView;	// Application needs to create this View and set the pointer.
			
			// Parameters from javascript for the creation of window associated with this View
			uint16_t	mLeft;
			uint16_t	mTop;
			uint16_t	mWidth;
			uint16_t	mHeight;
			bool		mResizable;
			bool		mScrollBars;
			
			bool		mFromJavaScript; // If true, the notification is sent as a result of window.open(...). If false, the notification is coming from <a target="_blank"/>. It can be used, for example, to decide the window creation parameters.
			CreateViewInfo()
				: mpView(NULL), mpCreatedView(NULL)
				, mLeft(0), mTop(0), mWidth(EA::WebKit::kViewWidthDefault), mHeight(EA::WebKit::kViewHeightDefault)
				, mResizable(true), mScrollBars(true)
				, mFromJavaScript(false)
			{

			}

		};
		// The user can provide an instance of this interface to the View class.
		// The user should return true if the user handled the notification, 
		// and false if not. A return value of false means that the user wants 
		// the caller to do default handling of the notification on its own.
		// To consider: some of these notifications may be View-specific and it
		// might be a good idea to make a View::SetViewNotification() for them.

		class ViewNotification
		{
		public:
			virtual ~ViewNotification() { }

			virtual bool CursorChanged   		(CursorChangeInfo&)     		{ return false; }
			virtual bool DrawScrollbar   		(ScrollbarDrawInfo&)    		{ return false; }
			virtual bool DrawScrollbarCorner	(EA::Raster::ISurface* pSurface, EA::Raster::Rect&)    		{ return false; }  // Draws the corner of the scrollbar
			virtual bool DrawFocusRing   		(FocusRingDrawInfo&)    		{ return false; }  // A focus ring is a highlight around a link or input element indicating it has focus.
			virtual bool DrawButton      		(ButtonDrawInfo&)       		{ return false; }
			virtual bool DrawTextArea    		(TextFieldDrawInfo&)    		{ return false; }
			virtual bool DrawPopupMenu   		(PopupMenuDrawInfo&)    		{ return false; }
 		    virtual bool ViewUpdate      		(ViewUpdateInfo&)       		{ return false; }  
			virtual bool Error       			(ErrorInfo&)            		{ return false; }  // This function is now deprecated so this should not be used anymore. Please use the WebkitError api in future. This function has been left here for backward compatibility with older versions
			virtual bool WebError       		(ErrorInfo&)            		{ return false; }
			virtual bool NetworkError			(NetworkErrorInfo&)				{ return false; }
			virtual bool AssertionFailure		(AssertionFailureInfo&) 		{ return false; }
			virtual bool DebugLog        		(DebugLogInfo&)         		{ return false; }
			virtual bool LinkSelected    		(LinkNotificationInfo&) 		{ return false; }
			virtual bool LoadUpdate      		(LoadInfo&)             		{ return false; }
			virtual bool ChooseFile      		(FileChooserInfo&)      		{ return false; }
			virtual bool GetString       		(StringInfo&)           		{ return false; }
			virtual bool Authenticate    		(AuthenticationInfo&)   		{ return false; }  // Called when there is a page authentication challenge by the server.
			virtual bool TextInputState  		(TextInputStateInfo&)   		{ return false; }
			virtual bool ClipboardEvent  		(ClipboardEventInfo&)   		{ return false; }
			virtual bool XMLHttpRequestEvent	(XMLHttpRequestEventInfo&)		{ return false; }
			virtual bool ViewProcessStatus      (ViewProcessInfo&)				{ return false; } // To notify for start and end of certain key processes
			virtual bool JavascriptMethodInvoked(JavascriptMethodInvokedInfo&)	{ return false; }
            
            // Called when javascript tries to call a method that doesn't exist in the binding object.
            virtual bool JavascriptMethodMissing(JavascriptMethodInvokedInfo &info) { return false; }

			virtual bool GetJavascriptProperty	(JavascriptPropertyInfo&)		{ return false; }
			virtual bool SetJavascriptProperty	(JavascriptPropertyInfo&)		{ return false; }
			virtual bool UriHistoryChanged 	    (UriHistoryChangedInfo&)		{ return false; } 
			virtual bool SetToolTip             (ToolTipInfo&)                  { return false; } // If it returns true, we skip the internal display of the tooltip string 
            virtual bool MovieUpdate	        (MovieUpdateInfo&)              { return false; }
			virtual bool CreateView				(CreateViewInfo&)				{ return false; } // Called for javascript window.open and <a> target="_blank"</a>. The application is responsbile for creating the View (and return true in case of a new View creation).
			virtual bool DrawEvent   		    (ViewUpdateInfo&)       		{ return false; }  // 
        };

		// This is called by the user so that the user is notified of significant
		// events during the browsing process. There can be only a single ViewNotification
		// in place, and if you want to support more than one then you should implement
		// a proxy ViewNotification which handles this.
		EAWEBKIT_API void              SetViewNotification(ViewNotification* pViewNotification);
		EAWEBKIT_API ViewNotification* GetViewNotification();




	}
}

#endif