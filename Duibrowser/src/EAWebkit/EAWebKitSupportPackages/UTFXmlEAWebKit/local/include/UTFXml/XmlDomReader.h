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
// Name: XmlDomReader.h
// 
// Copyright (c) 2006 Electronic Arts Inc.
// Created by: Paul Pedriana
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements a DOM reader which is similar in approach to the
// design of the TinyXML open source package. Since XmlDomReader builds upon
// our existing XmlReader and upon EASTL, the implemenation here is significantly
// simpler than that of TinyXML yet has the capability of being more flexible.
//
// This class can be used in shipping applications or it can be used for internal
// utility applications. Most of the time DOM functionality is something that isn't
// used in shipping applications of the type we make. As a result, XmlDomReader 
// strives to be efficient enough for game usage but doesn't go out of its way
// to be as lean as possible. A particular design characteristic of this class 
// that pertains to this design decision is that XmlDomReader builds the DOM into
// memory that it allocates as it goes. XmlDomReader does not work by making pointers
// into the original file data or XmlReader data.
//
// Memory allocation by XmlDomReader is linear. By this we mean that as the DOM is 
// built, memory is allocated only for the DOM data. No memory is allocated for 
// anything else and no memory is freed during the process. Thus you could provide
// a memory allocation that is a stack allocator (a.k.a. linear allocator) which 
// works by simply moving a pointer forward in a contiguous block of memory.
//
// In order to avoiding bloating this file we have avoided some of the TinyXML 
// functionality that is not frequently used. Feel free to contact the maintainer
// of this package if you feel that some functionality is missing.
//
// Features we may or may not want to add include.
//    - Ability to assign user data (void*) to nodes/attributes.
//    - float/int/double attribute reader/writer convenience functions.
//    - Algorithm functors for DomNodes such as node_less, node_equal_to, node_equal_to_string
//    - QueryInterface function for sub-nodes. TinyXML has it but in a cumbersome way with individual functions.
///////////////////////////////////////////////////////////////////////////////


#ifndef UTFXML_XMLDOMREADER_H
#define UTFXML_XMLDOMREADER_H


#include <UTFXml/XmlReader.h>
#include <UTFXml/internal/core_allocator_adapter.h>
#include <EASTL/list.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>
#include <new>


#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 4251) // 'x' needs to have dll-interface to be used by clients of class 'y'.
#endif


///////////////////////////////////////////////////////////////////////////////
// UTFXML_USE_DOM_ATTRIBUTE_PTR
//
// Defines as 0 or 1. 1 is the default.
// If enabled, then DomDocument attributes are held in a node mpAttributeList 
// (which may be NULL if empty) as opposed to being held in mAttributeList
// (which is always present). It turns out that the mAttributeList was using 
// more memory than desirable in the case of attribute-less nodes, and there  
// are typically many empty nodes in an XML document.
// This config option exists for allowing backward compatibility while migrating
// from using mAttributeList to mpAttributeList.
// This option is intended to be temporary while users transition from the
// previous system to the new system.
//
#ifndef UTFXML_USE_DOM_ATTRIBUTE_PTR
    #define UTFXML_USE_DOM_ATTRIBUTE_PTR 1
#endif


namespace EA {

    namespace IO {
        class IStream;
    }

    namespace XML {

        // Forward declarations
        class DomAttribute;
        class DomNode;
        class DomDocument;
        class DomElement;
        class DomComment;
        class DomCharacterData;
        class DomPrologue;
        class DomProcessingInstruction;
        class DomEntityRef;
        class DomDeclaration;
        class XmlDomReader;
        class IDomContentHandler;


        // Typedefs
        typedef eastl::basic_string<char8_t,  CoreAllocatorAdapter> tString8;
        typedef eastl::basic_string<char16_t, CoreAllocatorAdapter> tString16;
        typedef eastl::list<DomAttribute,     CoreAllocatorAdapter> tAttributeList;
        typedef eastl::list<DomNode*,         CoreAllocatorAdapter> tNodeList;


        /// kLengthNull
        /// Defines a value to be used for string conversion functions which means
        /// that the string length is specified by a wherever the terminating null
        /// character is. For the copying or converting of strings, the terminating
        /// null character is also copied to the destination.
        const size_t kLengthNull = (size_t)-1;



