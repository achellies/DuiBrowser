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

// EAWebKitUtfXmlInterface.h
// 6/19/09 CSidhall - Abstract Interface Wrapper for UTFXml package 
// Note: Some enums have been copied from UTFXml
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_UTFXML_INTERFACE_H
#define EAWEBKIT_UTFXML_INTERFACE_H

#include <EABase/EABase.h>



namespace EA
{
    namespace Internal
    {
        namespace XML
        {
            // Avoiding to include umachine.h here so define UChar
            #if U_SIZEOF_WCHAR_T==2
                typedef wchar_t XML_UChar;
            #else
                typedef uint16_t XML_UChar;
            #endif
           

            /// Error codes
            enum ResultCode {
                kSuccess,

                kErrorBase = 0x2a7c0000,        /// Base for errors
                kErrorInternal,                 /// Shouldn't happen

                // File IO errors
                kErrorRead,

                // Errors which are reported by this reader
                kErrorNoMemory,
                kErrorUnknownEncoding,
                kErrorIncorrectEncoding,
                kErrorPartialChar,
                kErrorRecursionDepthExceeded,
                kErrorSyntax,
                kErrorInvalidToken,
                kErrorUnclosedToken,
                kErrorUndefinedEntity,
                kErrorBadCharRef,
                kErrorMissingSemicolon,
                kErrorCantDefineEntitiesOnceParsing,
                kErrorIncompletePI,

                // Reserved for future use
                // kErrorNoElements,
                // kErrorTagMismatch,
                // kErrorDuplicatAttribute,
                // kErrorJunkAfterDocElement,
                // kErrorParamEntityRef,
                // kErrorRecursiveEntityRef,
                // kErrorBinaryEntityRef,
                // kErrorAttributeExternalEntityRef,
                // kErrorMisplacedXmlPi,
                // kErrorExternalEntityHandling,
                // kErrorEntityDeclaredInPe,
                // kErrorUnboundPrefix,
                // kErrorUndeclaringPrefix,
                // kErrorXmlDecl,
                // kErrorTextDecl,

                // Reserved for application use. The application can define error values starting at this value.
                kErrorUser = 0x1000
            };


           enum ReadEncoding {
                kReadEncodingUnknown,   /// The XmlReader will attempt to auto-detect encodings of this type. It's best if such files have leading encoding bytes.
                kReadEncodingUTF8,
                kReadEncodingUTF16LE,
                kReadEncodingUTF16BE,
              #ifdef EA_SYSTEM_LITTLE_ENDIAN
                kReadEncodingUTF16 = kReadEncodingUTF16LE,
              #else
                kReadEncodingUTF16 = kReadEncodingUTF16BE,
              #endif
        };

            /// XML Node types.
            enum NodeType {
                None,                       /// Either error or end of stream
                Document,                   /// The document itself, not the same as the !DOCTYPE declation element (below).

                // Content Nodes
                Element,                    /// Start element, possibly an 'empty' element. e.g. <AutoList>
                EndElement,                 /// End element. e.g. </AutoList>
                Comment,                    /// XML comment. e.g. <!-- this is a comment -->
                CharacterData,              /// Character data.
                ProcessingInstruction,      /// XML processing instruction e.g. <?xml-stylesheet href="standardstyle.css" title="Standard Stylesheet" type="text/css"?>
                Prologue,                   /// XML prologue (actually a special processing instruction). e.g. <?xml version="1.0" encoding="UTF-8" ?>
                EntityRef,                  /// An unresolved entity reference.

                // Declaration Nodes
                DocTypeDecl,                /// !DOCTYPE declaration. e.g. <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
                EntityDecl,                 /// !ENTITY declaration. e.g. <!ENTITY % final 'IGNORE' >
                ElementDecl,                /// !ELEMENT declaration. e.g. <!ELEMENT book (comments*, title, body, supplements?)>
                AttListDecl,                /// !ATTLIST declaration. e.g. <!ATTLIST pre xml:space (preserve) #FIXED 'preserve'>
                NotationDecl                /// !NOTATION declaration.
            };

        class IXmlReader;

//        typedef bool entity_lookup_t( IXmlReader * pReader, const char * pEntityName, void * pContext );
        typedef XML_UChar decode_entity_named(const char * name);  // User provided function

        class IXmlReader
        {
        public:           
            virtual ~IXmlReader(){}

            virtual void SetFlag( uint32_t nFlag, bool bSet ) = 0;
//            virtual void SetEntityRefResolver( entity_lookup_t * pEntityLookupFunc, void * pContext ) =0;
            virtual  void SetEntityNamedDecoder(decode_entity_named* pFunc) =0;       
            virtual void AddNamespaceAlias( const char * pAliasName, const char * pNamespace, bool bCopy ) =0;
            virtual void PushInputBuffer(const void * pBuffer, size_t nBufferLen, ReadEncoding encoding = kReadEncodingUTF8, 
                const char *pStreamURI = NULL,    bool bCopy = false ) = 0;
            virtual void AppendInputBuffer(const void * pBuffer,size_t nBufferLen, ReadEncoding encoding = kReadEncodingUTF8,
                const char *pStreamURI = NULL, bool bCopy = false) = 0;       
            virtual ResultCode GetResultCode() =0;
            virtual int32_t GetLineNumber() const =0; 
            virtual int32_t GetColumnNumber() const =0; 
            virtual NodeType GetNodeType() const =0; 
            virtual bool Read() =0;
            virtual bool IsEmptyElement() const =0;
            virtual bool GetIsCData() const =0;
            virtual const char *GetName() const = 0;
            virtual bool GetIsStandalone() const =0;
            virtual const char *GetNamespaceURI() const =0;
            virtual size_t GetNamePrefixLength() const = 0;
            virtual const char *GetValue() const = 0;
            virtual const char *GetDocTypeName() const =0;
            virtual const char *GetPublicId() const =0;
            virtual const char *GetSystemId() const =0;
            virtual const char *GetElementNamespace(int32_t index, const char** pNamespace = NULL) const =0;
            virtual int32_t GetElementNamespaceCount() const = 0;
            virtual int32_t GetAttributeCount() const =0;
            virtual const char *GetAttributeName( int nIndex ) const = 0;
            virtual const char *GetAttributeLocalName( int nIndex ) const = 0;
            virtual const char *GetAttributeValue( int nIndex ) const = 0;
        };
        
        }    
    }    
} // Namespace EA
#endif // Header include guard

