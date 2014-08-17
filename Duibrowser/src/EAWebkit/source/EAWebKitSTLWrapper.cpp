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
// EAWebKitSTLWrapper.cpp
// By Arpit Baldeva
///////////////////////////////////////////////////////////////////////////////

#include <EAWebKit/EAWebkitSTLWrapper.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>
#include <EAWebKit/EAWebKitView.h>
#include <EAAssert/eaassert.h>
#include <wtf/FastAllocBase.h>

namespace EA
{
	namespace WebKit
	{
		/******************************************EASTLFixedString8Wrapper******************************/
        EA_COMPILETIME_ASSERT(EASTLFixedString8Wrapper::kFixedStringSize == sizeof(FixedString8_128));
        EA_COMPILETIME_ASSERT(4 == EA_ALIGN_OF(FixedString8_128));

#if defined(EA_PLATFORM_PS3)
		EASTLFixedString8Wrapper::EASTLFixedString8Wrapper()
			: mString8(0)
		{
			mString8 = EAWEBKIT_NEW("fixedstring8_128") FixedString8_128();
		}

		EASTLFixedString8Wrapper::EASTLFixedString8Wrapper(const char8_t* str)
			: mString8(0)
		{
			mString8 = EAWEBKIT_NEW("fixedstring8_128") FixedString8_128(str);
		}

		EASTLFixedString8Wrapper::EASTLFixedString8Wrapper(const EASTLFixedString8Wrapper& rhs)
		{
			mString8 = EAWEBKIT_NEW("fixedstring8_128") FixedString8_128();
			*(reinterpret_cast<FixedString8_128*>(mString8)) = *(reinterpret_cast<FixedString8_128*> ((rhs).GetImpl()));
		}

		EASTLFixedString8Wrapper& EASTLFixedString8Wrapper::operator =(const EASTLFixedString8Wrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<FixedString8_128*>(mString8);
			mString8 = EAWEBKIT_NEW("fixedstring8_128") FixedString8_128();
			*(reinterpret_cast<FixedString8_128*>(mString8)) = *(reinterpret_cast<FixedString8_128*> ((rhs).GetImpl()));

			return *this;
		}

		EASTLFixedString8Wrapper::~EASTLFixedString8Wrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			FixedString8_128* stringPtr = reinterpret_cast<FixedString8_128*>(mString8);
			EAWEBKIT_DELETE stringPtr;
			mString8 = NULL;
		}

		void* EASTLFixedString8Wrapper::GetImpl() const
		{
			return mString8;
		}
#else

        EASTLFixedString8Wrapper::EASTLFixedString8Wrapper()
		{
            new(mString8) FixedString8_128();
		}

		EASTLFixedString8Wrapper::EASTLFixedString8Wrapper(const char8_t* str)
		{
            new(mString8) FixedString8_128(str);
		}

		EASTLFixedString8Wrapper::EASTLFixedString8Wrapper(const EASTLFixedString8Wrapper& rhs)
		{
            new(mString8) FixedString8_128();
            *(reinterpret_cast<FixedString8_128*>(mString8)) = *(reinterpret_cast<FixedString8_128*> ((rhs).GetImpl()));
		}

		EASTLFixedString8Wrapper& EASTLFixedString8Wrapper::operator =(const EASTLFixedString8Wrapper& rhs)
		{
			if(this == &rhs)
				return *this;

            reinterpret_cast<FixedString8_128*>(mString8)->~FixedString8_128();
            new(mString8) FixedString8_128();
			*(reinterpret_cast<FixedString8_128*>(mString8)) = *(reinterpret_cast<FixedString8_128*> ((rhs).GetImpl()));
			
			return *this;
		}

		EASTLFixedString8Wrapper::~EASTLFixedString8Wrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			FixedString8_128* stringPtr = reinterpret_cast<FixedString8_128*>(mString8);
            stringPtr->~FixedString8_128();
		}

		void* EASTLFixedString8Wrapper::GetImpl() const
		{
			return (void*)mString8;
		}
