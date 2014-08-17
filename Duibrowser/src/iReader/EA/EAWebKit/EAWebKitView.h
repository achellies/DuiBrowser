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
// EAWebKitView.h
// By Paul Pedriana 
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITVIEW_H
#define EAWEBKIT_EAWEBKITVIEW_H

#include <EAWebKit/EAWebKitConfig.h>
#include <EAWebKit/EAWebKitViewNotification.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitLinkHook.h>
#include <EAWebKit/EAWebKitForwardDeclarations.h>
#include <EAWebKit/EAWebkitSTLWrapper.h>
#include <EAWebKit/EAWebKitJavascriptValue.h>
#include <EARaster/EARaster.h>
#include <EARaster/EARasterColor.h>


namespace WebCore
{
    struct ResourceRequest;
	class HTMLElement;
	class Element;
	class Node;
}

namespace KJS 
{ 
	class Debugger; 
}

class BalObject;

namespace EA
{
	namespace WebKit
	{
		class NodeListContainer;
		class OverlaySurfaceArrayContainer;
		class EAWebKitJavascriptDebugger;
	}
}

namespace EA
{
    namespace WebKit
    {
		
		///////////////////////////////////////////////////////////////////////
		// structures and enums etc.
		///////////////////////////////////////////////////////////////////////

		// Initialization parameters for the View instance.
		struct EAWEBKIT_API ViewParameters
		{
			int   mWidth;                           // Defaults to kViewWidthDefault
			int   mHeight;                          // Defaults to kViewHeightDefault
			bool  mbScrollingEnabled;               // Defaults to true
			bool  mbHScrollbarEnabled;              // Defaults to true
			bool  mbVScrollbarEnabled;              // Defaults to true
			bool  mbHighlightingEnabled;            // Defaults to false
			bool  mbTransparentBackground;          // Defaults to false
			bool  mbTabKeyFocusCycle;               // Defaults to true
			bool  mbRedrawScrollbarOnCursorHover;   // Defaults to false
            EA::Raster::ISurface* mpViewSurface;    // Defaults to NULL (NULL makes EAWebKit create its own internal view surface)
                                                    // If passing a custom view surface, the format must be kPixelFormatTypeARGB.
			ViewParameters();
		};
		
		inline ViewParameters::ViewParameters()
			:	mWidth(kViewWidthDefault),
				mHeight(kViewHeightDefault),
				mbScrollingEnabled(true),
				mbHScrollbarEnabled(true),
				mbVScrollbarEnabled(true),
				mbHighlightingEnabled(false),
				mbTransparentBackground(false),
				mbTabKeyFocusCycle(true),
				mbRedrawScrollbarOnCursorHover(false),
                mpViewSurface(NULL)
			{
			}
        
		// Defines an overlay surface (such as a popup menu) on top of our main surface. This is handled by EAWebKit internally.
		struct OverlaySurfaceInfo
		{
			EA::Raster::ISurface* mpSurface;
			EA::Raster::Rect     mViewRect;
		};


		//////////////////////////////////////////////////////////////////////////
		// Debug Callbacks
		
#if EAWEBKIT_ENABLE_JUMP_NAVIGATION_DEBUGGING
		typedef void (*DrawNodeCallback)(int x, int y, int w, int h, int centreX, int centreY);
		typedef void (*DrawAxesCallback)(int centreX, int centreY, int minX, int minY, int maxX, int maxY);
#endif
		/////////////////////////////////////////////////////////////
		
		// 09/25/2010 - Note by Arpit Baldeva - This should be deprecated.
		typedef void (*KeyboardCallback)(WebCore::Element* element, const char16_t* initialText, const char16_t* helperText, int maxLength);

		// Jump directions enumeration when navigating through a controller
		enum JumpDirection
		{
			JumpUp=0,
			JumpDown,
			JumpLeft,
			JumpRight
		};

