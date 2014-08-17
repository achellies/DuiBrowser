/*
* This file is part of the internal font implementation.  It should not be included by anyone other than
* FontMac.cpp, FontWin.cpp and Font.cpp.
*
* Copyright (C) 2006 Apple Computer, Inc.
* Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com 
* Copyright (C) 2007 Holger Hans Peter Freyther
* All rights reserved.
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
* the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*/

#ifndef FontPlatformData_h
#define FontPlatformData_h

#include "GlyphBuffer.h"
#include "FontDescription.h"
#include <cairo.h>
#include <cairo-ft.h>

namespace WebCore {

class FontPlatformData {
public:
	FontPlatformData(WTF::HashTableDeletedValueType)
		: m_pattern(reinterpret_cast<char*>(-1))
		, m_size(0)
		, m_syntheticBold(0)
		, m_syntheticOblique(0)
		, m_scaledFont(0)
	{ }

	FontPlatformData()
		: m_pattern(0)
		, m_size(0)
		, m_syntheticBold(0)
		, m_syntheticOblique(0)
		, m_scaledFont(0)
	{ }

	FontPlatformData(const FontDescription&, const AtomicString& family);

	FontPlatformData(float size, bool bold, bool italic);
	FontPlatformData(cairo_font_face_t* fontFace, int size, bool bold, bool italic);

	~FontPlatformData();

	static bool init();

	bool isFixedPitch();
	float size() const { return m_size; }

	void setFont(cairo_t*) const;

	unsigned hash() const
	{
		uintptr_t hashCodes[1] = { reinterpret_cast<uintptr_t>(m_scaledFont) };
		unsigned len = sizeof(hashCodes) / sizeof(UChar);
		return StringImpl::computeHash( reinterpret_cast<UChar*>(hashCodes), len);
	}

	bool operator==(const FontPlatformData&) const;
	bool isHashTableDeletedValue() const {
        return m_pattern == reinterpret_cast<char*>(-1);
    };

	char *m_pattern;
	float m_size;
    bool m_syntheticBold;
    bool m_syntheticOblique;
	cairo_scaled_font_t* m_scaledFont;
};

}

#endif
