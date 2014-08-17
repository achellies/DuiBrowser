/*
 * Copyright (C) 2000 Peter Kelly (pmk@post.com)
 * Copyright (C) 2005, 2006, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2007 Samuel Weinig (sam@webkit.org)
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008 Holger Hans Peter Freyther
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
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
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2008-2009
*/

/////////////////////////////////////////////////////////////////////////////
// BCXmlTokenizerEA.cpp
// Modified by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ReadMe
//
// This file is an attempt to convert WebKit's XmlTokenizerQt.cpp to 
// use UTFXml::XmlReader instead of XmlReader. It turns out that
// XmlReader is much like the Qt XmlReader. An alternative is XmlTokenizerLibxml2
// which is a SAX reader and thus is more complicated with little benefit.
// To implement this code I sync'd forward to a version of WebKit's XmlTokenizer
// code (September 18, 2008) which made the separation between XmlTokenizerLibxml2
// and XmlReader cleaner. I may have screwed something up in that 
// integration. WebKit's code is churning too much to feel confident about
// manual integrations these days.
//
// Test page:
//     http://www.martinpayne.me.uk/experiments/www/XMLHttpRequest/XMLHttpRequest.xhtml
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "XMLTokenizer.h"

#if USE(UTFXML)

#include "CDATASection.h"
#include "CString.h"
#include "CachedScript.h"
#include "Comment.h"
#include "DocLoader.h"
#include "Document.h"
#include "DocumentFragment.h"
#include "DocumentType.h"
#include "EventNames.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameView.h"
#include "HTMLNames.h"
#include "HTMLScriptElement.h"
#include "HTMLLinkElement.h"
#include "HTMLStyleElement.h"
#include "HTMLTokenizer.h"
#include "ScriptController.h"
// #include "ScriptElement.h"
#include "ProcessingInstruction.h"
#include "ResourceError.h"
#include "ResourceHandle.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"
#include "TextResourceDecoder.h"
#include <wtf/Platform.h>
#include <wtf/StringExtras.h>
#include <wtf/Threading.h>
#include <wtf/Vector.h>
#include <string.h>
#include <stdio.h>
#include <EAWebkit/internal/EAWebKitString.h>
#include <EAWebkit/internal/EAWebKitUtfXmlWrapper.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
using namespace std;

// Some debug trace code we can enable/disable.
#ifndef EA_XMLTOKENIZER_TRACE
    #ifdef EA_DEBUG
        #define EA_XMLTOKENIZER_TRACE 0 // Set to 1 in order to enable XML debug tracing in debug builds.
    #else
        #define EA_XMLTOKENIZER_TRACE 0
    #endif
#endif