		// Optional parameters to tune Jump algorithm as per your application requirements.
		// It is advisable to keep default and analyze the navigation behavior of your application. If that does not suite your requirements, you may want to use following to tune it some.
		struct JumpNavigationParams
		{
			float mNavigationTheta;	// Angle in radians around the current node in which the new nodes should be searched. For example, if you specify 0.0f, it will mostly move horizontally or vertically.   
			bool mStrictAxesCheck;	// If you use strict axes check, only elements with their bounding rectangle completely on the intended side of Jump are considered. Otherwise, an element whose bounding rect starts at the opposite side but extends its width/height to the jump direction is considered as well. Setting it to true is going to be more efficient as well.
			float mNumLinesToAutoScroll; // This is used when using D-Pad to move up/down/left/right. If the player is at the edge of the screen and presses a directional button in order to navigate to a currently invisible area, the view will scroll based on this setting. 1 line is roughly 40 pixel at the time of writing and unlikely to change. This could be pretty handy if you have say a list of items(For example, music tracks listing) that the player may be interested in scrolling through.
			JumpNavigationParams()
				: mNavigationTheta(1.5f)// It turns out that 1.5 is a better default choice(It was the hard coded value originally before clean up) for game UI menus which usually have items with varying width in a column.
				, mStrictAxesCheck(true)
				, mNumLinesToAutoScroll(2.0f)
			{

			}
		};
		
		// Following is equivalent of tabbing (Tab key) or back tabbing (Shift + Tab Keys) behavior found in standard browsers.
		enum FocusDirection
		{
			FocusDirectionForward=0,
			FocusDirectionBackward
		};

		// View is a simplified interface to WebKit's WebView.
		// This class is not thread-safe, you cannot safely use it from multiple 
		// threads simultaneously. Nor can you create multiple View instances and 
		// use them from different threads. This is due to the design of WebKit 
		// itself and there are no plans by the WebKit community to change this.
		// The Qt documentation at http://doc.trolltech.com/4.4/qwebview.html describes
		// their WebView, which is similar to ours.
		// Note that you can create multiple instances of View class on same thread.
		

		class EAWEBKIT_API View
		{
		public:
			View();
			virtual ~View();

			//
			// APIs related to setup/tear down
			//
			virtual bool InitView(const ViewParameters& vp);
			virtual void ShutdownView();

			//
			// APIs related to Runtime
			//
			// Call this function repeatedly. Returns true if the surface was changed.
			virtual bool Tick();

			
			//
			// APIs related to URI navigation
			//
			virtual bool			SetURI(const char* pURI);
			virtual const char16_t* GetURI();
			virtual bool			CanGoBack(uint32_t count=1);       // Count is how many items to go back, default to 1, returns true if can go back the requested count.
			virtual bool			GoBack();//Don't need to call CanGoBack() in order to GoBack()
			virtual bool			CanGoForward(uint32_t count=1);    // Count is how many items it can go forward, default to 1, returns true if can go forward the requested count.
			virtual bool			GoForward();//Don't need to call CanGoForward() in order to GoForward()
			virtual void			Refresh(); //Refresh/Reload.
			virtual void			CancelLoad();

			//
			// APIs related to Input events
			//
			// These functions are called by the application to notify the View of input events that have occurred.
			// The x and y positions are relative to View origin, which is at the top-left corner and goes rightward and downward.
			virtual void OnKeyboardEvent(const KeyboardEvent& keyboardEvent);
			virtual void OnMouseMoveEvent(const MouseMoveEvent& mouseMoveEvent);
			virtual void OnMouseButtonEvent(const MouseButtonEvent& mouseButtonEvent);
			virtual void OnMouseWheelEvent(const MouseWheelEvent& mouseWheelEvent);
			virtual void OnFocusChangeEvent(bool bHasFocus);

									
			//
			// APIs related to Javascript
			//
			virtual bool EvaluateJavaScript(const char* pScriptSource, size_t length, EA::WebKit::JavascriptValue* pReturnValue = NULL ); // Returns true if it found a valid return type and stored it in the pReturnValue
			virtual bool EvaluateJavaScript(const char16_t* pScriptSource, size_t length, EA::WebKit::JavascriptValue* pReturnValue = NULL ); // Returns true if it found a valid return type and stored it in the pReturnValue
			virtual void AttachJavascriptDebugger(); //currently not implemented.

			
			//
			// APIs related to Javascript Binding
            //
			// - First register a bound object using CreateJavascriptBindings (eg. CreateJavascriptBindings("EA"))
			// - Then register methods & listen to callbacks through the ViewNotification (eg. RegisterJavascriptMethod("Trace"))
			// - eg. (in javascript)
			//         EA.Trace('message');
			//		   EA.PlaySoundEffect('bell')'
			virtual void CreateJavascriptBindings(const char* bindingObjectName);
			virtual void RegisterJavascriptMethod(const char* name);
			virtual void UnregisterJavascriptMethod(const char* name);
			virtual void RegisterJavascriptProperty(const char* name);
			virtual void UnregisterJavascriptProperty(const char* name);
			virtual void RebindJavascript();

			
						