        //////////////////////////////////////////////////////////////////////////
        /// IDomContentHandler
        ///
        /// This is an interface for doing a SAX-like callback iteration of 
        /// the DOM. It is like the IContentHandler we have in XmlCallbackReader
        /// except that it is for a DOM instead of a dynamically read document
        /// and thus the callbacks give you pointers directly to DomNodes and 
        /// DomAttributes. 
        ///
        /// If you return true from a IDomContentHandler function, recursive 
        /// parsing will continue. If you return false, no children of the 
        /// current node or its siblings will be iterated.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API IDomContentHandler
        {
        public:
            virtual ~IDomContentHandler() {}

            virtual bool StartDocument( DomDocument& domDocument );
            virtual bool EndDocument  ( DomDocument& domDocument );

            virtual bool StartElement( DomElement& domElement );
            virtual bool EndElement  ( DomElement& domElement );

            virtual bool Characters           ( DomCharacterData& domCharacterData );
            virtual bool Comment              ( DomComment& domComment );
            virtual bool ProcessingInstruction( DomProcessingInstruction& domProcessingInstruction );
            virtual bool SkippedEntity        ( DomEntityRef& domEntityRef );
            virtual bool Prologue             ( DomPrologue& domPrologue );
            virtual bool Declaration          ( DomDeclaration& domDeclaration );
        };



        //////////////////////////////////////////////////////////////////////////
        /// DomAttribute
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomAttribute
        {
        public:
            DomAttribute( ICoreAllocator * pAllocator = NULL );
            DomAttribute( const char8_t * pName, const char8_t * pValue, ICoreAllocator * pAllocator = NULL );
            DomAttribute( const DomAttribute& attribute );
            DomAttribute& operator=( const DomAttribute& attribute );

            ICoreAllocator* GetAllocator() const;
            void SetAllocator(ICoreAllocator* pAllocator);

        public:
            tString8 mName;
            tString8 mValue;
        };




