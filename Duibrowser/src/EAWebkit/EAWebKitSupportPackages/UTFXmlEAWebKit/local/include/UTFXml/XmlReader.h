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
// Name: XmlReader.h
// 
// Purpose: Classes for reading in hierarchical data structures from XML files
//
// Copyright (c) 2006 Electronic Arts Inc.
// Created by: Talin. Further developed and maintained by Paul Pedriana.
///////////////////////////////////////////////////////////////////////////////

#ifndef UTFXML_XMLREADER_H
#define UTFXML_XMLREADER_H

#include <EASTL/vector.h>
#include <EASTL/hash_map.h>
#include <UTFXml/internal/Config.h>
#include <UTFXml/XmlEncoding.h>
#include <UTFXml/XmlTokenBuffer.h>
#include <UTFXml/internal/core_allocator_adapter.h>


#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4251) // 'x' needs to have dll-interface to be used by clients of class 'y'.
#endif


namespace EA {

    // Forward declarations
    namespace IO {
        class IStream;
    }

    namespace XML {

        typedef Allocator::ICoreAllocator                      ICoreAllocator;
        typedef Internal::CoreAllocatorAdapter<ICoreAllocator> CoreAllocatorAdapter;


        //////////////////////////////////////////////////////////////////////////
        /// XmlReader implements a stream-based XML parser (a.k.a. StAX), modeled 
        /// after the .Net XmlReader class.
        ///
        /// Differences from the .Net version:
        ///    - Individual attributes are not nodes (you can't move to an attribute)
        ///    - Whitespace is not parsed (its just part of the text.)
        ///    - Currently does not support xml:lang
        ///    - A number of other API functions not supported
        ///
        /// Be careful about using Skip() followed by a Read(), as it may not do 
        /// what you expect it to do. There are a lot of code examples for 
        /// using the .Net XMLReader class which have subtle bugs because of this. 
        /// Skip will position you at the next sibling of the current node.
        /// If you subsequently call Read, it will skip over the sibling as well, 
        /// which may not be what you wanted. See Skip for more details.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API XmlReader {
        public:
            /// Typedef representing a character string whose encoding type is unknown
            typedef intptr_t ssize_t; // To do: Switch this from intptr_t to EASTL::ssize_t after EASTL 1.04.00 becomes widely available.

            //////////////////////////////////////////////////////////////////////////
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

            //////////////////////////////////////////////////////////////////////////
            /// Parsing flags
            enum Flags {
                kIncludeComments              = (1<<0),   /// Don't filter out comments.
                kIncludeProcessingInstructions= (1<<1),   /// Don't filter out processing instructions.
                kIncludeDocType               = (1<<2),   /// Don't filter out doctype declarations.
                kIncludePrologue              = (1<<3),   /// Don't filter out the XML prologue.
                kIncludeDeclarations          = (1<<4),   /// Don't filter out ATTLIST, ELEMENT, NOTATION.
                kIncludeNamespaces            = (1<<5),   /// Include namespace information.
                kIncludeUnresolvedEntityRef   = (1<<6),   /// Return unresolved entity refs (error otherwise).

              //To do: Implement the following
              //kIncludeIgnoredWhitespace     = (1<<7),   /// Don't filter out ignored whitespace.
              //kRetainEncoding               = (1<<8),   /// Leave strings in decoded form; don't decode them.
              //kRetainParentState            = (1<<9),   /// Retain tokens for parent elements of the current element.

                kFlagsDefault = 0
            };

            //////////////////////////////////////////////////////////////////////////
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

            //////////////////////////////////////////////////////////////////////////
            /// Entity resolver callback function type. If the entity is recognized, the callback
            /// should respond by pushing a new input stream on the reader.
            typedef bool entity_lookup_t( XmlReader * pReader, const char * pEntityName, void * pContext );

            //////////////////////////////////////////////////////////////////////////
            /// Default constructor
            XmlReader( ICoreAllocator * pAllocator = NULL, size_t bufferBlockSize = 0 );

            //////////////////////////////////////////////////////////////////////////
            /// Destructor
            ~XmlReader();