			//
			// APIs for Navigation using a Controller.
			//
			//
			// Since a typical console user won't have a keyboard/mouse, you can use following methods to navigate on a web page using the controller.
			// There is always an "invisible to the app cursor" inside EA::WebKit::View surface which is used for dealing with WebKit internals. When a cursor is
			// referred to in the following APIs, it is meant to be this internal cursor. Of course, an application can choose to draw the cursor
			// by obtaining its position from the API.
			//
			// All the Jump* APIs below move this cursor to the element. Note that this is equivalent to the cursor "hovering" on the element(onmouseover event) 
			// and not "focusing"(onfocus event).
			/////////////////////////////////

			
			// This is called by the navigation scheme.
			virtual void SetJumpNavigationParams(const JumpNavigationParams& jumpNavigationParams);

			// For quickly moving between elements with DPAD (NESW or standard tab index navigation)
			// Based on the jump direction, move the cursor to the next natural navigable element. (Natural == One found by the internal navigation algorithm)
			// Note that while the navigation algorithm mostly works as expected, there are times when it does not. This situation may arise due to an edge case or simply just how the 
			// application may want the Jump API to behave. You can solve such situations through your web page.
			// You can assign a custom attribute to an element with the navigation hints. You may also want to ignore navigation in a certain direction when on a certain element.

			// For example, if the cursor is currently over the element with the id "current", and you want it to jump to the element with id "next" regardless of the layout when the 
			// user presses right on d-pad(or any other button based on your input mapping) and you also want to ignore any left direction movement, the html may look like 
			// <input type="text"  id="current" navigationright="next" navigationleft="ignore"  />
			// <a href="some_page.htm" id="next" />

			// As of 09/20/2010, the tags are hard coded to "navigationup", "navigationdown", "navigationleft", "navigationright". Ignore is hard coded to "ignore".
			virtual bool JumpToNearestElement(EA::WebKit::JumpDirection direction);
			
			// 09/17/10 - Note by Arpit Baldeva - JumpToFirstLink should be deprecated now. We have added code that can move the mouse cursor to the focus element through javascript. 
			// So this can be handled in a page without requiring a custom tag.
			// Even if we have to keep this API, I am ambivalent if it should probably be written in terms of id or the class.
			
			// jumpToClass is a tag(for example, "navigation_jumpto_onpageload" class on an element on the page)
			// skipJumpIfAlreadyOverElement is currently not useful as it expects the caller to know if they are already over the element and then
			// skips the navigation. But if you already know that you are, then why would you call this API anyway? You would want it to remain
			// false. A correct implementation should call IsAlreadyOverNavigableElement() inside JumpToFirstLink.
			// If an element with jumpToClass is not found, this API tries to jump to the first navigable element found.
			virtual bool JumpToFirstLink(const char* jumpToClass, bool skipJumpIfAlreadyOverElement = false);
			
			// Use following to move the cursor to the element with the associated id on the page. This is used internally by the JumpToNearestElement if the web page provides the hint to
			// jump to an element. An application may find following useful as well.
			virtual bool JumpToId(const char* jumpToId);

