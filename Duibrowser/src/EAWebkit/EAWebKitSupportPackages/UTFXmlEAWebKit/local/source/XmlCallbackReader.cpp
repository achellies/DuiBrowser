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

/////////////////////////////////////////////////////////////////////////////
// Name: XmlCallbackReader.h
//
// Purpose: Classes for reading in hierarchical data structures from XML files
//
// Copyright (c) 2006 Electronic Arts Inc.
// Created by: Talin
/////////////////////////////////////////////////////////////////////////////

#include <UTFXml/XmlCallbackReader.h>


namespace EA {
namespace XML {


void IContentHandler::StartDocument()
{
}

void IContentHandler::EndDocument()
{
}

bool IContentHandler::StartElement( const char *, const char **, size_t )
{ 
    return true;
}

bool IContentHandler::EndElement( const char * )
{
    return true;
}

bool IContentHandler::Characters( const char *, size_t )
{
    return true;
}

bool IContentHandler::Comment( const char *, size_t )
{
    return true;
}

bool IContentHandler::ProcessingInstruction( const char *, const char * )
{
    return true;
}

bool IContentHandler::SkippedEntity( const char * )
{
    return true;
}



bool XmlCallbackReader::Parse(IContentHandler * pContentHandler)
{
    bool bShouldContinue = true;

    if (pContentHandler)
        mpContentHandler = pContentHandler;

    if (mpContentHandler)
        mpContentHandler->StartDocument();

    while (bShouldContinue && Read() ) {
        if (mpContentHandler) {
            const XmlReader::NodeType nodeType = GetNodeType();

            switch (nodeType) {
                case Element:
                    bShouldContinue = mpContentHandler->StartElement( mpTokenName, mAttributeArray.data(), mAttributeArray.size() / 2 );
                    if (bShouldContinue && IsEmptyElement())
                        bShouldContinue = mpContentHandler->EndElement( mpTokenName );
                    break;

                case EndElement:
                    bShouldContinue = mpContentHandler->EndElement( mpTokenName );
                    break;

                case CharacterData:
                    bShouldContinue = mpContentHandler->Characters( mpTokenValue, (size_t)mValueLength );
                    break;

                case Comment:
                    bShouldContinue = mpContentHandler->Comment( mpTokenValue, (size_t)mValueLength );
                    break;

                case ProcessingInstruction:
                    // As it stands now, the attribute list of the processing instruction is passed as a single unparsed string (mpTokenValue);
                    bShouldContinue = mpContentHandler->ProcessingInstruction( mpTokenName, mpTokenValue );
                    break;

                case EntityRef:
                    bShouldContinue = mpContentHandler->SkippedEntity( mpTokenName );
                    break;

                case None:
                case Document:
                case Prologue:
                case DocTypeDecl:
                case EntityDecl:
                case ElementDecl:
                case AttListDecl:
                case NotationDecl:
                    // To do: We should do something about these types so the user can at least see them.
                    break;
            }
        }
    }

    if (mResultCode == kSuccess) {
        if (mpContentHandler)
            mpContentHandler->EndDocument();
        return true;
    }

    return false;
}

} // namespace XML
} // namespace EA