            //////////////////////////////////////////////////////////////////////////
            /// Allocator get/set
            ICoreAllocator * GetAllocator() const;
            void SetAllocator( ICoreAllocator * pAllocator);

            //////////////////////////////////////////////////////////////////////////
            /// Reset the XmlReader to a newly constructed state such that new data 
            /// can be parsed. However, user options and the user-specified allocator 
            /// are preserved.
            void Reset();

            // -------- Input options

            //////////////////////////////////////////////////////////////////////////
            /// Set the current input stream. There is a stack of input streams;
            /// pushing a stream makes it the current stream on the stack. When the
            /// stream gets to the end it is popped and parsing continues using the
            // stream at the top of the stack.
            void PushInputStream(
                EA::IO::IStream * pStream,
                ReadEncoding encoding = kReadEncodingUTF8,
                const char * pStreamURI = NULL,     // URI of stream, for debugging purposes
                ssize_t nBufferSize = -1 );

            //////////////////////////////////////////////////////////////////////////
            /// Set the input buffer. If bCopy is not true, then input buffer is not
            /// copied, so it must stay around until parsing is complete.
            void PushInputBuffer(
                const void * pBuffer,
                size_t nBufferLen,
                ReadEncoding encoding = kReadEncodingUTF8,
                const char * pStreamURI = NULL,     // URI of stream, for debugging purposes
                bool bCopy = false );

            //////////////////////////////////////////////////////////////////////////
            /// Adds an input buffer to the back of the stack, as if appending data.
            /// This is useful for the case whereby XML to be parsed is being loaded
            /// asynchronously in the background.
            void AppendInputBuffer(
                const void * pBuffer,
                size_t nBufferLen,
                ReadEncoding encoding = kReadEncodingUTF8,
                const char * pStreamURI = NULL,     // URI of stream, for debugging purposes
                bool bCopy = false );

            //////////////////////////////////////////////////////////////////////////
            /// Set a flag
            void SetFlag( uint32_t nFlag, bool bSet );

            //////////////////////////////////////////////////////////////////////////
            /// Get a flag
            bool GetFlag( uint32_t nFlag ) const;

            //////////////////////////////////////////////////////////////////////////
            /// SetBufferBlockSize
            /// Sets the size of buffering blocks. Default is XmlTokenBuffer::kBlockSizeDefault.
            /// Typically you would call this before using the class instance.
            bool SetBufferBlockSize( size_t size );

            //////////////////////////////////////////////////////////////////////////
            /// Source path. Returns the path to the file being parsed (only used
            /// for error reporting.)
            const char * GetSourcePath() const;

            //////////////////////////////////////////////////////////////////////////
            /// Get the base URI for this document.
            void SetBaseURI( const char * baseURI );

            //////////////////////////////////////////////////////////////////////////
            /// Get the base URI for this document.
            const char * GetBaseURI() const;

            // -------- Namespace options

            //////////////////////////////////////////////////////////////////////////
            /// Adds a short alias for a namespace. This short alias will be preprended
            /// to any element name that belongs to that namespace when the name is
            /// returned by the API. So for example, if we define "xi" as an alias
            /// for "http://www.w3.org/2001/XInclude", then an "include" element in
            /// the XInclude namespace will be returned as "xi:include". Not that this
            /// is entirely independent of any aliases defined within the document,
            /// which are local to the document and not exposed at this level.
            ///
            /// Parameters:
            ///    pAlias - the short name of the namespace (e.g. "xi")
            ///    pNamespace - the expanded name of the namespace
            ///    bCopy - If true, makes a copy of the strings; otherwises uses the
            ///            original pointers passed in.
            void AddNamespaceAlias( const char * pAlias, const char * pNamespace, bool bCopy );

            //////////////////////////////////////////////////////////////////////////
            /// Look up the complete namespace for a given alias.
            const char * GetNamespaceFromAlias( const char * pAlias );

            //////////////////////////////////////////////////////////////////////////
            /// Returns the number of namespaces declared for the current element.
            /// For example, the following element has two declared namespaces:
            ///     <test xmlns='ns1' xmlns:tt='ns2' abc='def'>
            /// The namespaces of parent elements are not enumerated; only the 
            /// namespaces explicitly declared for the current element are enumerated.
            int32_t GetElementNamespaceCount() const;