			// Following behaves similar to pressing the "tab" key on the keyboard when navigating through a page on PC. As of 09/20/2010, the KeyboardEvent is ignored and only focus 
			// direction is used. It moves the cursor internally. However, it seems advisable that a team solely rely on the Jump* APIs for their navigation needs rather than mix and match.
			virtual void AdvanceFocus(EA::WebKit::FocusDirection direction, const EA::WebKit::KeyboardEvent& event);

			
			// An application can bind a button to an element on the page and call following to click the element with the specified id without the need of manual navigation by the user.
			// For example, <a href="home_page.htm" id="start_button" /> maps "start button" to the home_page.htm. The user can press start button from anywhere on the page and can navigate
			// to the binded link.
			// Note that you can only click <a/> or <button/> element.
			virtual bool ClickElementById(const char* id);

			// Same as above except that it takes a class as input.
			virtual bool ClickElementsByClass(const char* className);

			// Same as above except that it first searches for either a matching id and then class name.
			virtual bool ClickElementsByIdOrClass(const char* idOrClassName);

			// Simply emulate a mouse click at whatever location the cursor is.
			virtual bool Click();
			
			// Use following to figure out if the cursor is already over some element that can be navigated to using the navigation API. It is probably not much useful as you may only
			// want to know when moving from one page to another. And in that case, most likely you want to be on a specific element on the new page. 
			virtual bool IsAlreadyOverNavigableElement();
			
			// 09/23/2010 - Note by Arpit Baldeva - Make private. This should not be used by the application. And it should be renamed to MoveCursorToFocusedElement. It moves the cursor to the 
			// elements when advancing the focus using the tab style movement provided by WebKit (View::AdvanceFocus(...)).
			virtual void MoveMouseCursorToFocusElement();
			
			// 09/23/2010 - Note by Arpit Baldeva - Make private. This should not be used by the application. And it should be renamed to MoveCursorToNode. It moves the cursor to the 
			// node obtained by other high level APIs.
			// We scroll inside this function if told. This is requied if the application uses other View public API (JumpToId etc. type of API) and the actual element may be out of 
			// visible area. 
			virtual void MoveMouseCursorToNode(WebCore::Node* node, bool scrollIfNecessary = true);
			
			// 09/23/2010 - Note by Arpit Baldeva - Make private. This is used internally by EA::WebKit:View and the navigation is driven through the web page. 
			// This should be called UpdateNavigationHints(FromPage).
			virtual void UpdateCachedHints(WebCore::Node* node);
			
			// Access the current cursor position. Normally, in a custom application page, you don't require this and can(should) have the CSS handle the visual aspect of the navigation.
			// However, it can be handy to do any debugging by displaying a "mouse pointer/cursor" or in a demo application that renders any random page.
			virtual void GetCursorPosition(int& x, int& y) const; 

			// Normally, this is not required for an application. But can be handy for some debugging.
			virtual void SetCursorPosition(int x, int y);

			

#if EAWEBKIT_ENABLE_JUMP_NAVIGATION_DEBUGGING
			// Following are debug calls for navigation system. An application does not need to worry about it.
			// BEGIN
			//
			virtual void DrawFoundNodes(DrawNodeCallback callback);
			virtual void DrawBestNode(DrawNodeCallback callback);
			virtual void DrawSearchAxes(DrawAxesCallback callback);
			virtual void DrawRejectedByRadiusNodes(DrawNodeCallback callback);
			virtual void DrawRejectedByAngleNodes(DrawNodeCallback callback);
			virtual void DrawRejectedWouldBeTrappedNodes(DrawNodeCallback callback);
			//
			// END
			//
#endif
			
			
			
			//
			//APIs related to handling input fields.
			//
			//Use following two APIs to enter(set) and get the text from a selected <input/> or <textarea/> element when using emulated keyboard.
			//contentTextBuffer is expected to be null-terminated. This goes in the input field.
			//textWasAccepted is added to indicate if the emulated keyboard went away without user accepting the text. This way we can blur the focused element even if the input was  
			//discarded by user. It defaults to true in order to be backward compatible.One other way would be to introduce a new API to simply blur to input field.
			virtual void		EnterTextIntoSelectedInput(const char16_t* contentTextBuffer, bool textWasAccepted = true); 
			//Read both the contents of an input field and associated helper text(html "title" attribute).
			virtual uint32_t	GetTextFromSelectedInput(char16_t* contentTextBuffer, const uint32_t maxcontentTextBufferLength, char16_t* titleTextBuffer = 0, const uint32_t maxTitleTextBufferLength = 0 );

