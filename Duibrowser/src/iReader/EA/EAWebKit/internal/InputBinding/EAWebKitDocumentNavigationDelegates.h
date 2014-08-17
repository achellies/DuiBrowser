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
// EAWebKitDocumentNavigationDelegates.h
// By Chris Stott
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_EAWEBKITDOCUMENTNAVIGATIONDELEGATES_H
#define EAWEBKIT_EAWEBKITDOCUMENTNAVIGATIONDELEGATES_H

#include <EAWebKit/EAWebKitConfig.h>
#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif

namespace WebCore
{
	class Node;
	class Element;
}

namespace EA
{
	
	namespace WebKit
	{
		class View;

		class DelegateBase
		{
		public:
			DelegateBase(EA::WebKit::View* view) : mView(view)
			{
				// empty
			}

		protected:
			bool CanJumpToNode(WebCore::Node* node, bool checkHTMLElementType = true, bool checkSize = true);
			
			const EA::WebKit::View* GetView() const 
			{ 
				return mView; 
			}
			EA::WebKit::View* GetView() 
			{ 
				return mView; 
			}


		private:
			EA::WebKit::View* mView;
		};

		class JumpToFirstLinkDelegate : public DelegateBase
		{
		public:
			explicit JumpToFirstLinkDelegate(EA::WebKit::View* view)
				:	DelegateBase(view)
				,	mFoundElement(0)
			{

			}

			bool operator()(WebCore::Node* node);
			
			WebCore::Element* FoundElement() const { return mFoundElement; }

		protected:
			bool InterestedInNode(WebCore::Node* node);
			void ApplyAction(WebCore::Node* node);
			
		private:
			WebCore::Element* mFoundElement;
		};


		class JumpToElementWithClassDelegate : public DelegateBase
		{
		public:
			explicit JumpToElementWithClassDelegate(EA::WebKit::View* view, const char* jumpToClass)
				: DelegateBase(view)
				, mFoundElement(0)
				, mJumpToClass(jumpToClass)
			{

			}
			bool operator()(WebCore::Node* node);
			
			WebCore::Element* FoundElement() const 
			{ 
				return mFoundElement; 
			}

		protected:
			bool InterestedInNode(WebCore::Node* node);
			void ApplyAction(WebCore::Node* node);
			
		private:
			WebCore::Element*	mFoundElement;
			const char*			mJumpToClass;
		};

		class JumpToElementWithIdDelegate : public DelegateBase
		{
		public:
			explicit JumpToElementWithIdDelegate(EA::WebKit::View* view, const char* jumpToId)
				: DelegateBase(view) 
				, mFoundElement(0)
				, mJumpToId(jumpToId)
			{

			}

			bool operator()(WebCore::Node* node);
			
			WebCore::Element* FoundElement() const 
			{ 
				return mFoundElement; 
			}

		protected:
			bool InterestedInNode(WebCore::Node* node);
			void ApplyAction(WebCore::Node* node);
		private:
			WebCore::Element*	mFoundElement;
			const char*			mJumpToId;
		};

		class IsNodeNavigableDelegate : DelegateBase
		{
		public:
			IsNodeNavigableDelegate(EA::WebKit::View* view) : DelegateBase(view), mFoundNode(false) {}

			bool operator()(WebCore::Node* node, bool checkSize = true);
			
			bool FoundNode() const 
			{ 
				return mFoundNode; 
			}

		private:
			bool mFoundNode;
		};

		class ClickElementsByIdOrClassDelegate
		{
		public:
			ClickElementsByIdOrClassDelegate(const char* idOrClassName, bool includeId) 
				:	mIDOrClassName(idOrClassName)
				,	mReturnValue(false)
				,	mIncludeId(includeId)
			{
				// empty
			}

			bool operator()(WebCore::Node* node);
			
			bool GetReturnValue() const 
			{ 
				return mReturnValue; 
			}

		private:
			const char* mIDOrClassName;
			bool		mReturnValue;
			bool		mIncludeId;
		};

	}
}

#endif //EAWEBKIT_EAWEBKITDOCUMENTNAVIGATIONDELEGATES_H