            //////////////////////////////////////////////////////////////////////////
            /// Returns the Nth namespace prefix declared for the current element.
            /// If pNamespace is non-NULL, the namespace pointer is copied to it.
            /// Both strings are 0-terminated.
            /// For example, the following element has two declared namespaces:
            ///     <test xmlns='ns1' xmlns:tt='ns2' abc='def'>
            /// The first has a prefix of "" and a namespace of "ns1"; the second
            /// has a prefix of "tt" and a namespace of "ns2".
            /// The namespaces of parent elements are not enumerated; only the 
            /// namespaces explicitly declared for the current element are enumerated.
            const char* GetElementNamespace(int32_t index, const char** pNamespace = NULL) const;

            // -------- Entity resolution

            //////////////////////////////////////////////////////////////////////////
            /// For "simple" internal entities, you can just define them as a static
            /// text string. Note that the token reader will not make a copy of the
            /// entity name or entity value buffers, you'll need to hang on to them
            /// until after the parsing is complete.
            ///
            /// The standard XML entities [ &amp; &lt; &gt; &quot; &apos; ] are defined
            /// by default; you don't need to define them with this function. However, 
            /// you can override them with this function. The Reset function clears
            /// all entities defined with this function and restores the defaults.
            ///
            /// Example usage:
            ///     xmlReader.DefineEntity("nbsp", "\xa0");  // Non-breaking space char == 0xa0
            ///
            void DefineEntity( const char * pEntityName, const char * pEntityValue );

            //////////////////////////////////////////////////////////////////////////
            /// Set entity resolver callback. This is for more complex entity
            /// declarations.
            void SetEntityRefResolver( entity_lookup_t * pEntityLookupFunc, void * pContext );

            // -------- Parsing functions

            //////////////////////////////////////////////////////////////////////////
            /// Advance to the next token in the token stream.
            bool Read();

            //////////////////////////////////////////////////////////////////////////
            /// Checks to make sure that the current element is a start element,
            /// and advances to the next node.
            bool ReadStartElement();

            //////////////////////////////////////////////////////////////////////////
            /// Checks to make sure that the current element is an end element,
            /// and advances to the next node.
            bool ReadEndElement();
 
            //////////////////////////////////////////////////////////////////////////
            /// Skips the children of the current node and does an implict Read on 
            /// the next sibling node. This positions the reader at the subsequent  
            /// sibling (at the same depth) of the current node and makes it the current node.
            ///
            /// Both Skip and Read are defined as moving the current element pointer 
            /// by at least 1. Calling both of them together will move it by at least 2.
            /// In other words, after a Skip the current element pointer will be 
            /// positioned at the next sibling element. You can now access the properties 
            /// of that element, and you do not need to call Read since Skip implicitly 
            /// calls it for you. If you call Read again, it will position the current 
            /// element pointer to the element just after the sibling element, meaning 
            /// that you have now skipped over the sibling. 
            bool Skip();

            //////////////////////////////////////////////////////////////////////////
            /// Get the current result status
            ResultCode GetResultCode() const;

            //////////////////////////////////////////////////////////////////////////
            /// Return true if the reader is currently at the end of file
            bool IsEof() const;

            // -------- Location functions

            //////////////////////////////////////////////////////////////////////////
            /// Current line number within the current file (i.e. stream).
            /// The first line has a value of zero.
            /// The current line number refers to the line number that the current
            /// node (the most recent one that Read found) begins on.
            int32_t GetLineNumber() const;

            //////////////////////////////////////////////////////////////////////////
            /// Current column number within the current file (i.e. stream).
            /// The first column has a value of zero.
            /// The column number is the byte index for the given line.
            /// The current column number refers to the column that the current
            /// node (the most recent one that Read found) begins on.
            int32_t GetColumnNumber() const;