			//09/23/2010 - Note by Arpit Baldeva - Following two should be deprecated as the platform keyboard API deals with wide chars only.
			virtual void		EnterTextIntoSelectedInput(const char* textBuffer, bool textWasAccepted = true);
			virtual uint32_t	GetTextFromSelectedInput(char8_t* contentTextBuffer, const uint32_t maxcontentTextBufferLength, char8_t* titleTextBuffer = 0, const uint32_t maxTitleTextBufferLength = 0 );
			
			
			
			//
			// APIs related to setting the web page/element content from application 
			//
			virtual bool LoadResourceRequest(const WebCore::ResourceRequest& resourceRequest);
			virtual bool SetHTML(const char* pHTML, size_t length, const char* pBaseURL = NULL);
			virtual bool SetContent(const void* pData, size_t length, const char* pMimeType, const char* pEncoding = NULL, const char* pBaseURL = NULL);
			// 09/20/2010 - Note by Arpit Baldeva - This should be renamed to SetInnerHTMLById as it more easily understood. It remains to be seen why this should be exposed to the application.
			virtual void SetElementTextById(const char* id, const char* text);
			virtual void SetElementText(WebCore::HTMLElement* htmlElement, const char* text);

			
			//
			// Misc APIs
			//
			// Note by Arpit Baldeva: It seems to me that an application should not need it. However, this is currently in use.
			virtual LoadInfo& GetLoadInfo();
            //Get underlying Graphics Context of the EA::WebKit::View. The surface is the actual bits of the frame view.
			virtual EA::Raster::ISurface* GetSurface() const;  
			// View size in pixels. Includes scrollbars if present
			virtual void GetSize(int& w, int& h) const;
			virtual bool SetSize(int w, int h);
			
			// Accessors for setting the emulation mode on PC
			virtual bool IsEmulatingConsoleOnPC() const;
			virtual void SetEmulatingConsoleOnPC(bool emulatingConsoleOnPC);

			
			
			
			//
			// APIs related to Emulated Keyboard handling
			// 
			// 09/20/2010 - Note by Arpit Baldeva - Following two should be deprecated. The detection of an input field is now automatic and ViewNotification::TextInputState(...)
			// is called for showing a keyboard. 
			virtual void AttachEventsToInputs(KeyboardCallback callback);
			virtual void AttachEventToElementBtId(const char* id, KeyboardCallback callback);
			// 09/20/2010 - Note by Arpit Baldeva - Following should be deprecated as the keyboard input handling is automatic.
			virtual void SetInputElementValue(WebCore::HTMLElement* htmlElement, char16_t* text);
			//

			
			//
			// APIs related to Scroll abaldeva:TODO - Deprecate?
			//
			// Scrolls the view by a given x/y delta in pixels. 
			// This function will only be able to scroll on the main view. 
			virtual void Scroll(int x, int y);
			// This scroll works exactly like a mouse wheel - scroll lines are the number of lines to scroll down by
			// and scroll delta is the mouse zDelta. xScrollLines and yScrollLines are the speed at which one wants to scroll.
			virtual void Scroll(bool xAxisScroll, bool yAxisScroll, int xScrollLines, int xScrollDelta,	int yScrollLines, int yScrollDelta); 
			virtual void GetScrollOffset(int& x, int& y);//10/11/2010 - Note by Arpit Baldeva - This should be private.

					

			

			
			//
			// Add any API expected to be called by the application above this line.
			//
			
			
			
			
			
			
			
			
			
			
			
			
			//
			// BEGIN - APIs normally not needed to be called by an application.
			//
				// Returns estimated load progress as a value in the range of [0.0, 1.0]. An application normally does not need to call it as it can read this information from the 
				// ViewNotification::LoadUpdate.
				virtual double					GetEstimatedProgress() const;
				virtual const ViewParameters&	GetParameters() { return mViewParameters; }
				virtual LinkHookManager&		GetLinkHookManager() { return mLinkHookManager; }
				virtual TextInputStateInfo&		GetTextInputStateInfo();// 09/23/2010 - Note by Arpit Baldeva - This should be deprecated as it is not useful.             
				virtual void					ResetForNewLoad();//09/23/2010 - Note by Arpit Baldeva - This should be deprecated as it is not useful.

