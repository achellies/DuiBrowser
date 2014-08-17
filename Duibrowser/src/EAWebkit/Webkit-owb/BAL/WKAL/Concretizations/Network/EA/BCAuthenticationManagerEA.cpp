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
// EAWebKitAuthenticationManager.cpp
// By Nicki Vankoughnett
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Example page:
//     http://prism.library.cornell.edu/control/authBasic/authTest/ (username = "test", password = "this")
//     Parent page: http://prism.library.cornell.edu/control/authBasic/
//
// For Basic authentication, we receive a header like this:
//     WWW-Authenticate: Basic realm="WallyWorld"
// and need to resend the request with a header like this:
//     Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Note by Paul Pedriana (February 2009):
// There is a problem case which we don't currently address. Suppose the user 
// goes to a web page that doesn't require authentication. But that web page
// has 10 gif images on it, each of which requires the same authentication.
// The main page will launch the 10 gif file loads in parallel. Currently we will
// respond by putting 10 equivalent dialog boxes up on the screen, each of 
// which requires identical user responses. What we need to do is detect that
// there are multiple concurrent identical authorization requests and tie them
// to the same user input request. If these gif loads happened one after another
// instead of in parallel, then we wouldn't have this problem because we would
// have had the chance to save the user's response by the time the second 
// request comes in.
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include <EAWebKit/EAWebKitView.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>
#include "BCAuthenticationManagerEA.h"
#include "ResourceHandleManager.h"
#include "ResourceHandle.h"
#include "ResourceHandleClient.h"
#include "ResourceHandleInternal.h"
#include <EAWebKit/Internal/EAWebkitString.h>
#include <wtf/Vector.h>
#include "Base64.h"
#include "ResourceHandle.h"
#include "ResourceRequest.h"
#include "ResourceError.h"
#include "WebError.h"

