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
// Name: XmlReaderErrors.h
//
// Purpose: Error strings
//
// Copyright (c) 2006 Electronic Arts Inc.
// Created by: Talin
/////////////////////////////////////////////////////////////////////////////

#include <UTFXml/XmlReader.h>

namespace EA {
namespace XML {

UTFXML_API const char * GetXmlReaderResultString( XmlReader::ResultCode rc ) {

    switch (int( rc )) {
        case XmlReader::kSuccess:
            return "success";

        case XmlReader::kErrorRead:
            return "read error";

        case XmlReader::kErrorNoMemory:
            return "out of memory";

        case XmlReader::kErrorUnknownEncoding:
            return "unknown character encoding";

        case XmlReader::kErrorIncorrectEncoding:
            return "invalid encoded character";

        case XmlReader::kErrorPartialChar:
            return "incomplete encoded character";

        case XmlReader::kErrorRecursionDepthExceeded:
            return "recursion depth exceeded";

        case XmlReader::kErrorSyntax:
            return "syntax error";

        case XmlReader::kErrorInvalidToken:
            return "invalid token";

        case XmlReader::kErrorUnclosedToken:
            return "unclosed token";

        case XmlReader::kErrorUndefinedEntity:
            return "undefined entity";

        case XmlReader::kErrorBadCharRef:
            return "bad character reference";

        case XmlReader::kErrorMissingSemicolon:
            return "missing semicolon in entity reference";

        case XmlReader::kErrorCantDefineEntitiesOnceParsing:
            return "can't define entities after document parsing has begin";

        case XmlReader::kErrorIncompletePI:
            return "incomplete processing instruction";

        default:
            return "unknown error";
    }
}


} // namespace XML
} // namespace EA