        //////////////////////////////////////////////////////////////////////////
        /// Base node type for the DOM tree
        ///
        /// You must supply a valid non-null allocator when doing anything but 
        /// default constructing an instance of this class.
        /// To consider: Add child and sibling node finding functionality.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomNode
        {
        public:
            DomNode( XmlReader::NodeType nodeType = XmlReader::None, 
                     const char8_t * pName = NULL, size_t nNameLength = 0, 
                     DomNode* pParent = NULL, ICoreAllocator * pAllocator = NULL );
            DomNode( const DomNode& node );
            DomNode& operator=( const DomNode& node );

            virtual ~DomNode();

            ICoreAllocator* GetAllocator() const;
            void SetAllocator(ICoreAllocator* pAllocator);

            /// A destroy operation is the same thing as calling the object's 
            /// destructor and freeing its memory via the allocator associated
            /// with it. It is essentially the same thing as a custom member 
            /// operator delete. To consider: operator delete may be sufficient.
            void Destroy();

            /// A clone operation is the same thing as creating a new object and 
            /// copy-constructing it from this object. This is not the same thing
            /// as a custom member operator new because this function can be used
            /// to clone a subclass via a base class pointer.
            virtual DomNode* Clone() const = 0;

            /// Clears the contents of the node, leaving the node with a state of
            /// having no child nodes and no attributes. The contents will be 
            /// recursively cleared and destroyed. No nodes that may be above this 
            /// node will be affected.
            virtual void Clear();

            /// Gets the node name.
            /// We follow the Microsoft convention of naming. Some of the node types
            /// have a name that is a constant string (e.g. "#comment").
            ///
            ///  Node Type               Meaning of Name
            ///  ---------------------------------------------------------------------
            ///  AttListDecl             The declaration text.
            ///  CharacterData           #text
            ///  Comment                 #comment
            ///  Document                #document
            ///  DocTypeDecl             The document type name.
            ///  Element                 The element name (see example below).
            ///  EntityDecl              The name of the entity.
            ///  EntityRef               The name of the entity referenced.
            ///  NotationDecl            #xml-declaration
            ///  ProcessingInstruction   The target of the processing instruction.
            ///  Prologue                #prologue
            /// 
            /// For example, with the following XML:
            ///     <html>Hello</html>
            /// The Element node name is Hello.
            ///
            virtual const tString8& GetName();

            /// Gets the node value. 
            /// Returns a reference to the value. This reference is valid only for the 
            /// lifetime of the node.
            ///
            ///  Node Type               Meaning of Value
            ///  ---------------------------------------------------------------------
            ///  AttListDecl             The value of the attribute.
            ///  CharacterData           The content of the text or CDATA.
            ///  Comment                 The content of the comment.
            ///  Document                Always empty.
            ///  DocTypeDecl             Always empty.
            ///  Element                 Always empty. You can use the InnerText or InnerXml properties to access the string text within an element node.
            ///  EntityDecl              Always empty.
            ///  EntityRef               Always empty.
            ///  NotationDecl            Always empty.
            ///  ProcessingInstruction   The entire content of the processing instruction.
            ///  Prologue                The content of the declaration (that is, everything between <?xml and ?>).
            ///
            /// For example, with the following XML:
            ///     <p>A<b>B<I> C D </I>E</b>F</p>
            /// the <I> node has an value of:
            ///     " C D "
            ///
            virtual const tString8& GetValue();

            /// Returns the "inner text" of the node. The inner text is defined as
            /// the concatenation of the Value properties of child nodes. This 
            /// concatenation is done via depth-first recursion and thus the text 
            /// will be in the same reading order as you would see it in a page
            /// of XML. For example, with the following XML:
            ///     <p>A<b>B<I> C D </I>E</b>F</p>
            /// the <p> node has an inner text of:
            ///     "AB C D EF"
            ///
            virtual void GetInnerText(tString8& sInnerText);

            /// Iterate the contents of this node to the callback handler.
            /// This is also known as Accept in terms of the hierarchical 
            /// visitor pattern.
            ///
            /// If you return true from a IDomContentHandler function, recursive 
            /// parsing will continue. If you return false, no children of the 
            /// current node or its sibilings will be iterated.
            ///
            /// The pDomContentHandler argument must be non-NULL.
            /// The user-specified IDomContentHandler may modify the child nodes
            /// of the currently iterated node in any way. The handler may modify
            /// the sibling and parent nodes of the currently iterated node only
            /// in ways that don't modify the tree structure.
            //////////////////////////////////////////////////////////////////////////
            virtual bool Iterate(IDomContentHandler* pDomContentHandler) = 0;

            /// Get the top-most node in the hierarchy to which this node belongs.
            /// This function essentially walks the mpParent values upward.
            /// If the current node has no parent, the current node is returned.
            DomNode* GetTop();

            /// Get a child node based on its index.
            /// Returns mChildList.end if the index is out of range.
            tNodeList::iterator GetNodeIterator(int nIndex);

            /// Get a node based on its name.
            /// The name is matched in a case-insensitive way.
            /// Returns mChildList.end if the name is not found.
            tNodeList::iterator GetNodeIterator(const char8_t* pName);

            /// Find a node recursively based on its name.
            /// The name is matched in a case-insensitive way.
            /// Returns mChildList.end if the name is not found.
            // DomNode* FindNode(const char8_t* psName, bool bBreadthFirst);

            // Accessors
            DomDocument*              AsDomDocument();
            DomElement*               AsDomElement();
            DomComment*               AsDomComment();
            DomCharacterData*         AsDomCharacterData();
            DomPrologue*              AsDomPrologue();
            DomProcessingInstruction* AsDomProcessingInstruction();
            DomEntityRef*             AsDomEntityRef();
            DomDeclaration*           AsDomDeclaration();

        protected:
            bool CopyChildList(const DomNode& node);

        public:
            XmlReader::NodeType mNodeType;  // This can be any XmlReader::NodeType except EndElement.
            tString8            mName;      // For DomElement nodes, this stores the node name (e.g. "Hello" in <Hello> blah </Hello>. For other nodes, it stores their value. 
            DomNode*            mpParent;
            tNodeList           mChildList;
        };


        //////////////////////////////////////////////////////////////////////////
        /// Document node
        ///
        /// The document node is a node that represents the document itself.
        /// The node name is the file name or path associated with the document,
        /// though it can be any moniker that identifies the DOM as a whole.
        ///
        /// To consider: add functionality to store information about how to 
        /// write this document to disk (e.g. tab style). Alternatively this 
        /// can be done at a higher level by the XmlDomWriter.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomDocument : public DomNode
        {
        public:
            DomDocument(const char8_t * pName = NULL, ICoreAllocator * pAllocator = NULL);

            DomNode* Clone() const;

            bool Iterate(IDomContentHandler* pDomContentHandler);
        };



