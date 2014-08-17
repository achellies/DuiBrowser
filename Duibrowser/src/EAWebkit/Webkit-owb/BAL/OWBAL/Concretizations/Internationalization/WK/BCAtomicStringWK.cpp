/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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
* This file was modified by Electronic Arts Inc Copyright ?2009-2010
*/

#include "config.h"

#ifdef AVOID_STATIC_CONSTRUCTORS
#define ATOMICSTRING_HIDE_GLOBALS 1
#endif

#include "AtomicString.h"

#include "StaticConstructors.h"
#include "StringHash.h"
#include <kjs/identifier.h>
#include <wtf/HashSet.h>

using KJS::Identifier;
using KJS::UString;

namespace OWBAL {

static HashSet<StringImpl*>* stringTable;
#include <wtf/FastAllocBase.h>

struct CStringTranslator/*: public WTF::FastAllocBase*/ {
    static unsigned hash(const char* c)
    {
        return StringImpl::computeHash(c);
    }

    static bool equal(StringImpl* r, const char* s)
    {
        int length = r->length();
        const UChar* d = r->characters();
        for (int i = 0; i != length; ++i) {
            unsigned char c = s[i];
            if (d[i] != c)
                return false;
        }
        return s[length] == 0;
    }

    static void translate(StringImpl*& location, const char* const& c, unsigned hash)
    {
		location = EAWEBKIT_NEW("translate:StringImpl")StringImpl(c, strlen(c), hash); 
    }
};

bool operator==(const AtomicString& a, const char* b)
{ 
    StringImpl* impl = a.impl();
    if ((!impl || !impl->characters()) && !b)
        return true;
    if ((!impl || !impl->characters()) || !b)
        return false;
    return CStringTranslator::equal(impl, b); 
}

PassRefPtr<StringImpl> AtomicString::add(const char* c)
{
    if (!c)
        return 0;
    if (!*c)
        return StringImpl::empty();    
    pair<HashSet<StringImpl*>::iterator, bool> addResult = stringTable->add<const char*, CStringTranslator>(c);
    if (!addResult.second)
        return *addResult.first;
    return adoptRef(*addResult.first);
}

struct UCharBuffer {
    const UChar* s;
    unsigned length;
};

static inline bool equal(StringImpl* string, const UChar* characters, unsigned length)
{
    if (string->length() != length)
        return false;

    // FIXME: perhaps we should have a more abstract macro that indicates when
    // going 4 bytes at a time is unsafe
#if PLATFORM(ARM) || PLATFORM(SH4) || PLATFORM(MIPS)
    const UChar* stringCharacters = string->characters();
    for (unsigned i = 0; i != length; ++i) {
        if (*stringCharacters++ != *characters++)
            return false;
    }
    return true;
#else
    /* Do it 4-bytes-at-a-time on architectures where it's safe */

    const uint32_t* stringCharacters = reinterpret_cast<const uint32_t*>(string->characters());
    const uint32_t* bufferCharacters = reinterpret_cast<const uint32_t*>(characters);

    unsigned halfLength = length >> 1;
    for (unsigned i = 0; i != halfLength; ++i) {
        if (*stringCharacters++ != *bufferCharacters++)
            return false;
    }

    if (length & 1 &&  *reinterpret_cast<const uint16_t*>(stringCharacters) != *reinterpret_cast<const uint16_t*>(bufferCharacters))
        return false;

    return true;
#endif
}

struct UCharBufferTranslator/*: public WTF::FastAllocBase*/ {
    static unsigned hash(const UCharBuffer& buf)
    {
        return StringImpl::computeHash(buf.s, buf.length);
    }

    static bool equal(StringImpl* const& str, const UCharBuffer& buf)
    {
        return WebCore::equal(str, buf.s, buf.length);
    }