namespace WebCore {

// 6/26/09 CSidhall - Disabled for now moved directly into wrapper (see EAWEbKitUTFXMLWrapper.cpp)
/*
// This is a callback that UTFXml calls when it encounters an entity (e.g. &amp;) that it 
// is not familiar with. This function converts it to a Unicode character (e.g. &).
bool UTFXmlEntityLookup(EA::Internal::XML::IXmlReader* pReader, const char* pEntityName, void* )
{
    const UChar c = WebCore::decodeNamedEntity(pEntityName);

    if(c)
    {
        pReader->PushInputBuffer(&c, 1); // PushInputBuffer will copy c.
        return true;
    }

    return false;
}
*/

XMLTokenizer::XMLTokenizer(Document* _doc, FrameView* _view)
    : m_doc(_doc)
    , m_view(_view)
    , m_wroteText(false)
    , m_currentNode(_doc)
    , m_currentNodeIsReferenced(false)
    , m_sawError(false)
    , m_sawXSLTransform(false)
    , m_sawFirstElement(false)
    , m_isXHTMLDocument(false)
    , m_parserPaused(false)
    , m_requestingScript(false)
    , m_finishCalled(false)
    , m_errorCount(0)
    , m_lastErrorLine(0)
    , m_lastErrorColumn(0)
    , m_pendingScript(0)
    , m_scriptStartLine(0)
    , m_parsingFragment(false)
{
    // Create the reader interface
    mpXmlReader = EA::XMLWrapper::CreateXMLReaderWrapperInterface();
    EAW_ASSERT(mpXmlReader);
    if(mpXmlReader) {
        mpXmlReader->SetFlag(0xffffffff, true);

        // Set up the decoder to be used in the callback (static so one for all xmlReaders)
        mpXmlReader->SetEntityNamedDecoder( (EA::Internal::XML::decode_entity_named*) WebCore::decodeNamedEntity);
    }
}

XMLTokenizer::XMLTokenizer(DocumentFragment* fragment, Element* parentElement)
    : m_doc(fragment->document())
    , m_view(0)
    , m_wroteText(false)
    , m_currentNode(fragment)
    , m_currentNodeIsReferenced(fragment)
    , m_sawError(false)
    , m_sawXSLTransform(false)
    , m_sawFirstElement(false)
    , m_isXHTMLDocument(false)
    , m_parserPaused(false)
    , m_requestingScript(false)
    , m_finishCalled(false)
    , m_errorCount(0)
    , m_lastErrorLine(0)
    , m_lastErrorColumn(0)
    , m_pendingScript(0)
    , m_scriptStartLine(0)
    , m_parsingFragment(true)
{
    mpXmlReader = EA::XMLWrapper::CreateXMLReaderWrapperInterface();
    EAW_ASSERT(mpXmlReader);
    if(mpXmlReader) {
        mpXmlReader->SetFlag(0xffffffff, true);
        
        // Set up the decoder to be used in the callback (static so one for all xmlReaders)
        mpXmlReader->SetEntityNamedDecoder( (EA::Internal::XML::decode_entity_named*) WebCore::decodeNamedEntity);
    }

    if (fragment)
        fragment->ref();
    if (m_doc)
        m_doc->ref();

    // Add namespaces based on the parent node
    Vector<Element*> elemStack;
    while (parentElement) {
        elemStack.append(parentElement);
        
        Node* n = parentElement->parentNode();
        if (!n || !n->isElementNode())
            break;
        parentElement = static_cast<Element*>(n);
    }

    if (!elemStack.isEmpty())
    {
        for (Element* element = elemStack.last(); !elemStack.isEmpty(); elemStack.removeLast()) {
            if (NamedAttrMap* attrs = element->attributes()) {
                for (unsigned i = 0; i < attrs->length(); i++) {
                    Attribute*          attr       = attrs->attributeItem(i);
                    const AtomicString& sValue     = attr->value();
                    const AtomicString& sLocalName = attr->localName();

                    if (attr->localName() == "xmlns")
                        m_defaultNamespaceURI = sValue;
                    else if (attr->prefix() == "xmlns")
                    {
                        char8_t localName8[32];
                        EA::Internal::Strlcpy(localName8, sLocalName.characters(), sizeof(localName8), sLocalName.length());

                        char8_t value8[128];
                        EA::Internal::Strlcpy(value8, sValue.characters(), sizeof(value8), sValue.length());

                        mpXmlReader->AddNamespaceAlias(localName8, value8, true);
                    }
                }
            }
        }

        // If the parent element is not in document tree, there may be no xmlns attribute; just default to the parent's namespace.
        if (m_defaultNamespaceURI.isNull() && !parentElement->inDocument())
            m_defaultNamespaceURI = parentElement->namespaceURI();
    }
}


XMLTokenizer::~XMLTokenizer()
{
    if(mpXmlReader) {
        EA::XMLWrapper::DestroyXMLReaderWrapperInterface(mpXmlReader);
        mpXmlReader = 0;
    }
    setCurrentNode(0);

    if (m_parsingFragment && m_doc)
        m_doc->deref();

    if (m_pendingScript)
        m_pendingScript->removeClient(this);
}


// Note by Paul Pedriana: The name of this function (doWrite) doesn't mean that
// it's writing a new XML file, it means that more XML data is read to add to 
// the XML input stream to our XmlReader. So it's writing into the XML input buffer.
void XMLTokenizer::doWrite(const String& parseString)
{
    // Debug code
    // FILE* pFile = fopen("C:\\temp\\EAWebKitTest.xml", "w");
    // if(pFile)
    // {
    //     fwrite(parseString.characters(), 1, parseString.length(), pFile);
    //     fclose(pFile);
    // }
    // End debug code

    m_wroteText = true;

    if (parseString.length()) {
        mpXmlReader->AppendInputBuffer(parseString.characters(), parseString.length() * sizeof(char16_t), EA::Internal::XML::kReadEncodingUTF16, NULL, true);
        parse();
    }

    if (m_doc->decoder() && m_doc->decoder()->sawError()) {
        // If the decoder saw an error, report it as fatal (stops parsing)
        handleError(fatal, "XML encoding error", lineNumber(), columnNumber());
    }
}


void XMLTokenizer::initializeParserContext(const char* /*chunk*/)
{
    m_parserStopped   = false;
    m_sawError        = false;
    m_sawXSLTransform = false;
    m_sawFirstElement = false;
}


void XMLTokenizer::doEnd()
{
    #if ENABLE(XSLT)
        #warning Look at XMLTokenizerLibxml2.cpp
    #endif
                                      // QT uses PrematureEndOfDocumentError here, which we need to better emulate.
    if (mpXmlReader->GetResultCode() == EA::Internal::XML::kErrorUnclosedToken || (m_wroteText && !m_sawFirstElement)) {
        handleError(fatal, "XML incomplete content error", lineNumber(), columnNumber());
    }
}


#if ENABLE(XSLT)
    void* xmlDocPtrForString(DocLoader* docLoader, const String& source, const String& url)
    {
        if (source.isEmpty())
            return 0;

        // Parse in a single chunk into an xmlDocPtr
        // FIXME: Hook up error handlers so that a failure to parse the main document results in
        // good error messages.
        const UChar BOM = 0xFEFF;
        const unsigned char BOMHighByte = *reinterpret_cast<const unsigned char*>(&BOM);

        xmlGenericErrorFunc oldErrorFunc = xmlGenericError;
        void* oldErrorContext = xmlGenericErrorContext;
        
        setLoaderForLibXMLCallbacks(docLoader);        
        xmlSetGenericErrorFunc(0, errorFunc);
        
        xmlDocPtr sourceDoc = xmlReadMemory(reinterpret_cast<const char*>(source.characters()),
                                            source.length() * sizeof(UChar),
                                            url.latin1().data(),
                                            BOMHighByte == 0xFF ? "UTF-16LE" : "UTF-16BE", 
                                            XSLT_PARSE_OPTIONS);
        
        setLoaderForLibXMLCallbacks(0);
        xmlSetGenericErrorFunc(oldErrorContext, oldErrorFunc);
        
        return sourceDoc;
    }
#endif


int XMLTokenizer::lineNumber() const
{
    return mpXmlReader->GetLineNumber();
}


int XMLTokenizer::columnNumber() const
{
    return mpXmlReader->GetColumnNumber();
}


void XMLTokenizer::stopParsing()
{
    Tokenizer::stopParsing();
}


void XMLTokenizer::resumeParsing()
{
    ASSERT(m_parserPaused);
    
    m_parserPaused = false;

    // First, execute any pending callbacks
    parse();
    if (m_parserPaused)
        return;

    // Then, write any pending data
    SegmentedString rest = m_pendingSrc;
    m_pendingSrc.clear();
    write(rest, false);

    // Finally, if finish() has been called and write() didn't result
    // in any further callbacks being queued, call end()
    if (m_finishCalled && !m_parserPaused && !m_pendingScript)
        end();
}

#include <wtf/FastAllocBase.h>

// Used by parseAttributes below.
struct AttributeParseState/*: public WTF::FastAllocBase*/ {
    HashMap<String, String> attributes;
    bool gotAttributes;
};

// Used by parseAttributes below.
static void attributesStartElementNsHandler(AttributeParseState& state, const EA::Internal::XML::IXmlReader* pXmlReader)
{
    int32_t attributeCount = pXmlReader->GetAttributeCount();

    if (attributeCount > 0) {
        state.gotAttributes = true;

        for(int32_t i = 0; i < attributeCount; i++) {
            const char* pName  = pXmlReader->GetAttributeName(i);
            const char* pValue = pXmlReader->GetAttributeValue(i);

            String attrQName = pName;
            String attrValue = pValue;

            state.attributes.set(attrQName, attrValue);
        }
    }
}

HashMap<String, String> parseAttributes(const String& string, bool& attrsOK)
{
    using namespace EA::Internal::XML;

    AttributeParseState state;
    state.gotAttributes = false;

    IXmlReader* pXmlReader = EA::XMLWrapper::CreateXMLReaderWrapperInterface();
    if(!pXmlReader)
        return state.attributes;
    
    const String parseString = "<?xml version=\"1.0\"?><attrs " + string + " />";

    pXmlReader->PushInputBuffer(parseString.characters(), parseString.length() * sizeof(char16_t), EA::Internal::XML::kReadEncodingUTF16, NULL, false);

    bool bSuccess = true;

    while(bSuccess && pXmlReader->Read())
    {
        const NodeType nodeType = pXmlReader->GetNodeType();

        if (nodeType == EA::Internal::XML::Element)
            attributesStartElementNsHandler(state, pXmlReader);
    }

    EA::XMLWrapper::DestroyXMLReaderWrapperInterface(pXmlReader);

    attrsOK = state.gotAttributes;
    
    return state.attributes;
}



// This function seems to have the sole purpose of testing a chunk of XML code.
bool parseXMLDocumentFragment(const String& chunk, DocumentFragment* fragment, Element* parent)
{
    if (!chunk.length())
        return true;

    XMLTokenizer tokenizer(fragment, parent);
    
    tokenizer.write(String("<utfxml>"), false);
    tokenizer.write(chunk, false);
    tokenizer.write(String("</utfxml>"), false);
    tokenizer.finish();
    return !tokenizer.hasError();
}




///////////////////////////////////////////////////////////////////////////////
// We need to handle XML namespaces. The WebCore XML DOM expects elements and
// attributes to be created with WebCore QualifiedNames, where QualifiedName
// is an XML qualified name.
//
// See http://www.w3schools.com/XML/xml_namespaces.asp
// See http://www.w3.org/TR/REC-xml-names/#sec-namespaces
// See http://www.w3.org/TR/REC-xml-names/#ns-qualnames
//
// Example:
//    <h:table xmlns:h="http://www.w3.org/TR/html4/">
//       <h:tr>
//       <h:td>Apples</h:td>
//       <h:td>Bananas</h:td>
//       </h:tr>
//    </h:table>
//    
// The QualifiedName class has the following:
//    AtomicString m_prefix;        // Same as 'h' above
//    AtomicString m_localName;     // Same as 'table' above
//    AtomicString m_namespace;     // Same as 'http://www.w3.org.TR/html4/' above
//
///////////////////////////////////////////////////////////////////////////////

static inline void handleElementNamespaces(WebCore::Element* newElement, const EA::Internal::XML::IXmlReader* pXmlReader, WebCore::ExceptionCode& ec)
{
    for (int32_t i = 0, count = pXmlReader->GetElementNamespaceCount(); i < count; ++i)
    {
        const char* pNamespace;
        const char* pPrefix        = pXmlReader->GetElementNamespace(i, &pNamespace);
        String      namespaceURI   = pPrefix;
        String      namespaceQName = pPrefix && pPrefix[0] ? String("xmlns:") + pPrefix : String("xmlns");

        newElement->setAttributeNS("http://www.w3.org/2000/xmlns/", namespaceQName, namespaceURI, ec);

        if (ec) // exception setting attributes
            return;
    }
}


static inline void handleElementAttributes(WebCore::Element* newElement, const EA::Internal::XML::IXmlReader* pXmlReader, WebCore::ExceptionCode& ec)
{
    for (int32_t i = 0, count = pXmlReader->GetAttributeCount(); i < count; ++i)
    {
        /* Libxml2 implementation:
        String attrLocalName = toString(attributes[i].localname);
        int    valueLength   = (int) (attributes[i].end - attributes[i].value);
        String attrValue     = toString(attributes[i].value, valueLength);
        String attrPrefix    = toString(attributes[i].prefix);
        String attrURI       = attrPrefix.isEmpty() ? String() : toString(attributes[i].uri);
        String attrQName     = attrPrefix.isEmpty() ? attrLocalName : attrPrefix + ":" + attrLocalName;

           QXml implementation:
        const QXmlStreamAttribute &attr = attrs[i];
        String attrLocalName = attr.name();
        String attrValue     = attr.value();
        String attrURI       = attr.namespaceUri().isEmpty() ? String() : String(attr.namespaceUri());
        String attrQName     = attr.qualifiedName();
        */

        const char* pName      = pXmlReader->GetAttributeName(i);
        const char* pLocalName = pXmlReader->GetAttributeLocalName(i);
        const char* pValue     = pXmlReader->GetAttributeValue(i);

        String attrLocalName = pLocalName;
        String attrValue     = pValue;
        String attrQName     = pName;
        String attrURI       = (pName == pLocalName) ? String() : String(pXmlReader->GetNamespaceURI()); // This GetNamespaceURI() usage is probably wrong, though it may work in a lot of cases. We need to use the attribute's resolved namespace URI here.

        newElement->setAttributeNS(attrURI, attrQName, attrValue, ec);

        if (ec) // exception setting attributes
            return;
    }
}


void XMLTokenizer::parse()
{
    using namespace EA::Internal::XML;

    // XmlReader currently has a bug in which it doesn't iterate the XmlReader::Document "node type".
    // So we work around it by calling startDocument ourselves manually.
    bool bDocumentNodeSeen = false;

    while (!m_parserStopped && !m_parserPaused) {
        bool result = mpXmlReader->Read();

        if(!result)
        {
            endDocument();
            break;
        }

        if(!bDocumentNodeSeen)
        {
            bDocumentNodeSeen = true;
            startDocument();
        }

        const EA::Internal::XML::NodeType nodeType = mpXmlReader->GetNodeType();

        switch (nodeType) {
            // XmlReader doesn't seem to set the Document state, so this doesn't get called. 
            // We handle startDocument above manually.
            // case XmlReader::Document:
            //     startDocument();
            //     break;

            // XmlReader doesn't currently have an EndDocument type. Instead it returns false from Read.
            // case XmlReader::EndDocument:
            //     endDocument();
            //     break;

            case EA::Internal::XML::Element:
                parseStartElement();
                if(mpXmlReader->IsEmptyElement())
                    parseEndElement(); // XmlReader requires us to manually end a standalone element.
                break;

            case EA::Internal::XML::EndElement:
                parseEndElement();
                break;

            case EA::Internal::XML::CharacterData:
                if (mpXmlReader->GetIsCData())
                    parseCdata();
                else 
                    parseCharacters();
                break;

            case EA::Internal::XML::Comment:
                parseComment();
                break;

            case EA::Internal::XML::DocTypeDecl:
                parseDtd();
                break;

            case EA::Internal::XML::EntityRef:
                if (isXHTMLDocument()) {
                    const UChar c = WebCore::decodeNamedEntity(mpXmlReader->GetName());

                    if (m_currentNode->isTextNode() || enterText()) {
                        ExceptionCode ec = 0;
                        String str(&c, 1);
                        static_cast<Text*>(m_currentNode)->appendData(str, ec);
                    }
                }
                break;

            case EA::Internal::XML::ProcessingInstruction:
                parseProcessingInstruction();
                break;

            case EA::Internal::XML::Prologue:
            case EA::Internal::XML::ElementDecl:
            case EA::Internal::XML::AttListDecl:
            case EA::Internal::XML::NotationDecl:
                // Ignore these.
                break;

            case EA::Internal::XML::None:
            default: {
                const EA::Internal::XML::ResultCode result = mpXmlReader->GetResultCode();

                if (result != EA::Internal::XML::kErrorUnclosedToken) // For some reason, QtXml and Libxml2 have this 'if' in their code.
                    handleError(XMLTokenizer::fatal, "XML content error",  mpXmlReader->GetLineNumber(), mpXmlReader->GetColumnNumber());

                break;
            }
        }
    }
}


void XMLTokenizer::startDocument()
{
    initializeParserContext();
    ExceptionCode ec = 0;

    if (!m_parsingFragment) {
        m_doc->setXMLStandalone(mpXmlReader->GetIsStandalone(), ec);

        // To do: Implement this. Currently UTFXml's XmlReader doesn't have the functions below, but they are easy enough to implement.
        // Just read the version and encoding in <?xml version="1.0" encoding="UTF-8" standalone="no"?> in XmlReader::ParseProcessingInstruction(). 
        // const WebCore::String version = mpXmlReader->GetDocumentVersion();
        // if (!version.isEmpty())
        //     m_doc->setXMLVersion("1.0", ec);
        //
        // const WebCore::encoding = mpXmlReader->GetDocumentEncoding();
        // if (!encoding.isEmpty())
        //     m_doc->setXMLEncoding("UTF-8");
    }
}


void XMLTokenizer::parseStartElement()
{
    if (!m_sawFirstElement && m_parsingFragment) {  // If we are parsing a document fragment and this is the first element of it...
        // skip dummy element for fragments         // Ignore it, because we wrap document fragments in a bogus element in order to simply make it work like a well-formed XML file.
        m_sawFirstElement = true;
        return;
    }

    m_sawFirstElement = true;

    exitText();

    // To do: use EA::XML::Strlcpy instead of this .decode stuff, as it would be more friendly to the heap.
    WebCore::String localName;
    const char* pName = mpXmlReader->GetName();
    if(pName)  // This should always be valid.
        localName = UTF8Encoding().decode(pName, strlen(pName));

    WebCore::String uri;
    const char* pNamespaceURI = mpXmlReader->GetNamespaceURI();
    if(pNamespaceURI) // This may be NULL.
        uri = UTF8Encoding().decode(pNamespaceURI, strlen(pNamespaceURI));

    // Usually prefixLength is zero, as there won't usually be a namespace prefix.
    const size_t prefixLength = mpXmlReader->GetNamePrefixLength();
    WebCore::String prefix = UTF8Encoding().decode(pName, prefixLength);

    if (m_parsingFragment && uri.isNull()) {
        // ASSERT(prefix.isNull());
        uri = m_defaultNamespaceURI;
    }

    #if EA_XMLTOKENIZER_TRACE
        WTFLogEvent("XMLTokenizer start element: %s\n", pName);
    #endif

    WebCore::ExceptionCode ec = 0;
    WebCore::QualifiedName qName(prefix, localName, uri);
    RefPtr<Element> newElement = m_doc->createElement(qName, true, ec);
    if (!newElement) {
        stopParsing();
        return;
    }

    // WebCore expects that we copy all attributes to the dom element, including 
    // namespace attributes. All elements are expected to have namespaces. But 
    // UTFXml stores namespace attributes and regular attributes for a node 
    // separately, so we get them with the handleElementNamespaces and handleElementAttributes
    // functions.
    handleElementNamespaces(newElement.get(), mpXmlReader, ec);
    if (ec) {
        stopParsing();
        return;
    }

    handleElementAttributes(newElement.get(), mpXmlReader, ec);
    if (ec) {
        stopParsing();
        return;
    }

    if (newElement->hasTagName(HTMLNames::scriptTag))
        static_cast<HTMLScriptElement*>(newElement.get())->setCreatedByParser(true);
    else if (newElement->hasTagName(HTMLNames::styleTag))
        static_cast<HTMLStyleElement*>(newElement.get())->setCreatedByParser(true);
#if ENABLE(SVG)
    else if (newElement->hasTagName(SVGNames::styleTag))
        static_cast<SVGStyleElement*>(newElement.get())->setCreatedByParser(true);
#endif

    if (newElement->hasTagName(HTMLNames::scriptTag)
#if ENABLE(SVG)
     || newElement->hasTagName(SVGNames::scriptTag)
#endif
        )
        m_scriptStartLine = lineNumber();

    if (!m_currentNode->addChild(newElement.get())) {
        stopParsing();
        return;
    }

    setCurrentNode(newElement.get());
    if (m_view && !newElement->attached())
        newElement->attach();
}


void XMLTokenizer::parseEndElement()
{
    exitText();

    Node* n = m_currentNode;
    RefPtr<Node> parent = n->parentNode();
    n->finishParsingChildren();

    #if EA_XMLTOKENIZER_TRACE
        String sName16 = n->nodeName();
        const char16_t* pName16 = sName16.charactersWithNullTermination();
        WTFLogEvent("XMLTokenizer end element: %ls\n", pName16);
        if(wcscmp(pName16, L"abbr") == 0)
            WTFLogEvent("%s", "");
    #endif

    // don't load external scripts for standalone documents (for now)
    if (n->isElementNode() && m_view && (static_cast<Element*>(n)->hasTagName(HTMLNames::scriptTag) 
#if ENABLE(SVG)
                                      || static_cast<Element*>(n)->hasTagName(SVGNames::scriptTag)
#endif
       )) {
        ASSERT(!m_pendingScript);
        m_requestingScript = true;

        Element* scriptElement = static_cast<Element*>(n);        
        String scriptHref;
        
        if (static_cast<Element*>(n)->hasTagName(HTMLNames::scriptTag))
            scriptHref = scriptElement->getAttribute(HTMLNames::srcAttr);
#if ENABLE(SVG)
        else if (static_cast<Element*>(n)->hasTagName(SVGNames::scriptTag))
            scriptHref = scriptElement->getAttribute(XLinkNames::hrefAttr);
#endif

        if (!scriptHref.isEmpty()) {
            // we have a src attribute 
            const AtomicString& charset = scriptElement->getAttribute(HTMLNames::charsetAttr);
            if ((m_pendingScript = m_doc->docLoader()->requestScript(scriptHref, charset))) {
                m_scriptElement = scriptElement;
                m_pendingScript->addClient(this);
                    
                // m_pendingScript will be 0 if script was already loaded and ref() executed it
                if (m_pendingScript)
                    pauseParsing();
            } else 
                m_scriptElement = 0;

        } else {
            String scriptCode = "";
            for (Node* child = scriptElement->firstChild(); child; child = child->nextSibling()) {
                if (child->isTextNode() || child->nodeType() == Node::CDATA_SECTION_NODE)
                    scriptCode += static_cast<CharacterData*>(child)->data();
            }
            m_view->frame()->loader()->executeScript(m_doc->url().string(), m_scriptStartLine, scriptCode);
        }

        m_requestingScript = false;
    }

    setCurrentNode(parent.get());
}


void XMLTokenizer::parseCharacters()
{
    if (m_currentNode->isTextNode() || enterText()) {
        ExceptionCode ec = 0;

        const char* pText = mpXmlReader->GetValue();
        WebCore::String sText = UTF8Encoding().decode(pText, strlen(pText));

        static_cast<Text*>(m_currentNode)->appendData(sText, ec);
    }
}


void XMLTokenizer::parseProcessingInstruction()
{
    exitText();

    // ### handle exceptions
    int exception = 0;

    const char* pName = mpXmlReader->GetName();
    WebCore::String sName = UTF8Encoding().decode(pName, strlen(pName));

    const char* pValue = mpXmlReader->GetValue();
    WebCore::String sValue = UTF8Encoding().decode(pValue, strlen(pValue));

    RefPtr<ProcessingInstruction> pi = m_doc->createProcessingInstruction(sName, sValue, exception);

    if (exception)
        return;

    if (!m_currentNode->addChild(pi.get()))
        return;

    if (m_view && !pi->attached())
        pi->attach();

    pi->finishParsingChildren();

    #if ENABLE(XSLT)
        m_sawXSLTransform = !m_sawFirstElement && pi->isXSL();
        if (m_sawXSLTransform && !m_doc->transformSourceDocument()))
            stopParsing();
    #endif
}


