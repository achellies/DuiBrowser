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
// BCThreadingEAThread.cpp
// Written by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "Threading.h"

#include <wtf/HashMap.h>
#include <wtf/MathExtras.h>

#include <errno.h>




namespace WTF {


#ifndef BC_THREADING_EA_DISABLE_MP_THREADS 
Mutex* atomicallyInitializedStaticMutex = NULL;
#endif


///////////////////////////////////////////////////////////////////////////////
// Thread helpers
///////////////////////////////////////////////////////////////////////////////

#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
static EA::Thread::ThreadId mainThreadIdentifier; // More precisely, the thread that was the first to call initializeThreading().

static Mutex& threadMapMutex()
{
    static Mutex mutex;
    return mutex;
}
#endif

void initializeThreading()
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS  
    if (!atomicallyInitializedStaticMutex)
    {
        Mutex& mutex = threadMapMutex();
        atomicallyInitializedStaticMutex = &mutex;
        wtf_random_init();
        mainThreadIdentifier = EA::Thread::GetThreadId();
    }
#else
     wtf_random_init();
#endif


}



///////////////////////////////////////////////////////////////////////////////
// Thread id map
///////////////////////////////////////////////////////////////////////////////

/*

// EA_TODO: Get rid of HashMap and use a non-memory-allocating container (e.g. eastl::fixed_hash_map).

static HashMap<ThreadIdentifier, pthread_t>& threadMap()
{
    static HashMap<ThreadIdentifier, pthread_t> map;
    return map;
}


static ThreadIdentifier establishIdentifierForPthreadHandle(pthread_t& pthreadHandle)
{
    MutexLocker locker(threadMapMutex());

    static ThreadIdentifier identifierCount = 1;

    threadMap().add(identifierCount, pthreadHandle);
    
    return identifierCount++;
}


static ThreadIdentifier identifierByPthreadHandle(const pthread_t& pthreadHandle)
{
    MutexLocker locker(threadMapMutex());

    HashMap<ThreadIdentifier, pthread_t>::iterator i = threadMap().begin();

    for (; i != threadMap().end(); ++i)
    {
        if (pthread_equal(i->second, pthreadHandle))
            return i->first;
    }

    return 0;
}


static pthread_t pthreadHandleForIdentifier(ThreadIdentifier id)
{
    MutexLocker locker(threadMapMutex());
    
    return threadMap().get(id);
}


static void clearPthreadHandleForIdentifier(ThreadIdentifier id)
{
    MutexLocker locker(threadMapMutex());

    ASSERT(threadMap().contains(id));
    
    threadMap().remove(id);
}
*/


///////////////////////////////////////////////////////////////////////////////
// Thread
///////////////////////////////////////////////////////////////////////////////


/*
struct EAThreadFunctionData
{
    ThreadFunction webKitEntryPoint;
    void*          webKitArgument;
    void*          webKitReturnValue;
};

intptr_t EAThreadFunction(void* pContext)
{
    EAThreadFunctionData* const pEAThreadFunctionData = (EAThreadFunctionData*)pContext;
    pEAThreadFunctionData->webKitReturnValue = (*pEAThreadFunctionData->webKitEntryPoint)(pEAThreadFunctionData->webKitArgument);
    return (intptr_t)pEAThreadFunctionData->webKitReturnValue;
}
*/

#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
const size_t         kThreadArrayCapacity = 8;
EA::Thread::Thread   threadArray[kThreadArrayCapacity];
EA::Thread::ThreadId threadArrayUsage[kThreadArrayCapacity] =
{
    EA::Thread::kThreadIdInvalid, EA::Thread::kThreadIdInvalid, 
    EA::Thread::kThreadIdInvalid, EA::Thread::kThreadIdInvalid,
    EA::Thread::kThreadIdInvalid, EA::Thread::kThreadIdInvalid, 
    EA::Thread::kThreadIdInvalid, EA::Thread::kThreadIdInvalid
};
#endif

// Return ThreadIdentifier if successful or zero for failure.
ThreadIdentifier createThread(ThreadFunction entryPoint, void* data)
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS

    EAT_ASSERT(EA::Thread::kThreadIdInvalid == 0); // WebKit expects this.
    EA::Thread::ThreadId threadId = EA::Thread::kThreadIdInvalid;

    MutexLocker locker(threadMapMutex());

    // Find an empty slot.
    for(size_t i = 0; i < kThreadArrayCapacity; ++i)
    {
        if(threadArrayUsage[i] == EA::Thread::kThreadIdInvalid)
        {
            threadArrayUsage[i] = threadId = threadArray[i].Begin((EA::Thread::RunnableFunction)(uintptr_t)entryPoint, data);
            break;
        }
    }

    EAT_ASSERT(sizeof(ThreadIdentifier) >= sizeof(EA::Thread::ThreadId));
    return (ThreadIdentifier)threadId;