namespace EA
{
namespace WebKit
{


static void BasicEncodeNameAndPassword(const char16_t* pName16, const char16_t* pPassword16, WebCore::String& sEncoded)
{
    // We Base64 encode the name and password as per basic authentication.
    // This means we make an 8 bit nul-terminated Latin string in the form of "name:password" and Base64 encode it.
    const size_t nameLength16 = EA::Internal::Strlen(pName16);
    const size_t passLength16 = EA::Internal::Strlen(pPassword16);

    // We use Strlcpy here because it yields the strlen of the resulting 8 bit text, which isn't necessarly equal to the strlen of the 16 bit text.
    size_t nameLength8     = EA::Internal::Strlcpy(NULL, pName16,     0, nameLength16);
    size_t passwordLength8 = EA::Internal::Strlcpy(NULL, pPassword16, 0, passLength16);
    size_t totalLength8    = nameLength8 + passwordLength8 + 2;  //add two: for terminating NULL and for ':'

    // Copy text data into our 'in' vector.
    WTF::Vector<char> in(totalLength8);
    WTF::Vector<char> out;

    EA::Internal::Strlcpy(&in[0], pName16, nameLength8 + 1, nameLength16);
    in[nameLength8] = ':';
    EA::Internal::Strlcpy(&in[nameLength8 + 1], pPassword16, passwordLength8 + 1, passLength16);
    in[totalLength8 - 1] = 0;

    // Encode the text to our 'out' vector.
    WebCore::base64Encode(in, out);

    // Copy WebCore Vector to our FixedString16_256.
    sEncoded = L"Basic ";
    for(size_t i = 0, iEnd = out.size(); i != iEnd; ++i)
        sEncoded.append(out[i]);
}



AuthenticationManager::AuthenticationManager(WebCore::ResourceHandleManager* pResourceHandleManager)
  : mpResourceHandleManager(pResourceHandleManager)
  , mSavedCredentialsList()
  , mTransportInfoList()
  , mResourceHandleCancelMessageMap()
{
}


void AuthenticationManager::Init()
{
    // To do: Load saved credentials from disk.
    // We may want to encrypt these somehow.
}


void AuthenticationManager::Shutdown()
{
    // To do: Write saved credentials from disk.
    // We may want to encrypt these somehow.
}


void AuthenticationManager::BeginAuthorization(EA::WebKit::TransportInfo& tiNew)
{
    // We don't currently support any format except Basic. The other primary formats are Digest and NTLM.
    // As of this writing, BCResourceHandleManagerEA filters out these other formats for us, so we shouldn't see them.
    EAW_ASSERT(EA::Internal::Stricmp(tiNew.mAuthorizationType, L"Basic") == 0);

    mTransportInfoList.push_back();

    EA::WebKit::TransportInfo& ti = mTransportInfoList.back();
    ti = tiNew;
    ti.mbProcessed           = false;
    ti.mTransportHandlerData = 0;

    WebCore::ResourceHandle* pRH = (WebCore::ResourceHandle*)ti.mpRH;
    EAW_ASSERT(pRH != NULL);
    pRH->ref(); // We'll own this ref until we are finished with this job.
	
	WebCore::ResourceHandleInternal* pRHI = pRH->getInternal();
	WebCore::ResourceHandleClient*   pRHC = pRHI->client();
	pRHC->didReceiveAuthenticationChallenge(pRH, WebCore::AuthenticationChallenge());

    // Set the timeout to be a very long time, now that we are waiting for manual
    // user input to supply a name and password. Perhaps we should save the 
    // timeout interval for restoration later.
    WebCore::ResourceRequest& rRequest = const_cast<WebCore::ResourceRequest&>(pRH->request());  // ResourceHandle allows only const access to the ResourceRequest, but we need to set something.
    rRequest.setTimeoutInterval(1000000000.0);		
}


void AuthenticationManager::Tick()
{
    for(TransportInfoList::iterator it(mTransportInfoList.begin()); it != mTransportInfoList.end(); )
    {
        // Note that as of this writing we don't have a means for authentication requests to time-out.
        // However, the main user page request might time out. We should go and modify that timeout
        // value in our BeginAuthorization function, and set it to something very long.

        bool bRemoveEntry = false;

        EA::WebKit::TransportInfo& ti = *it;

        ViewNotification* pVN = EA::WebKit::GetViewNotification();

        if(pVN)
        {
            const bool bFirstTime = (ti.mbProcessed == false);
            ti.mbProcessed = true;

            EA::WebKit::AuthenticationInfo ai;

            ai.mpView        = ti.mpView;
            ai.mId           = (uintptr_t)&ti;     // We could use a numerical id, or we could use the address of the struct, which is unchanging for the life of the struct.
            ai.mUserContext  = ti.mTransportHandlerData;
            ai.mbBegin       = bFirstTime;
			ai.mpURL         = GetFixedString(ti.mURI)->c_str();
            ai.mpRealm       = GetFixedString(ti.mAuthorizationRealm)->c_str();
            ai.mpType        = ti.mAuthorizationType;
            ai.mName[0]      = 0;
            ai.mPassword[0]  = 0;
            ai.mPersistLevel = kAIPersistNone;
            ai.mResult       = kAIResultNone;

            pVN->Authenticate(ai);

            if(ai.mResult == kAIResultCancel)
            {
                Cancel(ti);
                bRemoveEntry = true;				
            }
            else if(ai.mResult == kAIResultOK)
            {
                Proceed(ti, ai);
                bRemoveEntry = true;
            }
            else
                ti.mTransportHandlerData = ai.mUserContext; // The user may have written this value, so save it for the next time.
        }
        else
        {
            Cancel(ti);
            bRemoveEntry = true;
        }

        if(bRemoveEntry)
            it = mTransportInfoList.erase(it);
        else
            ++it;
    }
}


bool AuthenticationManager::ProvideCredentialsHeader(EA::WebKit::TransportInfo& ti)
{
    // This is a function that would be called by the ResourceHandleManagerfor every URI load.
    // If this request looks like one that we can provide existing Basic credentials for,
    // then do so by directly writing the Authorization header and return true.

    // We check each of our saved credentials and see if any of them match the 
    // given URL/Realm combo. If so then we write the appropriate authorization
    // header into the TransportInfo to be sent to the server. If the info is 
    // out of date or incorrect then we'll just get back a new request for 
    // authorization. 

    SavedCredentials* pSC = NULL;

    WebCore::KURL   url(GetFixedString(ti.mURI)->c_str());
    WebCore::String sServer(url.host());
    WebCore::String sPath(url.path());

    // Remove any file name present in sPath, leaving just the directory with a trailing '/' char.
    int pathSepPos = sPath.reverseFind('/');

    if(pathSepPos >= 0) // If there is at least one '/' char...
    {
        sPath.truncate((unsigned)pathSepPos + 1);

        // Convert to EASTL string because WebCore strings can't do what we want.
        FixedString16_256 sPathEA(sPath.characters(), sPath.length());

        for(SavedCredentialsList::iterator it(mSavedCredentialsList.begin()); it != mSavedCredentialsList.end(); ++it)
        {
            SavedCredentials& sc = *it;

            if(sc.mServer.comparei(sServer.charactersWithNullTermination()) == 0)
            {
                // It turns out that the realm information isn't made available by the server until
                // the server gives you back a 401 or 407 error. Yet we are trying here to send out 
                // credentials on our initial GET before even seeing such an error. So we just do the
                // best we can and use the first server/path combination that matches. We could do 
                // better if we were doing HTTP pipelining, in which case we'd always be on the same
                // TCP/IP connection. We could also do somewhat better by using the most recent best
                // realm match instead of the first one.

                //if(sc.mRealm.comparei(ti.mAuthorizationRealm.c_str()) == 0)
                {
                    if(sPathEA.find(sc.mPath) == 0) // If sPath is equal to or is a child of sc.mPath...
                    {
                        pSC = &sc;
                        break;
                    }
                }
            }
        }
    }

    if(pSC) // If we found some credentials that seem to match...
    {
        WebCore::String  sCredentialString;
        FixedString16_32 mName(pSC->mName);
        FixedString16_32 mPassword(pSC->mPassword);

        MungeString(mName,     false);
        MungeString(mPassword, false);
        BasicEncodeNameAndPassword(mName.c_str(), mPassword.c_str(), sCredentialString);

        // We need to convert from webkit string to EASTL/EAWebKit string.
        EA::WebKit::HeaderMap::value_type entry(L"Authorization");
        entry.second.assign(sCredentialString.characters(), sCredentialString.length());

        EA::WebKit::HeaderMap::iterator it = GetHeaderMap(ti.mHeaderMapOut)->find_as(entry.first.c_str(), EA::WebKit::str_iless());

        if(it == GetHeaderMap(ti.mHeaderMapOut)->end())
            GetHeaderMap(ti.mHeaderMapOut)->insert(entry);
        else
            it->second.assign(entry.second.c_str(), entry.second.length());
    }
	

    return (pSC != NULL);
}


void AuthenticationManager::AddSavedCredentials(const char16_t* pURI, const char16_t* pRealm, const char16_t* pName, const char16_t* pPassword)
{
    // We need to be careful about saving credentials. We want to save only credentials that 
    // succeeded, and not just whatever the user supplied when pressing the OK button in the 
    // name/password dialog box. The implication is that credential saving should occur only
    // after the given URI is successfully loaded, which implies that we need to somehow hook
    // into that event. 

    // Additionally we need to deal with the case of our credential list getting filled with 
    // stale items. As passwords expire and as the user visits many sites, this list will grow
    // to contain items that we may not want any more.

    // We need to remove any existing credentials that match the URI+realm combination.

    WebCore::KURL   url(pURI);
    WebCore::String sServer(url.host());
    WebCore::String sPath(url.path());

    // First we want to remove any saved credentials that are a subset of these credentials.
    // Such credentials would have the same server as sServer, same realm as pRealm, and a 
    // directory path which is equal to or a child of the directory of sPath.

    // Remove any file name present in sPath, leaving just the directory with a trailing '/' char.
    int pathSepPos = sPath.reverseFind('/');

    if(pathSepPos >= 0) // If there is at least one '/' char...
    {
        SavedCredentials* pNewCredentials = NULL;

        sPath.truncate((unsigned)pathSepPos + 1);

        for(SavedCredentialsList::iterator it(mSavedCredentialsList.begin()); it != mSavedCredentialsList.end(); ++it)
        {
            SavedCredentials& sc = *it;

            if(sc.mServer.comparei(sServer.charactersWithNullTermination()) == 0)
            {
                if(sc.mRealm.comparei(pRealm) == 0)
                {
                    if(sc.mPath.find(sPath.charactersWithNullTermination()) == 0) // If sc.mPath is equal to or is a child of sPath...
                    {
                        if(sc.mPath.length() != sPath.length())
                           sc.mPath.assign(sPath.characters(), sPath.length());

                        sc.mName.assign(pName);
                        sc.mPassword.assign(pPassword);

                        pNewCredentials = &sc;
                        break;
                    }
                }
            }
        }

        if(!pNewCredentials)
        {
            mSavedCredentialsList.push_back();
            SavedCredentials& sc = mSavedCredentialsList.back();

            sc.mServer.assign(sServer.characters(), sServer.length());
            sc.mPath.assign(sPath.characters(), sPath.length());
            sc.mRealm.assign(pRealm);
            sc.mName.assign(pName);
            sc.mPassword.assign(pPassword);

            pNewCredentials = &sc;
        }

        if(pNewCredentials)
        {
            MungeString(pNewCredentials->mName,     true);
            MungeString(pNewCredentials->mPassword, true);
        }
    }
}


void AuthenticationManager::MungeString(FixedString16_32& s, bool /*bMunge*/)
{
    // We add a minor amount of hacker resistance to our user name and password strings by 
    // xor-ing them with a value. This makes it a little harder for hackers to do the classic
    // trick of searching for a string in the memory image of the heap.

    for(eastl_size_t i = 0, iEnd = s.length(); i < iEnd; i++)
    {
        uint8_t xorByte = (uint8_t)((uintptr_t)this >> ((i % 4) * 8));
        s[i] ^= xorByte;
    }
}

void AuthenticationManager::SaveCancelMessageData(const WebCore::ResourceHandle* pRH, const char* data, size_t length)
{
	ResourceHandleCancelMessageMap::iterator iter = mResourceHandleCancelMessageMap.find((uintptr_t)(pRH));
	if(iter != mResourceHandleCancelMessageMap.end())
	{
		iter->second.append(data,length);
	}
	else
	{
		ResourceHandleCancelMessageMap::mapped_type mapped(data,length);
		mResourceHandleCancelMessageMap.insert(ResourceHandleCancelMessagePair((uintptr_t)(pRH),mapped));
	}

}

void AuthenticationManager::Proceed(EA::WebKit::TransportInfo& ti, const EA::WebKit::AuthenticationInfo& ai)
{
    // We create an HTTP header of the form:
    //     Authorization: Basic <credential string>
    // where the credential string is in the form of name:password base-64 encoded.
    // So we have something that looks like this, for example:
    //     Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
 
    WebCore::String sCredentialString;
    BasicEncodeNameAndPassword(ai.mName, ai.mPassword, sCredentialString);

    // Submit a new job with mpResourceHandleManager, with the appropriate authorization headers set.
    WebCore::ResourceHandle* pRH = (WebCore::ResourceHandle*)ti.mpRH;
    WebCore::ResourceRequest& rRequest = const_cast<WebCore::ResourceRequest&>(pRH->request());  // ResourceHandle allows only const access to the ResourceRequest, but we need to add a header.

    // We write the value directly to the resource request, as that's what will be newly 
    // submitted to the ResourceHandleManager, not our TransportInfo and its header map.
    rRequest.addHTTPHeaderField(WebCore::String(L"Authorization"), sCredentialString);
    rRequest.setTimeoutInterval(ti.mTimeoutInterval);  // Restore the original timeout interval for this request.
    mpResourceHandleManager->add(pRH);  // Takes over the ref we have.

	ResourceHandleCancelMessageMap::iterator iter = mResourceHandleCancelMessageMap.find((uintptr_t)(pRH));
	if(iter != mResourceHandleCancelMessageMap.end())
	{
		mResourceHandleCancelMessageMap.erase(iter);
	}
	// We save the credentials for future use. 
    // However, note that these credentials might fail in the future. 
    // In reality we don't want to save the credentials here but we need to 
    // have them be saved if the ResourceHandleManager is successful in the
    // first application of the credentials.
	AddSavedCredentials(GetFixedString(ti.mURI)->c_str(), GetFixedString(ti.mAuthorizationRealm)->c_str(), ai.mName, ai.mPassword);

    // pRH->deref(); Don't need this if we are resubmitting the job, because the ResourceHandleManager takes over the ref.

    // We don't remove ti from our mTransportInfoList, as we expect the caller to do that.
}


void AuthenticationManager::Cancel(EA::WebKit::TransportInfo& ti)
{
    using namespace WebCore;

    ResourceHandle*         pRH  = (ResourceHandle*)ti.mpRH;
    ResourceHandleInternal* pRHI = pRH->getInternal();
    ResourceHandleClient*   pRHC = pRHI->client();

    EAW_ASSERT((pRH != NULL) && (pRHI != NULL));
	if(pRHC)
		pRHC->didCancelAuthenticationChallenge(pRH, AuthenticationChallenge());
    
	pRHI->m_response.setHTTPStatusCode(ti.mResultCode);
    
	if(pRHC)
        pRHC->didReceiveResponse(pRH, pRHI->m_response);
    
	pRHI->m_response.setResponseFired(true);
	
	ResourceHandleCancelMessageMap::iterator iter = mResourceHandleCancelMessageMap.find((uintptr_t)(pRH));
	if( iter != mResourceHandleCancelMessageMap.end())
	{
		if(pRHC)
			pRHC->didReceiveData(pRH, iter->second.data(), (size_t)iter->second.length(), 0);
		
		mResourceHandleCancelMessageMap.erase(iter);
	}
	
	if(pRHC)
		pRHC->didFinishLoading(pRH);	
	
    pRH->deref();

    // We don't remove ti from our mTransportInfoList, as we expect the caller to do that.
}



} //namespace WebKit
} //namespace EA