				//
				// APIs related to Overlay Surfaces
				//
				//abaldeva:TODO - Make overlay surface related APIs private?
				// Add or move an overlay surface.
				// An overlay surface is a surface that is drawn on top of the WebKit View and 
				// can be moved around. This allows for the implementation of overlay windows 
				// on top of the main View Surface.
				virtual void SetOverlaySurface(EA::Raster::ISurface* pSurface, const EA::Raster::Rect& viewRect);
				// Remove an existing overlay surface.
				virtual void RemoveOverlaySurface(EA::Raster::ISurface* pSurface);
				//abaldeva:TODO - make private
				virtual void BlitOverlaySurfaces();

				//
				// APIs internal to EAWebKit use. An application is not expected to call following.
				//
				virtual bool SetModalInput(ModalInputClient* pModalInputClient);
				virtual ModalInputClient* GetModalInputClient() const;

				// This is called by our WebKit-level code whenever an area of the view has been redrawn. It does any internal housekeeping and then
				// calls the user-installed ViewNotification. View updates should go through this function instead of directly calling the user-installed
				// ViewNotification callback. Users of EAWebKit shouldn't normally need to use this function unless manually manipulating the draw Surface.
				virtual void ViewUpdated(int x = 0, int y = 0, int w = 0, int h = 0);

				// Triggers a forced HTML-level redraw of an area of the view. Users of EAWebKit shouldn't normally need to use this function unless  
				// manually manipulating the draw Surface.
				virtual void RedrawArea(int x = 0, int y = 0, int w = 0, int h = 0);



				///////////////////////////////
				// WebKit Accessors APIs. An application is not expected to call following.

				// Get the underlying WebKit WebView object.
				// The WebView is the highest level container for a web browser window;
				// It is the container of the doc/view and corresponds to a single web
				// browser view, such as a tab in FireFox. Multiple WebViews correspond
				// to multiple tabs, each with its own URL and page history.
				// From WebView you can get the WebView's Frame, FrameView, Page, etc.
				virtual ::WebView* GetWebView() const;

				// Update 10/19/2010 - Note by Arpit Baldeva: 
				// In APIs below, such as GetWebFrame(), main Frame of the page is returned by default.
				// If you pass in focusFrame boolean to be true, focused frame is returned. Note that it is possible to get NULL when 
				// looking for focused frame as a frame does not get focus unless the user clicks on it.

				// Get the View's WebFrame.
				// The WebFrame is the doc portion of the doc/view model, but it seems
				// to be a higher level wrapper for the Frame class, which itself is
				// a doc model. It remains to be understood why there are separate
				// WebFrame and Frame classes instead of a single class.
				// Update - Indeed, the WebFrame serves as a high level wrapper of lower level WebCore::Frame and WebCore::FrameView classes. Some ports of WebKit have 
				// WebFrameView wrapper for WebCore::FrameView. Our's does not.
				virtual ::WebFrame* GetWebFrame(bool focusFrame = false) const;

				// Get the View's Frame.
				// The Frame is the the lower level implementation of the doc part of 
				// the doc/view model.
				virtual WebCore::Frame* GetFrame(bool focusFrame = false) const;


				// Get the View's FrameView.
				// The FrameView is the 'view' part of the doc/view model.
				virtual WebCore::FrameView* GetFrameView(bool focusFrame = false) const;
				
				// Get the View's top level Document.
				// The document represents the HTML content of a web page.
				virtual WebCore::Document* GetDocument(bool focusFrame = false) const;

				// Get the View's top level Page.
				// The page represents a lower level of the Frame's data.
				// It's not clear why Page is separated from Frame in the class
				// hierarchy.
				virtual WebCore::Page* GetPage() const;

				
			
