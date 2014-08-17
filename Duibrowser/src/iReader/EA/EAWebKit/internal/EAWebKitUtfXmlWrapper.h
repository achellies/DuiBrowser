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
// EAWebKitUTFXLMWrapper.h
// 6/24/09 CSidhall - Wrapper for UTFXML package 
///////////////////////////////////////////////////////////////////////////////

// Note: Location of this file is currently inside EAWebKit but could be moved to an outside lib or 
// the outside app if needed

#ifndef EAWEBKIT_UTFXML_WRAPPER_H
#define EAWEBKIT_UTFXML_WRAPPER_H

#include <EABase/EABase.h>
#include <EAWebKit/EAWebKitUtfXmlInterface.h>
#include <coreallocator/icoreallocator_interface.h>
#include <EAWebKit/EAWebKitConfig.h>
#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif

// Include Note: We don't want to include UTFXML headers here but use them instead in the cpp file as this isolates UTFXML from EAWebkit so
// that a user can set up his own lib if desired.

namespace EA
{
    namespace XMLWrapper
    {

        #if U_SIZEOF_WCHAR_T==2
            typedef wchar_t UChar;
        #else
            typedef uint16_t UChar;
        #endif

        typedef UChar decode_entity_named(const char * name);  // User provided function
        
        class XmlReaderProxy: public EA::Internal::XML::IXmlReader
        {
        public:
            XmlReaderProxy();
           ~XmlReaderProxy();   

            void SetFlag(uint32_t nFlag, bool bSet);
            void AddNamespaceAlias(const char * pAliasName, const char * pNamespace, bool bCopy);
            void AppendInputBuffer(const void * pBuffer,size_t nBufferLen, EA::Internal::XML::ReadEncoding encoding = EA::Internal::XML::kReadEncodingUTF8,
                                   const char * pStreamURI = NULL, bool bCopy = false); 
            void PushInputBuffer(const void * pBuffer, size_t nBufferLen, EA::Internal::XML::ReadEncoding encoding = EA::Internal::XML::kReadEncodingUTF8, 
                                 const char * pStreamURI = NULL,    bool bCopy = false);
            const char* GetElementNamespace(int32_t index, const char** pNamespace = NULL) const;
            EA::Internal::XML::ResultCode GetResultCode();
            int32_t GetLineNumber() const;
            int32_t GetColumnNumber() const ;
            EA::Internal::XML::NodeType GetNodeType() const;
            bool Read(); 
            bool IsEmptyElement() const;
            bool GetIsCData() const;
            const char *GetName() const;
            bool GetIsStandalone() const;        
            const char *GetNamespaceURI() const;
            size_t GetNamePrefixLength() const;
            const char *GetValue() const;
            const char *GetDocTypeName() const;
            const char *GetPublicId()const;
            const char *GetSystemId()const;
            int32_t GetElementNamespaceCount() const;
            int32_t GetAttributeCount() const;
            const char *GetAttributeName( int nIndex ) const;
            const char *GetAttributeLocalName( int nIndex ) const;
            const char *GetAttributeValue( int nIndex ) const;
            void SetEntityNamedDecoder(EA::Internal::XML::decode_entity_named* pFunc);

            // User provided decoder function for callback                   
            static decode_entity_named* spEntityNamedDecoder;   

        private:
            void* mpReader;         // Basically XmlReader or equivalent
        
        };

       ///////////////////////////////////////////////////////////////////////
        // UTFXML Wrapper API (used by EAWebkit - see BCXMLTokenizerEA.cpp)
        ///////////////////////////////////////////////////////////////////////

        EA::Internal::XML::IXmlReader* CreateXMLReaderWrapperInterface();
        void DestroyXMLReaderWrapperInterface(EA::Internal::XML::IXmlReader* pXMLInterface);

    } // Namespace UTFXMLWrapper

} // Namespace EA
#endif // Header include guard

