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
// EAWebKitDOMWalker.h
// By Chris Stott
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_EAWEBKITDOMWALKER_H
#define EAWEBKIT_EAWEBKITDOMWALKER_H

#include "EAWebKitEventListener.h"
#include "HTMLElement.h"
#include "Element.h"
#include "Document.h"

#include <EAWebKit/EAWebKitConfig.h>
#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif

#ifndef HTMLElement_h
#error HTMLElement_h
#endif

#ifndef StyledElement_h
#error StyledElement_h
#endif

#ifndef Element_h
#error Element_h
#endif

namespace EA
{
	namespace WebKit
	{

		// Note by Arpit Baldeva: 09/22/2010 - Following is deprecation candidate now and hence not modifed/moved.
		class InputDelegate
		{
		public:
			explicit InputDelegate(KeyboardCallback callback)
				: mCallback(callback)
			{

			}

			bool operator()(WebCore::Node* node)
			{
				if (InterestedInNode(node))
				{
					ApplyAction(node);
				}

				return true;
			}

		protected:
			bool InterestedInNode(WebCore::Node* node)
			{
				if (node->nodeType() == WebCore::Node::ELEMENT_NODE)
				{
					WebCore::Element* element = (WebCore::Element*)node;

					if (element->isHTMLElement())
					{
						WebCore::HTMLElement* htmlElement = (WebCore::HTMLElement*)element;

						if (htmlElement->tagName()=="INPUT")
						{
							WebCore::HTMLInputElement* inputElement = (WebCore::HTMLInputElement*)htmlElement;

							if (inputElement->type() == "text" || inputElement->type() == "password")
							{
								return true;
							}
						}
					}
				}

				return false;
			}

			void ApplyAction(WebCore::Node* node)
			{
				// THESE should be HTMLElements, but I'm getting the weirdest compile errors currently
				WebCore::Element* htmlElement = (WebCore::Element*)node;
				KeyboardEventListener* listener = new KeyboardEventListener(mCallback, htmlElement);
				htmlElement->setHTMLEventListener(WebCore::eventNames().focusEvent, WTF::adoptRef(listener));
				htmlElement->blur();
			}
		private:
			KeyboardCallback mCallback;
		};

		// Note by Arpit Baldeva: 09/22/2010 - Following is deprecation candidate now and hence not modifed/moved.
		class TextAreaDelegate
		{
		public:
			explicit TextAreaDelegate(KeyboardCallback callback)
				: mCallback(callback)
			{

			}


			bool operator()(WebCore::Node* node)
			{
				if (InterestedInNode(node))
				{
					ApplyAction(node);
				}

				return true;
			}

		protected:
			bool InterestedInNode(WebCore::Node* node)
			{
				if (node->nodeType() == WebCore::Node::ELEMENT_NODE)
				{
					WebCore::Element* element = (WebCore::Element*)node;

					if (element->isHTMLElement())
					{
						WebCore::HTMLElement* htmlElement = (WebCore::HTMLElement*)element;

						//abaldeva:TODO - Wondering why a wide char comparison here as opposed to char comparison in above.
						if (wcscmp(htmlElement->tagName().charactersWithNullTermination(),L"TEXTAREA") == 0)
						{
							return true;
						}
					}
				}

				return false;
			}

			void ApplyAction(WebCore::Node* node)
			{
				WebCore::Element* htmlElement = (WebCore::Element*)node;
				KeyboardEventListener* listener = new KeyboardEventListener(mCallback, htmlElement);

				htmlElement->setHTMLEventListener(WebCore::eventNames().focusEvent, WTF::adoptRef(listener));

				htmlElement->blur();
			}

		private:
			KeyboardCallback mCallback;
		};



		
		
		template <typename Delegate>
		class DOMWalker
		{
		public:
			DOMWalker(WebCore::Document* document, Delegate& delegate)
				:	mDelegate(delegate)
				,	mShouldContinue(true)
			{
				WalkNodes(document);
			}

		private:
			Delegate& mDelegate;
			bool mShouldContinue;


			void WalkNodes(WebCore::Node* rootNode)
			{
				// Note by Arpit Baldeva - Changed the recursive algorithm to an iterative algorithm. This results in 25 To 40% increase in efficiency.
				while(rootNode)
				{
					mShouldContinue = mDelegate(rootNode);
					if(!mShouldContinue)
						break;

					rootNode = rootNode->traverseNextNode();
				}
				/* Old recursive code
				if (mShouldContinue)
				{
					//////////////////////////////////////////////////////////////////////////
					// WORRY ABOUT YOURSELF
					mShouldContinue = mDelegate(rootNode);

					if (mShouldContinue)
					{
						//////////////////////////////////////////////////////////////////////////
						// THEN, FIND THE CHILDREN
						if (rootNode && rootNode->childNodeCount() > 0)
						{
							PassRefPtr<WebCore::NodeList> children = rootNode->childNodes();

							const uint32_t length = children->length();

							for (uint32_t i=0; i < length; ++i)
							{
								WebCore::Node* child = children->item(i);
								if (child)
								{
									WalkNodes(child);
								}
							}
						}
					}
				}
				*/
			}
		};	

		// TODO : Make this a direction trait?
		template <typename Delegate>
		class ReverseDOMWalker
		{
		public:
			ReverseDOMWalker(WebCore::Node* node, Delegate& delegate)
				:	mDelegate(delegate)
				,	mShouldContinue(true)
			{
				WalkNodes(node);

			}

		private:
			Delegate& mDelegate;
			bool mShouldContinue;


			void WalkNodes(WebCore::Node* childNode)
			{
				if (mShouldContinue)
				{
					//////////////////////////////////////////////////////////////////////////
					// WORRY ABOUT YOURSELF
					mShouldContinue = mDelegate(childNode);

					if (mShouldContinue)
					{
						//////////////////////////////////////////////////////////////////////////
						// THEN, GO TO THE PARENT
						if (childNode && childNode->parentNode())
						{
							WebCore::Node* parentNode = childNode->parentNode();

							WalkNodes(parentNode);
						}
					}
				}
			}
		};	

	} // namespace WebKit
} // namespace EA

#endif //EAWEBKIT_EAWEBKITDOMWALKER_H