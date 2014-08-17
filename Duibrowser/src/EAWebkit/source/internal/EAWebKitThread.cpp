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
DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// EAWebKitThread.cpp 
// 5/14/09 CSidhall - Note: this code was copied from EAStdC Thread.inl which 
// was maintained by Paul Pedriana.
//
///////////////////////////////////////////////////////////////////////////////

#include <EABase/eabase.h>
#include <EAWebKit/internal/EAWebKitThread.h>



#ifdef  USE_EATHREAD_LIBRARY
#if defined(EA_PLATFORM_WINDOWS)
    #pragma warning(push, 0)
    #include <windows.h>
    #pragma warning(pop)
#elif defined(EA_PLATFORM_XENON)
    #pragma warning(push, 0)
    #include <comdecl.h>
    #pragma warning(pop)
#elif defined(EA_PLATFORM_PS3)
     #include <sys/synchronization.h>
#elif defined(EA_PLATFORM_UNIX)
    #include <unistd.h>      // sbrk(), getpagesize(), mmap, munmap, etc.
    #include <sys/mman.h>    // mmap, etc.
    #include <fcntl.h>       // open
    #if PPM_THREAD_SAFETY_SUPPORTED
        #include <pthread.h>
    #endif
#endif



namespace EA
{
    namespace Internal
    {

        #if defined(_MSC_VER) || defined(EA_PLATFORM_WINDOWS) || defined(EA_PLATFORM_XENON)
            #define mpCS ((CRITICAL_SECTION*)mpMutexData)

            Mutex::Mutex()
            {
                #if defined(EA_PLATFORM_XENON) || !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0403)
                    InitializeCriticalSection(mpCS);
                #else
                    InitializeCriticalSectionAndSpinCount(mpCS, 10);
                #endif
            }

            Mutex::~Mutex()
            {
                DeleteCriticalSection(mpCS);
            }

            void Mutex::Lock()
            {
                EnterCriticalSection(mpCS);
            }

            void Mutex::Unlock()
            {
                LeaveCriticalSection(mpCS);
            }

            #undef mCS

        #elif defined(EA_PLATFORM_PS3)
            #define mpMutex ((sys_lwmutex_t*)mpMutexData)

            Mutex::Mutex()
            {
                sys_lwmutex_attribute_t lwattr;
                sys_lwmutex_attribute_initialize(lwattr);
                lwattr.attr_recursive = SYS_SYNC_RECURSIVE;

                sys_lwmutex_create(mpMutex, &lwattr);

                #ifdef EA_DEBUG
                    sys_lwmutex_lock(mpMutex, 0);
                    sys_lwmutex_unlock(mpMutex);
                #endif
            }

            Mutex::~Mutex()
            {
                sys_lwmutex_destroy(mpMutex);
            }

            void Mutex::Lock()
            {
                sys_lwmutex_lock(mpMutex, 0);
            }

            void Mutex::Unlock()
            {
                sys_lwmutex_unlock(mpMutex);
            }

            #undef mpMutex

          #elif defined(EA_PLATFORM_UNIX)
            #define mpMutex ((pthread_mutex_t*)mpMutexData)

            Mutex::Mutex()
            {
                pthread_mutexattr_t attr;

                pthread_mutexattr_init(&attr);
                pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE); 
                pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
                pthread_mutex_init(mpMutex, &attr);
                pthread_mutexattr_destroy(&attr);
            }

            Mutex::~Mutex()
            {
                pthread_mutex_destroy(mpMutex);
            }

            void Mutex::Lock()
            {
                pthread_mutex_lock(mpMutex);
            }

            void Mutex::Unlock()
            {
                pthread_mutex_unlock(mpMutex);
            }

            #undef mpMutex

         #else

            Mutex::Mutex()
            {
            }

            Mutex::~Mutex()
            {
            }

            void Mutex::Lock()
            {
            }

            void Mutex::Unlock()
            {
            }

        #endif

    }
}

#else  //!USE_EATHREAD_LIBRARY
// 8/17/09 CSidahll - For now, don't support threading.  EAWebkit is not really threaded anyway currently.
// By not using threading, we don't need to expose platform specific header files.
namespace EA
{
    namespace Internal
    {
           
            Mutex::Mutex()
            {
            }

            Mutex::~Mutex()
            {
            }

            void Mutex::Lock()
            {
            }

            void Mutex::Unlock()
            {
            }

    }
}
#endif //  USE_EATHREAD_LIBRARY