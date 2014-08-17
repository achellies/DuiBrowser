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
// BCAuthenticationManagerEA.h
// By Nicki Vankoughnett
///////////////////////////////////////////////////////////////////////////////


#ifndef BCAUTHENTICATIONMANAGEREA_H
#define BCAUTHENTICATIONMANAGEREA_H


#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>
#include <EAWebKit/EAWebKitTransport.h>
#include <EASTL/list.h>
#include <EASTL/hash_map.h>


namespace WebCore
{
    class ResourceHandle;
    class ResourceHandleManager;
}


namespace EA
{
    namespace WebKit
    {
        class View;
        class AuthenticationInfo;


        class AuthenticationManager
        {
        public:
            AuthenticationManager(WebCore::ResourceHandleManager* pResourceHandleManager);

            void Init();
            void Shutdown();

            void BeginAuthorization(EA::WebKit::TransportInfo& ti);
            void Tick();
            bool HasJobs() const { return !mTransportInfoList.empty(); }
            bool ProvideCredentialsHeader(EA::WebKit::TransportInfo& ti);
			void SaveCancelMessageData(const WebCore::ResourceHandle* pRH, const char* data, size_t length);

        protected:
            struct SavedCredentials
            {
                FixedString16_32 mServer;       // e.g. www.bozo.com
                FixedString16_256    mPath;         // e.g. /abc/def/ghi.html
                FixedString16_32 mRealm;        // e.g. WallyWorld
                FixedString16_32 mName;
                FixedString16_32 mPassword;
            };

            typedef eastl::list<SavedCredentials, EASTLAllocator> SavedCredentialsList;
            typedef eastl::list<EA::WebKit::TransportInfo, EASTLAllocator> TransportInfoList;
			
			typedef eastl::pair<const uintptr_t, FixedString8_256> ResourceHandleCancelMessagePair;
			typedef eastl::hash_map<uintptr_t, FixedString8_256, eastl::hash<uintptr_t>,
				eastl::equal_to<uintptr_t>, EASTLAllocator> ResourceHandleCancelMessageMap;


            void Proceed(EA::WebKit::TransportInfo& ti, const EA::WebKit::AuthenticationInfo& ai);
            void Cancel(EA::WebKit::TransportInfo& ti);
            void AddSavedCredentials(const char16_t* pURI, const char16_t* pRealm, const char16_t* pName, const char16_t* pPassword);
            void MungeString(FixedString16_32& s, bool bMunge);

        protected:
            WebCore::ResourceHandleManager* mpResourceHandleManager;
            SavedCredentialsList            mSavedCredentialsList;
            TransportInfoList               mTransportInfoList;
			ResourceHandleCancelMessageMap	mResourceHandleCancelMessageMap;
        };

    } // namespace WebKit

} // namespace EA


#endif // Header include guard
