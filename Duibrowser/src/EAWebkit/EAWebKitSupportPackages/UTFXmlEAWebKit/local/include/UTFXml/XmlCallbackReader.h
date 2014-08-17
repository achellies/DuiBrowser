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
// Copyright (c) 2006, Electronic Arts Inc. All rights reserved.
// Written by Talin and Paul Pedriana
//
// SAX-style callback parser for XML.
/////////////////////////////////////////////////////////////////////////////

#ifndef UTFXML_XMLCALLBACKREADER_H
#define UTFXML_XMLCALLBACKREADER_H

#include <UTFXml/internal/Config.h>
#ifndef UTFXML_XMLREADER_H
#include <UTFXml/XmlReader.h>
#endif

namespace EA {

    namespace XML {

        //////////////////////////////////////////////////////////////////////////
        /// Callback interface for XML events.
        ///
        /// This interface is not pure-virtual; rather it provides a default 
        /// implementation of each function. Thus you can override just the 
        /// functions you want and let this base class handle any others.
        ///
        /// The pAttributes parameter is an array of string pointers, with the 
        /// array being pairs of name and value. pAttributes[0] is the first 
        /// name, while pAttributes[1] is the value associated with pAttributes[0].
        /// And so on for pAttributes[2] and [3]. nAttributeCount is equal to 
        /// the number of pairs of attributes and thus is equal to the total
        /// number of pAttributes entries divided by two.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API IContentHandler {
        public:
            virtual ~IContentHandler() {}

            virtual void StartDocument();
            virtual void EndDocument();

            virtual bool StartElement( const char * pElementName, const char ** pAttributes, size_t nAttributeCount );
            virtual bool EndElement( const char * pElementName );

            virtual bool Characters( const char * pText, size_t nLength ); // To consider: Change this to CharacterData()
            virtual bool Comment( const char * pText, size_t nLength );

            virtual bool ProcessingInstruction( const char * pTarget, const char * pData );
            virtual bool SkippedEntity( const char * pName ); // To consider: Change this to EntityRef().
        };


        //////////////////////////////////////////////////////////////////////////
        /// A SAX-style XML Reader, using XmlReader as a base.
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API XmlCallbackReader : public XmlReader {
        public:
            /// Default constructor
            XmlCallbackReader( Allocator::ICoreAllocator * pAllocator = NULL, size_t bufferBlockSize = 0 );

            /// Parse the document specified with PushInputStream or PushInputBuffer, 
            /// calling IContentHandler in the process.
            bool Parse(IContentHandler * pContentHandler = NULL);

            /// This allows you to dynamically change the content handler, 
            /// possibly while in the process of iteration. 
            void SetContentHandler( IContentHandler * pContentHandler );

            /// Get the content handler.
            IContentHandler * GetContentHandler() const;

        protected:
            IContentHandler * mpContentHandler;
        };

    } // namespace XML

} // namespace EA







namespace EA {
    namespace XML {

        inline XmlCallbackReader::XmlCallbackReader( Allocator::ICoreAllocator * pAllocator, size_t bufferBlockSize ) 
          : XmlReader( pAllocator, bufferBlockSize )
        {
        }

        inline void XmlCallbackReader::SetContentHandler( IContentHandler * pContentHandler )
        {
            mpContentHandler = pContentHandler;
        }

        inline IContentHandler * XmlCallbackReader::GetContentHandler() const
        {
            return mpContentHandler;
        }
    }
}


#endif // Header include guard





