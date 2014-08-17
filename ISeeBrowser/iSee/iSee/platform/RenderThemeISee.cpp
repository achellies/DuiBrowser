/*
* This file is part of the WebKit project.
*
* Copyright (C) 2006 Apple Computer, Inc.
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

#include "config.h"
#include "RenderThemeISee.h"

#include "CSSValueKeywords.h"
#include "Document.h"
#include "GraphicsContext.h"
#include "PlatformString.h"
#include "RenderView.h"

namespace WebCore {

const int SCROLL_BUTTON_WIDTH = 15;
const Color HOVERED_COLOR = Color(210, 210, 240, 255);

RenderTheme* theme()
{
	static RenderThemeISee iSeeTheme;
	return &iSeeTheme;
}

RenderThemeISee::RenderThemeISee()
{
}

RenderThemeISee::~RenderThemeISee()
{	
}

Color RenderThemeISee::platformActiveSelectionBackgroundColor() const
{
	return Color(181, 213, 255);
#if 0
	COLORREF color = GetSysColor(COLOR_HIGHLIGHT);
	return Color(GetRValue(color), GetGValue(color), GetBValue(color), 255);
#endif
}

Color RenderThemeISee::platformInactiveSelectionBackgroundColor() const
{
	return Color(212, 212, 212);
#if 0
	COLORREF color = GetSysColor(COLOR_GRAYTEXT);
	return Color(GetRValue(color), GetGValue(color), GetBValue(color), 255);
#endif
}

Color RenderThemeISee::platformActiveSelectionForegroundColor() const
{
	return Color(56, 117, 215);
#if 0
	COLORREF color = GetSysColor(COLOR_HIGHLIGHTTEXT);
	return Color(GetRValue(color), GetGValue(color), GetBValue(color), 255);
#endif
}

Color RenderThemeISee::platformInactiveSelectionForegroundColor() const
{
	return Color::white;
}

bool RenderThemeISee::supportsFocus(EAppearance appearance)
{
	switch (appearance) {
	case PushButtonAppearance:
    case DefaultButtonAppearance:
	case ButtonAppearance:
	case TextFieldAppearance:
	case TextAreaAppearance:
	case MenulistAppearance:
		return true;
	default:
		return false;
	}
}

static void drawButtonBox(GraphicsContext* g, const IntRect& rs, bool pressed, bool hovered, bool focused)
{
	g->save();

	IntRect r(rs); r.inflate(-1);
	if (!pressed)
		r.move(-1, -1);
	g->fillRect(r, (pressed || hovered) ? HOVERED_COLOR : Color(192, 192, 192, 255));
	g->setStrokeStyle(SolidStroke);

	g->setStrokeColor(pressed ? Color(128, 128, 128, 255) : Color(255, 255, 255, 255));
	g->drawLine(IntPoint(r.x(), r.y()), IntPoint(r.x(), r.bottom()));
	g->drawLine(IntPoint(r.x(), r.y()), IntPoint(r.right(), r.y()));

	g->setStrokeColor(pressed ? Color(255, 255, 255, 255) : Color(128, 128, 128, 255));
	g->drawLine(IntPoint(r.right(), r.y()), IntPoint(r.right(), r.bottom()));
	g->drawLine(IntPoint(r.x(), r.bottom()), IntPoint(r.right() + 1, r.bottom()));

	if (focused)
	{
		g->setFillColor(Color(0, 0, 0, 0));
		g->setStrokeStyle(SolidStroke);
		
		r.inflate(1); r.setWidth(r.width() + 1); r.setHeight(r.height() + 1);
		g->setStrokeColor(Color(128, 128, 255, 255));
		g->drawRect(r);
		
		r.inflate(-1);
		g->setStrokeColor(Color(64, 64, 191, 255));
		g->drawRect(r);
	}

	g->restore();
}

static void drawCheckBox(GraphicsContext* g, const IntRect& rs, bool pressed, bool hovered, bool checked, bool focused)
{
	g->save();

	IntRect r(rs); r.inflate(-1);
	
	g->setFillColor((pressed || hovered) ? HOVERED_COLOR : Color(255, 255, 255, 255));
	g->setStrokeStyle(SolidStroke);
	g->setStrokeColor(Color(64, 64, 64, 255));
	g->setStrokeThickness(1.0);
	g->drawRect(r);

	if (focused)
	{
		g->setFillColor(Color(0, 0, 0, 0));
		g->setStrokeStyle(SolidStroke);
		
		IntRect f(r); f.inflate(1);
		g->setStrokeColor(Color(128, 128, 255, 255));
		g->drawRect(f);
		
		f.inflate(-1);
		g->setStrokeColor(Color(64, 64, 191, 255));
		g->drawRect(f);
	}

	if (checked)
	{
		r.inflate(-3);
		g->setFillColor(Color(0, 0, 0, 255));
		g->setStrokeStyle(NoStroke);
		g->drawRect(r);
	}

	g->restore();
}

static void drawRadioBox(GraphicsContext* g, const IntRect& rs, bool pressed, bool hovered, bool checked, bool focused)
{
	g->save();

	IntRect r(rs); r.inflate(-1);
	
	g->setFillColor((pressed || hovered) ? HOVERED_COLOR : Color(255, 255, 255, 255));
	g->setStrokeStyle(SolidStroke);
	g->setStrokeColor(Color(64, 64, 64, 255));
	g->setStrokeThickness(1.0);
	g->drawEllipse(r);

	if (focused)
	{
		g->setFillColor(Color(0, 0, 0, 0));
		g->setStrokeStyle(SolidStroke);
		g->setStrokeThickness(1.0);

		IntRect f(r); f.inflate(1);
		g->setStrokeColor(Color(128, 128, 255, 255));
		g->drawEllipse(f);

		f.inflate(-1);
		g->setStrokeColor(Color(64, 64, 191, 255));
		g->drawEllipse(f);
	}

	if (checked)
	{
		r.inflate(-3);
		g->setFillColor(Color(0, 0, 0, 255));
		g->setStrokeStyle(NoStroke);
		g->drawEllipse(r);
	}
	
	g->restore();
}

bool RenderThemeISee::paintButton(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
	switch (o->style()->appearance()) {
	case PushButtonAppearance:
	case ButtonAppearance:
		drawButtonBox(i.context, r, isPressed(o), isHovered(o), isFocused(o));
		break;
	case CheckboxAppearance:
		drawCheckBox(i.context, r, isPressed(o), isHovered(o), isChecked(o), isFocused(o));
		break;
	case RadioAppearance:
		drawRadioBox(i.context, r, isPressed(o), isHovered(o), isChecked(o), isFocused(o));
		break;
	case ListboxAppearance:
	case MenulistAppearance:
	case TextFieldAppearance:
	case TextAreaAppearance:
		break;
	}

	return false;
}

void RenderThemeISee::setCheckboxSize(RenderStyle* style) const
{
	// If the width and height are both specified, then we have nothing to do.
	if (!style->width().isIntrinsicOrAuto() && !style->height().isAuto())
		return;

	// FIXME:  A hard-coded size of 13 is used.  This is wrong but necessary for now.  It matches Firefox.
	// At different DPI settings on Windows, querying the theme gives you a larger size that accounts for
	// the higher DPI.  Until our entire engine honors a DPI setting other than 96, we can't rely on the theme's
	// metrics.
	if (style->width().isIntrinsicOrAuto())
		style->setWidth(Length(13, Fixed));
	if (style->height().isAuto())
		style->setHeight(Length(13, Fixed));
}

bool RenderThemeISee::paintTextField(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
	GraphicsContext* g = i.context;
	g->save();

	g->setFillColor(isEnabled(o) ? Color(255, 255, 255, 255) : Color(192, 192, 192, 255));
	g->setStrokeStyle(SolidStroke);
	g->setStrokeColor(Color(128, 128, 128, 255));
	g->setStrokeThickness(1.0);
	g->drawRect(r);

	g->restore();

	return false;
}

void RenderThemeISee::adjustMenuListStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
	// Height is locked to auto.
	style->setHeight(Length(Auto));

	// White-space is locked to pre
	style->setWhiteSpace(PRE);

	// Add in the padding that we'd like to use.
	const int buttonWidth = SCROLL_BUTTON_WIDTH;
	style->setPaddingLeft(Length(2, Fixed));
	style->setPaddingRight(Length(buttonWidth + 2, Fixed));
	style->setPaddingTop(Length(1, Fixed));
	style->setPaddingBottom(Length(1, Fixed));
}

bool RenderThemeISee::paintMenuList(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
	// FIXME: All these inflate() calls are bogus, causing painting problems,
    // as well as sizing wackiness in Classic mode
    paintTextField(o, i, r);

    const int buttonWidth = SCROLL_BUTTON_WIDTH;
    IntRect buttonRect(r.right() - buttonWidth - 1, r.y(), buttonWidth, r.height());
    buttonRect.inflateY(-2);
    paintMenuListButton(o, i, buttonRect);

	if (supportsFocus(o->style()->appearance()) && isFocused(o))
	{
		GraphicsContext* g = i.context;
		g->save();
		
		g->setFillColor(Color(0, 0, 0, 0));
		g->setStrokeStyle(SolidStroke);
		
		IntRect f(r); f.inflate(1);
		g->setStrokeColor(Color(128, 128, 255, 255));
		g->drawRect(f);
		
		f.inflate(-1);
		g->setStrokeColor(Color(64, 64, 191, 255));
		g->drawRect(f);

		g->restore();
	}

	return false;
}

bool RenderThemeISee::paintMenuListButton(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
	drawButtonBox(i.context, r, isPressed(o), isHovered(o), false);
	
	GraphicsContext* g = i.context;
	g->save();

	int y = r.y() + (r.height() - 4) / 2;
	g->setStrokeStyle(SolidStroke);
	g->setStrokeColor(Color(0, 0, 0, 255));
	g->drawLine(IntPoint(r.x() + 4, y), IntPoint(r.x() + 11, y)); y++;
	g->drawLine(IntPoint(r.x() + 5, y), IntPoint(r.x() + 10, y)); y++;
	g->drawLine(IntPoint(r.x() + 6, y), IntPoint(r.x() + 9, y)); y++;
	g->drawLine(IntPoint(r.x() + 7, y), IntPoint(r.x() + 8, y));

	g->restore();
	
	return false;
}

void RenderThemeISee::systemFont(int propId, FontDescription& fontDescription) const
{
    static FontDescription systemFont;
    static FontDescription smallSystemFont;
    static FontDescription menuFont;
    static FontDescription labelFont;
    static FontDescription miniControlFont;
    static FontDescription smallControlFont;
    static FontDescription controlFont;

    FontDescription* cachedDesc;
    float fontSize = 0;
    switch (propId) {
        case CSSValueSmallCaption:
            cachedDesc = &smallSystemFont;
            break;
        case CSSValueMenu:
            cachedDesc = &menuFont;
            break;
        case CSSValueStatusBar:
            cachedDesc = &labelFont;
            if (!labelFont.isAbsoluteSize())
                fontSize = 10.0f;
            break;
        case CSSValueWebkitMiniControl:
            cachedDesc = &miniControlFont;
            break;
        case CSSValueWebkitSmallControl:
            cachedDesc = &smallControlFont;
            break;
        case CSSValueWebkitControl:
            cachedDesc = &controlFont;
            break;
        default:
            cachedDesc = &systemFont;
            if (!systemFont.isAbsoluteSize())
                fontSize = 13.0f;
    }

    if (fontSize) {
        cachedDesc->setIsAbsoluteSize(true);
        cachedDesc->setGenericFamily(FontDescription::NoFamily);
        cachedDesc->firstFamily().setFamily("Lucida Grande");
        cachedDesc->setSpecifiedSize(fontSize);
        cachedDesc->setWeight(FontWeightNormal);
        cachedDesc->setItalic(false);
    }
    fontDescription = *cachedDesc;
}

} // namespace
