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
// XmlWriter.h
//
// Purpose: Classes for writing out XML files
// Copyright (c) 2007 Electronic Arts Inc. -- All Rights Reserved WorldWide.
// Created by: Talin '03
// ported to utf by Colin Andrews 9/05
// ported to EAOS by Talin '07
///////////////////////////////////////////////////////////////////////////////

#ifndef UTFXML_XMLWRITER_H
#define UTFXML_XMLWRITER_H

#include <UTFXml/internal/Config.h>
#include <UTFXml/XmlEncoding.h>
#include <EAIO/EAStream.h>

namespace EA {

    namespace XML {
        

        //////////////////////////////////////////////////////////////////////////
        /// A simple interface for writing XML.
        class UTFXML_API XmlWriter
        {
        public:
            /// String parameters which take size_t lengths can also use kSizeTypeNull
            /// to mean that the strlen of the string should be used to determine
            /// its length.
            static const size_t kSizeTypeNull = (size_t)-1;

            //////////////////////////////////////////////////////////////////////////
            /// Default constructor
            XmlWriter( bool bFormatPretty = true,  WriteEncoding output = kWriteEncodingUTF8);
            virtual ~XmlWriter();

            //////////////////////////////////////////////////////////////////////////
            /// Set the destination stream for the XML - output will fail if this is unset 
            void SetOutputStream( EA::IO::IStream *pStream, bool bFormatPretty = true );

            //////////////////////////////////////////////////////////////////////////
            /// Default number of space chars per indent.
            static const size_t kIndentSpacesDefault = 2;

            //////////////////////////////////////////////////////////////////////////
            /// Set the number of spaces per indent level for pretty format.
            void SetIndentSpaces( size_t nIndentSpaces );

            //////////////////////////////////////////////////////////////////////////
            // Sets the line ending type to write. \n or \r\n
            void SetLineEnd(LineEnd lineEnd);

            //////////////////////////////////////////////////////////////////////////
            /// Write the header at the beginning of the XML file. 
            /// The header is also known as a "prologue".
            /// Note that the header string is currently always thus:
            ///     <?xml version="1.0" encoding="UTF-8"?>
            bool WriteXmlHeader();

            //////////////////////////////////////////////////////////////////////////
            /// Begin a new element
            bool BeginElement( const char8_t * psElementName );

            //////////////////////////////////////////////////////////////////////////
            /// End the current element
            bool EndElement( const char8_t * psElementName );

            //////////////////////////////////////////////////////////////////////////
            /// Begin a new processing intruction
            bool BeginProcessingInstruction( const char8_t * psElementName );

            //////////////////////////////////////////////////////////////////////////
            /// End the current processing instruction
            bool EndProcessingInstruction();

            //////////////////////////////////////////////////////////////////////////
            /// Append a string attribute to the most recently written element or processing instruction,
            /// assuming that it has not been closed.
            /// This should be called before any character data or child elements are written.
            /// XML special characters in the attribute value will be escaped as entities.
            bool AppendAttribute( const char8_t * psAttrName,  const char8_t * psAttrValue );
            bool AppendAttribute( const char8_t * psAttrName,  const char16_t * psAttrValue );

            //////////////////////////////////////////////////////////////////////////
            /// VPrintF version of AppendAttribute.
            bool AppendAttributeF( const char8_t * psAttrName,  const char8_t * psAttrValueFmt, ... );

            //////////////////////////////////////////////////////////////////////////
            /// Write a string of character data. XML special characters will be escaped as entities.
            bool WriteCharData( const char8_t *  psCharData, size_t nCount = kSizeTypeNull);
            bool WriteCharData( const char16_t * psCharData, size_t nCount = kSizeTypeNull);

            //////////////////////////////////////////////////////////////////////////
            /// Write a string of unencoded character data. Consecutive calls to WriteCDATA will be
            /// merged into a single CDATA block.
            /// It is an error to include the string "]]>" in the character data.
            bool WriteCDATA( const char8_t *  psCharData, size_t nCount = kSizeTypeNull);
            bool WriteCDATA( const char16_t * psCharData, size_t nCount = kSizeTypeNull);

            //////////////////////////////////////////////////////////////////////////
            /// Write a comment tag, including the enclosing <!-- -->. 
            /// For compatibility with the XML standard, a run of dashes (----) will
            /// be converted into a single dash. No other encoding is performed.
            /// If bNewlineBefore is true, then a newline will be written before the comment is written.
            /// If bNewlineAfter is true, then a newline will be written after the comment.
            /// If nSpacesBeforeComment is non-zero, then spaces are written before the <!--.
            bool WriteComment( const char8_t * psCommentText, bool bNewlineBefore = true, bool bNewlineAfter = true, size_t nSpacesBeforeComment = 0 );
            bool WriteComment( const char16_t * psCommentText, bool bNewlineBefore = true, bool bNewlineAfter = true, size_t nSpacesBeforeComment = 0 );

            //////////////////////////////////////////////////////////////////////////
            /// Write an XML Name as defined in section 2.3 of the XML standard.
            /// XML names include tag names and attribute names, for example.
            /// The most important thing this function does is encode whitespace and
            /// do validity checking.
            bool WriteName( const char8_t * psName );

        protected:
            enum EState {
                kStateChars,
                kStateElement,
                kStateProcessingInstruction,
                kStateCDATA,
            };

            bool WriteText( const char8_t *  psText, size_t nChars );
            bool WriteText( const char16_t * psText, size_t nChars );
            bool CloseCurrentElement();
            bool WriteNewline();
            bool WriteWhitespace( size_t nChars );
            bool WriteIndent();
            bool WriteEscapedString( const char8_t *  psText, size_t nChars );
            bool WriteEscapedString( const char16_t * psText, size_t nChars );

        protected:
            WriteEncoding     mWriteEncoding;   /// The text encoding (e.g. UTF8) that we write.
            bool              mbFormatPretty;   /// True if we format the output to have automatic indentation.
            bool              mbSimpleElement;  /// True if the current element has no child elements. 
            EState            mnState;          /// Used to tell what we are currently writing.
            size_t            mnCharCount;      /// Number of chars written. Used to help pretty-printing.
            size_t            mnIndentLevel;    /// Level of indentation. Starts at zero, increases by one for each indentation.
            size_t            mnIndentSpaces;   /// Number of space chars per indent. Total indent at any time = mnIndentLevel * mnIndentSpaces.
            LineEnd           mLineEnd;         /// Defines whether we use Windows or standard indentation.
            EA::IO::IStream * mpOutputStream;   /// The output we write to.
        };

    } // namespace XML

} // namespace EA


#endif // Header include guard


