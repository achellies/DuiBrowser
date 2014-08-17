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
// EAWebKitUTFXLMWrapper.cpp
// 6/24/09 CSidhall - Wrapper for UTFXML package 
///////////////////////////////////////////////////////////////////////////////

// Note: Location of this file is currently inside EAWebKit but could be moved to an outside lib or 
// the outside app if needed

#include <EAWebKit/Internal/EAWebKitUtfXmlWrapper.h>
#include <EABase/EABase.h>
#include <coreallocator/icoreallocator_interface.h>
#include <EAAssert/eaassert.h>
#include <wtf/FastAllocBase.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebkit/internal/EAWebKitAssert.h>
#include <UTFXml/XmlReader.h>   // The lib we want to wrap...

namespace EA
{
    namespace XMLWrapper
    {
       
// Small helper function to find allocator used (other solutions possible like having the user set an allocator but this avoids demo code changes)
static EA::WebKit::Allocator* GetAllocator_Helper()
{
    EA::WebKit::Allocator* pAllocator = EA::WebKit::GetAllocator();        
    EA_ASSERT_MSG(pAllocator, "User needs to set the WebKit allocator!");
    return pAllocator;
}

//+ Grabbed and modified these following macro/templates from EAText 
#define UTFXML_WRAPPER_NEW(Class, pAllocator, pName) \
new ((pAllocator)->Malloc(sizeof(Class), 0, pName ) )Class

template <typename T>
inline void delete_object(T* pObject, size_t size, EA::WebKit::Allocator* pAllocator)
{
    if(pObject) // As per the C++ standard, deletion of NULL results in a no-op.
    {
        pObject->~T();
        pAllocator->Free(pObject, size);
    }
}

#define UTFXML_WRAPPER_DELETE(pObject, size, pAllocator) delete_object(pObject, size, pAllocator)

// Callback: Can't be 
bool UTFXmlEntityLookup(EA::XML::XmlReader* pReader, const char* pEntityName, void* /*pContext*/)
{
    if(!XmlReaderProxy::spEntityNamedDecoder)
        return false;

    const UChar c = (*XmlReaderProxy::spEntityNamedDecoder)(pEntityName);
    if(c)
    {
        pReader->PushInputBuffer(&c, 1); // PushInputBuffer will copy c.
        return true;
    }
    return false;
}

//////////////////////////
// --- XmlReaderProxy ---
//////////////////////////

// Statics
decode_entity_named* XmlReaderProxy::spEntityNamedDecoder =0;   

// 
XmlReaderProxy::XmlReaderProxy()
    :mpReader(0)
{
    // Check that lib and interface are in sync
    EA_COMPILETIME_ASSERT( (int) EA::XML::kReadEncodingUTF16 == (int) EA::Internal::XML::kReadEncodingUTF16 );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::kErrorUnclosedToken == (int) EA::Internal::XML::kErrorUnclosedToken );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::kSuccess == (int) EA::Internal::XML::kSuccess );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::Document == (int) EA::Internal::XML::Document );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::Element == (int) EA::Internal::XML::Element );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::EndElement == (int) EA::Internal::XML::EndElement );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::Comment == (int) EA::Internal::XML::Comment );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::CharacterData == (int) EA::Internal::XML::CharacterData );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::ProcessingInstruction == (int) EA::Internal::XML::ProcessingInstruction );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::Prologue == (int) EA::Internal::XML::Prologue );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::EntityRef == (int) EA::Internal::XML::EntityRef );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::DocTypeDecl == (int) EA::Internal::XML::DocTypeDecl );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::EntityDecl == (int) EA::Internal::XML::EntityDecl );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::ElementDecl == (int) EA::Internal::XML::ElementDecl );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::AttListDecl == (int) EA::Internal::XML::AttListDecl );
    EA_COMPILETIME_ASSERT( (int) EA::XML::XmlReader::NotationDecl == (int) EA::Internal::XML::NotationDecl );
    
    // Need to create the reader
    EA::WebKit::Allocator* pAllocator = GetAllocator_Helper();
    if(!pAllocator)
        return;
    mpReader = UTFXML_WRAPPER_NEW(EA::XML::XmlReader, pAllocator, "UTFReader");    

    // Set up the resolver 
    if(mpReader) {
        EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);        
        pReader->SetEntityRefResolver(UTFXmlEntityLookup, NULL);   
    }
}

XmlReaderProxy::~XmlReaderProxy()   
{
    if(mpReader) {
        EA::WebKit::Allocator* pAllocator = GetAllocator_Helper();   

        //delete_object<EA::XML::XmlReader> (mpReader, sizeof(EA::XML::XmlReader), pAllocator); 
        EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);        
        
        UTFXML_WRAPPER_DELETE(pReader, sizeof(EA::XML::XmlReader), pAllocator);   
        mpReader =0;
    }
}   

void XmlReaderProxy::SetFlag( uint32_t nFlag, bool bSet )
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(pReader)
        pReader->SetFlag(nFlag,bSet);
}

void XmlReaderProxy::SetEntityNamedDecoder(EA::Internal::XML::decode_entity_named* pFunc)
{
    // User provided decoder function used in callback                   
    spEntityNamedDecoder = (decode_entity_named*) pFunc;   
}

