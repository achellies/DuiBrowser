/*
 * Copyright (C) 2000 Peter Kelly (pmk@post.com)
 * Copyright (C) 2005, 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Samuel Weinig (sam@webkit.org)
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#ifndef XMLTokenizer_h
#define XMLTokenizer_h

#include "CachedResourceClient.h"
#include "SegmentedString.h"
#include "StringHash.h"
#include "Tokenizer.h"
#include <wtf/HashMap.h>
#include <wtf/OwnPtr.h>
#include <EAWebKit/EAWebKitConfig.h> // For WTF_USE_UTFXML / WTF_USE_LIBXML2 defines


///////////////////////////////////////////////////////////////////////////////
// Note by Paul Pedriana regarding this EA modification
// This file is a modified version of the original /WebCore/dom/XMLTokenizer.h file.
// It is an override of that file for the purposes of supporting EA's UTFXml as an 
// alternative to Libxml2 open source. Aside from avoiding open source, an advantage
// of using our own tech is that it lowers the additional footprint created by EAWebKit,
// as a number of users are likely to be running UTFXml already.
// The files we have overridden include:
//     XMLTokenizer.h             (as BCXMLTokenizer.h, this file)
//     XMLTokenizer.cpp           (as BCXMLTokenizer.cpp, originally in /WebCore/dom/XMLTokenizer.cpp)
//     XMLTokenizerLibxml2.cpp    (as BCXMLTokenizerLibxml2.cpp, originally in /WebCore/dom/XMLTokenizer.cpp. We have this override in order to allow falling back to libxml2 if needed.)
// and we have created a new file:
//     BCXMLTokenizerEA.cpp       (this is an alternative to XMLTokenizerLibxml2.cpp which uses UTFXml)  
// We have disabled the WebCore/dom/XMLTokenizer cpp files in our build file and added BCXMLTokenizerEA.cpp
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Added by Paul Pedriana 
// This sets UTFXml usage in case it isn't already defined by the build system.
// Normally in our nant build we have this defined in the EAWebKit.build file,
// and that setting will take precedence.
#ifndef WTF_USE_LIBXML2
    #if USE(QXMLSTREAM) || USE(UTFXML)
        #define WTF_USE_LIBXML2 0
    #else
        #define WTF_USE_LIBXML2 1
    #endif
#endif
///////////////////////////////////////////////////////////////////////////////


#if USE(QXMLSTREAM)
#include <QtXml/qxmlstream.h>
#elif USE(UTFXML)
    #include <EAWebKit/EAWebKitUtfXmlInterface.h>
#else
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#endif

namespace WebCore {

    class Node;
    class CachedScript;
    class DocLoader;
    class DocumentFragment;
    class Document;
    class Element;
    class FrameView;
    class PendingCallbacks;
    class ScriptElement;

    class XMLTokenizer : public Tokenizer, public CachedResourceClient {
    public:
        XMLTokenizer(Document*, FrameView* = 0);
        XMLTokenizer(DocumentFragment*, Element*);
        ~XMLTokenizer();

        enum ErrorType { warning, nonFatal, fatal };

        // from Tokenizer
        virtual bool write(const SegmentedString&, bool appendData);
        virtual void finish();
        virtual bool isWaitingForScripts() const;
        virtual void stopParsing();

        void end();

        void pauseParsing();
        void resumeParsing();

        void setIsXHTMLDocument(bool isXHTML) { m_isXHTMLDocument = isXHTML; }
        bool isXHTMLDocument() const { return m_isXHTMLDocument; }

        // from CachedResourceClient
        virtual void notifyFinished(CachedResource* finishedObj);


        void handleError(ErrorType type, const char* m, int lineNumber, int columnNumber);

        virtual bool wellFormed() const { return !m_sawError; }

        int lineNumber() const;
        int columnNumber() const;

#if USE(QXMLSTREAM) || USE(UTFXML)
private:
        void parse();
        void startDocument();
        void parseStartElement();
        void parseEndElement();
        void parseCharacters();
        void parseProcessingInstruction();
        void parseCdata();
        void parseComment();
        void endDocument();
        void parseDtd();
        bool hasError() const;
#else
public:
        // callbacks from parser SAX
        void error(ErrorType, const char* message, va_list args) WTF_ATTRIBUTE_PRINTF(3, 0); 
        void startElementNs(const xmlChar* xmlLocalName, const xmlChar* xmlPrefix, const xmlChar* xmlURI, int nb_namespaces,
                            const xmlChar** namespaces, int nb_attributes, int nb_defaulted, const xmlChar** libxmlAttributes);
        void endElementNs();
        void characters(const xmlChar* s, int len);
        void processingInstruction(const xmlChar* target, const xmlChar* data);
        void cdataBlock(const xmlChar* s, int len);
        void comment(const xmlChar* s);
        void startDocument(const xmlChar* version, const xmlChar* encoding, int standalone);
        void internalSubset(const xmlChar* name, const xmlChar* externalID, const xmlChar* systemID);
        void endDocument();
#endif
    private:
        friend bool parseXMLDocumentFragment(const String& chunk, DocumentFragment* fragment, Element* parent);

        // static void eventuallyMarkAsParserCreated(Element* element);
        void initializeParserContext(const char* chunk = 0);
        void setCurrentNode(Node*);

        void insertErrorMessageBlock();

        bool enterText();
        void exitText();

        void doWrite(const String&);
        void doEnd();

        Document* m_doc;
        FrameView* m_view;

        String m_originalSourceForTransform;

#if USE(QXMLSTREAM)
        QXmlStreamReader m_stream;
        bool m_wroteText;
#elif USE(UTFXML)
        EA::Internal::XML::IXmlReader* mpXmlReader;
        bool m_wroteText;
#else
        xmlParserCtxtPtr m_context;
        OwnPtr<PendingCallbacks> m_pendingCallbacks;
        Vector<xmlChar> m_bufferedText;
#endif
        Node* m_currentNode;
        bool m_currentNodeIsReferenced;

        bool m_sawError;
        bool m_sawXSLTransform;
        bool m_sawFirstElement;
        bool m_isXHTMLDocument;

        bool m_parserPaused;
        bool m_requestingScript;
        bool m_finishCalled;

        int m_errorCount;
        int m_lastErrorLine;
        int m_lastErrorColumn;
        String m_errorMessages;

        CachedScript* m_pendingScript;
        RefPtr<Element> m_scriptElement;
        int m_scriptStartLine;

        bool m_parsingFragment;
        String m_defaultNamespaceURI;

        typedef HashMap<String, String> PrefixForNamespaceMap;
        PrefixForNamespaceMap m_prefixToNamespaceMap;
        SegmentedString m_pendingSrc;
    };

    #if ENABLE(XSLT)
    void* xmlDocPtrForString(DocLoader*, const String& source, const String& url);
    void setLoaderForLibXMLCallbacks(DocLoader*);
    #endif

    HashMap<String, String> parseAttributes(const String&, bool& attrsOK);
    bool parseXMLDocumentFragment(const String&, DocumentFragment*, Element* parent = 0);

    // bool isScriptElement(Element*);
    // ScriptElement* castToScriptElement(Element*);

} // namespace WebCore

#endif // XMLTokenizer_h