            //////////////////////////////////////////////////////////////////////////
            /// Current byte index within the current file (i.e. stream).
            /// The current byte index refers to the byte index that the current
            /// node (the most recent one that Read found) begins on.
            /// If the XML content spreads across multiple files (streams) or includes
            /// an external entity reference (XML equivalent to #include), then 
            /// this byte index refers only to the currently parsing stream. If you 
            /// want to absolute XML content byte position, use GetAbsoluteByteIndex().
            /// The first byte has a value of zero.
            ssize_t GetCurrentByteIndex() const;

            /// Returns the absolute byte index in the XML content stream. If there
            /// is just one stream for the XML content (as is most common) then this 
            /// results in the same value as GetCurrentByteIndex. If the XML content
            /// is spread across multiple streams or if there is an external entity
            /// reference whereby an XML file includes content that is read from an
            /// external document, this byte index measures the byte index as if all
            /// these files were a single file. This result is equal to the total 
            /// number of bytes read by this class.
            ssize_t GetAbsoluteByteIndex() const { return 0; } // Not currently implemented.

            //////////////////////////////////////////////////////////////////////////
            /// Return the depth of the current token
            int32_t GetDepth() const;

            // -------- State attributes

            //////////////////////////////////////////////////////////////////////////
            /// Return the type of the next token in the XML stream
            NodeType GetNodeType() const;

            //////////////////////////////////////////////////////////////////////////
            // Tells if CharacterData is CData or just characters. Only has meaning
            // if GetNodeType is CharacterData.
            bool GetIsCData() const
                { return mIsCData; }

            //////////////////////////////////////////////////////////////////////////
            /// Returns the text name of this token.
            /// The result will depend on the current node type:
            ///    Element - the current element name. This is the qualified name, of the format [<prefix>:]<localName>
            ///    DocumentType - The document type name.
            ///    Processing Instruction - The target of the processing instruction
            ///    Declaration (DOCTYPE, etc.) - the name of the declaration
            ///    Entity Ref - the name of the entity referenced.
            const char * GetName() const;

            //////////////////////////////////////////////////////////////////////////
            /// Returns the name of this element, without a namespace prefix that might be present.
            /// In most XML docments there is only a local name and not a namespace prefix.
            const char * GetLocalName() const;

            //////////////////////////////////////////////////////////////////////////
            /// Returns the namespace prefix length of this element without the local name.
            /// It includes the ':' char.
            /// This is the same as GetLocalName() - GetName().
            /// The length is returned instead of a string because returning a string would
            /// require additional memory allocation and maintenance.
            size_t GetNamePrefixLength() const
                { return (size_t)(GetLocalName() - GetName()); }

            //////////////////////////////////////////////////////////////////////////
            /// Returns the complete, expanded namespace of this element.
            /// Returns NULL if the node type is not Element or EndElement; returns NULL if
            /// there is no active namespace for the element.
            const char * GetNamespaceURI() const;

            //////////////////////////////////////////////////////////////////////////
            /// Return the text of the token (element name, text data, etc.).
            /// Note that Text tokens may have embedded NUL characters (but will
            /// be NUL terminated) -- use ValueLength() if you aren't sure.
            /// For elements, this text will be the element name. For text and comments,
            /// it will be the content of the element. For processing instruction, it will
            /// be the target (such as "jsp" for <?jsp args?>. For namespaces, it will be
            /// the namespace prefix. For doctype declarations it will be the doctype name.
            const char * GetValue() const;

            //////////////////////////////////////////////////////////////////////////
            /// Returns the length of the token text.
            size_t GetValueLength() const;

            //////////////////////////////////////////////////////////////////////////
            /// Return the number of attribs. Returns zero if the current token is not a start element.
            /// Namespace declaration attributes are not counted; they are stored in the namespace 
            /// stack instead.
            int32_t GetAttributeCount() const;

            //////////////////////////////////////////////////////////////////////////
            /// Returns true if the current node has attributes.
            /// Namespace declaration attributes are not counted; they are stored in the namespace 
            /// stack instead.
            bool HasAttributes() const;

            //////////////////////////////////////////////////////////////////////////
            /// Returns true if the current node has a value
            bool HasValue() const;

            //////////////////////////////////////////////////////////////////////////
            /// Returns true if the current node is an empty element element
            bool IsEmptyElement() const;