void XMLTokenizer::parseCdata()
{
    exitText();

    const char* pText = mpXmlReader->GetValue();
    WebCore::String sText = UTF8Encoding().decode(pText, strlen(pText));
    RefPtr<Node> newNode = new CDATASection(m_doc, sText);

    if (!m_currentNode->addChild(newNode.get()))
        return;

    if (m_view && !newNode->attached())
        newNode->attach();
}


void XMLTokenizer::parseComment()
{
    exitText();

    const char* pText = mpXmlReader->GetValue();
    WebCore::String sText = UTF8Encoding().decode(pText, strlen(pText));
    RefPtr<Node> newNode = new Comment(m_doc, sText);

    m_currentNode->addChild(newNode.get());

    if (m_view && !newNode->attached())
        newNode->attach();
}


void XMLTokenizer::endDocument()
{
    exitText();
}


bool XMLTokenizer::hasError() const
{
    return mpXmlReader->GetResultCode() != EA::Internal::XML::kSuccess;
}


void XMLTokenizer::parseDtd()
{
    const char* name     = mpXmlReader->GetDocTypeName();
    const char* publicId = mpXmlReader->GetPublicId();
    const char* systemId = mpXmlReader->GetSystemId();

    if(publicId && strstr(publicId, "DTD XHTML")) // e.g. "-//W3C//DTD XHTML 1.0 Transitional//EN"
        setIsXHTMLDocument(true); // controls if we replace entities or not.

    if (!m_parsingFragment)
    {
		WebCore::String sName     = (name ? UTF8Encoding().decode(name,     strlen(name)) : WebCore::String(""));
		WebCore::String sPublicId = (publicId ? UTF8Encoding().decode(publicId, strlen(publicId)) : WebCore::String(""));
		WebCore::String sSystemId = (systemId ? UTF8Encoding().decode(systemId, strlen(systemId)) : WebCore::String(""));

        m_doc->addChild(DocumentType::create(m_doc, sName, sPublicId, sSystemId));
    }
}

} // namespace WebCore

#endif

