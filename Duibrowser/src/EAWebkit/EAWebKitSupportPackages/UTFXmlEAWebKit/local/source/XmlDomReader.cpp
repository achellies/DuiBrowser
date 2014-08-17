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
// Name: XmlDomReader.cpp
// 
// Copyright (c) 2006 Electronic Arts Inc.
// Created by: Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include <UTFXml/internal/Config.h>
#include <UTFXml/XmlDomReader.h>
#include <new>
#include EA_ASSERT_HEADER


namespace EA {
namespace XML {

namespace ReaderLocal {
    tString8 sValueEmpty(Internal::CoreAllocatorAdapter<ICoreAllocator>("", NULL));
}

///////////////////////////////////////////////////////////////////////////////
// IDomContentHandler
///////////////////////////////////////////////////////////////////////////////

bool IDomContentHandler::StartDocument( DomDocument& )
{
    return true;
}

bool IDomContentHandler::EndDocument( DomDocument& )
{
    return true;
}

bool IDomContentHandler::StartElement( DomElement& )
{
    return true;
}

bool IDomContentHandler::EndElement( DomElement& )
{
    return true;
}

bool IDomContentHandler::Characters( DomCharacterData& )
{
    return true;
}

bool IDomContentHandler::Comment( DomComment& )
{
    return true;
}

bool IDomContentHandler::ProcessingInstruction( DomProcessingInstruction& )
{
    return true;
}

bool IDomContentHandler::SkippedEntity( DomEntityRef& )
{
    return true;
}

bool IDomContentHandler::Prologue( DomPrologue& )
{
    return true;
}

bool IDomContentHandler::Declaration( DomDeclaration& )
{
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// InnerTextGenerator
///////////////////////////////////////////////////////////////////////////////

class InnerTextGenerator : public IDomContentHandler
{
public:
    tString8& mInnerText;

    InnerTextGenerator(tString8& innerText) : mInnerText(innerText){ }

    InnerTextGenerator& operator=(const InnerTextGenerator&) { return *this; } // We define this merely to shut up the compiler. It is unused and goes away at link time.

    bool StartDocument( DomDocument& domDocument )
        { mInnerText += domDocument.GetValue(); return true; }

    bool StartElement( DomElement& domElement )
        { mInnerText += domElement.GetValue(); return true; }

    bool Characters( DomCharacterData& domCharacterData )
        { mInnerText += domCharacterData.GetValue(); return true; }

    bool Comment( DomComment& domComment )
        { mInnerText += domComment.GetValue(); return true; }

    bool ProcessingInstruction( DomProcessingInstruction& domProcessingInstruction )
        { mInnerText += domProcessingInstruction.GetValue(); return true; }

    bool SkippedEntity( DomEntityRef& domEntityRef )
        { mInnerText += domEntityRef.GetValue(); return true; }

    bool Prologue( DomPrologue& domPrologue )
        { mInnerText += domPrologue.GetValue(); return true; }

    bool Declaration( DomDeclaration& domDeclaration )
        { mInnerText += domDeclaration.GetValue(); return true; }
};



///////////////////////////////////////////////////////////////////////////////
// DomAttribute
///////////////////////////////////////////////////////////////////////////////

DomAttribute::DomAttribute( ICoreAllocator * pAllocator )
  : mName (CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttribute", pAllocator)),
    mValue(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttribute", pAllocator))
{
}

DomAttribute::DomAttribute( const char8_t * pName, const char8_t * pValue, ICoreAllocator * pAllocator )
  : mName (pName,  CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttribute", pAllocator)),
    mValue(pValue, CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttribute", pAllocator))
{
}

DomAttribute::DomAttribute( const DomAttribute& attribute )
{
    ICoreAllocator* const pAllocator = const_cast<tString8&>(attribute.mName).get_allocator().get_allocator();

    mName.set_allocator(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttribute", pAllocator));
    mName = attribute.mName;

    mValue.set_allocator(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttribute", pAllocator));
    mValue = attribute.mValue;
}

DomAttribute& DomAttribute::operator=( const DomAttribute& attribute )
{
    mName  = attribute.mName;
    mValue = attribute.mValue;

    return *this;
}

void DomAttribute::SetAllocator(ICoreAllocator* pAllocator)
{
    mName .set_allocator(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttribute", pAllocator));
    mValue.set_allocator(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttribute", pAllocator));
}



///////////////////////////////////////////////////////////////////////////////
// DomNode
///////////////////////////////////////////////////////////////////////////////

DomNode::DomNode( XmlReader::NodeType nodeType, const char8_t * pName, size_t nNameLength, DomNode* pParent, ICoreAllocator * pAllocator )
  : mNodeType(nodeType),
    mName(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomNode", pAllocator)),
    mpParent(pParent),
    mChildList(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomNode", pAllocator))
{
    if(nNameLength == kLengthNull)
        nNameLength = (pName ? strlen(pName) : 0);
    mName.assign(pName, (eastl_size_t)nNameLength);
}

DomNode::DomNode( const DomNode& node )
  : mNodeType(node.mNodeType),
    mName(node.mName),
    mpParent(node.mpParent),
    mChildList() // Don't copy the list, as we need to do a deep copy below.
{
    // EASTL containers don't copy the allocator in their operator= functions
    // so we either need to have one already or we need to get one.
    ICoreAllocator* const pAllocator = node.GetAllocator();
    mChildList.set_allocator(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomNode", pAllocator));
    CopyChildList(node);
}

void DomNode::Destroy()
{
    ICoreAllocator* const pAllocator = GetAllocator();
    DomNode*        const pThis      = this;

    pThis->~DomNode();
    pAllocator->Free(pThis);
}

void DomNode::SetAllocator(ICoreAllocator* pAllocator)
{
    mName     .set_allocator(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomNode", pAllocator));
    mChildList.set_allocator(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomNode", pAllocator));
}

void DomNode::Clear()
{
    // As it stands now, DomNode::Clear only clears child nodes.
    ICoreAllocator* const pAllocator = GetAllocator();

    for(tNodeList::iterator it = mChildList.begin(); it != mChildList.end(); ++it)
    {
        DomNode* const pDomNode = *it;
        pDomNode->~DomNode();
        pAllocator->Free(pDomNode);
    }

    mChildList.clear();
}

const tString8& DomNode::GetName()
{
    switch (mNodeType)
    {
        case XmlReader::CharacterData:
            // We need to return "#text" somehow.
            break;

        case XmlReader::Comment:
            // We need to return "#comment" somehow.
            break;

        case XmlReader::Document:
            // We need to return "#document" somehow.
            break;

        case XmlReader::NotationDecl:
            // We need to return "#xml-declaration" somehow.
            break;

        case XmlReader::Prologue:
            // #prologue
            break;

        case XmlReader::None:
        case XmlReader::AttListDecl:
        case XmlReader::DocTypeDecl:
        case XmlReader::Element:
        case XmlReader::EntityDecl:
        case XmlReader::EntityRef:
        case XmlReader::ProcessingInstruction:
        case XmlReader::EndElement:
        case XmlReader::ElementDecl:
            break;
    }

    return mName;
}


const tString8& DomNode::GetValue()
{
    // We attempt to follow the convention of the Microsoft XmlDocument implementation.

    switch (mNodeType)
    {
        case XmlReader::AttListDecl:
        case XmlReader::CharacterData:
        case XmlReader::Comment:
        case XmlReader::ProcessingInstruction:
        case XmlReader::Prologue:
        case XmlReader::ElementDecl:
            return mName;

        case XmlReader::None:
        case XmlReader::Document:
        case XmlReader::DocTypeDecl:
        case XmlReader::Element:
        case XmlReader::EndElement:
        case XmlReader::EntityDecl:
        case XmlReader::EntityRef:
        case XmlReader::NotationDecl:
            break;
    }

    return ReaderLocal::sValueEmpty;
}

void DomNode::GetInnerText(tString8& sInnerText)
{
    sInnerText.clear();

    InnerTextGenerator generator(sInnerText);

    Iterate(&generator);
}

DomNode& DomNode::operator=( const DomNode& node )
{
    // Leave our allocator as-is.
    mNodeType  = node.mNodeType;
    mpParent   = node.mpParent;
    mName      = node.mName;

    CopyChildList(node);

    return *this;
}

bool DomNode::CopyChildList(const DomNode& node)
{
    // We need to clone the child list. Doing so will result in recursive cloning.
    // Note that we can't just say mChildList = node.mChildList, as we need to do
    // a deep copy of the DOM structure.

    for(tNodeList::const_iterator it = node.mChildList.begin(); it != node.mChildList.end(); ++it)
    {
        const DomNode* pDomNode = *it;

        DomNode* const pNew = pDomNode->Clone();

        if(pNew)
            mChildList.push_back(pNew);
        else
            return false;
    }

    return true;
}

DomNode* DomNode::GetTop()
{
    DomNode* pCurrent = this;

    while(pCurrent->mpParent)
        pCurrent = pCurrent->mpParent;

    return pCurrent;
}

tNodeList::iterator DomNode::GetNodeIterator( int nIndex )
{
    int i = 0;
    tNodeList::iterator it = mChildList.begin();

    while((i < nIndex) && (it != mChildList.end()))
    {
        ++i;
        ++it;
    }

    return it;
}

tNodeList::iterator DomNode::GetNodeIterator(const char8_t * pName)
{
    // To do: Implement bRecursive.

    tNodeList::iterator it = mChildList.begin();

    while(it != mChildList.end())
    {
        DomNode* pDomNode = *it;

        if(pDomNode->mName.comparei(pName) == 0)
            break;
        ++it;
    }

    return it;
}

DomDocument* DomNode::AsDomDocument()
{
    EA_ASSERT(mNodeType == EA::XML::XmlReader::Document);
    return static_cast<DomDocument*>(this);
}

DomElement* DomNode::AsDomElement()
{
    EA_ASSERT(mNodeType == EA::XML::XmlReader::Element);
    return static_cast<DomElement*>(this);
}

DomComment* DomNode::AsDomComment()
{
    EA_ASSERT(mNodeType == EA::XML::XmlReader::Comment);
    return static_cast<DomComment*>(this);
}

DomCharacterData* DomNode::AsDomCharacterData()
{
    EA_ASSERT(mNodeType == EA::XML::XmlReader::CharacterData);
    return static_cast<DomCharacterData*>(this);
}

DomPrologue* DomNode::AsDomPrologue()
{
    EA_ASSERT(mNodeType == EA::XML::XmlReader::Prologue);
    return static_cast<DomPrologue*>(this);
}

DomProcessingInstruction* DomNode::AsDomProcessingInstruction()
{
    EA_ASSERT(mNodeType == EA::XML::XmlReader::ProcessingInstruction);
    return static_cast<DomProcessingInstruction*>(this);
}

DomEntityRef* DomNode::AsDomEntityRef()
{
    EA_ASSERT(mNodeType == EA::XML::XmlReader::EntityRef);
    return static_cast<DomEntityRef*>(this);
}

DomDeclaration* DomNode::AsDomDeclaration()
{
    EA_ASSERT(mNodeType == EA::XML::XmlReader::DocTypeDecl  ||
              mNodeType == EA::XML::XmlReader::EntityDecl   ||
              mNodeType == EA::XML::XmlReader::ElementDecl  ||
              mNodeType == EA::XML::XmlReader::AttListDecl  ||
              mNodeType == EA::XML::XmlReader::NotationDecl);
    return static_cast<DomDeclaration*>(this);
}



///////////////////////////////////////////////////////////////////////////////
// DomDocument
///////////////////////////////////////////////////////////////////////////////

DomNode* DomDocument::Clone() const
{
    ICoreAllocator* const pAllocator = GetAllocator();
    void*           const pMemory    = pAllocator->Alloc(sizeof(DomDocument), UTFXML_ALLOC_PREFIX "DomDocument", 0);

    if(pMemory)
        return new(pMemory) DomDocument(*this);
    return NULL;
}

bool DomDocument::Iterate(IDomContentHandler* pDomContentHandler)
{
	if(pDomContentHandler->StartDocument(*this)) 
	{
        for(tNodeList::iterator it = mChildList.begin(); it != mChildList.end(); ++it)
        {
            DomNode* const pDomNode = *it;

            if(!pDomNode->Iterate(pDomContentHandler))
                break;
        }
    }

	return pDomContentHandler->EndDocument(*this);
}





///////////////////////////////////////////////////////////////////////////////
// DomElement
///////////////////////////////////////////////////////////////////////////////

DomElement::DomElement(const char8_t * pName, DomNode* pParent, ICoreAllocator * pAllocator)
  : DomNode( XmlReader::Element, pName, kLengthNull, pParent, pAllocator ),
    #if UTFXML_USE_DOM_ATTRIBUTE_PTR
      mpAttributeList(NULL) // Null/empty by default.
    #else
      mAttributeList(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomNode", pAllocator))
    #endif
{
}

DomElement::DomElement( const DomElement& domElement )
  : DomNode(domElement),
    #if UTFXML_USE_DOM_ATTRIBUTE_PTR
      mpAttributeList(NULL) // We copy domElement.mAttributeList below.
    #else
      mAttributeList(domElement.mAttributeList)
    #endif
{
    #if UTFXML_USE_DOM_ATTRIBUTE_PTR
        if(domElement.mpAttributeList)
            mpAttributeList = CreateAttributeList(domElement.mpAttributeList);
    #endif
}

DomNode* DomElement::Clone() const
{
    ICoreAllocator* const pAllocator = GetAllocator();
    void*           const pMemory    = pAllocator->Alloc(sizeof(DomElement), UTFXML_ALLOC_PREFIX "DomElement", 0);

    if(pMemory)
        return new(pMemory) DomElement(*this);
    return NULL;
}

void DomElement::Clear()
{
    // As it stands now, DomElement::Clear only clears child nodes and element attributes.
    DomNode::Clear();

    #if UTFXML_USE_DOM_ATTRIBUTE_PTR
        DestroyAttributeList(mpAttributeList);  // OK if it is NULL.
        mpAttributeList = NULL;
    #else
        mAttributeList.clear();
    #endif
}

bool DomElement::Iterate(IDomContentHandler* pDomContentHandler)
{
	if(pDomContentHandler->StartElement(*this)) 
	{
        for(tNodeList::iterator it = mChildList.begin(); it != mChildList.end(); ++it)
        {
            DomNode* const pDomNode = *it;

            if(!pDomNode->Iterate(pDomContentHandler))
                break;
        }
    }

	return pDomContentHandler->EndElement(*this);
}


tAttributeList::iterator DomElement::GetAttributeIterator( int nIndex )
{
    // The list::size() function is O(n) instead of O(1) and so we 
    // code this function like so instead of using list::size().

    tAttributeList::iterator it;
    int i = 0;

    #if UTFXML_USE_DOM_ATTRIBUTE_PTR
        // It's best if the user checks for (mpAttributeList != NULL) before calling this function.
        if(!mpAttributeList)
            mpAttributeList = CreateAttributeList();

        // We necessarily assume CreateAttributeList succeeds. 
        for(it = mpAttributeList->begin(); (i < nIndex) && (it != mpAttributeList->end()); ++it)
            ++i;
    #else
        for(it = mAttributeList.begin(); (i < nIndex) && (it != mAttributeList.end()); ++it)
            ++i;
    #endif

    return it;
}

tAttributeList::iterator DomElement::GetAttributeIterator( const char * psAttrName )
{
    tAttributeList::iterator it;

    #if UTFXML_USE_DOM_ATTRIBUTE_PTR
        // It's best if the user checks for (mpAttributeList != NULL) before calling this function.
        if(!mpAttributeList)
            mpAttributeList = CreateAttributeList();

        // We necessarily assume CreateAttributeList succeeds. 
        for(it = mpAttributeList->begin(); it != mpAttributeList->end(); ++it)
        {
            DomAttribute& domAttribute = *it;

            if(domAttribute.mName.comparei(psAttrName) == 0)
                break;
        }
    #else
        for(it = mAttributeList.begin(); it != mAttributeList.end(); ++it)
        {
            DomAttribute& domAttribute = *it;

            if(domAttribute.mName.comparei(psAttrName) == 0)
                break;
        }
    #endif

    return it;
}

DomAttribute* DomElement::GetAttribute( int nIndex )
{
    tAttributeList::iterator it = GetAttributeIterator(nIndex);

    #if UTFXML_USE_DOM_ATTRIBUTE_PTR
        if(it != mpAttributeList->end())
            return &*it;
    #else
        if(it != mAttributeList.end())
            return &*it;
    #endif

    return NULL;
}

DomAttribute* DomElement::GetAttribute( const char * psAttrName )
{
    tAttributeList::iterator it = GetAttributeIterator(psAttrName);

    #if UTFXML_USE_DOM_ATTRIBUTE_PTR
        if(it != mpAttributeList->end())
            return &*it;
    #else
        if(it != mAttributeList.end())
            return &*it;
    #endif

    return NULL;
}

const char * DomElement::GetAttributeValue( int nIndex ) const
{
    const DomAttribute* const pDomElement = const_cast<DomElement*>(this)->GetAttribute(nIndex);

    if(pDomElement)
        return pDomElement->mValue.c_str();
    return NULL;
}

const char * DomElement::GetAttributeValue( const char * psAttrName ) const
{
    const DomAttribute* const pDomElement = const_cast<DomElement*>(this)->GetAttribute(psAttrName);

    if(pDomElement)
        return pDomElement->mValue.c_str();
    return NULL;
}


#if UTFXML_USE_DOM_ATTRIBUTE_PTR

tAttributeList* DomElement::CreateAttributeList(tAttributeList* pCopy)
{
    tAttributeList* pAttributeList = NULL;
    ICoreAllocator* pAllocator     = DomNode::GetAllocator(); // Call parent class GetAllocator.

    if(pAllocator)
    {
        pAttributeList = (tAttributeList*)pAllocator->Alloc(sizeof(tAttributeList), UTFXML_ALLOC_PREFIX "DomAttributeList", 0);

        if(pAttributeList)
        {
            if(pCopy)
                pAttributeList = new(pAttributeList) tAttributeList(*pCopy);
            else
                pAttributeList = new(pAttributeList) tAttributeList;

            pAttributeList->set_allocator(CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomAttributeList", pAllocator));
        }
    }

    return pAttributeList;
}

void DomElement::DestroyAttributeList(tAttributeList* pAttributeList)
{
    if(pAttributeList)
    {
        pAttributeList->~tAttributeList();
        DomNode::GetAllocator()->Free(pAttributeList);
    }
}

#endif



///////////////////////////////////////////////////////////////////////////////
// DomComment
///////////////////////////////////////////////////////////////////////////////

DomNode* DomComment::Clone() const
{
    ICoreAllocator* const pAllocator = GetAllocator();
    void*           const pMemory    = pAllocator->Alloc(sizeof(DomComment), UTFXML_ALLOC_PREFIX "DomComment", 0);

    if(pMemory)
        return new(pMemory) DomComment(*this);
    return NULL;
}




///////////////////////////////////////////////////////////////////////////////
// DomCharacterData
///////////////////////////////////////////////////////////////////////////////

DomCharacterData::DomCharacterData(const char8_t * pText, size_t nTextLength, bool bCDATA, DomNode* pParent, ICoreAllocator * pAllocator)
  : DomNode(XmlReader::CharacterData, pText, nTextLength, pParent, pAllocator),
    mbCDATA(bCDATA),
    mbPreserveCDATA(true)
{
}

DomCharacterData::DomCharacterData( const DomCharacterData& domCharacterData )
  : DomNode(domCharacterData),
    mbCDATA(domCharacterData.mbCDATA),
    mbPreserveCDATA(domCharacterData.mbPreserveCDATA)
{
}

DomNode* DomCharacterData::Clone() const
{
    ICoreAllocator* const pAllocator = GetAllocator();
    void*           const pMemory    = pAllocator->Alloc(sizeof(DomCharacterData), UTFXML_ALLOC_PREFIX "DomCharacterData", 0);

    if(pMemory)
        return new(pMemory) DomCharacterData(*this);
    return NULL;
}




///////////////////////////////////////////////////////////////////////////////
// DomPrologue
///////////////////////////////////////////////////////////////////////////////

DomPrologue::DomPrologue(const char8_t * pVersion, const char8_t * pEncoding, 
                         const char8_t * pStandalone, DomNode* pParent, ICoreAllocator * pAllocator)
  : DomNode( XmlReader::Comment, "xml", 3, pParent, pAllocator ),
    mVersion   (pVersion,    CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomPrologue", pAllocator)),
    mEncoding  (pEncoding,   CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomPrologue", pAllocator)),
    mStandalone(pStandalone, CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomPrologue", pAllocator))
{
}

DomPrologue::DomPrologue( const DomPrologue& domPrologue )
  : DomNode( domPrologue ),
    mVersion(domPrologue.mVersion),
    mEncoding(domPrologue.mEncoding),
    mStandalone(domPrologue.mStandalone)
{
}

DomNode* DomPrologue::Clone() const
{
    ICoreAllocator* const pAllocator = GetAllocator();
    void*           const pMemory    = pAllocator->Alloc(sizeof(DomPrologue), UTFXML_ALLOC_PREFIX "DomPrologue", 0);

    if(pMemory)
        return new(pMemory) DomPrologue(*this);
    return NULL;
}




///////////////////////////////////////////////////////////////////////////////
// DomProcessingInstruction
///////////////////////////////////////////////////////////////////////////////

DomProcessingInstruction::DomProcessingInstruction(const char8_t * pName, const char8_t * pValue, 
                                                    DomNode* pParent, ICoreAllocator * pAllocator)
  : DomNode( XmlReader::ProcessingInstruction, pName, kLengthNull, pParent, pAllocator ),
    mValue(pValue, CoreAllocatorAdapter(UTFXML_ALLOC_PREFIX "DomProcessingInstruction", pAllocator))
{
}

DomProcessingInstruction::DomProcessingInstruction( const DomProcessingInstruction& domProcessingInstruction )
  : DomNode(domProcessingInstruction),
    mValue(domProcessingInstruction.mValue)
{
}

DomNode* DomProcessingInstruction::Clone() const
{
    ICoreAllocator* const pAllocator = GetAllocator();
    void*           const pMemory    = pAllocator->Alloc(sizeof(DomProcessingInstruction), UTFXML_ALLOC_PREFIX "DomProcessingInstruction", 0);

    if(pMemory)
        return new(pMemory) DomProcessingInstruction(*this);
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
// DomEntityRef
///////////////////////////////////////////////////////////////////////////////

DomNode* DomEntityRef::Clone() const
{
    ICoreAllocator* const pAllocator = GetAllocator();
    void*           const pMemory    = pAllocator->Alloc(sizeof(DomEntityRef), UTFXML_ALLOC_PREFIX "DomEntityRef", 0);

    if(pMemory)
        return new(pMemory) DomEntityRef(*this);
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
// DomDeclaration
///////////////////////////////////////////////////////////////////////////////

DomNode* DomDeclaration::Clone() const
{
    ICoreAllocator* const pAllocator = GetAllocator();
    void*           const pMemory    = pAllocator->Alloc(sizeof(DomDeclaration), UTFXML_ALLOC_PREFIX "DomDeclaration", 0);

    if(pMemory)
        return new(pMemory) DomDeclaration(*this);
    return NULL;
}




///////////////////////////////////////////////////////////////////////////////
// XmlDomReader
///////////////////////////////////////////////////////////////////////////////

bool XmlDomReader::Build(DomNode& domNode)
{
    bool            bSuccess = true;
    void*           pMemory = NULL;
    DomNode*        pCurrentParent = &domNode;
    ICoreAllocator* pAllocator = domNode.GetAllocator();

    if(!pAllocator) {
        pAllocator = mpCoreAllocator;
        domNode.SetAllocator(mpCoreAllocator);
    }

    while (bSuccess && Read()) {
        const XmlReader::NodeType nodeType = GetNodeType();

        switch (nodeType) {
            case Element: {
                pMemory = pAllocator->Alloc(sizeof(DomElement), UTFXML_ALLOC_PREFIX "DomElement", 0);

                if(pMemory) {
                    DomElement* const pDomElement = new(pMemory) DomElement(mpTokenValue, pCurrentParent, pAllocator);
                    pCurrentParent->mChildList.push_back(pDomElement);

                    for(eastl_size_t i = 0, iEnd = mAttributeArray.size(); i < iEnd; i += 2)
                    {
                        const char * const pName  = mAttributeArray[i + 0];
                        const char * const pValue = mAttributeArray[i + 1];
                        tAttributeList*    pAttributeList;

                        #if UTFXML_USE_DOM_ATTRIBUTE_PTR
                            if(!pDomElement->mpAttributeList)
                                pDomElement->mpAttributeList = pDomElement->CreateAttributeList();
                            pAttributeList = pDomElement->mpAttributeList;
                        #else
                            pAttributeList = &pDomElement->mAttributeList;
                        #endif

                        pAttributeList->push_back();
                        DomAttribute& domAttribute = pAttributeList->back();
                        domAttribute.SetAllocator(pAllocator);
                        domAttribute.mName  = pName;
                        domAttribute.mValue = pValue;
                    }

                    if(!IsEmptyElement())
                        pCurrentParent = pDomElement; 
                }

                break;
            }

            case EndElement: {
                if(pCurrentParent) {
                    pCurrentParent = pCurrentParent->mpParent;
                    EA_ASSERT(pCurrentParent); // If this fails then the user has mismatched elements.
                }
                else
                    pCurrentParent = &domNode; // This is an error fallback.
                break;
            }

            case CharacterData: {
                pMemory = pAllocator->Alloc(sizeof(DomCharacterData), UTFXML_ALLOC_PREFIX "DomCharacterData", 0);

                if(pMemory) {
                    DomCharacterData* const pDomCharacterData = new(pMemory) DomCharacterData(mpTokenValue, (size_t)mValueLength, false, pCurrentParent, pAllocator);
                    pCurrentParent->mChildList.push_back(pDomCharacterData);
                }
                break;
            }

            case Comment: {
                pMemory = pAllocator->Alloc(sizeof(DomComment), UTFXML_ALLOC_PREFIX "DomComment", 0);

                if(pMemory) {
                    DomComment* const pDomComment = new(pMemory) DomComment(mpTokenValue, (size_t)mValueLength, pCurrentParent, pAllocator);
                    pCurrentParent->mChildList.push_back(pDomComment);
                }
                break;
            }

            case ProcessingInstruction: {
                pMemory = pAllocator->Alloc(sizeof(DomProcessingInstruction), UTFXML_ALLOC_PREFIX "DomProcessingInstruction", 0);

                if(pMemory) {
                    DomProcessingInstruction* const pDomProcessingInstruction = new(pMemory) DomProcessingInstruction(mpTokenName, mpTokenValue, pCurrentParent, pAllocator);
                    pCurrentParent->mChildList.push_back(pDomProcessingInstruction);
                }
                break;
            }

            case EntityRef: {
                pMemory = pAllocator->Alloc(sizeof(DomEntityRef), UTFXML_ALLOC_PREFIX "DomEntityRef", 0);

                if(pMemory) {
                    DomEntityRef* const pDomEntityRef = new(pMemory) DomEntityRef(mpTokenName, pCurrentParent, pAllocator);
                    pCurrentParent->mChildList.push_back(pDomEntityRef);
                }
                break;
            }

            case Prologue: {
                pMemory = pAllocator->Alloc(sizeof(DomPrologue), UTFXML_ALLOC_PREFIX "DomPrologue", 0);

                if(pMemory) {
                    // XmlReader doesn't provide the prologue information at this time.
                    // We need to rectify this if the user wants to use the prologue information.
                    // Currently we fake a prologue here but the values could be wrong.
                    DomPrologue* const pDomPrologue = new(pMemory) DomPrologue("version=\"1.0\"", "encoding=\"UTF-8\"", "standalone=\"yes\"", pCurrentParent, pAllocator);
                    pCurrentParent->mChildList.push_back(pDomPrologue);
                }
                break;
            }

            case DocTypeDecl:
            case EntityDecl:
            case ElementDecl:
            case AttListDecl:
            case NotationDecl: {
                pMemory = pAllocator->Alloc(sizeof(DomDeclaration), UTFXML_ALLOC_PREFIX "DomDeclaration", 0);

                if(pMemory) {
                    DomDeclaration* const pDomDeclaration = new(pMemory) DomDeclaration(nodeType, mpTokenName, pCurrentParent, pAllocator);
                    pCurrentParent->mChildList.push_back(pDomDeclaration);
                }
            }

            case None:
            default:
                EA_FAIL_MESSAGE("XmlDomReader::Build: Unexpected node type.");
                break;
        }

        bSuccess = (pMemory != NULL);
    }

    return bSuccess && (mResultCode == kSuccess);
}


} // namespace XML
} // namespace EA















