// -*- mode: c++; c-basic-offset: 4 -*-
/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 2005 Apple Computer, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

/*
* This file was modified by Electronic Arts Inc Copyright ?2009-2010
*/

/////////////////////////////////////////////////////////////////////////////
// BCFastMallocWK.h
//
// Copyright (c) 2008, Electronic Arts Inc. All rights reserved.
// Written by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#ifndef WTF_FastMalloc_h
#define WTF_FastMalloc_h

#include <wtf/Platform.h>
#include <stdlib.h>
#include <new>

namespace WTF {

	void *fastMalloc(size_t n, int flags = 0, const char* pName = "EAWebKit fastMalloc");
	void *fastMallocAligned(size_t n, size_t alignment, size_t offset = 0, int flags = 0, const char* pName = "EAWebKit fastMallocAligned");
	void *fastZeroedMalloc(size_t n,int flags = 0, const char* pName = "EAWebKit fastZeroedMalloc");
	void *fastCalloc(size_t n_elements, size_t element_size, int flags = 0, const char* pName = "EAWebKit fastCalloc");
	void  fastFree(void* p);
	char* fastStrDup(const char*);
	void *fastRealloc(void* p, size_t n, int flags = 0);

#ifndef NDEBUG    
    void fastMallocForbid();
    void fastMallocAllow();
#endif


    // This defines a type which holds an unsigned integer and is the same 
    // size as the minimally aligned memory allocation.
    typedef unsigned long long AllocAlignmentInteger;

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)

    namespace Internal {

        // Malloc validation is a scheme whereby a tag is attached to an 
        // allocation which identifies how it was originally allocated. 
        // This allows us to verify that the freeing operation matches the 
        // allocation operation. If memory is allocated with operator new[] 
        // but freed with free or delete, this system would detect that. 
        // In the implementation here, the tag is an integer prepended to 
        // the allocation memory which is assigned one of the AllocType
        // enumeration values. An alternative implementation of this 
        // scheme could store the tag somewhere else or ignore it. 
        // Users of FastMalloc don't need to know or care how this tagging 
        // is implemented. 

        enum AllocType {                    // Start with an unusual number instead of zero, because zero is common.
            AllocTypeMalloc = 0x375d6750,   // Encompasses fastMalloc, fastZeroedMalloc, fastCalloc, fastRealloc.
            AllocTypeClassNew,              // Encompasses class operator new from FastAllocBase. 
            AllocTypeClassNewArray,         // Encompasses class operator new[] from FastAllocBase. 
            AllocTypeFastNew,               // Encompasses fastNew.
            AllocTypeFastNewArray,          // Encompasses fastNewArray.
            AllocTypeNew,                   // Encompasses global operator new.
            AllocTypeNewArray               // Encompasses global operator new[].
        };

        // Return the AllocType tag associated with the allocated block p.
        inline AllocType fastMallocMatchValidationType(const void* p)
        {
            const AllocAlignmentInteger* pType = reinterpret_cast<const AllocAlignmentInteger*>(static_cast<const char*>(p) - sizeof(AllocAlignmentInteger));
            return static_cast<AllocType>(*pType);
        }

        // Return the address of the AllocType tag associated with the allocated block p.
        inline AllocAlignmentInteger* fastMallocMatchValidationValue(void* p)
        {
            return reinterpret_cast<AllocAlignmentInteger*>(static_cast<char*>(p) - sizeof(AllocAlignmentInteger));
        }

        // Set the AllocType tag to be associaged with the allocated block p.
        inline void setFastMallocMatchValidationType(void* p, AllocType allocType)
        {
            AllocAlignmentInteger* pType = reinterpret_cast<AllocAlignmentInteger*>(static_cast<char*>(p) - sizeof(AllocAlignmentInteger));
            *pType = static_cast<AllocAlignmentInteger>(allocType);
        }

        static int fastMallocMatchFailureCount = 0;  // This is for debugging only.

        inline void fastMallocMatchFailed(void* /*p*/)
        {
            // Handle a detected alloc/free mismatch.
            fastMallocMatchFailureCount++;
        }
    }

    // This is a higher level function which is used by FastMalloc-using code.
    inline void fastMallocMatchValidateMalloc(void* p, WTF::Internal::AllocType allocType)
    {
        if (p)
            Internal::setFastMallocMatchValidationType(p, allocType);
    }

    // This is a higher level function which is used by FastMalloc-using code.
    inline void fastMallocMatchValidateFree(void* p, WTF::Internal::AllocType allocType)
    {
        if (p) {
            if (Internal::fastMallocMatchValidationType(p) != allocType)
                Internal::fastMallocMatchFailed(p);
            Internal::setFastMallocMatchValidationType(p, WTF::Internal::AllocTypeMalloc);  // Set it to this so that fastFree thinks it's OK.
        }
    }

	
#else 

    // These could alternatively be inline functions which do nothing.
    #define fastMallocMatchValidateMalloc(p, type)
    #define fastMallocMatchValidateFree(p, type)

#endif

} // namespace WTF

using WTF::fastMalloc;
using WTF::fastZeroedMalloc;
using WTF::fastCalloc;
using WTF::fastRealloc;
using WTF::fastFree;
using WTF::fastStrDup;
#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)
	using WTF::fastMallocMatchValidateMalloc;
	using WTF::fastMallocMatchValidateFree;
#endif
#ifndef NDEBUG    
    using WTF::fastMallocForbid;
    using WTF::fastMallocAllow;
#endif

#if COMPILER(GCC) && PLATFORM(DARWIN)
    #define WTF_PRIVATE_INLINE __private_extern__ inline __attribute__((always_inline))
#elif COMPILER(GCC)
    #define WTF_PRIVATE_INLINE inline __attribute__((always_inline))
#elif COMPILER(MSVC)
    #define WTF_PRIVATE_INLINE __forceinline
#else
    #define WTF_PRIVATE_INLINE inline
#endif

#ifndef _CRTDBG_MAP_ALLOC

#if !defined(USE_SYSTEM_MALLOC) || !(USE_SYSTEM_MALLOC)
    WTF_PRIVATE_INLINE void* operator new(size_t s)     { return fastMalloc(s); }
    WTF_PRIVATE_INLINE void  operator delete(void* p)   { fastFree(p); }
    WTF_PRIVATE_INLINE void* operator new[](size_t s)   { return fastMalloc(s); }
    WTF_PRIVATE_INLINE void  operator delete[](void* p) { fastFree(p); }

    extern "C" {
        void releaseFastMallocFreeMemory();
    }
#endif

#endif // _CRTDBG_MAP_ALLOC

#endif /* WTF_FastMalloc_h */