#else
    return 0;
#endif
}


// Return 0 on success, same as pthread_join().
int waitForThreadCompletion(ThreadIdentifier threadID, void** result)
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS    
    EAT_ASSERT(sizeof(ThreadIdentifier) >= sizeof(EA::Thread::ThreadId));
    const EA::Thread::ThreadId eathreadId = (EA::Thread::ThreadId)threadID;

    MutexLocker locker(threadMapMutex());

    for(size_t i = 0; i < kThreadArrayCapacity; ++i)
    {
        if(threadArrayUsage[i] == eathreadId)
        {
            intptr_t threadReturnValue = 0;

            // const EA::Thread::Thread::Status status = 
            threadArray[i].WaitForEnd(EA::Thread::kTimeoutNone, &threadReturnValue);
            *result = (void*)threadReturnValue;
            threadArrayUsage[i] = EA::Thread::kThreadIdInvalid;
            return 0;
        }
    }

    return -1;
#else
    return 0;
#endif

}


void detachThread(ThreadIdentifier threadID)
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
    EAT_ASSERT(sizeof(ThreadIdentifier) >= sizeof(EA::Thread::ThreadId));
    const EA::Thread::ThreadId eathreadId = (EA::Thread::ThreadId)threadID;

    MutexLocker locker(threadMapMutex());

    for(size_t i = 0; i < kThreadArrayCapacity; ++i)
    {
        if(threadArrayUsage[i] == eathreadId)
        {
            // EA_TODO
            // We need to implement a way to detach in EAThread, else we will accumulate 
            // zombie threads on some platforms. See EAThreadData::mbAttached 
            // to deal with this.
            // threadArray[i].Detach();

            threadArrayUsage[i] = EA::Thread::kThreadIdInvalid;
            break;
        }
    }
#endif

}


ThreadIdentifier currentThread()
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
    EAT_ASSERT(sizeof(ThreadIdentifier) >= sizeof(EA::Thread::ThreadId));
    const EA::Thread::ThreadId threadId = EA::Thread::GetThreadId();

    return (ThreadIdentifier) threadId;
#else
    return 0;
#endif

}


bool isMainThread()
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
    return (EA::Thread::GetThreadId() == mainThreadIdentifier);
#else
    return 0;
#endif

}




///////////////////////////////////////////////////////////////////////////////
// Mutex
///////////////////////////////////////////////////////////////////////////////

Mutex::Mutex()
{
    // Nothing to do.
}

Mutex::~Mutex()
{
    // Nothing to do.
}

void Mutex::lock()
{
    //m_mutex.Lock();
}
    
bool Mutex::tryLock()
{
    // 5/14/09 CS - Removed as not supported currently    
    //    return m_mutex.Lock(EA::Thread::kTimeoutImmediate) > 0;
	return false;
}


void Mutex::unlock()
{
	//m_mutex.Unlock();
}



///////////////////////////////////////////////////////////////////////////////
// ThreadCondition
///////////////////////////////////////////////////////////////////////////////

ThreadCondition::ThreadCondition()
{ 
    // Nothing to do.
}


ThreadCondition::~ThreadCondition()
{
    // Nothing to do.
}


void ThreadCondition::wait(Mutex& mutex)
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
    // const EA::Thread::Condition::Result result = 
    m_condition.Wait(&mutex.impl());
#endif
}


bool ThreadCondition::timedWait(Mutex& mutex, double interval)
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
    // 'interval' is defined to be floating point seconds.
    // We convert seconds to milliseconds (to EA::Thread::ThreadTime).
    EA::Thread::ThreadTime timeoutTime = EA::Thread::GetThreadTime() + (EA::Thread::ThreadTime)(interval * 1000);

    const EA::Thread::Condition::Result result = m_condition.Wait(&mutex.impl(), timeoutTime);

    return (result == EA::Thread::Condition::kResultOK);
#else
    return 0;
#endif

}


void ThreadCondition::signal()
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
    m_condition.Signal(false);
#endif

}


void ThreadCondition::broadcast()
{
#ifndef BC_THREADING_EA_DISABLE_MP_THREADS
    m_condition.Signal(true);
#endif
}
    

} // namespace WTF
