/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

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

/////////////////////////////////////////////////////////////////////////////
// BCThreadingEA.h
// Created by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#ifndef Threading_h
#define Threading_h

#include <wtf/FastAllocBase.h>
#include <wtf/Assertions.h>
#include <wtf/Locker.h>
#include <wtf/Noncopyable.h>
//#include <EAWebKit/internal/EAWebKitThread.h>

#define BC_THREADING_EA_DISABLE_MP_THREADS 1   //+ 5/14/09 CSidhall - We don't support multiple threading so removed threading support

#define WTF_USE_EATHREAD 0

#if USE(EATHREAD)
//- 5/14/09 CS - Removed EAThread dependencies
#elif PLATFORM(WIN_OS)
    #include <windows.h>
#elif PLATFORM(DARWIN)
    #include <libkern/OSAtomic.h>
#elif COMPILER(GCC) && !PLATFORM(PS3)
    #if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2))
        #include <ext/atomicity.h>
    #else
        #include <bits/atomicity.h>
    #endif
#endif

#if USE(PTHREADS)
    #include <pthread.h>
#endif

#if PLATFORM(GTK)
    typedef struct _GMutex GMutex;
    typedef struct _GCond GCond;
#endif

#if PLATFORM(QT)
    class QMutex;
    class QWaitCondition;
#endif

#include <stdint.h>


// For portability, we do not use thread-safe statics natively supported by some compilers (e.g. gcc).
#define AtomicallyInitializedStatic(T, name) \
    WTF::atomicallyInitializedStaticMutex->lock(); \
    static T name; \
    WTF::atomicallyInitializedStaticMutex->unlock();



namespace WTF {


typedef uint32_t ThreadIdentifier;
typedef void* (*ThreadFunction)(void* argument);


// Returns 0 if thread creation failed
ThreadIdentifier createThread(ThreadFunction, void*);
ThreadIdentifier currentThread();
bool             isMainThread();
int              waitForThreadCompletion(ThreadIdentifier, void**);
void             detachThread(ThreadIdentifier);


#if USE(EATHREAD) 
typedef void*     PlatformMutex;

    #ifndef BC_THREADING_EA_DISABLE_MP_THREADS
        // 5/14/09 CS - This won't even compile because the include was removed  but left here as code reference
        typedef EA::Thread::Condition PlatformCondition;
    #endif

#elif USE(PTHREADS)
    typedef pthread_mutex_t PlatformMutex;
    typedef pthread_cond_t PlatformCondition;

#elif PLATFORM(GTK)
    typedef GMutex* PlatformMutex;
    typedef GCond* PlatformCondition;

#elif PLATFORM(QT)
    typedef QMutex* PlatformMutex;
    typedef QWaitCondition* PlatformCondition;

#elif PLATFORM(WIN_OS)
    struct PlatformMutex/*: public WTF::FastAllocBase*/ {
        CRITICAL_SECTION m_internalMutex;
        size_t m_recursionCount;
    };
    struct PlatformCondition/*: public WTF::FastAllocBase*/ {
        size_t m_timedOut;
        size_t m_blocked;
        size_t m_waitingForRemoval;
        HANDLE m_gate;
        HANDLE m_queue;
        HANDLE m_mutex;
    };

#else
    typedef void* PlatformMutex;
    typedef void* PlatformCondition;
#endif



class Mutex : Noncopyable {
public:
#if NO_MACRO_NEW
        // Placement operator new.
        void* operator new(size_t, void* p) { return p; }
        void* operator new[](size_t, void* p) { return p; }

        void* operator new(size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNew);
            return p;
        }

        void operator delete(void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNew);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }

        void* operator new[](size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNewArray);
            return p;
        }

        void operator delete[](void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNewArray);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }
#endif //NO_MACRO_NEW
public:
    Mutex();
    ~Mutex();

    void lock();
    bool tryLock();
    void unlock();
    
public:
    PlatformMutex& impl() { return m_mutex; }
private:
    PlatformMutex m_mutex;
};

typedef Locker<Mutex> MutexLocker;



class ThreadCondition : Noncopyable {
public:
#if NO_MACRO_NEW
        // Placement operator new.
        void* operator new(size_t, void* p) { return p; }
        void* operator new[](size_t, void* p) { return p; }

        void* operator new(size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNew);
            return p;
        }

        void operator delete(void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNew);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }

        void* operator new[](size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNewArray);
            return p;
        }

        void operator delete[](void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNewArray);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }
#endif //NO_MACRO_NEW
public:
    ThreadCondition();
    ~ThreadCondition();
    
    void wait(Mutex& mutex);
    // Returns true if the condition was signaled before the timeout, false if the timeout was reached
    bool timedWait(Mutex&, double interval);
    void signal();
    void broadcast();
    
