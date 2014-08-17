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
// EAWebkitSTLWrapper.h
// By Arpit Baldeva
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITSTLWRAPPER_H
#define EAWEBKIT_EAWEBKITSTLWRAPPER_H


///////////////////////////////////////////////////////////////////////
// The purpose of this file is to stop the template expansion inside the main app exe(and thus exe contain code due to the template instantiation) for the users of the EAWebkit.
// This is required for LGPL compliance.
// We keep all the stl related classes in this single file to start with. It can be broken down if necessary in future.
///////////////////////////////////////////////////////////////////////

#include <EABase/config/eacompilertraits.h>
#include <EABase/eabase.h>

// Compatibility with older versions of EABase
#ifndef EA_ALIGN
#define EA_ALIGN EA_PREFIX_ALIGN
#endif

//Update: 12/22/2010 - On PS3, the actual stl object is newed in the wrapper classes instead of placement new in an aligned buffer. This is
// because we have two different compilers on PS3. It is possible that using a different compiler/different compiler pre-processors on application
// side can cause the alignment to go wonky. So for now, we revert to our old style on PS3.

namespace EA
{
    namespace WebKit
    {
		//This wraps eastl::fixed_string<char8_t,  128, true, EASTLAllocator>.
		class EASTLFixedString8Wrapper
		{
		public:
            static const int kFixedStringSize = 148;

            EASTLFixedString8Wrapper();
			EASTLFixedString8Wrapper(const char8_t* str);
			EASTLFixedString8Wrapper(const EASTLFixedString8Wrapper& rhs);
			EASTLFixedString8Wrapper& operator = (const EASTLFixedString8Wrapper& rhs);
			~EASTLFixedString8Wrapper();
			void* GetImpl() const;
		private:
#if defined(EA_PLATFORM_PS3)
			void* mString8;
#else
			EA_ALIGN(4) char8_t mString8[kFixedStringSize];
#endif
		};

		//This wraps eastl::fixed_string<char16_t,  128, true, EASTLAllocator>.
		class EASTLFixedString16Wrapper
		{
		public:
            static const int kMaxFixedStringSize = 276;

			EASTLFixedString16Wrapper();
			EASTLFixedString16Wrapper(const char16_t* str);
			EASTLFixedString16Wrapper(const EASTLFixedString16Wrapper& rhs);
			EASTLFixedString16Wrapper& operator = (const EASTLFixedString16Wrapper& rhs);
			~EASTLFixedString16Wrapper();
			void* GetImpl() const;
		private:
#if defined(EA_PLATFORM_PS3)
			void* mString16;
#else
			EA_ALIGN(4) char8_t mString16[kMaxFixedStringSize];
#endif
		};
		
		//This wraps typedef eastl::fixed_multimap<FixedString16_64, FixedString16_64, 8, true, fstr_iless, EASTLAllocator> HeaderMap;
		class EASTLHeaderMapWrapper
		{
		public:
			EASTLHeaderMapWrapper();
			EASTLHeaderMapWrapper(const EASTLHeaderMapWrapper& rhs);
			EASTLHeaderMapWrapper& operator = (const EASTLHeaderMapWrapper& rhs);
			~EASTLHeaderMapWrapper();
			void* GetImpl() const;
		private:
			void* mHeaderMap;
		};

        class EASTLVectorJavaScriptValueWrapper
        {
        public:
            static const int kSize = 16;

            EASTLVectorJavaScriptValueWrapper();
            EASTLVectorJavaScriptValueWrapper(const EASTLVectorJavaScriptValueWrapper& rhs);
            EASTLVectorJavaScriptValueWrapper& operator = (const EASTLVectorJavaScriptValueWrapper& rhs);
            ~EASTLVectorJavaScriptValueWrapper();
            void* GetImpl() const;
        private:
#if defined(EA_PLATFORM_PS3)
			void* mVector;
#else
			EA_ALIGN(4) char8_t mVector[kSize];
#endif
        };

        class EASTLJavascriptValueHashMapWrapper
        {
        public:
            static const int kSize = 32;

            EASTLJavascriptValueHashMapWrapper();
            EASTLJavascriptValueHashMapWrapper(const EASTLJavascriptValueHashMapWrapper& rhs);
            EASTLJavascriptValueHashMapWrapper& operator =(const EASTLJavascriptValueHashMapWrapper& rhs);
            ~EASTLJavascriptValueHashMapWrapper();
            void* GetImpl() const;

        private:
#if defined(EA_PLATFORM_PS3)
			void* mHashMap;
#else
			EA_ALIGN(4) char8_t mHashMap[kSize];
#endif
        };

        class EASTLJavascriptValueHashMapIteratorWrapper
        {
        public:
            static const int kSize = 8;

            EASTLJavascriptValueHashMapIteratorWrapper();
            EASTLJavascriptValueHashMapIteratorWrapper(const EASTLJavascriptValueHashMapIteratorWrapper& rhs);
            EASTLJavascriptValueHashMapIteratorWrapper& operator =(const EASTLJavascriptValueHashMapIteratorWrapper& rhs);
            ~EASTLJavascriptValueHashMapIteratorWrapper();
            void* GetImpl() const;

        private:
#if defined(EA_PLATFORM_PS3)
			void* mIterator;
#else
			EA_ALIGN(4) char8_t mIterator[kSize];
#endif
        };
    }
}


#endif // Header include guard