#endif


		/******************************************EASTLFixedString16Wrapper******************************/
        EA_COMPILETIME_ASSERT(EASTLFixedString16Wrapper::kMaxFixedStringSize == sizeof(FixedString16_128));
        EA_COMPILETIME_ASSERT(4 == EA_ALIGN_OF(FixedString16_128));

#if defined(EA_PLATFORM_PS3)
		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper()
			: mString16(0)
		{
			mString16 = EAWEBKIT_NEW("fixedstring16_128") FixedString16_128();
		}

		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper(const char16_t* str)
			: mString16(0)
		{
			mString16 = EAWEBKIT_NEW("fixedstring16_128") FixedString16_128(str);
		}

		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper(const EASTLFixedString16Wrapper& rhs)
		{
			mString16 = EAWEBKIT_NEW("fixedstring16_128") FixedString16_128();
			*(reinterpret_cast<FixedString16_128*>(mString16)) = *(reinterpret_cast<FixedString16_128*> ((rhs).GetImpl()));
		}

		EASTLFixedString16Wrapper& EASTLFixedString16Wrapper::operator =(const EASTLFixedString16Wrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<FixedString16_128*>(mString16);
			mString16 = EAWEBKIT_NEW("fixedstring16_128") FixedString16_128();
			*(reinterpret_cast<FixedString16_128*>(mString16)) = *(reinterpret_cast<FixedString16_128*> ((rhs).GetImpl()));

			return *this;
		}

		void* EASTLFixedString16Wrapper::GetImpl() const
		{
			return mString16;
		}

		EASTLFixedString16Wrapper::~EASTLFixedString16Wrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			FixedString16_128* stringPtr = reinterpret_cast<FixedString16_128*>(mString16);
			EAWEBKIT_DELETE stringPtr;
			mString16 = NULL;
		}
#else
		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper()
		{
            new(mString16) FixedString16_128();
		}

		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper(const char16_t* str)
		{
            new(mString16) FixedString16_128(str);
		}

		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper(const EASTLFixedString16Wrapper& rhs)
		{
            new(mString16) FixedString16_128();
            *(reinterpret_cast<FixedString16_128*>(mString16)) = *(reinterpret_cast<FixedString16_128*> ((rhs).GetImpl()));
		}

		EASTLFixedString16Wrapper& EASTLFixedString16Wrapper::operator =(const EASTLFixedString16Wrapper& rhs)
		{
			if(this == &rhs)
				return *this;

            reinterpret_cast<FixedString16_128*>(mString16)->~FixedString16_128();
            new(mString16) FixedString16_128();
			*(reinterpret_cast<FixedString16_128*>(mString16)) = *(reinterpret_cast<FixedString16_128*> ((rhs).GetImpl()));

			return *this;
		}
		
		void* EASTLFixedString16Wrapper::GetImpl() const
		{
			return (void*)mString16;
		}

		EASTLFixedString16Wrapper::~EASTLFixedString16Wrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			FixedString16_128* stringPtr = reinterpret_cast<FixedString16_128*>(mString16);
            stringPtr->~FixedString16_128();
		}