private:

#ifndef BC_THREADING_EA_DISABLE_MP_THREADS     
    PlatformCondition m_condition;
#endif

};



#if USE(EATHREAD)
    // We assume that int == int32_t
    inline void atomicIncrement(int* addend) { EA::Internal::AtomicIncrement(reinterpret_cast<uint32_t*> (addend)); }
    inline int atomicDecrement(int* addend)  { return EA::Internal::AtomicDecrement(reinterpret_cast<uint32_t*>(addend)); }

#elif PLATFORM(WIN_OS) && !COMPILER(MSVC7)
    #define WTF_USE_LOCKFREE_THREADSAFESHARED 1

    #if COMPILER(MINGW)
        inline void atomicIncrement(int* addend) { InterlockedIncrement(reinterpret_cast<long*>(addend)); }
        inline int atomicDecrement(int* addend) { return InterlockedDecrement(reinterpret_cast<long*>(addend)); }
    #else
		#if defined(EA_PLATFORM_WINCE)
			inline void atomicIncrement(int * addend) { InterlockedIncrement(reinterpret_cast<long *>(addend)); }
			inline int atomicDecrement(int * addend) { return InterlockedDecrement(reinterpret_cast<long *>(addend)); }
		#else
			inline void atomicIncrement(int volatile* addend) { InterlockedIncrement(reinterpret_cast<long volatile*>(addend)); }
			inline int atomicDecrement(int volatile* addend) { return InterlockedDecrement(reinterpret_cast<long volatile*>(addend)); }
		#endif
    #endif

#elif PLATFORM(DARWIN)
    #define WTF_USE_LOCKFREE_THREADSAFESHARED 1

    inline void atomicIncrement(int volatile* addend) { OSAtomicIncrement32Barrier(const_cast<int*>(addend)); }
    inline int atomicDecrement(int volatile* addend) { return OSAtomicDecrement32Barrier(const_cast<int*>(addend)); }

#elif COMPILER(GCC) && !PLATFORM(PS3)
    #define WTF_USE_LOCKFREE_THREADSAFESHARED 1

    inline void atomicIncrement(int volatile* addend) { __gnu_cxx::__atomic_add(addend, 1); }
    inline int atomicDecrement(int volatile* addend) { return __gnu_cxx::__exchange_and_add(addend, -1) - 1; }

#endif



template<class T> class ThreadSafeShared : Noncopyable {
public:
#if NO_MACRO_NEW
        // Placement operator new.
        void* operator new(size_t, void* p) { return p; }
        void* operator new[](size_t, void* p) { return p; }

        void* operator new(size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNew);
            return p;
        }

        void operator delete(void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNew);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }

        void* operator new[](size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNewArray);
            return p;
        }

        void operator delete[](void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNewArray);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }
#endif //NO_MACRO_NEW
public:
    ThreadSafeShared(int initialRefCount = 1)
        : m_refCount(initialRefCount)
    {
    }

    void ref()
    {
        #if USE(LOCKFREE_THREADSAFESHARED)
                atomicIncrement(&m_refCount);
        #else
                MutexLocker locker(m_mutex);
                ++m_refCount;
        #endif
    }

    void deref()
    {
        #if USE(LOCKFREE_THREADSAFESHARED)
            if (atomicDecrement(&m_refCount) <= 0)
        #else
            {
                MutexLocker locker(m_mutex);
                --m_refCount;
            }
            if (m_refCount <= 0)
        #endif
                delete static_cast<T*>(this);
    }

    bool hasOneRef()
    {
        return refCount() == 1;
    }

    int refCount() const
    {
        #if !USE(LOCKFREE_THREADSAFESHARED)
            MutexLocker locker(m_mutex);
        #endif
        return static_cast<int const volatile &>(m_refCount);
    }

private:
    int m_refCount;
    #if !USE(LOCKFREE_THREADSAFESHARED)
        mutable Mutex m_mutex;
    #endif
};



// This function must be called from the main thread. It is safe to call it repeatedly.
// Darwin is an exception to this rule: it is OK to call it from any thread, the only requirement is that the calls are not reentrant.
void initializeThreading();

extern Mutex* atomicallyInitializedStaticMutex;

} // namespace WTF



using WTF::Mutex;
using WTF::MutexLocker;
using WTF::ThreadCondition;
using WTF::ThreadIdentifier;
using WTF::ThreadSafeShared;

using WTF::createThread;
using WTF::currentThread;
using WTF::isMainThread;
using WTF::detachThread;
using WTF::waitForThreadCompletion;

#endif // Threading_h
