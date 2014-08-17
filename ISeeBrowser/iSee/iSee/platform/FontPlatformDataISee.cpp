/*
* Copyright (C) 2006 Apple Computer, Inc.
* Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com 
 * Copyright (C) 2007, 2008 Alp Toker <alp@atoker.com>
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
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "FontPlatformData.h"

#include "CString.h"
#include "PlatformString.h"
#include "FontDescription.h"

#include <assert.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace WebCore {

// FIXME
class FreeTypeFonts
{
public:
	FreeTypeFonts()
	: m_library(0), m_face(0)
	{	
	}
	~FreeTypeFonts()
	{
		if (m_face)
			FT_Done_Face(m_face);
		if (m_library)
			FT_Done_FreeType(m_library);
	}
	void init()
	{
		FT_Init_FreeType (&m_library);
		if (m_library)
		{
			const char *filename = getenv("Tahoma");
			int id = 0;
			FT_New_Face (m_library, filename, id, &m_face);
		}
	}

	FT_Face getMatchFace(const char *family, bool italic, bool bold)
	{
		return m_face;
	}

	FT_Face *getMatchFaces(const char *family, int *length)
	{
		*length = 1;
		return &m_face;
	}

private:
	FT_Library m_library;
	FT_Face m_face;
};
static FreeTypeFonts g_freetypeFonts;

FontPlatformData::FontPlatformData(const FontDescription& fontDescription, const AtomicString& familyName)
: m_pattern(0)
, m_size(fontDescription.computedSize())
, m_syntheticBold(false)
, m_syntheticOblique(false)
, m_scaledFont(0)
{
	FontPlatformData::init();

	CString familyNameString = familyName.string().utf8();
	const char* fcfamily = familyNameString.data();
	int fcitalic = fontDescription.italic();
	int fcbold = fontDescription.weight() >= FontWeight600 ? 1 : 0;
	
	int type = fontDescription.genericFamily();
	switch (type) {
	case FontDescription::SerifFamily:
		fcfamily = "serif";
		break;
	case FontDescription::SansSerifFamily:
		fcfamily = "sans-serif";
		break;
	case FontDescription::MonospaceFamily:
		fcfamily = "monospace";
		break;
	case FontDescription::NoFamily:
	case FontDescription::StandardFamily:
	default:
		fcfamily = "sans-serif";
	}
	m_pattern = (char *)malloc(strlen(fcfamily) + 3 /* size */ + 1 /* italic */ + 1 /* bold */ + 1 /* null */);
	if (0 == m_pattern)
		return;
	sprintf(m_pattern, "%s%03d%d%d", fcfamily, static_cast<int>(fontDescription.computedSize()), fcitalic, fcbold);

	FT_Face face = g_freetypeFonts.getMatchFace(fcfamily, fcitalic, fcbold);
	if (0 == face)
	{
		free(m_pattern), m_pattern = 0;
		return;
	}

	cairo_font_face_t* fontFace = cairo_ft_font_face_create_for_ft_face(face, FT_LOAD_DEFAULT);
	cairo_matrix_t fontMatrix;
	cairo_matrix_t ctm;
	cairo_matrix_init_scale(&fontMatrix, fontDescription.computedSize(), fontDescription.computedSize());
	cairo_matrix_init_identity(&ctm);
	cairo_font_options_t* options = cairo_font_options_create();
	m_scaledFont = cairo_scaled_font_create(fontFace, &fontMatrix, &ctm, options);
	cairo_font_face_destroy(fontFace);
	cairo_font_options_destroy(options);
}

FontPlatformData::FontPlatformData(float size, bool bold, bool italic)
: m_pattern(0)
, m_size(size)
, m_syntheticBold(bold)
, m_syntheticOblique(italic)
, m_scaledFont(0)
{
}

FontPlatformData::FontPlatformData(cairo_font_face_t* fontFace, int size, bool bold, bool italic)
: m_pattern(0)
, m_size(size)
, m_syntheticBold(bold)
, m_syntheticOblique(italic)
, m_scaledFont(0)
{
	cairo_matrix_t fontMatrix;
	cairo_matrix_init_scale(&fontMatrix, size, size);
	cairo_matrix_t ctm;
	cairo_matrix_init_identity(&ctm);
	cairo_font_options_t* options = cairo_font_options_create();

	// We force antialiasing and disable hinting to provide consistent
	// typographic qualities for custom fonts on all platforms.
	cairo_font_options_set_hint_style(options, CAIRO_HINT_STYLE_NONE);
	cairo_font_options_set_antialias(options, CAIRO_ANTIALIAS_GRAY);

	m_scaledFont = cairo_scaled_font_create(fontFace, &fontMatrix, &ctm, options);
	cairo_font_options_destroy(options);
}

bool FontPlatformData::init()
{
	static bool isInitialized = false;
	if (isInitialized)
		return true;
	isInitialized = true;
	g_freetypeFonts.init();
	return true;
}

FontPlatformData::~FontPlatformData()
{
}

bool FontPlatformData::isFixedPitch()
{
	FT_Face face = cairo_ft_scaled_font_lock_face(m_scaledFont);
	if (face)
	{
		bool r = FT_IS_FIXED_WIDTH(face) ? true : false;
		cairo_ft_scaled_font_unlock_face(m_scaledFont);
		return r;
	}
	return false;
}

void FontPlatformData::setFont(cairo_t* cr) const
{
	ASSERT(m_scaledFont);

	cairo_set_scaled_font(cr, m_scaledFont);
}

bool FontPlatformData::operator==(const FontPlatformData& other) const
{
	// 空的结构，认为相等
	if (NULL == m_pattern && NULL == other.m_pattern)
		return true;
#if 0 // 暂时用不着
	// 字体标志字符串比较
	if (m_pattern && m_pattern != reinterpret_cast<char*>(-1) && 
		other.m_pattern && other.m_pattern != reinterpret_cast<char*>(-1) && 
		strcmp(m_pattern, other.m_pattern) == 0)
		return true;
#endif

	// 比较FT_Face
	if (m_scaledFont && other.m_scaledFont)
	{
		FT_Face face1 = cairo_ft_scaled_font_lock_face(m_scaledFont);
		FT_Face face2 = cairo_ft_scaled_font_lock_face(other.m_scaledFont);
		if (face1 && face1 == face2 && m_size == other.m_size)
		{
			cairo_ft_scaled_font_unlock_face(m_scaledFont);
			cairo_ft_scaled_font_unlock_face(other.m_scaledFont);
			return true;
		}
		if (face1)
			cairo_ft_scaled_font_unlock_face(m_scaledFont);
		if (face2)
			cairo_ft_scaled_font_unlock_face(other.m_scaledFont);
	}

	return false;
}

}