#endif


		/******************************************EASTLHeaderMapWrapper******************************/
		EASTLHeaderMapWrapper::EASTLHeaderMapWrapper()
			: mHeaderMap(0)
		{
			mHeaderMap = EAWEBKIT_NEW("HeaderMap") HeaderMap();
		}

		EASTLHeaderMapWrapper::EASTLHeaderMapWrapper(const EASTLHeaderMapWrapper& rhs)
		{
			mHeaderMap = EAWEBKIT_NEW("HeaderMap") HeaderMap();
			*(reinterpret_cast<HeaderMap*>(mHeaderMap)) = *(reinterpret_cast<HeaderMap*> ((rhs).GetImpl()));
		}

		EASTLHeaderMapWrapper& EASTLHeaderMapWrapper::operator =(const EASTLHeaderMapWrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<HeaderMap*>(mHeaderMap);
			mHeaderMap = EAWEBKIT_NEW("HeaderMap") HeaderMap();
			*(reinterpret_cast<HeaderMap*>(mHeaderMap)) = *(reinterpret_cast<HeaderMap*> ((rhs).GetImpl()));

			return *this;
		}

		EASTLHeaderMapWrapper::~EASTLHeaderMapWrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			HeaderMap* headerMapPtr = reinterpret_cast<HeaderMap*>(mHeaderMap);
			EAWEBKIT_DELETE headerMapPtr;
			mHeaderMap = NULL;
		}
		
		void* EASTLHeaderMapWrapper::GetImpl() const
		{
			return mHeaderMap;
		}
		
        /******************************************EASTLVectorJavaScriptValueWrapper******************************/
        EA_COMPILETIME_ASSERT(EASTLVectorJavaScriptValueWrapper::kSize == sizeof(VectorJavaScriptValue));
        EA_COMPILETIME_ASSERT(4 == EA_ALIGN_OF(VectorJavaScriptValue));

#if defined(EA_PLATFORM_PS3)
		EASTLVectorJavaScriptValueWrapper::EASTLVectorJavaScriptValueWrapper()
			: mVector(0)
		{
			mVector = EAWEBKIT_NEW("VectorJavaScriptValue") VectorJavaScriptValue();
		}

		EASTLVectorJavaScriptValueWrapper::EASTLVectorJavaScriptValueWrapper(const EASTLVectorJavaScriptValueWrapper& rhs)
		{
			mVector = EAWEBKIT_NEW("VectorJavaScriptValue") VectorJavaScriptValue();
			*(reinterpret_cast<VectorJavaScriptValue*>(mVector)) = *(reinterpret_cast<VectorJavaScriptValue*> ((rhs).GetImpl()));
		}

		EASTLVectorJavaScriptValueWrapper& EASTLVectorJavaScriptValueWrapper::operator =(const EASTLVectorJavaScriptValueWrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<VectorJavaScriptValue*>(mVector);
			mVector = EAWEBKIT_NEW("VectorJavaScriptValue") VectorJavaScriptValue();
			*(reinterpret_cast<VectorJavaScriptValue*>(mVector)) = *(reinterpret_cast<VectorJavaScriptValue*> ((rhs).GetImpl()));

			return *this;
		}

		EASTLVectorJavaScriptValueWrapper::~EASTLVectorJavaScriptValueWrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			VectorJavaScriptValue* vectorPtr = reinterpret_cast<VectorJavaScriptValue*>(mVector);
			EAWEBKIT_DELETE vectorPtr;
			mVector = NULL;
		}

		void* EASTLVectorJavaScriptValueWrapper::GetImpl() const
		{
			return mVector;
		}
#else
		EASTLVectorJavaScriptValueWrapper::EASTLVectorJavaScriptValueWrapper()
		{
            new(mVector) VectorJavaScriptValue();
		}

        EASTLVectorJavaScriptValueWrapper::EASTLVectorJavaScriptValueWrapper(const EASTLVectorJavaScriptValueWrapper& rhs)
        {
            new(mVector) VectorJavaScriptValue();
            *(reinterpret_cast<VectorJavaScriptValue*>(mVector)) = *(reinterpret_cast<VectorJavaScriptValue*> ((rhs).GetImpl()));
        }

        EASTLVectorJavaScriptValueWrapper& EASTLVectorJavaScriptValueWrapper::operator =(const EASTLVectorJavaScriptValueWrapper& rhs)
        {
            if(this == &rhs)
                return *this;

            *(reinterpret_cast<VectorJavaScriptValue*>(mVector)) = *(reinterpret_cast<VectorJavaScriptValue*> ((rhs).GetImpl()));
            return *this;
        }
		
		void* EASTLVectorJavaScriptValueWrapper::GetImpl() const
		{
			return (void*)mVector;
		}

		EASTLVectorJavaScriptValueWrapper::~EASTLVectorJavaScriptValueWrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
            reinterpret_cast<VectorJavaScriptValue*>(mVector)->~VectorJavaScriptValue();
		}