            //////////////////////////////////////////////////////////////////////////
            /// Name of the Nth attribute. Returns NULL if the current token is not a start element.
            /// This is the fully qualified name, of the format [<prefix>:]<localName>
            /// Namespace declaration attributes are not counted; they are stored in the namespace 
            /// stack instead. 
            /// For example, the following element:
            ///     <test xmlns='ns1' xmlns:tt='ns2' abc='def' ns0.xyz='pdq'>
            /// has a two attributes with the names "abc" and "ns0.xyz" and values
            /// of "def" and "pdq", respectively. The first two xmlns entries are 
            /// namespace declarations and are stored by us separately.
            /// The second attribute (ns0.xyz) is one with a namespace prefix.
            /// The GetAttributeLocalName function would return just the "xyz" portion.
            const char * GetAttributeName( int nIndex ) const;

            //////////////////////////////////////////////////////////////////////////
            /// Local name of the Nth attribute. The local name of an attribute is the 
            /// same as the name of the attribute, except when there is a prefix.
            /// See GetAttributeName for info about this.
            const char * GetAttributeLocalName( int nIndex ) const;

            //////////////////////////////////////////////////////////////////////////
            /// Value of the Nth attribute.
            /// Returns NULL if the current token is not a start element or index is out of range.
            /// Namespace declaration attributes are not counted; they are stored in the namespace 
            /// stack instead.
            const char * GetAttributeValue( int nIndex ) const;

            //////////////////////////////////////////////////////////////////////////
            /// Get the value of the named attribute.
            /// Returns NULL if the current token is not a start element
            /// or if the attribute is not found.
            const char * GetAttributeValue( const char * psAttrName ) const;

            /// Get the value of the Nth attribute.
            /// Returns NULL if the current token is not a start element or index is out of range.
            /// Namespace declaration attributes are not counted; they are stored in the namespace 
            /// stack instead.
            const char *operator[]( int nIndex );

            /// Get the value of the named attribute.
            /// Returns NULL if the current token is not a start element
            /// or if the attribute is not found.
            /// Namespace declaration attributes are not counted; they are stored in the namespace 
            /// stack instead.
            const char *operator[]( const char *psAttrName );

            // -------- Document attributes

            //////////////////////////////////////////////////////////////////////////
            /// If this token is a document type declaration, this returns the DOCTYPE name.
            const char * GetDocTypeName() const;

            //////////////////////////////////////////////////////////////////////////
            /// If this token is a document type declaration, this returns the system id.
            const char * GetSystemId() const;

            //////////////////////////////////////////////////////////////////////////
            /// If this token is a document type declaration, this returns the public id.
            const char * GetPublicId() const;

            //////////////////////////////////////////////////////////////////////////
            /// Returns true if the document type declaration doesn't specify this 
            /// document as standalone="no" (e.g. <?xml version="1.0" encoding="UTF-8" standalone="no"?>)
            /// By default a document is standalone.
            bool GetIsStandalone() const;

        public:
            // These functions are public so that we can unit test them

            static bool IsNameStartChar( int32_t c );   /// Return true if c is a name start character
            static bool IsNameChar( int32_t c );        /// Return true if c is a name character

            ReadEncoding GetCurrentEncoding() const;        /// Return the encoding of the current stream

        protected:
            //////////////////////////////////////////////////////////////////////////
            /// Current state of the reader
            enum ReaderState {
                State_Undecided,                        /// Don't know what comes next
                State_Text,                             /// Parsing text
                State_Element,                          /// Parsing an element
                State_CDATA,                            /// Inside a CDATA section
                State_EntityRef                         /// Unresolved entity reference
            };

            //////////////////////////////////////////////////////////////////////////
            // A stream of text input. This may be the main file, an included file, or an entity.
            struct InputStream {
                enum ReadResult {
                    EoF                 = -1,   // No more data.
                    IOError             = -2,   // Stream read error.
                    PartialChar         = -3,   // A UTF8 sequence was incomplete.
                    IncorrectEncoding   = -4    // A UTF8 sequence was invalid.
                };

                typedef int32_t (InputStream::*DecoderFunc)();  /// Character decoding function type

