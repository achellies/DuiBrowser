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
// BCResourceHandleWK.h
// Modified by Paul Pedriana - 2008
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Note by Paul Pedriana:
// This is a modified version of the BCResourceHandleWK.h file, which itself 
// is a modified version of the original WebKit ResourceHandle.h file. 
// This version cleans up a bunch of the ifdefs that a found in the original
// file and which serve to make it harder to read and understand. However,
// a resync to the WebKit ResourceHandle.h/cpp files may require examining 
// the files for changes in the WebKit source that we need to know about.
/////////////////////////////////////////////////////////////////////////////


#ifndef ResourceHandle_h
#define ResourceHandle_h

#include "AuthenticationChallenge.h"
#include "HTTPHeaderMap.h"
#include <wtf/OwnPtr.h>


namespace WKAL {

class AuthenticationChallenge;
class Credential;
class FormData;
class Frame;
class KURL;
class ResourceError;
class ResourceHandleClient;
class ResourceHandleInternal;
class ResourceRequest;
class ResourceResponse;
class SchedulePair;
class SharedBuffer;
class SubresourceLoader;
class SubresourceLoaderClient;

template <typename T> class Timer;

class ResourceHandle : public RefCounted<ResourceHandle> {
private:
    ResourceHandle(const ResourceRequest&, ResourceHandleClient*, bool defersLoading, bool shouldContentSniff, bool mightDownloadFromHandle);

    enum FailureType {
        BlockedFailure,
        InvalidURLFailure
    };

public:
    static void staticFinalize(); // Added by EA to avoid lost memory.

    // FIXME: should not need the Frame
    static PassRefPtr<ResourceHandle> create(const ResourceRequest&, ResourceHandleClient*, Frame*, bool defersLoading, bool shouldContentSniff, bool mightDownloadFromHandle = false);

    static void loadResourceSynchronously(const ResourceRequest&, ResourceError&, ResourceResponse&, Vector<char>& data, Frame* frame);
    static bool willLoadFromCache(ResourceRequest&);
    
    ~ResourceHandle();

    void didReceiveAuthenticationChallenge(const AuthenticationChallenge&);
    void receivedCredential(const AuthenticationChallenge&, const Credential&);
    void receivedRequestToContinueWithoutCredential(const AuthenticationChallenge&);
    void receivedCancellation(const AuthenticationChallenge&);

    PassRefPtr<SharedBuffer> bufferedData();
    static bool supportsBufferedData();

    ResourceHandleInternal* getInternal() { return d.get(); }

    // Used to work around the fact that you don't get any more NSURLConnection callbacks until you return from the one you're in.
    static bool loadsBlocked();    
    
    void clearAuthentication();
    void cancel();

    // The client may be 0, in which case no callbacks will be made.
    ResourceHandleClient* client() const;
    void setClient(ResourceHandleClient*);

    void setDefersLoading(bool);
      
    const ResourceRequest& request() const;

    void fireFailure(Timer<ResourceHandle>*);

private:
    void scheduleFailure(FailureType);
    bool start(Frame*);

    friend class ResourceHandleInternal;
    OwnPtr<ResourceHandleInternal> d;
};

}

#endif // ResourceHandle_h