#endif
        /******************************************EASTLJavascriptValueHashMapWrapper******************************/
        EA_COMPILETIME_ASSERT(EASTLJavascriptValueHashMapWrapper::kSize == sizeof(HashMapJavaScriptValue));
        EA_COMPILETIME_ASSERT(4 == EA_ALIGN_OF(HashMapJavaScriptValue));

#if defined(EA_PLATFORM_PS3)
		EASTLJavascriptValueHashMapWrapper::EASTLJavascriptValueHashMapWrapper()
			: mHashMap(0)
		{
			mHashMap = EAWEBKIT_NEW("HashMapJavaScriptValue") HashMapJavaScriptValue();
		}

		EASTLJavascriptValueHashMapWrapper::EASTLJavascriptValueHashMapWrapper(const EASTLJavascriptValueHashMapWrapper& rhs)
		{
			mHashMap = EAWEBKIT_NEW("HashMapJavaScriptValue") HashMapJavaScriptValue();
			*(reinterpret_cast<HashMapJavaScriptValue*>(mHashMap)) = *(reinterpret_cast<HashMapJavaScriptValue*> ((rhs).GetImpl()));
		}

		EASTLJavascriptValueHashMapWrapper& EASTLJavascriptValueHashMapWrapper::operator =(const EASTLJavascriptValueHashMapWrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<HashMapJavaScriptValue*>(mHashMap);
			mHashMap = EAWEBKIT_NEW("HashMapJavaScriptValue") HashMapJavaScriptValue();
			*(reinterpret_cast<HashMapJavaScriptValue*>(mHashMap)) = *(reinterpret_cast<HashMapJavaScriptValue*> ((rhs).GetImpl()));

			return *this;
		}

		EASTLJavascriptValueHashMapWrapper::~EASTLJavascriptValueHashMapWrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			HashMapJavaScriptValue* hashMapPtr = reinterpret_cast<HashMapJavaScriptValue*>(mHashMap);
			EAWEBKIT_DELETE hashMapPtr;
			mHashMap = NULL;
		}

		void* EASTLJavascriptValueHashMapWrapper::GetImpl() const
		{
			return mHashMap;
		}
#else

		EASTLJavascriptValueHashMapWrapper::EASTLJavascriptValueHashMapWrapper()
        {
            new(mHashMap) HashMapJavaScriptValue();
        }

        EASTLJavascriptValueHashMapWrapper::EASTLJavascriptValueHashMapWrapper(const EASTLJavascriptValueHashMapWrapper& rhs)
        {
            new(mHashMap) HashMapJavaScriptValue();
            *(reinterpret_cast<HashMapJavaScriptValue*>(mHashMap)) = *(reinterpret_cast<HashMapJavaScriptValue*> ((rhs).GetImpl()));
        }

        EASTLJavascriptValueHashMapWrapper& EASTLJavascriptValueHashMapWrapper::operator =(const EASTLJavascriptValueHashMapWrapper& rhs)
        {
            if(this == &rhs)
                return *this;

            *(reinterpret_cast<HashMapJavaScriptValue*>(mHashMap)) = *(reinterpret_cast<HashMapJavaScriptValue*> ((rhs).GetImpl()));
            return *this;
        }

        void* EASTLJavascriptValueHashMapWrapper::GetImpl() const
        {
            return (void*)mHashMap;
        }

        EASTLJavascriptValueHashMapWrapper::~EASTLJavascriptValueHashMapWrapper()
        {
            //Need to reinterpre_cast here otherwise the destructor won't execute
            reinterpret_cast<HashMapJavaScriptValue*>(mHashMap)->~HashMapJavaScriptValue();
        }