void XmlReaderProxy::AddNamespaceAlias( const char * pAliasName, const char * pNamespace, bool bCopy )
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(pReader)
        pReader->AddNamespaceAlias(pAliasName,pNamespace, bCopy);
}

void XmlReaderProxy::AppendInputBuffer(const void * pBuffer,size_t nBufferLen, EA::Internal::XML::ReadEncoding encoding,
    const char * pStreamURI, bool bCopy) 
 {
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(pReader)
        pReader->AppendInputBuffer(pBuffer,nBufferLen, (EA::XML::ReadEncoding) encoding,pStreamURI,bCopy); 
 }

 void XmlReaderProxy::PushInputBuffer(const void * pBuffer, size_t nBufferLen, EA::Internal::XML::ReadEncoding encoding, 
   const char * pStreamURI,    bool bCopy )
 {
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(pReader)
        pReader->PushInputBuffer(pBuffer, nBufferLen, (EA::XML::ReadEncoding) encoding, pStreamURI, bCopy);
 }

const char* XmlReaderProxy::GetElementNamespace(int32_t index, const char** pNamespace) const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)            
        return 0;
    return pReader->GetElementNamespace(index, pNamespace);
}


EA::Internal::XML::ResultCode XmlReaderProxy::GetResultCode()
{
    EA::XML::XmlReader::ResultCode code = EA::XML::XmlReader::kErrorUser;  
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(pReader) 
        code =pReader->GetResultCode();
    return (EA::Internal::XML::ResultCode) code;
}

int32_t XmlReaderProxy::GetLineNumber() const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader) 
        return 0;
    return pReader->GetLineNumber(); 
}

int32_t XmlReaderProxy::GetColumnNumber() const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader) 
        return 0;
    return pReader->GetColumnNumber();
}

EA::Internal::XML::NodeType XmlReaderProxy::GetNodeType() const 
{
    EA::XML::XmlReader::NodeType type = EA::XML::XmlReader::None;
    
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(pReader) 
        type= pReader->GetNodeType();
    return (EA::Internal::XML::NodeType) type;

}

bool XmlReaderProxy::Read() 
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return false;
    return pReader->Read();
}

bool XmlReaderProxy::IsEmptyElement() const
{
   EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return false;   
    return pReader->IsEmptyElement();
}

bool XmlReaderProxy::GetIsCData() const 
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return false;
    return pReader->GetIsCData();
}    

const char* XmlReaderProxy::GetName() const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)   
        return 0;

    return pReader->GetName();
}

bool XmlReaderProxy::GetIsStandalone() const        
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return false;
        
    return pReader->GetIsStandalone(); 

}

const char* XmlReaderProxy::GetNamespaceURI() const 
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;
    return pReader->GetNamespaceURI();
}

size_t XmlReaderProxy::GetNamePrefixLength() const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return false;
    return pReader->GetNamePrefixLength();
}

const char* XmlReaderProxy::GetValue() const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;
    return pReader->GetValue();
}

const char* XmlReaderProxy::GetDocTypeName() const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;
    return pReader->GetDocTypeName();
}

const char* XmlReaderProxy::GetPublicId() const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;
    return pReader->GetPublicId();
}

const char* XmlReaderProxy::GetSystemId() const
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;       
    return pReader->GetSystemId();
}

int32_t XmlReaderProxy::GetElementNamespaceCount() const
{
   EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;       
    return pReader->GetElementNamespaceCount();
}

int32_t XmlReaderProxy::GetAttributeCount() const 
{
    EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;       
    return pReader->GetAttributeCount();
}

const char* XmlReaderProxy::GetAttributeName( int nIndex ) const
{
   EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;       
    return  pReader->GetAttributeName(nIndex);   
}

const char* XmlReaderProxy::GetAttributeLocalName( int nIndex ) const
{
   EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;       
    return  pReader->GetAttributeLocalName(nIndex);   
}

const char * XmlReaderProxy::GetAttributeValue( int nIndex ) const
{
  EA::XML::XmlReader* pReader = reinterpret_cast<EA::XML::XmlReader*> (mpReader);
    if(!pReader)
        return 0;       
    return pReader->GetAttributeValue(nIndex);
}


///////////////////////////////////////////////////////////////////////
// UTFXML 
///////////////////////////////////////////////////////////////////////

EA::Internal::XML::IXmlReader* CreateXMLReaderWrapperInterface()
{
	EA::XMLWrapper::XmlReaderProxy* pProxy = EAWEBKIT_NEW("XmlReaderProxy")EA::XMLWrapper::XmlReaderProxy();// WTF::fastNew<EA::XMLWrapper::XmlReaderProxy>(); 
    EAW_ASSERT(pProxy);
    return (EA::Internal::XML::IXmlReader*) pProxy;
}

void DestroyXMLReaderWrapperInterface(EA::Internal::XML::IXmlReader* pXMLInterface)
{
    EAWEBKIT_DELETE ((EA::XMLWrapper::XmlReaderProxy*) pXMLInterface);//WTF::fastDelete<EA::XMLWrapper::XmlReaderProxy>((EA::XMLWrapper::XmlReaderProxy*) pXMLInterface);
}

   } // Namespace XMLWrapper


} // Namespace EA