			//
			// END - APIs normally not needed to be called by an application.
			//


		private:
			virtual void ScrollOnJump(bool vertical, float numLinesDelta);
			// Overloaded the public API with a private implementation detail. In case the element is not found when player is on edge, we scroll once to bring
			// newer elements in the visible area and try again. 
			virtual bool JumpToNearestElement(EA::WebKit::JumpDirection direction, bool scrollIfElementNotFound);

			virtual void QueueRegionToDrawUpdate(int x, int y, int width, int height);
			
			::WebView*						    mpWebView;
			EA::Raster::ISurface*				mpSurface;
			ViewParameters						mViewParameters;
			LoadInfo							mLoadInfo;
			EA::Raster::Point					mCursorPos;
			ModalInputClient*					mpModalInputClient;    // There can only be one at a time.
			OverlaySurfaceArrayContainer*	    mOverlaySurfaceArrayContainer;
			LinkHookManager						mLinkHookManager;
			TextInputStateInfo					mTextInputStateInfo;   // For tracking if text edit mode is on or off
			KJS::Debugger*						mDebugger;

			NodeListContainer*					mNodeListContainer;

			WebCore::Frame*						mBestNodeFrame;//Frame where the last best node was found.
			//abaldeva: Feel like following should be a higher level struct. Unfortunately, we can't expose WebCore::IntRect.
			int									mBestNodeX;
			int									mBestNodeY;
			int									mBestNodeWidth;
			int									mBestNodeHeight;
			int									mCentreX;
			int									mCentreY;
#if EAWEBKIT_ENABLE_JUMP_NAVIGATION_DEBUGGING		
			int									mAxesX;
			int									mAxesY;
			int									mMinX;
			int									mMinY;
			int									mMaxX;
			int									mMaxY;
#endif
			EASTLFixedString8Wrapper			mCachedNavigationUpId;
			EASTLFixedString8Wrapper			mCachedNavigationDownId;
			EASTLFixedString8Wrapper			mCachedNavigationLeftId;
			EASTLFixedString8Wrapper			mCachedNavigationRightId;

			EASTLFixedString16Wrapper			mURI;
			BalObject*							mJavascriptBindingObject;
			EASTLFixedString8Wrapper			mJavascriptBindingObjectName;//changed this from char* to EASTLFixedString8Wrapper. We can't rely on char* to remain valid across CreateJavascriptBindings/RebindJavascript calls.
			JumpNavigationParams				mJumpNavigationParams;
            bool                                mOwnsViewSurface;     // Set if the surface is created by EAWebkit.
			bool								mEmulatingConsoleOnPC;
        };


		// View enumeration.
        // These functions are not thread-safe with respect to lifetime management
        // of View instances. You cannot be creating and destroying Views in other
        // threads while using these functions. However, these functions will execute
        // a memory read barrier upon reading the View maintenance data structures
        // to handle the case whereby you may have manipulated Views from other threads
        // but know that there is no further manipulation occurring.

		EAWEBKIT_API View* CreateView();
		EAWEBKIT_API void  DestroyView(View* pView);

		EAWEBKIT_API int   GetViewCount();                           // Get the current number of EA::WebKit::Views.
        EAWEBKIT_API View* GetView(int index);                       // Get the nth EA::WebKit::View, in range if [0, GetViewCount).
        EAWEBKIT_API bool  IsViewValid(View* pView);                 // 
        EAWEBKIT_API View* GetView(::WebView* pWebView);             // Get an EA::WebKit::View from a WebKit WebView.
        EAWEBKIT_API View* GetView(::WebFrame* pWebFrame);           // Get an EA::WebKit::View from a WebKit WebFrame.
        EAWEBKIT_API View* GetView(WebCore::Frame* pFrame);          // Get an EA::WebKit::View from a WebCore::Frame.
        EAWEBKIT_API View* GetView(WebCore::FrameView* pFrameView);  // Get an EA::WebKit::View from a WebCore::FrameView.
        
    } // namespace WebKit

} // namespace EA

#endif // Header include guard