                EA::IO::IStream * mpStreamPtr;            /// Stream pointer
                ReadEncoding      mEncoding;              /// ReadEncoding for this stream
                DecoderFunc       mpDecoderFunc;          /// Character decoding function
                uint8_t *         mpInputBufferStart;     /// Buffer for parsing characters (start)
                uint8_t *         mpInputBufferEnd;       /// Buffer for parsing characters (end)
                uint8_t *         mpInputBufferCapacity;  /// Buffer for parsing characters (end)
                uint8_t *         mpInputBufferPos;       /// Parse position in stream
                bool              mOwnBuffer;             /// True if we own this buffer and need to free it.
                InputStream *     mpNext;                 /// Next stream

                const char *      mpStreamURI;            /// URI of stream (for debugging)
                int32_t           mPrevChar;              /// Previous character value.
                int32_t           mLineIndex;             /// Line number in the source buffer. 0-based.
                int32_t           mColumnIndex;           /// Column index in the source buffer. 0-based.
                int32_t           mNextLineIndex;         /// Line number of current read position. 0-based.
                int32_t           mNextColumnIndex;       /// Column number of current read position. 0-based.
                ssize_t           mByteIndex;             /// Byte index in the source buffer. 0-based.

                ssize_t FillBuffer();                   /// Fill buffer
                int32_t ReadChar();                     /// Read character
                int32_t ReadCharUTF8();                 /// Read a UTF-8 char
                int32_t ReadCharUTF16BE();              /// Read a UTF-16 char (big-endian)
                int32_t ReadCharUTF16LE();              /// Read a UTF-16 char (little-endian)
            };

            //////////////////////////////////////////////////////////////////////////
            // A namespace declaration
            struct NSDeclaration {
                const char *    mpLocalName;            /// The local name for this namespace. Used as a prefix in a qualified name.
                const char *    mpNamespace;            /// The namespace. Must be in the form of a URI.
                const char *    mpAlias;                /// The application name for this namespace
                int32_t         mDepth;                 /// The tree depth at which it was defined
                NSDeclaration * mpPrev;                 /// Previous nested declaration
            };

            //////////////////////////////////////////////////////////////////////////
            // A namespace alias
            struct NSAlias {
                bool            mOwnStrings;            /// True if we need to free these strings.
                const char *    mpNamespace;            /// The namespace. Must be in the form of a URI.
                const char *    mpAlias;                /// The application name for this namespace
                NSAlias    *    mpNext;                 /// Next in chain
            };

            typedef eastl::hash_map<const char *, const char *, eastl::hash<const char *>, 
                                    eastl::str_equal_to<const char *>, CoreAllocatorAdapter > tEntityMap;

            typedef eastl::vector<const char *, CoreAllocatorAdapter> tAttributeArray; // name/value pairs [0] = name, [1] = value, etc.


            //////////////////////////////////////////////////////////////////////////
            // Instance variables

            ICoreAllocator *    mpCoreAllocator;        /// Allocation pool we're drawing from

            uint32_t            mFlags;                 /// Reader flags

            ResultCode          mResultCode;
            int32_t             mElementDepth;          /// Current nesting depth of the parser
            int32_t             mInputDepth;            /// Current nesting depth of input streams

            InputStream *       mpInputStack;           /// Stack of input streams.
            InputStream *       mpTopNamedStream;       /// Topmost stream that has a debugging name.

            NSAlias *           mpNamespaceAliases;     /// Alias list for namespaces
            NSDeclaration *     mpNamespaceDecls;       /// Local names for namespaces
            NSDeclaration *     mDefaultNS;             /// Current default namespace
            NSDeclaration *     mElementNS;             /// Current element namespace

            const char*         mBuiltInEntities[10];   /// amp = &, lt = <, etc.
            tEntityMap          mEntityDecls;           /// Internal Entity declaration map
            entity_lookup_t *   mpEntityResolver;       /// Pointer to entity resolve
            void *              mpEntityResolverContext;/// Context pointer for entity resolver

