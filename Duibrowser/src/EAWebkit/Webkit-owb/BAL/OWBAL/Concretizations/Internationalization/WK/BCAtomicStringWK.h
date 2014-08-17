/*
 * Copyright (C) 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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

#ifndef AtomicString_h
#define AtomicString_h

#include <wtf/FastAllocBase.h>
#include "AtomicStringImpl.h"
#include "PlatformString.h"

namespace OWBAL {

class AtomicString/*: public WTF::FastAllocBase*/ {
public:
    static void init();

    //+daw ca 23/07 static and global management
	static void staticFinalize();
	void externalDeref();
	static void unInit();

private:
	static bool m_st_bInitialized;
public:



    AtomicString() { }
    AtomicString(const char* s) : m_string(add(s)) { }
    AtomicString(const UChar* s, int length) : m_string(add(s, length)) { }
    AtomicString(const UChar* s) : m_string(add(s)) { }
    AtomicString(const KJS::UString& s) : m_string(add(s)) { }
    AtomicString(const KJS::Identifier& s) : m_string(add(s)) { }
    AtomicString(StringImpl* imp) : m_string(add(imp)) { }
    AtomicString(AtomicStringImpl* imp) : m_string(imp) { }
    AtomicString(const String& s) : m_string(add(s.impl())) { }

    static AtomicStringImpl* find(const KJS::Identifier&);

    operator const String&() const { return m_string; }
    const String& string() const { return m_string; };

    operator KJS::UString() const;

    AtomicStringImpl* impl() const { return static_cast<AtomicStringImpl *>(m_string.impl()); }
    
    const UChar* characters() const { return m_string.characters(); }
    unsigned length() const { return m_string.length(); }
    
    UChar operator[](unsigned int i) const { return m_string[i]; }
    
    bool contains(UChar c) const { return m_string.contains(c); }
    bool contains(const AtomicString& s, bool caseSensitive = true) const
        { return m_string.contains(s.string(), caseSensitive); }

    int find(UChar c, int start = 0) const { return m_string.find(c, start); }
    int find(const AtomicString& s, int start = 0, bool caseSentitive = true) const
        { return m_string.find(s.string(), start, caseSentitive); }
    
    bool startsWith(const AtomicString& s, bool caseSensitive = true) const
        { return m_string.startsWith(s.string(), caseSensitive); }
    bool endsWith(const AtomicString& s, bool caseSensitive = true) const
        { return m_string.endsWith(s.string(), caseSensitive); }
    
    int toInt(bool* ok = 0) const { return m_string.toInt(ok); }
    double toDouble(bool* ok = 0) const { return m_string.toDouble(ok); }
    float toFloat(bool* ok = 0) const { return m_string.toFloat(ok); }
    bool percentage(int& p) const { return m_string.percentage(p); }
    Length* toLengthArray(int& len) const { return m_string.toLengthArray(len); }
    Length* toCoordsArray(int& len) const { return m_string.toCoordsArray(len); }

    bool isNull() const { return m_string.isNull(); }
    bool isEmpty() const { return m_string.isEmpty(); }

    static void remove(StringImpl*);
    
#ifdef __OBJC__
    AtomicString(NSString* s) : m_string(add(String(s).impl())) { }
    operator NSString*() const { return m_string; }
#endif
#if PLATFORM(SYMBIAN)
    AtomicString(const TDesC& s) : m_string(add(String(s).impl())) { }
    operator TPtrC() const { return m_string; }
#endif
#if PLATFORM(QT)
    AtomicString(const QString& s) : m_string(add(String(s).impl())) { }
    operator QString() const { return m_string; }
#endif

private:
    String m_string;
    
    static PassRefPtr<StringImpl> add(const char*);
    static PassRefPtr<StringImpl> add(const UChar*, int length);
    static PassRefPtr<StringImpl> add(const UChar*);
    static PassRefPtr<StringImpl> add(StringImpl*);
    static PassRefPtr<StringImpl> add(const KJS::UString&);
    static PassRefPtr<StringImpl> add(const KJS::Identifier&);
};

