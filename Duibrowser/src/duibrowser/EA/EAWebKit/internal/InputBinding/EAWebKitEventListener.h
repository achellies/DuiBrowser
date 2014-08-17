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
// EAWebKitEventListener.h
// By Chris Stott
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_EAWEBKITEVENTLISTENER_H
#define EAWEBKIT_EAWEBKITEVENTLISTENER_H

#include <EventListener.h>
#include "HTMLInputElement.h"
#include <platform/text/PlatformString.h>
#include <platform/text/CString.h>
#include <EAWebKit/EAWebKitConfig.h>
#include "XMLHttpRequestProgressEvent.h"
#include "EventNames.h"
#include "webcore/html/HTMLInputElement.h"
#include "webcore/html/HTMLTextAreaElement.h"

#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif


namespace EA
{
	namespace WebKit
	{
		class KeyboardEventListener : public WebCore::EventListener
		{
		public:
			explicit KeyboardEventListener(KeyboardCallback callback, WebCore::Element* element) : mCallback(callback), mElement(element) {}
			virtual ~KeyboardEventListener() {}
			virtual void handleEvent(WebCore::Event* event, bool isWindowEvent)
			{
				if(mElement->tagName() == "INPUT")
				{
					WebCore::HTMLInputElement* inputElement = (WebCore::HTMLInputElement*)mElement;
					
					mCallback(mElement, (char16_t*)inputElement->value().charactersWithNullTermination(), (char16_t*)inputElement->alt().charactersWithNullTermination(), inputElement->maxLength());
				}
				else if (mElement->tagName() == "TEXTAREA")
				{
					WebCore::HTMLTextAreaElement* inputElement = (WebCore::HTMLTextAreaElement*)mElement;
					WebCore::String alt = inputElement->getAttribute("alt").string();

					WebCore::NamedAttrMap* attributeMap = inputElement->attributes();
					WebCore::Attribute* maxLengthAttribute = attributeMap->getAttributeItem("maxlength");


					int maxLength = maxLengthAttribute ? maxLengthAttribute->value().toInt() : inputElement->cols()*inputElement->rows();

					mCallback(mElement, (char16_t*)inputElement->value().charactersWithNullTermination(), (char16_t*)alt.charactersWithNullTermination(), maxLength);
				}
				else
				{
					mCallback(mElement, L"", L"", 32);
				}

			}
		private:
			KeyboardCallback mCallback;
			WebCore::Element* mElement;
		};


		class XMLHttpRequestEventListener : public WebCore::EventListener
		{
		public:
			explicit XMLHttpRequestEventListener(){}
			virtual ~XMLHttpRequestEventListener() {}

			virtual void handleEvent(WebCore::Event* event, bool isWindowEvent)
			{
				if (event->isXMLHttpRequestProgressEvent())
				{
					WebCore::XMLHttpRequestProgressEvent* progressEvent = static_cast<WebCore::XMLHttpRequestProgressEvent*>(event);

					EA::WebKit::XMLHttpRequestEventInfo info;

					info.mURI = progressEvent->uri();

					if (progressEvent->type() == WebCore::eventNames().abortEvent)
					{
						info.mEventType = EA::WebKit::kXHRLETAbort;
					}
					else if (progressEvent->type() == WebCore::eventNames().errorEvent)
					{
						info.mEventType = EA::WebKit::kXHRLETError;
					}
					else if (progressEvent->type() == WebCore::eventNames().loadEvent)
					{
						info.mEventType = EA::WebKit::kXHRLETLoad;
					}
					else if (progressEvent->type() == WebCore::eventNames().loadstartEvent)
					{
						info.mEventType = EA::WebKit::kXHRLETLoadStart;
					}
					else if (progressEvent->type() == WebCore::eventNames().progressEvent)
					{
						info.mEventType = EA::WebKit::kXHRLETProgress;
					}

					info.mLoaded = progressEvent->loaded();
					info.mTotal = progressEvent->total();
					info.mLengthComputable = progressEvent->lengthComputable();

					EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
					if(pViewNotification)
						pViewNotification->XMLHttpRequestEvent(info);
				}
			}
		};
	} // namespace WebKit
} // namespace EA


#endif //EAWEBKIT_EAWEBKITEVENTLISTENER_H