            int32_t             mChar;                  /// Current character
            ReaderState         mState;                 /// Current reader state
            NodeType            mNodeType;              /// Type of the current parsed token
            bool                mIsEmptyElement;        /// True if this element is empty. For example, these are empty: <xyz/> <pdq a="b" c="d"/>, while these are not: <xyz></xyz> <xyz>pdq</xyz>.
            bool                mIsCData;               /// True if the current NodeType is CharacterData and it comes from CDATA as opposed to regular characters.
            bool                mIsStandalone;          /// Returns true if the DTD doesn't define the document as standlone="no". Defaults to true.

            const char *        mpTokenName;            /// Name of this token
            const char *        mpTokenValue;           /// Textual value of this token
            ssize_t             mValueLength;           /// Length of the value token
            const char *        mpPendingEntityRef;     /// Entity reference to be output

            XmlTokenBuffer      mTokenBuffer;           /// Items that stay around until all parsing complete
            tAttributeArray     mAttributeArray;        /// Array of currently parsed attributes, in pairs. mAttributeArray[0] is a name, [1] is its value, [2] is the next name, [3] is its value, etc.

            const char *        mpDocTypeName;          /// DOCTYPE System ID
            const char *        mpSystemId;             /// DOCTYPE System ID
            const char *        mpPublicId;             /// DOCTYPE Public ID

            int32_t             mLineIndex;             /// Line number in the source buffer. 0-based.
            int32_t             mColumnIndex;           /// Column index in the source buffer. 0-based.
            ssize_t             mByteIndex;             /// Byte index in the source buffer. 0-based.


            //////////////////////////////////////////////////////////////////////////
            // Functions

            void            Init();
            void            PopInputStream();
            void            ReadChar();
            bool            SkipChar( int32_t c );
            bool            SkipWS();
            void            AppendByte( char c );
            void            AppendBytes( const uint8_t * c, ssize_t length );
            void            AppendChar( int32_t c );
            const char *    TokenText( ssize_t * pLengthOut = NULL );  // Finish the current token and return a pointer to it.
            ReadEncoding    DetectEncoding( uint8_t *& pBuffer, const uint8_t * pBufferEnd );
            const char *    ParseName();
            bool            ParseEntityRef();
            bool            ParseCharacterRef();
            bool            ParseEntityOrCharacterRef();
            bool            ParseComment();
            bool            ParseDeclaration();
            bool            ParseProcessingInstruction();
            bool            ParseElement();
            bool            ResolveEntity( const char * pEntityName );
            void            Fatal( ResultCode rc );
            void            PushNamespace( const char * nsLocalName, const char * nsExpandedName );
            void            PopNamespace();
            NSDeclaration * LookupNamespaceFromPrefix( const char * pQName );
            const char *    NormalizeNamespace( NSDeclaration * pNS, const char * pQName );
            void            NormalizeNamespaces();
            InputStream*    CreateStreamFromBuffer(const void * pBuffer, size_t nBufferLen, ReadEncoding encoding, const char * pStreamURI, bool bCopy, ReadEncoding defaultEncoding);

        public:
            // Temporary deprecated functions for backward compatibility.
            const char* GetAttribute(int nIndex) const             { return GetAttributeValue(nIndex); }
            const char* GetAttribute(const char* psAttrName) const { return GetAttributeValue(psAttrName); }
        };


        //////////////////////////////////////////////////////////////////////////
        /// Standalone function to convert the result code into a string.
        ///
        /// Returns a read-only C string describing the error associated with the
        /// given ResultCode. The string will be lower case and have no terminating
        /// punctuation. This string is not localized; it is in US English.
        /// Returns, for example "read error" for kErrorRead.
        /// Returns "success" for a ResultCode of kSuccess.
        ///
        UTFXML_API const char * GetXmlReaderResultString( XmlReader::ResultCode rc );


    } // namespace XML

} // namespace EA





//////////////////////////////////////////////////////////////////////////
// Inlines
//////////////////////////////////////////////////////////////////////////

namespace EA {

    namespace XML {

        inline
        ICoreAllocator * XmlReader::GetAllocator() const {
            return mpCoreAllocator;
        }

        inline
        void XmlReader::SetAllocator( ICoreAllocator * pAllocator) {
            mpCoreAllocator = pAllocator;
        }