    static void translate(StringImpl*& location, const UCharBuffer& buf, unsigned hash)
    {
        location = new StringImpl(buf.s, buf.length, hash); 
    }
};

struct HashAndCharacters {
    unsigned hash;
    const UChar* characters;
    unsigned length;
};

struct HashAndCharactersTranslator/*: public WTF::FastAllocBase*/ {
    static unsigned hash(const HashAndCharacters& buffer)
    {
        ASSERT(buffer.hash == StringImpl::computeHash(buffer.characters, buffer.length));
        return buffer.hash;
    }

    static bool equal(StringImpl* const& string, const HashAndCharacters& buffer)
    {
        return WebCore::equal(string, buffer.characters, buffer.length);
    }

    static void translate(StringImpl*& location, const HashAndCharacters& buffer, unsigned hash)
    {
        location = new StringImpl(buffer.characters, buffer.length, hash); 
    }
};

PassRefPtr<StringImpl> AtomicString::add(const UChar* s, int length)
{
    if (!s)
        return 0;

    if (length == 0)
        return StringImpl::empty();
    
    UCharBuffer buf = { s, length }; 
    pair<HashSet<StringImpl*>::iterator, bool> addResult = stringTable->add<UCharBuffer, UCharBufferTranslator>(buf);
    if (!addResult.second)
        return *addResult.first;
    return adoptRef(*addResult.first);
}

PassRefPtr<StringImpl> AtomicString::add(const UChar* s)
{
    if (!s)
        return 0;

    int length = 0;
    while (s[length] != UChar(0))
        length++;

    if (length == 0)
        return StringImpl::empty();

    UCharBuffer buf = {s, length}; 
    pair<HashSet<StringImpl*>::iterator, bool> addResult = stringTable->add<UCharBuffer, UCharBufferTranslator>(buf);
    if (!addResult.second)
        return *addResult.first;
    return adoptRef(*addResult.first);
}

PassRefPtr<StringImpl> AtomicString::add(StringImpl* r)
{
    if (!r || r->m_inTable)
        return r;

    if (r->length() == 0)
        return StringImpl::empty();
    
    StringImpl* result = *stringTable->add(r).first;
    if (result == r)
        r->m_inTable = true;
    return result;
}

void AtomicString::remove(StringImpl* r)
{
    stringTable->remove(r);
}

PassRefPtr<StringImpl> AtomicString::add(const KJS::Identifier& identifier)
{
    if (identifier.isNull())
        return 0;

    UString::Rep* string = identifier.ustring().rep();
    unsigned length = string->size();
    if (!length)
        return StringImpl::empty();

    HashAndCharacters buffer = { string->computedHash(), string->data(), length }; 
    pair<HashSet<StringImpl*>::iterator, bool> addResult = stringTable->add<HashAndCharacters, HashAndCharactersTranslator>(buffer);
    if (!addResult.second)
        return *addResult.first;
    return adoptRef(*addResult.first);
}

PassRefPtr<StringImpl> AtomicString::add(const KJS::UString& ustring)
{
    if (ustring.isNull())
        return 0;

    UString::Rep* string = ustring.rep();
    unsigned length = string->size();
    if (!length)
        return StringImpl::empty();

    HashAndCharacters buffer = { string->hash(), string->data(), length }; 
    pair<HashSet<StringImpl*>::iterator, bool> addResult = stringTable->add<HashAndCharacters, HashAndCharactersTranslator>(buffer);
    if (!addResult.second)
        return *addResult.first;
    return adoptRef(*addResult.first);
}

AtomicStringImpl* AtomicString::find(const KJS::Identifier& identifier)
{
    if (identifier.isNull())
        return 0;

    UString::Rep* string = identifier.ustring().rep();
    unsigned length = string->size();
    if (!length)
        return static_cast<AtomicStringImpl*>(StringImpl::empty());

    HashAndCharacters buffer = { string->computedHash(), string->data(), length }; 
    HashSet<StringImpl*>::iterator iterator = stringTable->find<HashAndCharacters, HashAndCharactersTranslator>(buffer);
    if (iterator == stringTable->end())
        return 0;
    return static_cast<AtomicStringImpl*>(*iterator);
}

AtomicString::operator UString() const
{
    return m_string;
}

DEFINE_GLOBAL(AtomicString, nullAtom)
DEFINE_GLOBAL(AtomicString, emptyAtom, "")
DEFINE_GLOBAL(AtomicString, textAtom, "#text")
DEFINE_GLOBAL(AtomicString, commentAtom, "#comment")
DEFINE_GLOBAL(AtomicString, starAtom, "*")

//+daw ca 28/07 static and global management, move static definition from HTMLInputElement
DEFINE_GLOBAL(AtomicString, buttonHTMLInputElement, "button")
DEFINE_GLOBAL(AtomicString, checkboxHTMLInputElement, "checkbox")
DEFINE_GLOBAL(AtomicString, fileHTMLInputElement, "file")
DEFINE_GLOBAL(AtomicString, hiddenHTMLInputElement, "hidden")
DEFINE_GLOBAL(AtomicString, imageHTMLInputElement, "image")
DEFINE_GLOBAL(AtomicString, passwordHTMLInputElement, "password")
DEFINE_GLOBAL(AtomicString, radioHTMLInputElement, "radio")
DEFINE_GLOBAL(AtomicString, rangeHTMLInputElement, "range")
DEFINE_GLOBAL(AtomicString, resetHTMLInputElement, "reset")
DEFINE_GLOBAL(AtomicString, searchHTMLInputElement, "search")
DEFINE_GLOBAL(AtomicString, submitHTMLInputElement, "submit")
DEFINE_GLOBAL(AtomicString, textHTMLInputElement, "text")

// Added by Paul Pedriana, 1/2009.
DEFINE_GLOBAL(AtomicString, optionAtom, "option")
DEFINE_GLOBAL(AtomicString, optgroupAtom, "optgroup")
DEFINE_GLOBAL(AtomicString, legendAtom, "legend")
DEFINE_GLOBAL(AtomicString, keygenAtom, "keygen")
DEFINE_GLOBAL(AtomicString, fieldsetAtom, "keygen")

//+daw ca 31/07 static and global management, move static definition from Document
DEFINE_GLOBAL(AtomicString, xmlnsNamespaceURI, "http://www.w3.org/2000/xmlns/")
DEFINE_GLOBAL(AtomicString, xmlns, "xmlns")
DEFINE_GLOBAL(AtomicString, xml, "xml")

//+daw ca 01/08 static and global management, move static definition from HTMLSelectElement
DEFINE_GLOBAL(AtomicString, selectMultiple, "select-multiple")
DEFINE_GLOBAL(AtomicString, selectOne, "select-one")

//+daw ca 01/08 static and global management, move static definition from HTMLButtonElement
DEFINE_GLOBAL(AtomicString, submitHTMLButtonElement, "submit")
DEFINE_GLOBAL(AtomicString, buttonHTMLButtonElement, "button")
DEFINE_GLOBAL(AtomicString, resetHTMLButtonElement, "reset")

// 

DEFINE_GLOBAL(AtomicString, noneHTMLTableElement, "none")
DEFINE_GLOBAL(AtomicString, solidHTMLTableElement, "solid")
DEFINE_GLOBAL(AtomicString, insetHTMLTableElement, "inset")
DEFINE_GLOBAL(AtomicString, solidcolsHTMLTableElement, "solid-cols")
DEFINE_GLOBAL(AtomicString, solidrowsHTMLTableElement, "solid-rows")

DEFINE_GLOBAL(AtomicString, javascriptHTMLScriptElement, "javascript")
DEFINE_GLOBAL(AtomicString, javascript10HTMLScriptElement, "javascript1.0")
DEFINE_GLOBAL(AtomicString, javascript11HTMLScriptElement, "javascript1.1")
DEFINE_GLOBAL(AtomicString, javascript12HTMLScriptElement, "javascript1.2")
DEFINE_GLOBAL(AtomicString, javascript13HTMLScriptElement, "javascript1.3")
DEFINE_GLOBAL(AtomicString, javascript14HTMLScriptElement, "javascript1.4")
DEFINE_GLOBAL(AtomicString, javascript15HTMLScriptElement, "javascript1.5")
DEFINE_GLOBAL(AtomicString, javascript16HTMLScriptElement, "javascript1.6")
DEFINE_GLOBAL(AtomicString, javascript17HTMLScriptElement, "javascript1.7")
DEFINE_GLOBAL(AtomicString, livescriptHTMLScriptElement, "livescript")
DEFINE_GLOBAL(AtomicString, ecmascriptHTMLScriptElement, "ecmascript")
DEFINE_GLOBAL(AtomicString, jscriptHTMLScriptElement, "jscript")

DEFINE_GLOBAL(AtomicString, textareaHTMLTextAreaElement, "textarea")
//-daw ca

//+daw ca 23/07 static and global management
bool AtomicString::m_st_bInitialized = false;


void AtomicString::init()
{
    if (!m_st_bInitialized) 
	{
        stringTable = new HashSet<StringImpl*>;

        // Use placement new to initialize the globals.
        new ((void*)&nullAtom) AtomicString;
        new ((void*)&emptyAtom) AtomicString("");
        new ((void*)&textAtom) AtomicString("#text");
        new ((void*)&commentAtom) AtomicString("#comment");
        new ((void*)&starAtom) AtomicString("*");

		new ((void*)&buttonHTMLInputElement) AtomicString("button");
		new ((void*)&checkboxHTMLInputElement) AtomicString("checkbox");
		new ((void*)&fileHTMLInputElement) AtomicString("file");
		new ((void*)&hiddenHTMLInputElement) AtomicString("hidden");
		new ((void*)&imageHTMLInputElement) AtomicString("image");
		new ((void*)&passwordHTMLInputElement) AtomicString("password");
		new ((void*)&radioHTMLInputElement) AtomicString("radio");
		new ((void*)&rangeHTMLInputElement) AtomicString("range");
		new ((void*)&resetHTMLInputElement) AtomicString("reset");
		new ((void*)&searchHTMLInputElement) AtomicString("search");
		new ((void*)&submitHTMLInputElement) AtomicString("submit");
		new ((void*)&textHTMLInputElement) AtomicString("text");

        // Added by Paul Pedriana, 1/2009.
		new ((void*)&optionAtom) AtomicString("option");
		new ((void*)&optgroupAtom) AtomicString("optgroup");
		new ((void*)&legendAtom) AtomicString("legend");
		new ((void*)&keygenAtom) AtomicString("keygen");
		new ((void*)&fieldsetAtom) AtomicString("keygen");

		new ((void*)&xmlnsNamespaceURI) AtomicString("http://www.w3.org/2000/xmlns/");
		new ((void*)&xmlns) AtomicString("xmlns");
		new ((void*)&xml) AtomicString("xml");

		new ((void*)&selectMultiple) AtomicString("select-multiple");
		new ((void*)&selectOne) AtomicString("select-one");

		new ((void*)&submitHTMLButtonElement) AtomicString("submit");
		new ((void*)&buttonHTMLButtonElement) AtomicString("button");
		new ((void*)&resetHTMLButtonElement) AtomicString("reset");

		new ((void*)&noneHTMLTableElement) AtomicString("none");
		new ((void*)&solidHTMLTableElement) AtomicString("solid");
		new ((void*)&insetHTMLTableElement) AtomicString("inset");
		new ((void*)&solidcolsHTMLTableElement) AtomicString("solid-cols");
		new ((void*)&solidrowsHTMLTableElement) AtomicString("solid-rows");

		new ((void*)&javascriptHTMLScriptElement) AtomicString("javascript");
		new ((void*)&javascript10HTMLScriptElement) AtomicString("javascript1.0");
		new ((void*)&javascript11HTMLScriptElement) AtomicString("javascript1.1");
		new ((void*)&javascript12HTMLScriptElement) AtomicString("javascript1.2");
		new ((void*)&javascript13HTMLScriptElement) AtomicString("javascript1.3");
		new ((void*)&javascript14HTMLScriptElement) AtomicString("javascript1.4");
		new ((void*)&javascript15HTMLScriptElement) AtomicString("javascript1.5");
		new ((void*)&javascript16HTMLScriptElement) AtomicString("javascript1.6");
		new ((void*)&javascript17HTMLScriptElement) AtomicString("javascript1.7");
		new ((void*)&livescriptHTMLScriptElement) AtomicString("livescript");
		new ((void*)&ecmascriptHTMLScriptElement) AtomicString("ecmascript");
		new ((void*)&jscriptHTMLScriptElement) AtomicString("jscript");

		new ((void*)&textareaHTMLTextAreaElement) AtomicString("textarea");
        m_st_bInitialized = true;
    }
}

void AtomicString::staticFinalize()
{
    // 6/16//09 CSidhall - Added pointer check for crash on exit
    if(stringTable){
    	stringTable->clear();
    	delete stringTable;
	    stringTable = NULL;
    }
}

void AtomicString::unInit()
{
	(*(WebCore::AtomicString*)(&nullAtom)).externalDeref();
	(*(WebCore::AtomicString*)(&emptyAtom)).externalDeref();
	(*(WebCore::AtomicString*)(&textAtom)).externalDeref();
	(*(WebCore::AtomicString*)(&commentAtom)).externalDeref();
	(*(WebCore::AtomicString*)(&starAtom)).externalDeref();

	(*(WebCore::AtomicString*)(&buttonHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&checkboxHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&fileHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&hiddenHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&imageHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&passwordHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&radioHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&rangeHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&resetHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&searchHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&submitHTMLInputElement)).externalDeref();
	(*(WebCore::AtomicString*)(&textHTMLInputElement)).externalDeref();

    // Added by Paul Pedriana, 1/2009.
	(*(WebCore::AtomicString*)(&optionAtom)).externalDeref();
	(*(WebCore::AtomicString*)(&optgroupAtom)).externalDeref();
	(*(WebCore::AtomicString*)(&legendAtom)).externalDeref();
	(*(WebCore::AtomicString*)(&keygenAtom)).externalDeref();
	(*(WebCore::AtomicString*)(&fieldsetAtom)).externalDeref();

	(*(WebCore::AtomicString*)(&xmlnsNamespaceURI)).externalDeref();
	(*(WebCore::AtomicString*)(&xmlns)).externalDeref();
	(*(WebCore::AtomicString*)(&xml)).externalDeref();

	(*(WebCore::AtomicString*)(&selectMultiple)).externalDeref();
	(*(WebCore::AtomicString*)(&selectOne)).externalDeref();

	(*(WebCore::AtomicString*)(&submitHTMLButtonElement)).externalDeref();
	(*(WebCore::AtomicString*)(&buttonHTMLButtonElement)).externalDeref();
	(*(WebCore::AtomicString*)(&resetHTMLButtonElement)).externalDeref();

	(*(WebCore::AtomicString*)(&noneHTMLTableElement)).externalDeref();
	(*(WebCore::AtomicString*)(&solidHTMLTableElement)).externalDeref();
	(*(WebCore::AtomicString*)(&insetHTMLTableElement)).externalDeref();
	(*(WebCore::AtomicString*)(&solidcolsHTMLTableElement)).externalDeref();
	(*(WebCore::AtomicString*)(&solidrowsHTMLTableElement)).externalDeref();

	(*(WebCore::AtomicString*)(&javascriptHTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&javascript10HTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&javascript11HTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&javascript12HTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&javascript13HTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&javascript14HTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&javascript15HTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&javascript16HTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&javascript17HTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&livescriptHTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&ecmascriptHTMLScriptElement)).externalDeref();
	(*(WebCore::AtomicString*)(&jscriptHTMLScriptElement)).externalDeref();

	(*(WebCore::AtomicString*)(&textareaHTMLTextAreaElement)).externalDeref();
	m_st_bInitialized = false;
}

void AtomicString::externalDeref()
{
	m_string.externalDeref();
}
//-daw ca


}
