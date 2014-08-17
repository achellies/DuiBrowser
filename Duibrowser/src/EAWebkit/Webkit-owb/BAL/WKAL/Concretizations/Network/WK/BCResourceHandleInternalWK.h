// -*- mode: c++; c-basic-offset: 4 -*-
/*
 * Copyright (C) 2004, 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2008-2009
*/

/////////////////////////////////////////////////////////////////////////////
// BCResourceHandleInternalWK.h
// Modified by Paul Pedriana - 2008
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Note by Paul Pedriana:
// This is a modified version of the BCResourceHandleInternalWK.h file, which itself 
// is a modified version of the original WebKit ResourceHandleInternal.h file. 
// This version cleans up a bunch of the ifdefs that a found in the original
// file and which serve to make it harder to read and understand. However,
// a resync to the WebKit ResourceHandleInternal.h/cpp files may require examining 
// the files for changes in the WebKit source that we need to know about.
/////////////////////////////////////////////////////////////////////////////


#ifndef ResourceHandleInternal_h
#define ResourceHandleInternal_h

#include <wtf/FastAllocBase.h>
#include "ResourceHandle.h"
#include "ResourceRequest.h"
#include "AuthenticationChallenge.h"
#include "Timer.h"
#include "FormDataStreamCurl.h"  //Note: This isn't actually a Curl-related file. We should rename it.


namespace WKAL {
    class ResourceHandleClient;

    class ResourceHandleInternal : Noncopyable {
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
        ResourceHandleInternal(ResourceHandle* loader, const ResourceRequest& request, ResourceHandleClient* c, bool defersLoading, bool shouldContentSniff, bool mightDownloadFromHandle)
            : m_client(c)
            , m_request(request)
            , status(0)
            , m_defersLoading(defersLoading)
            , m_shouldContentSniff(shouldContentSniff)
            , m_mightDownloadFromHandle(mightDownloadFromHandle)
            , m_cancelled(false)
            , m_formDataStream(loader)
            , m_failureTimer(loader, &ResourceHandle::fireFailure)
        {
        }
        
       ~ResourceHandleInternal();

        ResourceHandleClient* client() { return m_client; }

    public:
        ResourceHandleClient* m_client;
        ResourceRequest       m_request;
        int                   status;
        bool                  m_defersLoading;
        bool                  m_shouldContentSniff;
        bool                  m_mightDownloadFromHandle;

        ResourceResponse    m_response;
        bool                m_cancelled;
        FormDataStream      m_formDataStream;
        Vector<char>        m_postBytes;

        AuthenticationChallenge     m_currentWebChallenge;
        ResourceHandle::FailureType m_failureType;
        Timer<ResourceHandle>       m_failureTimer;
    };

} // namespace WebCore

#endif // ResourceHandleInternal_h