        //////////////////////////////////////////////////////////////////////////
        /// Element node
        ///
        /// This corresponds to the XmlReader::Element (and EndElement) node type.
        /// The element is a container class. It has a name and can contain other 
        /// elements, character data, comments, and misc nodes. Elements also 
        /// contain zero or more attributes (DomAttribute / tAttributeList).
        ///
        /// We provide some basic inline functions to work with Attributes, but you
        /// can access the attributes more freely by directly working with the list.
        ///
        /// To consider: Perhaps we want to have both const and non-const accessors.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomElement : public DomNode
        {
        public:
            DomElement(const char8_t* pName = NULL, DomNode* pParent = NULL, ICoreAllocator* pAllocator = NULL);
            DomElement( const DomElement& domElement );
           ~DomElement( );

            DomNode* Clone() const;
            void Clear();

            bool Iterate(IDomContentHandler* pDomContentHandler);

            // Returns true if any attributes exist. If no attributes exist, 
            // then mpAttributeList is NULL. 
            bool AttributesExist() const;

            // GetAttributeIterator returns mpAttributeList->end()/mAttributeList.end() if the attribute doesn't exist.
            // This function creates mpAttributeList if not created already, even if it is empty. This is so that
            // it can return a valid iterator. It is best to call AttributesExist before possibly using this function.
            // To consider: Move these Attribute functions to an/the AttributeList class. 
            tAttributeList::iterator GetAttributeIterator( int nIndex );
            tAttributeList::iterator GetAttributeIterator( const char8_t* psAttrName );

            // GetAttribute returns NULL if the attribute doesn't exist.
            // To consider: Remove these Attribute functions, as they are essentially 
            // just convenience wrappers around GetAttributeIterator.
            DomAttribute* GetAttribute( int nIndex );
            DomAttribute* GetAttribute( const char8_t* psAttrName );

            // GetAttributeValue returns NULL if the attribute doesn't exist.
            const char8_t* GetAttributeValue( int nIndex ) const;
            const char8_t* GetAttributeValue( const char8_t* psAttrName ) const;

        public:
            #if UTFXML_USE_DOM_ATTRIBUTE_PTR
                tAttributeList* mpAttributeList;
            #else
                tAttributeList  mAttributeList;
            #endif

        protected:
            #if UTFXML_USE_DOM_ATTRIBUTE_PTR
                friend class XmlDomReader;

                tAttributeList* CreateAttributeList(tAttributeList* pCopy = NULL);
                void            DestroyAttributeList(tAttributeList* pAttributeList);
            #endif
        };



        //////////////////////////////////////////////////////////////////////////
        /// Comment node
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomComment : public DomNode
        {
        public:
            DomComment(const char8_t* pText = NULL, size_t nTextLength = 0, DomNode* pParent = NULL, ICoreAllocator* pAllocator = NULL);

            DomNode* Clone() const;

            bool Iterate(IDomContentHandler* pDomContentHandler);
        };



        //////////////////////////////////////////////////////////////////////////
        /// CharacterData node
        ///
        /// Text can be implemented in an XML file in one of two ways: 
        ///     normal text
        ///     CDATA text
        /// There is no symantic difference between these in an XML file;
        /// CDATA text is merely a way to encode literal text in a more convenient
        /// way than normal text. Users usually should treat CDATA text the same
        /// as normal text when reading character data from an XML file. 
        /// The distinction between these two types is significant to us here 
        /// mostly with respect to writing out a DOM to a file and how much we
        /// want to preserve the original formatting.
        ///
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomCharacterData : public DomNode
        {
        public:
            DomCharacterData(const char8_t * pText = NULL, size_t nTextLength = 0, bool bCDATA = false,
                             DomNode* pParent = NULL, ICoreAllocator * pAllocator = NULL);
            DomCharacterData( const DomCharacterData& domCharacterData );

            DomNode* Clone() const;

            bool Iterate(IDomContentHandler* pDomContentHandler);

        public:
            bool mbCDATA;           /// If true, this data originated as CDATA in the XML file.
            bool mbPreserveCDATA;   /// If true, then we should write out this data as CDATA when writing to files.
        };



