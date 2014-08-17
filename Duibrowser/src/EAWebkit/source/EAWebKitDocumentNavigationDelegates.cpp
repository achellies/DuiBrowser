/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EAWebKitDocumentNavigationDelegates.cpp
// By Chris Stott
///////////////////////////////////////////////////////////////////////////////

#include <EAWebKit/internal/InputBinding/EAWebKitDocumentNavigationDelegates.h>
#include <EAWebKit/EAWebKitView.h>

#include "Node.h"
#include "HTMLElement.h"
#include "HTMLNames.h"
#include "IntRect.h"
#include "RenderStyle.h"

namespace EA
{
	namespace WebKit
	{
		bool DelegateBase::CanJumpToNode(WebCore::Node* node, bool checkHTMLElementType, bool checkSize)
		{
			if ((node->nodeType() == WebCore::Node::ELEMENT_NODE) && node->isHTMLElement())
			{
				WebCore::HTMLElement* htmlElement = (WebCore::HTMLElement*)node;
				if(checkHTMLElementType)
				{
					//Following overhead is negligible
					if(!(htmlElement->hasTagName(WebCore::HTMLNames::aTag) || htmlElement->hasTagName(WebCore::HTMLNames::inputTag) || htmlElement->hasTagName(WebCore::HTMLNames::textareaTag)
						|| htmlElement->hasTagName(WebCore::HTMLNames::buttonTag) || htmlElement->hasTagName(WebCore::HTMLNames::selectTag)))
					{
						return false;
					}
				}

				// We don't check against size unless specified. It is pretty expensive to call getRect and caller may already be doing that.
				if(checkSize)
				{
					WebCore::IntRect rect = htmlElement->getRect();					
					bool bigEnough = (rect.width() >= 1 && rect.height() >= 1);// Avoid 0 size elements 
					if(!bigEnough)
					{
						return false;
					}
				}
				
				if (htmlElement->computedStyle()->visibility()==WebCore::VISIBLE && htmlElement->computedStyle()->display()!=WebCore::NONE)
				{
					// If the element does not have class or if it does, it is not supposed to be ignored.
					if (!htmlElement->hasClass() || !htmlElement->classNames().contains("navigation_ignore")) 
					{	
						return true;						
					}
				}
			}

			return false;
		}


		//
		// JumpToFirstLinkDelegate
		//
		bool JumpToFirstLinkDelegate::operator() (WebCore::Node* node)
		{
			if (InterestedInNode(node))
			{
				ApplyAction(node);
				// by returning false we'll taking the domwalker that we've found the node we want, and to stop.
				return false;
			}
			return true;
		}

		bool JumpToFirstLinkDelegate::InterestedInNode(WebCore::Node* node)
		{
			return CanJumpToNode(node);
		}

		void JumpToFirstLinkDelegate::ApplyAction(WebCore::Node* node)
		{
			mFoundElement = (WebCore::Element*) node;
			GetView()->MoveMouseCursorToNode(mFoundElement);
		}


		//
		// JumpToElementWithClassDelegate
		//
		bool JumpToElementWithClassDelegate::operator ()(WebCore::Node* node)
		{
			if (InterestedInNode(node))
			{
				ApplyAction(node);
				// by returning false we'll taking the domwalker that we've found the node we want, and to stop.
				return false;
			}
			return true;
		}

		bool JumpToElementWithClassDelegate::InterestedInNode( WebCore::Node* node )
		{
			if (CanJumpToNode(node, false)) //We allow jumping to an arbitrary element through class mechanism.
			{
				WebCore::HTMLElement* htmlElement = (WebCore::HTMLElement*)node;
				if (htmlElement->hasClass() && htmlElement->classNames().contains(mJumpToClass))
				{
					return true;						
				}
			}
			return false;
		 }

		void JumpToElementWithClassDelegate::ApplyAction(WebCore::Node* node)
		{
			mFoundElement = (WebCore::Element*) node;
			GetView()->MoveMouseCursorToNode(mFoundElement);
		}

		
		//
		// JumpToElementWithIdDelegate
		//
		bool JumpToElementWithIdDelegate::operator ()(WebCore::Node* node)
		{
			if (InterestedInNode(node))
			{
				ApplyAction(node);
				// by returning false we'll taking the domwalker that we've found the node we want, and to stop.
				return false;
			}
			return true;
		}

		bool JumpToElementWithIdDelegate::InterestedInNode(WebCore::Node* node)
		{
			if (CanJumpToNode(node, false)) //We allow jumping to an arbitrary element through class mechanism.
			{
				WebCore::HTMLElement* htmlElement = (WebCore::HTMLElement*)node;
				if (htmlElement->id() == mJumpToId)
				{
					return true;
				}
			}
			return false;
		}

		void JumpToElementWithIdDelegate::ApplyAction(WebCore::Node* node)
		{
			mFoundElement = (WebCore::Element*) node;
			GetView()->MoveMouseCursorToNode(mFoundElement);
		}

		
		//
		// IsNodeNavigableDelegate
		//
		bool IsNodeNavigableDelegate::operator ()(WebCore::Node* node, bool checkSize)
		{
			bool canJump = CanJumpToNode(node, true, checkSize);
			if (canJump)
			{
				mFoundNode = true;
			}

			// by returning false we'll taking the domwalker that we've found the node we want, and to stop.
			return !canJump;
		}

		
		
		//
		// ClickElementsByIdOrClassDelegate
		//
		bool ClickElementsByIdOrClassDelegate::operator ()(WebCore::Node* node)
		{
			// We don't call CanJumpToNode here from the DelegateBase. In fact, we don't derive this class from DelegateBase.
			// The reason is you may want to be able to click something hidden but not navigate over it.
			
			// A note from Chris Stott
			// Imagine binding triangle button to an invisible anchor. 
			// <a href="#" id="triangle_button" style="display:none" onclick="doSomething(); return false;" />
			// We definitely need that distinction between jumping delegates and clicking delegates.
			//
			
			if ((node->nodeType() == WebCore::Node::ELEMENT_NODE) && node->isHTMLElement())
			{
					WebCore::HTMLElement* htmlElement = (WebCore::HTMLElement*)node;

					//Note by Arpit Baldeva: Changed following for two reasons.
					// 1. It matches the public interface of the EA::WebKit::View which is ClickElementsByIdOrClass.
					// 2. Id is more specific than class so you would want to generally click an element with matching Id than class.

					// To consider: Wouldn't the normal expectation of the user be that the entire tree is traversed for a matching Id and if not found, then the entire tree should be traversed
					// for matching the class? The way this API is written, the intention seems to be that Web developers can mix and match class and id. I am not so web development savvy and 
					// thus not sure if it is considered sloppy in the Web development or is standard practice.

					// test id, and then the class
					if ((mIncludeId && htmlElement->id() == mIDOrClassName) || (htmlElement->hasClass() && htmlElement->classNames().contains(mIDOrClassName)))
					{
						htmlElement->click();
						mReturnValue = true;
					}
			}
			return true;
		}

	}
}

