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
// EAWebKitSort.h
// 
//////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKITSORT_H
#define EAWEBKITSORT_H

#include <string>
#include <wtf/FastAllocBase.h>
#include <EAWebkit/internal/EAWebKitAssert.h>
#include <EAWebKit/EAWebKitConfig.h>
#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif


namespace EA {

namespace Internal {
    
   // 5/7/09 CSidhall - Note: The merge sort was extracted from EASTL which was 
   // written by Paul Pedriana but modified here not to use iterators.  
   // Also the copy and destruct templates were replaced.

    // Copies each object to a dest buffer
    template <typename T>
     inline void copy_helper(T* pDestination, T* pSource, int nCount)
     {
        while (nCount--)
        {
            *pDestination = *pSource;
            pSource++;
            pDestination++;
        }
    }

    // Pointer destructor 
    template <typename T>
    inline void destroy_pointer(T *pObj)
    {
        pObj->T::~T();
    }
    
    // Explicit call of the destructor if a trivial destructor exists
    template <typename T>
    inline void destruct_helper(T* pBuffer, int nCount)
    {
        while (nCount--)
        {
            destroy_pointer<T>(pBuffer);
            pBuffer++;
        }
    }

    /// merge
    ///
    /// This function merges two sorted input sorted ranges into a result sorted range.
    /// This merge is stable in that no element from the first range will be changed
    /// in order relative to other elements from the first range.
    ///
    template <typename T, typename Compare>
    void merge_helper(T* pFirst1, T* pLast1, T* pFirst2, T* pLast2, T* pResult, Compare compare)
    {
        while((pFirst1 < pLast1) && (pFirst2 < pLast2))
        {
            if(compare(*pFirst2, *pFirst1))
            {
                EAW_ASSERT(!compare(*pFirst1, *pFirst2)); // Validate that the compare function is sane.
                *pResult = *pFirst2;
                ++pFirst2;
            }
            else
            {
                *pResult = *pFirst1;
                ++pFirst1;
            }
            ++pResult;
        }
        
        // Copy the leftovers back into the main buffer
        int size = pLast1 - pFirst1;
        if(size > 0)
        {
           int size1 = size * sizeof(*pFirst1);
           memcpy(pResult, pFirst1, size1);
           pResult +=size;
        }
        int size2 = (char*) pLast2 - (char*) pFirst2;
        memcpy(pResult, pFirst2, size2);
    }

    template <typename T, typename StrictWeakOrdering>
    void merge_sort_buffer(T *pFirst, int nCount, T* pBuffer, StrictWeakOrdering compare)
    {
        if(nCount > 1)
        {
            const int nMid = nCount >> 1;
            T* pHalf = pFirst + nMid;
 
            if(nMid > 1)
            {
                const int nQ1 = nMid >> 1;
                T* pPart = pFirst + nQ1;

                merge_sort_buffer<T, StrictWeakOrdering>(pFirst, nQ1, pBuffer, compare);
                merge_sort_buffer<T, StrictWeakOrdering>(pPart,  (nMid - nQ1), pBuffer + nQ1, compare);
                merge_helper<T, StrictWeakOrdering> (pFirst, pPart, pPart, pHalf, pBuffer, compare);
            }
            else
                *pBuffer = *pFirst;
 
            if((nCount - nMid) > 1)
            {
                const int nQ3= (nMid + nCount) >> 1;
                T* pPart = pFirst + nQ3;

                merge_sort_buffer<T, StrictWeakOrdering>(pHalf, (nQ3 - nMid), pBuffer + nMid, compare);
                merge_sort_buffer<T, StrictWeakOrdering>(pPart, (nCount - nQ3), pBuffer + nQ3,  compare);
                merge_helper<T, StrictWeakOrdering> (pHalf, pPart, pPart, (pFirst + nCount), (pBuffer + nMid), compare);
            }
            else
                *(pBuffer + nMid) = *pHalf;
                merge_helper<T, StrictWeakOrdering> (pBuffer, (pBuffer + nMid), (pBuffer + nMid), (pBuffer + nCount), pFirst, compare);
        }    
    }

    /// merge_sort
    ///
    /// Implements the MergeSort algorithm.
    /// This algorithm allocates memory via the user-supplied allocator. Use merge_sort_buffer
    /// function if you want a version which doesn't allocate memory.
    /// Note that merge_sort requires a random access iterator, which usually means 
    /// an array (eg. vector, deque).
    /// 
    template <typename T, typename StrictWeakOrdering>
    void merge_sort(T* pSource, const int nCount, StrictWeakOrdering compare)
    {
        if(nCount > 1)
        {
            // We need to allocate an array of nCount value_type objects as a temporary buffer.
            int size = nCount * sizeof(*pSource);
            const int MERGE_BUFFER_ALIGN_SIZE = 16; // Could make it platform specific
            T* pBuffer = (T*) WTF::fastMallocAligned(size, MERGE_BUFFER_ALIGN_SIZE);
            if(!pBuffer)
            {
                EAW_ASSERT(0);             
                return;
            }
            memset(pBuffer,0, size);

            copy_helper(pBuffer, pSource, nCount);
            
            merge_sort_buffer<T,StrictWeakOrdering> (pSource, nCount, pBuffer, compare);
            
            destruct_helper(pBuffer, nCount);
            
            WTF::fastFree(pBuffer);
        }
    }        

} // namespace Internal

} //namespace EA

#endif //  EAWEBKITSORT_H

