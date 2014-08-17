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
// EAWebkitAllocator.h
// By Arpit Baldeva (Isolated Paul Pedriana's code in EAWebkit.h)
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITALLOCATOR_H
#define EAWEBKIT_EAWEBKITALLOCATOR_H


///////////////////////////////////////////////////////////////////////
// The purpose of this file is to isolate the allocator and EASTL allocator related code from EAWebkit.h.
// This is due to refactoring of code for LGPL compliance. This file can be included from both main app and internal EAWebkit
// source modules.
///////////////////////////////////////////////////////////////////////

#include <EABase/eabase.h>
#include <EAWebKit/EAWebKitConfig.h>

namespace EA
{
	namespace WebKit
	{
		///////////////////////////////////////////////////////////////////////
		// Allocator
		///////////////////////////////////////////////////////////////////////

		// The user must set the allocator before any EAWebKit view is created.
		//
		// We would prefer to use ICoreAllocator, but ICoreAllocator doesn't support Realloc.
		// WebKit requires realloc, and it's impossible to implement realloc on top of just
		// malloc and free; you need to know the underlying heap. So what we do is map 
		// the functions to PPMalloc GeneralAllocator's interface.

		class Allocator
		{
		public:
			virtual ~Allocator() { }

			virtual void* Malloc(size_t size, int flags, const char* pName) = 0;
			virtual void* MallocAligned(size_t size, size_t alignment, size_t offset, int flags, const char* pName) = 0;
			virtual void  Free(void* p, size_t size) = 0;
			virtual void* Realloc(void* p, size_t size, int flags) = 0;
		};
		
		EAWEBKIT_API Allocator* GetAllocator();


	}
}

namespace EA
{
    namespace WebKit
    {
		class Allocator;

		///////////////////////////////////////////////////////////////////////
		// EASTLAllocator
		///////////////////////////////////////////////////////////////////////

		// Example usage:
		//  eastl::vector<int, EA::WebKit::EASTLAllocator> intVector;

		class EASTLAllocator
		{
		public:
			explicit EASTLAllocator(const char* pName = NULL);
			EASTLAllocator(const EASTLAllocator& x);
			EASTLAllocator(const EASTLAllocator& x, const char* pName);
			EASTLAllocator& operator=(const EASTLAllocator&);

			void* allocate(size_t n, int flags = 0);
			void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
			void  deallocate(void* p, size_t n);

			const char* get_name() const;
			void        set_name(const char* pName);
		};

		bool operator==(const EASTLAllocator& a, const EASTLAllocator& b);
		bool operator!=(const EASTLAllocator& a, const EASTLAllocator& b);

		inline EASTLAllocator::EASTLAllocator(const char*)
		{ }

		inline EASTLAllocator::EASTLAllocator(const EASTLAllocator&)
		{ }

		inline EASTLAllocator::EASTLAllocator(const EASTLAllocator&, const char*)
		{ }

		inline EASTLAllocator& EASTLAllocator::operator=(const EASTLAllocator&)
		{ return *this; }

		inline void* EASTLAllocator::allocate(size_t n, int flags)
		{ 
			return EA::WebKit::GetAllocator()->Malloc(n, flags, "EA::WebKit::EASTLAllocator");
		}

		inline void* EASTLAllocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
		{
			return EA::WebKit::GetAllocator()->MallocAligned(n, alignment, offset, flags, "EA::WebKit::EASTLAllocator");
		}

		inline void EASTLAllocator::deallocate(void* p, size_t n)
		{ 
			return EA::WebKit::GetAllocator()->Free(p, n); 
		}

		inline const char* EASTLAllocator::get_name() const
		{ 
			return "EA::WebKit::EASTLAllocator"; 
		}

		inline void EASTLAllocator::set_name(const char*)
		{ }

		inline bool operator==(const EASTLAllocator&, const EASTLAllocator&)
		{ return true; }  // All instances are equal.

		inline bool operator!=(const EASTLAllocator& a, const EASTLAllocator&)
		{ return false; } // All instances are equal.


    }
}


#endif // Header include guard