        //////////////////////////////////////////////////////////////////////////
        /// Prologue node
        ///
        /// This corresponds to the XmlReader::Prologue node type.
        /// If a prologue node is present in an XML file, it must be the first
        /// node. A prologue is a specialized processing instruction.
        /// Example prologue node in an XML file:
        ///     <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomPrologue : public DomNode
        {
        public:
            DomPrologue(const char8_t * pVersion = NULL,
                        const char8_t * pEncoding = NULL, 
                        const char8_t * pStandalone = NULL, 
                        DomNode* pParent = NULL, ICoreAllocator * pAllocator = NULL);
            DomPrologue( const DomPrologue& domPrologue );

            DomNode* Clone() const;

            bool Iterate(IDomContentHandler* pDomContentHandler);

        public:
            tString8 mVersion;
            tString8 mEncoding;
            tString8 mStandalone;
        };



        //////////////////////////////////////////////////////////////////////////
        /// Processing instruction node
        ///
        /// This is a generic processing instruction node, which covers nodes
        /// other than the specialized prologue processing instruction node.
        /// It corresponds to the XmlReader::ProcessingInstruction node type.
        /// Example processing instruction node in an XML file:
        ///     <?xml-stylesheet href="standardstyle.css" title="Standard Stylesheet" type="text/css"?>
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomProcessingInstruction : public DomNode
        {
        public:
            // Currently we support tracking just the instruction name (e.g. "xml")
            // and the property list as a single string, as that's our base XmlReader 
            // supports at this time.
            DomProcessingInstruction(const char8_t * pName = NULL, const char8_t * pValue = NULL, 
                                     DomNode* pParent = NULL, ICoreAllocator * pAllocator = NULL);
            DomProcessingInstruction( const DomProcessingInstruction& domProcessingInstruction );

            DomNode* Clone() const;

            bool Iterate(IDomContentHandler* pDomContentHandler);

        public:
            tString8 mValue; // Ideally this should be storing a DomAttribute set, but we are currently stuck with XmlReader's limitations.
        };



        //////////////////////////////////////////////////////////////////////////
        /// EntityRef node
        ///
        /// This stores the contents of an entity reference that was not resolvable.
        /// It corresponds to the XmlReader::EntityRef node type.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomEntityRef : public DomNode
        {
        public:
            // Currently we support just the entire node contents as text,
            // as that's our base XmlReader supports at this time.
            DomEntityRef(const char8_t * pText = NULL, DomNode* pParent = NULL, ICoreAllocator * pAllocator = NULL);

            DomNode* Clone() const;

            bool Iterate(IDomContentHandler* pDomContentHandler);
        };



        //////////////////////////////////////////////////////////////////////////
        /// Declaration node
        ///
        /// This corresponds to any of the XmlReader::xxxxDecl node types, such as
        /// DocTypeDecl, EntityDecl, ElementDecl, AttListDecl, NotationDecl.
        /// Example declaration node in an XML file:
        ///     <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API DomDeclaration : public DomNode
        {
        public:
            // Currently we support just the entire node contents as text,
            // as that's our base XmlReader supports at this time.
            DomDeclaration(XmlReader::NodeType nodeType = XmlReader::None, 
                           const char8_t * pText = NULL, DomNode* pParent = NULL, 
                           ICoreAllocator * pAllocator = NULL);

            DomNode* Clone() const;

            bool Iterate(IDomContentHandler* pDomContentHandler);
        };



        //////////////////////////////////////////////////////////////////////////
        /// A DOM building reader
        ///
        /// We implement a DOM (Document Object Model) reader via inheriting from
        /// an iterative XmlReader and via EASTL containers. Since the tree is 
        /// implemented with EASTL containers, most of the functionality needed
        /// to iterate and manipulate the nodes and their contents can be done
        /// via standard STL functionality. This results in the code here being
        /// much simpler than the equivalent code you'll find in things like
        /// the TinyXML package.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API XmlDomReader : public XmlReader
        {
        public:
            /// Default constructor
            /// The pAllocator is the allocator used for the XmlReader, which may
            /// be different from the allocator the user chooses to associated with
            /// the DOM that is build with the Build function.
            XmlDomReader( ICoreAllocator * pAllocator = NULL, size_t bufferBlockSize = 0 );

            /// Build the DOM. 
            /// It is expected that you have called XmlReader base functions to 
            /// set up the class instance before doing this Parse.
            /// The domNode parameter may be any type of DomNode, but typically the
            /// user would provide a DomDocument node.
            /// The DOM is built into the domNode with the ICoreAllocator that is 
            /// associated with the domNode. If domNode's allocator is NULL then
            /// the allocator of this XmlDomReader class will be used.
            bool Build(DomNode& domNode);
        };


    } // namespace XML

} // namespace EA





