/*
Copyright (C) 2009 Electronic Arts, Inc.  All rights reserved.

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


#ifndef EA_ALLOCATOR_ICOREALLOCATORMACROHELPERS_H
#define EA_ALLOCATOR_ICOREALLOCATORMACROHELPERS_H

#ifdef _MSC_VER
#pragma once
#endif

/// @brief This header file defines helper methods and functions needed by icoreallocatormacros

#include <stddef.h>     // size_t definition
#include <new>          // for placement new and array placement new

namespace EA
{
    namespace Allocator
    {
        namespace detail
        {
            /// @remarks
            /// Distinguisher for our new overload. It's called opNonArrayNew because only the non-array versions of our
            /// new/delete overloads are valid. For arrays users should be using CORE_NEW_ARRAY/CORE_DELETE_ARRAY.

            struct opNonArrayNew { int v; };

            /// @remarks
            /// Destructs and deletes an object created by by an ICoreAllocator operator new overload (or CORE_NEW)
            /// Use CORE_DELETE instead of calling this directly.

            template <typename T>
            inline void DeleteObject(Allocator::ICoreAllocator* allocator, T* object)
            {
                if(object) // As per the C++ standard, deletion of NULL results in a no-op.
                {
                    object->~T();
                    allocator->Free(object);
                }
            }

            /// @remarks
            /// Creates an array of objects of type T.
            /// Use CORE_NEW_ARRAY instead of calling this directly.

            template <typename T>
            inline T* CreateArray(Allocator::ICoreAllocator* allocator, size_t count, const char* name, unsigned int flags)
            {
                // To do: Use type traits to detect that T is a built-in type and not generate a size_t prefix if so.

                // Make the allocation with an extra size_t in front to allow us to store count
                size_t* p = (size_t*)allocator->Alloc((sizeof(size_t)*2) + (sizeof(T) * count), name, flags);

                // put the size_t count in front of the array allocation.
                *p = count;

                //use placement new to construct each object
                T* first = reinterpret_cast<T*>(p+2);
                T* end = first+count;
                for (T* cur = first; cur < end; cur++)
                {
                    new(cur)T();
                }
                return first;
            }

            /// @remarks
            /// Creates an array of objects of type T.
            /// Use CORE_NEW_ARRAY instead of calling this directly.

            template <typename T>
            inline T* CreateArray(Allocator::ICoreAllocator* allocator, size_t count, const ICoreAllocator::DebugParams debugParams, unsigned int flags)
            {
                // To do: Use type traits to detect that T is a built-in type and not generate a size_t prefix if so.

                // Make the allocation with an extra size_t in front to allow us to store count
                size_t* p = (size_t*)allocator->AllocDebug((sizeof(size_t)*2) + (sizeof(T) * count), debugParams, flags);

                // put the size_t count in front of the array allocation.
                *p = count;

                //use placement new to construct each object
                T* first = reinterpret_cast<T*>(p+2);
                T* end = first+count;
                for (T* cur = first; cur < end; cur++)
                {
                    new(cur)T();
                }
                return first;
            }

            /// @remarks
            /// Creates an array of objects of type T.
            /// Use CORE_NEW_ARRAY_ALIGN instead of calling this directly

            template <typename T>
            inline T* CreateArray(Allocator::ICoreAllocator* allocator, size_t count, const char* name, unsigned int flags, unsigned int align)
            {
                // To do: Use type traits to detect that T is a built-in type and not generate a size_t prefix if so.

                // Make the allocation with an extra size_t in front to allow us to store count
                size_t* p = (size_t*)allocator->Alloc((sizeof(size_t)*2) + (sizeof(T) * count), name, flags, align, (sizeof(size_t)*2));

                // put the size_t count in front of the array allocation.
                *p = count;

                //use placement new to construct each object
                T* first = reinterpret_cast<T*>(p+2);
                T* end = first+count;
                for (T* cur = first; cur < end; cur++)
                {
                    new(cur)T();
                }
                return first;
            }

            /// @remarks
            /// Creates an array of objects of type T.
            /// Use CORE_NEW_ARRAY_ALIGN instead of calling this directly

            template <typename T>
            inline T* CreateArray(Allocator::ICoreAllocator* allocator, size_t count, const ICoreAllocator::DebugParams debugParams, unsigned int flags, unsigned int align)
            {
                // To do: Use type traits to detect that T is a built-in type and not generate a size_t prefix if so.

                // Make the allocation with an extra size_t in front to allow us to store count
                size_t* p = (size_t*)allocator->AllocDebug((sizeof(size_t)*2) + (sizeof(T) * count), debugParams, flags, align, (sizeof(size_t)*2));

                // put the size_t count in front of the array allocation.
                *p = count;

                //use placement new to construct each object
                T* first = reinterpret_cast<T*>(p+2);
                T* end = first+count;
                for (T* cur = first; cur < end; cur++)
                {
                    new(cur)T();
                }
                return first;
            }


            /// @remarks
            /// Destructs and Deletes an array created by CreateArray.
            /// Use CORE_DELETE_ARRAY instead of calling this directly.

            template <typename T>
            void DeleteArray(Allocator::ICoreAllocator* allocator, T* array)
            {
                if(array) // As per the C++ standard, deletion of NULL results in a no-op.
                {
                    // Destruct objects in reverse order.
                    T* arrayEnd = array + ((size_t*)array)[-2];
                    while(arrayEnd-- != array)
                        arrayEnd->~T();

                    allocator->Free(static_cast<void*>((size_t*)array - 2));
                }
            }
        } // namespace detail
    } // namespace Allocator
} // namespace EA


/// @remarks
/// This overload is provided for use by CORE_NEW* macros, there should be no need for a user to call this overload directly.

inline void* operator new(size_t size, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const char* name, unsigned int flags)
{
    return allocator->Alloc(size, name, flags);
}

/// @remarks
/// This overload is provided for use by CORE_NEW* macros, there should be no need for a user to call this overload directly.

inline void* operator new(size_t size, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const EA::Allocator::ICoreAllocator::DebugParams debugParams, unsigned int flags)
{
    return allocator->AllocDebug(size, debugParams, flags);
}

/// @remarks
/// This overload is provided for use by CORE_NEW* macros, there should be no need for a user to call this overload directly.

inline void* operator new(size_t size, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const char* name, unsigned int flags, unsigned int align, unsigned int alignOffset = 0)
{
    return allocator->Alloc(size, name, flags, align, alignOffset);
}

/// @remarks
/// This overload is provided for use by CORE_NEW* macros, there should be no need for a user to call this overload directly.

inline void* operator new(size_t size, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const EA::Allocator::ICoreAllocator::DebugParams debugParams, unsigned int flags, unsigned int align, unsigned int alignOffset = 0)
{
    return allocator->AllocDebug(size, debugParams, flags, align, alignOffset);
}

/// @remarks
/// This overload is provided in case the compiler needs to generate an operator delete call to match our operator new overload due to an exception. There should be no need for a user to call this overload directly.

inline void operator delete(void* p, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const char*, unsigned int)
{
    return allocator->Free(p);
}

/// @remarks
/// This overload is provided in case the compiler needs to generate an operator delete call to match our operator new overload due to an exception. There should be no need for a user to call this overload directly.

inline void operator delete(void* p, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const EA::Allocator::ICoreAllocator::DebugParams, unsigned int)
{
    return allocator->Free(p);
}

/// @remarks
/// This overload is provided in case the compiler needs to generate an operator delete call to match our operator new overload due to an exception. There should be no need for a user to call this overload directly.

inline void operator delete(void* p, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const char*, unsigned int, unsigned int, unsigned int)
{
    return allocator->Free(p);
}

/// @remarks
/// This overload is provided in case the compiler needs to generate an operator delete call to match our operator new overload due to an exception. There should be no need for a user to call this overload directly.

inline void operator delete(void* p, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const EA::Allocator::ICoreAllocator::DebugParams, unsigned int, unsigned int, unsigned int)
{
    return allocator->Free(p);
}

/// @remarks
/// DO NOT CALL THIS METHOD or try to use CORE_NEW to create an array (such as 'widgetArray = CORE_NEW(allocator, "myarray", 0) Widget[5];').
/// Doing so will cause a link error. Use CORE_NEW_ARRAY and CORE_DELETE_ARRAY instead.

void* operator new[](size_t size, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const char* name, unsigned int flags);

/// @remarks
/// DO NOT CALL THIS METHOD or try to use CORE_NEW to create an array (such as 'widgetArray = CORE_NEW(allocator, "myarray", 0) Widget[5];').
/// Doing so will cause a link error. Use CORE_NEW_ARRAY and CORE_DELETE_ARRAY instead.

void* operator new[](size_t size, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const EA::Allocator::ICoreAllocator::DebugParams debugParams, unsigned int flags);

/// @remarks
/// DO NOT CALL THIS METHOD or try to use CORE_NEW to create an array (such as 'widgetArray = CORE_NEW(allocator, "myarray", 0) Widget[5];').
/// Doing so will cause a link error. Use CORE_NEW_ARRAY and CORE_DELETE_ARRAY instead.

void* operator new[](size_t size, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const char* name, unsigned int flags, unsigned int align, unsigned int alignOffset = 0);

/// @remarks
/// DO NOT CALL THIS METHOD or try to use CORE_NEW to create an array (such as 'widgetArray = CORE_NEW(allocator, "myarray", 0) Widget[5];').
/// Doing so will cause a link error. Use CORE_NEW_ARRAY and CORE_DELETE_ARRAY instead.

void* operator new[](size_t size, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const EA::Allocator::ICoreAllocator::DebugParams debugParams, unsigned int flags, unsigned int align, unsigned int alignOffset = 0);

/// @remarks
/// DO NOT CALL THIS METHOD, doing so will cause a link error. Use CORE_DELETE_ARRAY instead.

void operator delete[](void* p, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const char*, unsigned int);

/// @remarks
/// DO NOT CALL THIS METHOD, doing so will cause a link error. Use CORE_DELETE_ARRAY instead.

void operator delete[](void* p, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const EA::Allocator::ICoreAllocator::DebugParams, unsigned int);

/// @remarks
/// DO NOT CALL THIS METHOD, doing so will cause a link error. Use CORE_DELETE_ARRAY instead.

void operator delete[](void* p, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const char*, unsigned int, unsigned int, unsigned int);

/// @remarks
/// DO NOT CALL THIS METHOD, doing so will cause a link error. Use CORE_DELETE_ARRAY instead.

void operator delete[](void* p, EA::Allocator::detail::opNonArrayNew, EA::Allocator::ICoreAllocator* allocator, const EA::Allocator::ICoreAllocator::DebugParams, unsigned int, unsigned int, unsigned int);


#endif
