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
// Name:    XmlWriter.cpp
// 
// Purpose: Classes for writing out XML files
//
// Copyright (c) 2003 Electronic Arts Inc.
// Created by: Talin
///////////////////////////////////////////////////////////////////////////////


#include <EABase/eabase.h>
#include <UTFXml/internal/Config.h>
#include <UTFXml/XmlWriter.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <stdio.h>
#include EA_ASSERT_HEADER


#ifdef _MSC_VER
    #pragma warning(disable: 4244)  // conversion from 'const ptrdiff_t' to 'int32_t', possible loss of data. This is a bogus warning resulting from a bug in VC++.

    #define vsnprintf_local _vsnprintf
#else
    #define vsnprintf_local  vsnprintf
#endif



namespace EA
{
namespace XML
{



////////////////////////////////////////////////////////////////////////////////
// XmlWriter
//
XmlWriter::XmlWriter( bool bFormatPretty, WriteEncoding output )
  : mWriteEncoding(output),
    mbFormatPretty( bFormatPretty ),
    mbSimpleElement( false ),
    mnState( kStateChars ),
    mnCharCount( 0 ),
    mnIndentLevel( 0 ),
    mnIndentSpaces( kIndentSpacesDefault ),
    mLineEnd( kLineEndDefault ),
    mpOutputStream( NULL )
{
}


////////////////////////////////////////////////////////////////////////////////
// ~XmlWriter
//
XmlWriter::~XmlWriter()
{
    SetOutputStream(NULL);
}


////////////////////////////////////////////////////////////////////////////////
// WriteWhitespace
//
bool XmlWriter::WriteNewline()
{
    if(mLineEnd == kLineEndWindows)
        return WriteText( "\r\n", 2); 
    return WriteText( "\n", 1); 
}


////////////////////////////////////////////////////////////////////////////////
// WriteWhitespace
//
bool XmlWriter::WriteWhitespace( size_t nChars )
{
    const size_t kSpacesCount = 32;
    const char   kSpaces[kSpacesCount + 1] = "                                ";

    // Since nIndent may be > than kSpacesCount, we allow for writing the indent in pieces.
    while (nChars > 0) {
        const size_t nCharsToWrite = ((nChars < kSpacesCount) ? nChars : kSpacesCount);

        if (!WriteText( kSpaces, nCharsToWrite )) 
            return false;

        nChars -= nCharsToWrite;
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
// WriteIndent
//
bool XmlWriter::WriteIndent()
{
    if (mbFormatPretty) {
        // Write a newline, regardless of how many spaces follow.
        // Don't write a newline if we have written nothing at all yet to the file.
        if (mnCharCount) {
            if (!WriteNewline()) 
                return false;
        }

        if (!WriteWhitespace(mnIndentLevel * mnIndentSpaces))
            return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
bool XmlWriter::WriteEscapedString( const char *psText, size_t nChars )
{
    if(nChars == kSizeTypeNull)
        nChars = strlen(psText);

    const char * psMark = psText;

    for(size_t i = 0; i < nChars; ++i, ++psText)
    {
        const char c = *psText;

        // Look for a char that needs to be escaped
        switch (c)
        {
            case '&':
            case '<':
            case '>':
            case '\"':
            case '\'':
            {
                // Write all the text since the last escaped char.
                if ((psMark < psText) && !WriteText( psMark, (size_t)(psText - psMark))) 
                    return false;

                switch (c)
                {
                    case '&':
                        if (!WriteText( "&amp;",  5 )) 
                            return false; 
                        break;

                    case '<':
                        if (!WriteText( "&lt;",   4 )) 
                            return false;
                        break;

                    case '>':
                        if (!WriteText( "&gt;",   4 )) 
                            return false;
                        break;

                    case '\"': 
                        if (!WriteText( "&quot;", 6 )) 
                            return false;
                        break;

                    case '\'':
                        if (!WriteText( "&apos;", 6 )) 
                            return false;
                        break;
                }

                psMark = psText + 1;
                break;
            }

            default:
                break;
        }
    }

    // Write the end of the string since the last escaped char.
    if ((psMark < psText) && !WriteText( psMark, (size_t)(psText - psMark) ))
       return false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool XmlWriter::WriteEscapedString( const char16_t *psText, size_t nChars  )
{
    if(nChars == kSizeTypeNull)
        nChars = wcslen(psText);

    const char16_t * psMark = psText;

    for(size_t i = 0; i < nChars; ++i, ++psText)
    {
        const char16_t c = *psText;

        // Look for a char that needs to be escaped
        switch (c) 
        {
            case '&':
            case '<':
            case '>':
            case '\"':
            case '\'': 
            {
                // Write all the text since the last escaped char
                if ((psMark < psText) && !WriteText( psMark, (size_t)(psText - psMark))) 
                    return false;

                switch (c) 
                {
                    case '&':
                        if (!WriteText( L"&amp;",  5 )) 
                            return false; 
                        break;
                    case '<':
                        if (!WriteText( L"&lt;",   4 )) 
                            return false;
                        break;
                    case '>':
                        if (!WriteText( L"&gt;",   4 )) 
                            return false;
                        break;
                    case '\"': 
                        if (!WriteText( L"&quot;", 6 )) 
                            return false;
                    case '\'':
                        if (!WriteText( L"&apos;", 6 )) 
                            return false;
                        break;
                }

                // Set the new mark position
                psMark = psText + 1;
                break;
            }

            default:
                break;
        }
    }

    // Write the end of the string since the last escaped char
    if ((psMark < psText) && !WriteText( psMark, (size_t)(psText - psMark) ))
        return false;

    return true;
}


////////////////////////////////////////////////////////////////////////////////
// CloseCurrentElement
//
bool XmlWriter::CloseCurrentElement()
{
    switch (mnState) {
        case kStateElement:
            if (!WriteText( ">", 1 )) 
                return false;
            break;

        case kStateProcessingInstruction:
            if (!WriteText( "?>", 2 ))
                return false;
            break;

        case kStateCDATA:
            if (!WriteText( "]]>", 3 ))
                return false;
            break;

        case kStateChars:
            break;
    }

    mnState = kStateChars;
    return true;
}


////////////////////////////////////////////////////////////////////////////////
// BeginElement
//
bool XmlWriter::BeginElement( const char *psElementName ) {
    if (CloseCurrentElement()
        && WriteIndent()
        && WriteText( "<", 1 )
        && WriteName( psElementName ))
    {
        mnState = kStateElement;
        mbSimpleElement = true;
        mnIndentLevel++;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////
// EndElement
//
bool XmlWriter::EndElement( const char *psElementName ) {
    EA_ASSERT(mnIndentLevel > 0);

    mnIndentLevel--;

    switch (mnState) {
        case kStateProcessingInstruction:
            EA_FAIL_MESSAGE( "XmlWriter: Cannot close element because we are in a processing instruction." );
            return false;

        case kStateElement:
            mnState = kStateChars;
            mbSimpleElement = false;
            return WriteText( "/>", 2 );

        case kStateCDATA:
            CloseCurrentElement();
            // Fall through.

        case kStateChars:
            if (!mbSimpleElement ) { // If the current element has no child elements...
                if (!WriteIndent() ) // Then write an indent before writing the element end.
                    return false;
            }

            mbSimpleElement = false;

            return ( WriteText( "</", 2 )
                    && WriteText( psElementName, kSizeTypeNull )
                    && WriteText( ">", 1 ) );
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////
// BeginElement
//
bool XmlWriter::BeginProcessingInstruction( const char *psInstructionName )
{
   if (CloseCurrentElement()
       && WriteIndent()
       && WriteText( "<?", 2 )
       && WriteText( psInstructionName, kSizeTypeNull ))
   {
      mbSimpleElement = false;
      mnState = kStateProcessingInstruction;

      return true;
   }

   return false;
}


////////////////////////////////////////////////////////////////////////////////
// EndProcessingInstruction
//
bool XmlWriter::EndProcessingInstruction()
{
    mbSimpleElement = false;

    switch (mnState) {
        case kStateProcessingInstruction:
            mnState = kStateChars;
            return WriteText( "?>", 2 );

        case kStateElement:
            EA_FAIL_MESSAGE( "XmlWriter: Cannot close processing instruction because we are in an element." );
            mnState = kStateChars;
            return false;

        case kStateChars:
            EA_FAIL_MESSAGE( "XmlWriter: Cannot close processing instruction because it was already closed." );
            mnState = kStateChars;
            return false;

        case kStateCDATA:
            EA_FAIL_MESSAGE( "XmlWriter: Cannot close processing instruction because we are in a CDATA section." );
            return false;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////
// AppendAttribute
//
bool XmlWriter::AppendAttribute( const char *psAttrName, const char *psValue )
{
   EA_ASSERT_MESSAGE( mnState != kStateChars, "Attempt to append an attribute to an element that has already been closed." );

   if ((mnState != kStateElement) && (mnState != kStateProcessingInstruction))
        return false;

   return (WriteText( " ", 1 )
         && WriteName( psAttrName )
         && WriteText( "=\"", 2 )
         && WriteEscapedString( psValue, kSizeTypeNull )
         && WriteText( "\"", 1 ) );
}


////////////////////////////////////////////////////////////////////////////////
// AppendAttribute
//
bool XmlWriter::AppendAttribute( const char *psAttrName, const char16_t *psValue )
{
   EA_ASSERT_MESSAGE( mnState != kStateChars, "Attempt to append an attribute to an element that has already been closed." );

   if ((mnState != kStateElement) && (mnState != kStateProcessingInstruction))
        return false;

   return (WriteText( " ", 1 )
         && WriteName( psAttrName )
         && WriteText( "=\"", 2 )
         && WriteEscapedString( psValue, kSizeTypeNull )
         && WriteText( "\"", 1 ) );
}


////////////////////////////////////////////////////////////////////////////////
bool XmlWriter::AppendAttributeF( const char *psAttrName, const char *psValueFmt, ... )
{
    EA_ASSERT_MESSAGE( mnState != kStateChars, "Attempt to append an attribute to an element that has already been closed." );

    if ((mnState != kStateElement) && (mnState != kStateProcessingInstruction)) 
        return false;

    /// For now, we assume that 256 is the max length for a formatted attribute.
    char    sBuffer[ 256 ];
    va_list arg;
    va_start( arg, psValueFmt );

    const int result = vsnprintf_local( sBuffer, sizeof(sBuffer), psValueFmt, arg );

    va_end( arg );

    if ((unsigned)result >= sizeof(sBuffer)) // If (result < 0 or result >= sizeof(sBuffer))...
        return false;

    return (WriteText( " ", 1 )
        && WriteName( psAttrName )
        && WriteText( "=\"", 2 )
        && WriteEscapedString( sBuffer, (unsigned)result )
        && WriteText( "\"", 1 ) );
}


////////////////////////////////////////////////////////////////////////////////
bool XmlWriter::WriteCharData( const char *psCharData, size_t nCount )
{
   return CloseCurrentElement() && WriteEscapedString( psCharData, nCount );
}

////////////////////////////////////////////////////////////////////////////////
bool XmlWriter::WriteCharData( const char16_t *psCharData, size_t nCount )
{
    return CloseCurrentElement() && WriteEscapedString( psCharData, nCount );
}

////////////////////////////////////////////////////////////////////////////////
// CDATA text must contain the sequence "]]>" as that sequence is reserved for
// ending a CDATA section.
//
bool XmlWriter::WriteCDATA( const char *psCharData, size_t nCount )
{
    // Verify that there is no "]]>" sequence in the input text.
    #if (defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED) || defined(EA_ASSERT_ENABLED)  // If the EATrace or EAAssert packages are configured to enable assertions...
        // We can't use a built-in function such as strstr because our text might not
        // be 0-terminated. We could however use the STL find algorithm.
        const size_t nStrlen        = ((nCount == kSizeTypeNull) ? strlen(psCharData) : nCount);
        const size_t nStrlenToCheck = ((nStrlen > 2) ? (nStrlen - 2) : 0);

        for(size_t i = 0; i < nStrlenToCheck; ++i)
        {
            if((psCharData[i] == ']') && (psCharData[i + 1] == ']') && (psCharData[i + 2] == '>'))
            {
                EA_FAIL_MESSAGE("XmlWriter: User-specified CDATA has embedded ]]> sequence.");
                break;  
            }
        }
    #endif

    if (mnState != kStateCDATA) {
        if (!CloseCurrentElement())
            return false;
        if (!WriteText( "<![CDATA[", 9 ))
            return false;

      mnState = kStateCDATA;
   }

   return WriteText( psCharData, nCount );
}

////////////////////////////////////////////////////////////////////////////////
bool XmlWriter::WriteCDATA( const char16_t *psCharData, size_t nCount )
{
    // Verify that there is no "]]>" sequence in the input text.
    #if (defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED) || defined(EA_ASSERT_ENABLED)  // If the EATrace or EAAssert packages are configured to enable assertions...
        // We can't use a built-in function such as strstr because our text might not
        // be 0-terminated. We could however use the STL find algorithm.
        const size_t nStrlen        = ((nCount == kSizeTypeNull) ? wcslen(psCharData) : nCount);
        const size_t nStrlenToCheck = ((nStrlen > 2) ? (nStrlen - 2) : 0);

        for(size_t i = 0; i < nStrlenToCheck; ++i)
        {
            if((psCharData[i] == ']') && (psCharData[i + 1] == ']') && (psCharData[i + 2] == '>'))
            {
                EA_FAIL_MESSAGE("XmlWriter: User-specified CDATA has embedded ]]> sequence.");
                break;  
            }
        }
    #endif

    if (mnState != kStateCDATA) {
        if (!CloseCurrentElement())
            return false;
        if (!WriteText( "<![CDATA[", 9 ))
            return false;

        mnState = kStateCDATA;
    }

    return WriteText( psCharData, nCount );
}

////////////////////////////////////////////////////////////////////////////////
// WriteComment
//
bool XmlWriter::WriteComment( const char *psCommentText, bool bNewlineBefore, bool bNewlineAfter, size_t nSpacesBeforeComment )
{
    const char * psMark  = psCommentText;

    if (!CloseCurrentElement())
        return false;

    if (bNewlineBefore)
    {
        mbSimpleElement = false;
        if (!WriteIndent())
            return false;
    }

    if (nSpacesBeforeComment && !WriteWhitespace(nSpacesBeforeComment))
        return false;

    if (!WriteText( "<!-- ", 5 ))
        return false;

    // Write the string, except remove all runs of dashes
    while (const char c = *psCommentText) {
        if (c == '-') {
            psCommentText++;     // Include a single dash in the comment

            if ((psMark < psCommentText) && !WriteText( psMark, (size_t)((psCommentText - psMark) + 1) ))
                return false;

            while (*psCommentText == '-')
                psCommentText++;
        }
        else
            psCommentText++;
    }

    // Write the end of the string since the last escaped char
    if ((psMark < psCommentText) && !WriteText( psMark, (size_t)(psCommentText - psMark) ))
        return false;

    if (!WriteText( " -->", 4 ))
        return false;

    if (bNewlineAfter)
    {
        mbSimpleElement = false;
        if (!WriteIndent())
            return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
// WriteComment
//
bool XmlWriter::WriteComment( const char16_t *psCommentText, bool bNewlineBefore, bool bNewlineAfter, size_t nSpacesBeforeComment )
{
    const char16_t * psMark  = psCommentText;

    if (!CloseCurrentElement())
        return false;

    if (bNewlineBefore)
    {
        mbSimpleElement = false;
        if (!WriteIndent())
            return false;
    }

    if (nSpacesBeforeComment && !WriteWhitespace(nSpacesBeforeComment))
        return false;

    if (!WriteText( "<!-- ", 5 ))
        return false;

    // Write the string, except remove all runs of dashes
    while (const char16_t c = *psCommentText) {
        if (c == '-') {
            psCommentText++;     // Include a single dash in the comment

            if ((psMark < psCommentText) && !WriteText( psMark, (size_t)((psCommentText - psMark) + 1) ))
                return false;

            while (*psCommentText == '-')
                psCommentText++;
        }
        else
            psCommentText++;
    }

    // Write the end of the string since the last escaped char
    if ((psMark < psCommentText) && !WriteText( psMark, (size_t)(psCommentText - psMark) ))
        return false;

    if (!WriteText( " -->", 4 ))
        return false;

    if (bNewlineAfter)
    {
        mbSimpleElement = false;
        if (!WriteIndent())
            return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
// WriteName
//
// The XML standard specifies:
//    [4] NameChar ::= Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender
//    [5] Name     ::= (Letter | '_' | ':') (NameChar)*
//    [6] Names    ::= Name (#x20 Name)*
//    [7] Nmtoken  ::= (NameChar)+
//    [8] Nmtokens ::= Nmtoken (#x20 Nmtoken)*
//
bool XmlWriter::WriteName( const char *psName )
{
    #if (defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED) || defined(EA_ASSERT_ENABLED)  // If the EATrace or EAAssert packages are configured to enable assertions...
        const size_t length = strlen(psName);
        EA_ASSERT_FORMATTED((length > 0) && (length < 256), ("XmlWriter: Invalid element/tag name length: %u", (unsigned)length));

        // We may want to extend this invalid char list, but the ones here are the primary bad chars
        const size_t pos = strcspn(psName, " <>\"\'&\t\r\n");
        EA_ASSERT_FORMATTED(pos == length, ("XmlWriter: Invalid xml element/tag name: %s", psName));

        // As can be seen above, actually only letters, '_', and ':' may begin a name.
        EA_ASSERT_FORMATTED((length == 0) || !isdigit(psName[0]), ("XmlWriter: Invalid xml element/tag name first char: %s", psName));
    #endif

    return WriteText(psName, kSizeTypeNull);
}


////////////////////////////////////////////////////////////////////////////////
// WriteXmlHeader
//
bool XmlWriter::WriteXmlHeader()
{
    // We don't want any newlines written before the header, so temporarily
    // disable pretty formatting, which would write such newlines.
    const bool bPrettySaved = mbFormatPretty;
    mbFormatPretty = false;

    bool bResult = BeginProcessingInstruction( "xml" ) && AppendAttribute( "version", "1.0" );

    mbFormatPretty = bPrettySaved;

    if(bResult) {
        const char* pAttribute;

        switch(mWriteEncoding) {
            case kWriteEncodingUTF16:
                pAttribute = "UTF-16";
                break;

            case kWriteEncodingLatin1:
                pAttribute = "ISO-8859-1";
                break;

            case kWriteEncodingASCII:
                pAttribute = "US-ASCII";
                break;

            case kWriteEncodingUTF8:
            default:
                pAttribute = "UTF-8";
                break;
        }

        if(AppendAttribute( "encoding", pAttribute ))
            bResult = EndProcessingInstruction();
    }

    return bResult;
}



////////////////////////////////////////////////////////////////////////////////
// SetOutputStream
//
void XmlWriter::SetOutputStream( EA::IO::IStream *pStream, bool bFormatPretty )
{
    if(pStream != mpOutputStream)
    {
        EA::IO::IStream* const pTemp = mpOutputStream; // Create temporary to prevent possible problems with re-entrancy.
        if(pStream)
            pStream->AddRef();
        mpOutputStream = pStream;
        if(pTemp)
            pTemp->Release();
    }

    mbFormatPretty = bFormatPretty;
}


///////////////////////////////////////////////////////////////////////////////
// SetIndentSpaces
//
void XmlWriter::SetLineEnd(LineEnd lineEnd)
{
    mLineEnd = lineEnd;
}


///////////////////////////////////////////////////////////////////////////////
// SetIndentSpaces
//
void XmlWriter::SetIndentSpaces( size_t nIndentSpaces )
{
    EA_ASSERT(nIndentSpaces < 256); // Sanity check
    mnIndentSpaces = nIndentSpaces;
}


///////////////////////////////////////////////////////////////////////////////
bool XmlWriter::WriteText( const char8_t *psText, size_t nChars )
{
    if (mpOutputStream) {
        if(nChars == kSizeTypeNull)
            nChars = strlen(psText);
        mnCharCount += nChars;  // This isn't right, but it suffices for our current uses.

        return WriteEncoded( mpOutputStream, psText, (EA::IO::size_type)nChars, kWriteEncodingUTF8, mWriteEncoding );
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
bool XmlWriter::WriteText( const char16_t *psText, size_t nChars )
{
    if (mpOutputStream) {
        if(nChars == kSizeTypeNull)
            nChars = wcslen(psText);
        mnCharCount += nChars;  // This isn't right, but it suffices for our current uses.

        return WriteEncoded( mpOutputStream, psText, (EA::IO::size_type)nChars, mWriteEncoding );
    }

    return false;
}


} // namespace XML

} // namespace EA


#undef vsnprintf_local