        inline
        XmlReader::ResultCode XmlReader::GetResultCode() const {
            return mResultCode;
        }

        inline
        int32_t XmlReader::GetLineNumber() const { 
            return mLineIndex;
        }

        inline
        int32_t XmlReader::GetColumnNumber() const { 
            return mColumnIndex;
        }

        inline
        XmlReader::ssize_t XmlReader::GetCurrentByteIndex() const { 
            return mByteIndex;
        }

        inline
        int32_t XmlReader::GetDepth() const { 
            return mElementDepth;
        }

        inline
        XmlReader::NodeType XmlReader::GetNodeType() const { 
            return mNodeType;
        }

        inline
        bool XmlReader::IsEmptyElement() const {
            return mNodeType == Element && mIsEmptyElement;
        }

        inline
        const char * XmlReader::operator[]( int nIndex ) {
            return GetAttribute( nIndex );
        }

        inline
        const char * XmlReader::operator[]( const char *psAttrName ) {
            return GetAttribute( psAttrName );
        }

        inline
        void XmlReader::AppendByte( char c ) {
            // Append a single byte to the current token
            mTokenBuffer.AppendByte( (uint8_t)c );
        }

        inline
        void XmlReader::AppendBytes( const uint8_t * c, ssize_t length ) {
            // Append multiple bytes to the current token
            mTokenBuffer.AppendBytes( c, length );
        }

        inline
        void XmlReader::AppendChar( int32_t c ) {
            // Append a UTF-8 encoded char to the current token
            mTokenBuffer.AppendEncodedChar( (uint32_t)c );
        }

        inline
        const char * XmlReader::TokenText( ssize_t * pLengthOut ) {
            if (pLengthOut)
                *pLengthOut = mTokenBuffer.TokenLength();
            const char * const pResult = (const char *)mTokenBuffer.FinishToken();
            return pResult;
        }

        inline
        void XmlReader::SetFlag( uint32_t nFlag, bool bSet ) {
            if (bSet)
                mFlags |= nFlag;
            else
                mFlags &= ~nFlag;
        }

        inline
        bool XmlReader::GetFlag( uint32_t nFlag ) const {
            return (mFlags & nFlag) != 0;
        }

        inline
        bool XmlReader::ReadStartElement() {
            if (mNodeType == Element)
                return Read();
            return false;
        }

        inline
        bool XmlReader::ReadEndElement() {
            if (mNodeType == EndElement)
                return Read();
            return false;
        }
         
        inline
        bool XmlReader::IsEof() const {
            return mpInputStack == NULL;
        }

        inline
        int32_t XmlReader::GetAttributeCount() const {
            return int32_t( mAttributeArray.size() / 2 );
        }

        inline
        bool XmlReader::HasAttributes() const {
            return !mAttributeArray.empty();
        }

        inline
        const char * XmlReader::GetDocTypeName() const {
            if (mNodeType == DocTypeDecl)
                return mpDocTypeName;
            return NULL;
        }

        inline
        const char * XmlReader::GetSystemId() const {
            if (mNodeType == DocTypeDecl)
                return mpSystemId;
            return NULL;
        }

        inline
        const char * XmlReader::GetPublicId() const {
            if (mNodeType == DocTypeDecl)
                return mpPublicId;
            return NULL;
        }

        inline 
        bool XmlReader::GetIsStandalone() const {
            return mIsStandalone;
        }

        inline
        ReadEncoding XmlReader::GetCurrentEncoding() const {
            if (mpInputStack)
                return mpInputStack->mEncoding;
            return kReadEncodingUnknown;
        }

        inline
        void XmlReader::SetEntityRefResolver( entity_lookup_t * pEntityLookupFunc, void * pContext ) {
            mpEntityResolver        = pEntityLookupFunc;
            mpEntityResolverContext = pContext;
        }

        inline
        bool XmlReader::ResolveEntity( const char * pEntityName ) {
            if (mpEntityResolver) 
                return (*mpEntityResolver)( this, pEntityName, mpEntityResolverContext );
            return false;
        }


    } // namespace XML

} // namespace EA


#ifdef _MSC_VER
	#pragma warning(pop)
#endif


#endif // Header include guard