//////////////////////////////////////////////////////////////////////////
// Inlines
//////////////////////////////////////////////////////////////////////////

namespace EA {
    namespace XML {

        // CreateNode
        //
        // Example usage:
        //     DomNode* pParentNode;
        //     ...
        //     DomElement* pElement = CreateNode<DomElement>(pParentNode->GetAllocator());
        //     pParentNode->mChildList.push_back(pElement);
        //
        // template <typename NodeType>
        // NodeType* CreateNode(ICoreAllocator* pAllocator, const char8_t* pAllocatorName = "UTFXml/DomNode")
        // {
        //     void* const pMemory = pAllocator->Alloc(sizeof(NodeType), pAllocatorName, 0);
        //     if(pMemory)
        //         return new(pMemory) NodeType(*pNode);
        //     return NULL;
        // }


        inline ICoreAllocator* DomAttribute::GetAllocator() const
        {
            // It turns out that as of this writing, EASTL containers get_allocator function is 
            // not a const function, so we currently need to resort to const_cast.
            return const_cast<tString8&>(mName).get_allocator().get_allocator();
        }


        inline DomNode::~DomNode()
        {
            Clear();
        }

        inline ICoreAllocator* DomNode::GetAllocator() const
        {
            // It turns out that as of this writing, EASTL containers get_allocator function is 
            // not a const function, so we currently need to resort to const_cast.
            return const_cast<tString8&>(mName).get_allocator().get_allocator();
        }


        inline DomDocument::DomDocument(const char8_t* pName, ICoreAllocator* pAllocator)
          : DomNode( XmlReader::Document, pName, kLengthNull, NULL, pAllocator)
        {
        }


        inline DomElement::~DomElement()
        {
            #if UTFXML_USE_DOM_ATTRIBUTE_PTR
                DestroyAttributeList(mpAttributeList);
            #endif
        }

        inline bool DomElement::AttributesExist() const
        {
            #if UTFXML_USE_DOM_ATTRIBUTE_PTR
                return (mpAttributeList && !mpAttributeList->empty());
            #else
                return !mAttributeList.empty();
            #endif
        }


        inline DomComment::DomComment(const char8_t* pText, size_t nTextLength, DomNode* pParent, ICoreAllocator* pAllocator)
          : DomNode( XmlReader::Comment, pText, nTextLength, pParent, pAllocator )
        {
        }

        inline bool DomComment::Iterate(IDomContentHandler* pDomContentHandler)
        {
	        return pDomContentHandler->Comment(*this);
        }


        inline bool DomCharacterData::Iterate(IDomContentHandler* pDomContentHandler)
        {
	        return pDomContentHandler->Characters(*this);
        }


        inline bool DomPrologue::Iterate(IDomContentHandler* pDomContentHandler)
        {
	        return pDomContentHandler->Prologue(*this);
        }


        inline bool DomProcessingInstruction::Iterate(IDomContentHandler* pDomContentHandler)
        {
	        return pDomContentHandler->ProcessingInstruction(*this);
        }


        inline DomEntityRef::DomEntityRef(const char8_t * pText, DomNode* pParent, ICoreAllocator * pAllocator)
          : DomNode( XmlReader::EntityRef, pText, kLengthNull, pParent, pAllocator )
        {
        }

        inline bool DomEntityRef::Iterate(IDomContentHandler* pDomContentHandler)
        {
	        return pDomContentHandler->SkippedEntity(*this);
        }


        inline DomDeclaration::DomDeclaration(XmlReader::NodeType nodeType, const char8_t * pText, 
                                       DomNode* pParent, ICoreAllocator * pAllocator)
          : DomNode( nodeType, pText, kLengthNull, pParent, pAllocator )
        {
        }

        inline bool DomDeclaration::Iterate(IDomContentHandler* pDomContentHandler)
        {
	        return pDomContentHandler->Declaration(*this);
        }


        inline XmlDomReader::XmlDomReader( Allocator::ICoreAllocator * pAllocator, size_t bufferBlockSize ) 
          : XmlReader( pAllocator, bufferBlockSize )
        {
        }
    }
}


#ifdef _MSC_VER
    #pragma warning(pop)
#endif


#endif // Header include guard