#endif
        /******************************************EASTLJavascriptValueHashMapIteratorWrapper******************************/
        EA_COMPILETIME_ASSERT(EASTLJavascriptValueHashMapIteratorWrapper::kSize == sizeof(HashMapJavaScriptValue::iterator));
        EA_COMPILETIME_ASSERT(4 == EA_ALIGN_OF(HashMapJavaScriptValue::iterator));

#if defined(EA_PLATFORM_PS3)
		EASTLJavascriptValueHashMapIteratorWrapper::EASTLJavascriptValueHashMapIteratorWrapper()
			: mIterator(0)
		{
			mIterator = EAWEBKIT_NEW("HashMapJavaScriptValue::iterator") HashMapJavaScriptValue::iterator();
		}

		EASTLJavascriptValueHashMapIteratorWrapper::EASTLJavascriptValueHashMapIteratorWrapper(const EASTLJavascriptValueHashMapIteratorWrapper& rhs)
		{
			mIterator = EAWEBKIT_NEW("HashMapJavaScriptValue::iterator") HashMapJavaScriptValue::iterator();
			*(reinterpret_cast<HashMapJavaScriptValue::iterator*>(mIterator)) = *(reinterpret_cast<HashMapJavaScriptValue::iterator*> ((rhs).GetImpl()));
		}

		EASTLJavascriptValueHashMapIteratorWrapper& EASTLJavascriptValueHashMapIteratorWrapper::operator =(const EASTLJavascriptValueHashMapIteratorWrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<HashMapJavaScriptValue::iterator*>(mIterator);
			mIterator = EAWEBKIT_NEW("HashMapJavaScriptValue::iterator") HashMapJavaScriptValue::iterator();
			*(reinterpret_cast<HashMapJavaScriptValue::iterator*>(mIterator)) = *(reinterpret_cast<HashMapJavaScriptValue::iterator*> ((rhs).GetImpl()));

			return *this;
		}

		EASTLJavascriptValueHashMapIteratorWrapper::~EASTLJavascriptValueHashMapIteratorWrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			HashMapJavaScriptValue::iterator* iteratorPtr = reinterpret_cast<HashMapJavaScriptValue::iterator*>(mIterator);
			EAWEBKIT_DELETE iteratorPtr;
			mIterator = NULL;
		}

		void* EASTLJavascriptValueHashMapIteratorWrapper::GetImpl() const
		{
			return mIterator;
		}
#else
		EASTLJavascriptValueHashMapIteratorWrapper::EASTLJavascriptValueHashMapIteratorWrapper()
        {
            new(mIterator) HashMapJavaScriptValue::iterator();
        }

        EASTLJavascriptValueHashMapIteratorWrapper::EASTLJavascriptValueHashMapIteratorWrapper(const EASTLJavascriptValueHashMapIteratorWrapper& rhs)
        {
            new(mIterator) HashMapJavaScriptValue::iterator();
            *(reinterpret_cast<HashMapJavaScriptValue::iterator*>(mIterator)) = *(reinterpret_cast<HashMapJavaScriptValue::iterator*> ((rhs).GetImpl()));
        }

        EASTLJavascriptValueHashMapIteratorWrapper& EASTLJavascriptValueHashMapIteratorWrapper::operator =(const EASTLJavascriptValueHashMapIteratorWrapper& rhs)
        {
            if(this == &rhs)
                return *this;

            *(reinterpret_cast<HashMapJavaScriptValue::iterator*>(mIterator)) = *(reinterpret_cast<HashMapJavaScriptValue::iterator*> ((rhs).GetImpl()));
            return *this;
        }

        void* EASTLJavascriptValueHashMapIteratorWrapper::GetImpl() const
        {
            return (void*)mIterator;
        }

        EASTLJavascriptValueHashMapIteratorWrapper::~EASTLJavascriptValueHashMapIteratorWrapper()
        {
            //Need to reinterpre_cast here otherwise the destructor won't execute
            reinterpret_cast<HashMapJavaScriptValue::iterator*>(mIterator)->~hashtable_iterator();
        }
#endif

	} // namespace WebKit
} // namespace EA