inline bool operator==(const AtomicString& a, const AtomicString& b) { return a.impl() == b.impl(); }
bool operator==(const AtomicString& a, const char* b);
inline bool operator==(const AtomicString& a, const String& b) { return equal(a.impl(), b.impl()); }
inline bool operator==(const char* a, const AtomicString& b) { return b == a; }
inline bool operator==(const String& a, const AtomicString& b) { return equal(a.impl(), b.impl()); }

inline bool operator!=(const AtomicString& a, const AtomicString& b) { return a.impl() != b.impl(); }
inline bool operator!=(const AtomicString& a, const char *b) { return !(a == b); }
inline bool operator!=(const AtomicString& a, const String& b) { return !equal(a.impl(), b.impl()); }
inline bool operator!=(const char* a, const AtomicString& b) { return !(b == a); }
inline bool operator!=(const String& a, const AtomicString& b) { return !equal(a.impl(), b.impl()); }

inline bool equalIgnoringCase(const AtomicString& a, const AtomicString& b) { return equalIgnoringCase(a.impl(), b.impl()); }
inline bool equalIgnoringCase(const AtomicString& a, const char* b) { return equalIgnoringCase(a.impl(), b); }
inline bool equalIgnoringCase(const AtomicString& a, const String& b) { return equalIgnoringCase(a.impl(), b.impl()); }
inline bool equalIgnoringCase(const char* a, const AtomicString& b) { return equalIgnoringCase(a, b.impl()); }
inline bool equalIgnoringCase(const String& a, const AtomicString& b) { return equalIgnoringCase(a.impl(), b.impl()); }

// Define external global variables for the commonly used atomic strings.
#ifndef ATOMICSTRING_HIDE_GLOBALS
    extern const AtomicString nullAtom;
    extern const AtomicString emptyAtom;
    extern const AtomicString textAtom;
    extern const AtomicString commentAtom;
    extern const AtomicString starAtom;
    //+daw ca 31/07 static and global management, declare new statics to atomicstring
	extern const AtomicString buttonHTMLInputElement;
	extern const AtomicString checkboxHTMLInputElement;
	extern const AtomicString fileHTMLInputElement;
	extern const AtomicString hiddenHTMLInputElement;
	extern const AtomicString imageHTMLInputElement;
	extern const AtomicString passwordHTMLInputElement;
	extern const AtomicString radioHTMLInputElement;
	extern const AtomicString rangeHTMLInputElement;
	extern const AtomicString resetHTMLInputElement;
	extern const AtomicString searchHTMLInputElement;
	extern const AtomicString submitHTMLInputElement;
	extern const AtomicString textHTMLInputElement;

    // Added by Paul Pedriana, 1/2009.
    extern const AtomicString optionAtom;
    extern const AtomicString optgroupAtom;
    extern const AtomicString legendAtom;
    extern const AtomicString keygenAtom;
    extern const AtomicString fieldsetAtom;

	extern const AtomicString xmlnsNamespaceURI;
	extern const AtomicString xmlns;
	extern const AtomicString xml;

	extern const AtomicString selectMultiple;
	extern const AtomicString selectOne;

	extern const AtomicString submitHTMLButtonElement;
	extern const AtomicString buttonHTMLButtonElement;
	extern const AtomicString resetHTMLButtonElement;

	extern const AtomicString noneHTMLTableElement;
	extern const AtomicString solidHTMLTableElement;
	extern const AtomicString insetHTMLTableElement;
	extern const AtomicString solidcolsHTMLTableElement;
	extern const AtomicString solidrowsHTMLTableElement;

	extern const AtomicString javascriptHTMLScriptElement;
	extern const AtomicString javascript10HTMLScriptElement;
	extern const AtomicString javascript11HTMLScriptElement;
	extern const AtomicString javascript12HTMLScriptElement;
	extern const AtomicString javascript13HTMLScriptElement;
	extern const AtomicString javascript14HTMLScriptElement;
	extern const AtomicString javascript15HTMLScriptElement;
	extern const AtomicString javascript16HTMLScriptElement;
	extern const AtomicString javascript17HTMLScriptElement;
	extern const AtomicString livescriptHTMLScriptElement;
	extern const AtomicString ecmascriptHTMLScriptElement;
	extern const AtomicString jscriptHTMLScriptElement;

	extern const AtomicString textareaHTMLTextAreaElement;
#endif

} // namespace WebCore

#endif // AtomicString_h
