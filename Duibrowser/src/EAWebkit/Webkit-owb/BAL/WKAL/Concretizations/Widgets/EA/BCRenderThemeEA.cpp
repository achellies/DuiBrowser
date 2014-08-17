/*
 * Copyright (C) 2008 Pleyo.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Pleyo nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY PLEYO AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PLEYO OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2008-2009
*/

///////////////////////////////////////////////////////////////////////////////
// BCRenderThemeEA.cpp
// Modified by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "RenderObject.h"
#include "RenderThemeEA.h"
#include "GraphicsContext.h"
#include "Pen.h"
#include <EAWebKit/EAWebKit.h>
#include <stdio.h>


// Constants
const int kPopupMenuArrowSize     = 16;
const int kPopupMenuBorderPadding = 2;


namespace WKAL {


void RenderThemeBal::addIntrinsicMargins(RenderStyle* style) const
{
    // Cut out the intrinsic margins completely if we end up using a small font size
    if (style->fontSize() < 11)
        return;

    // Intrinsic margin value.
    const int m = 2;

    // FIXME: Using width/height alone and not also dealing with min-width/max-width is flawed.
    if (style->width().isIntrinsicOrAuto()) {
        if (style->marginLeft().quirk())
            style->setMarginLeft(Length(m, Fixed));
        if (style->marginRight().quirk())
            style->setMarginRight(Length(m, Fixed));
    }

    if (style->height().isAuto()) {
        if (style->marginTop().quirk())
            style->setMarginTop(Length(m, Fixed));
        if (style->marginBottom().quirk())
            style->setMarginBottom(Length(m, Fixed));
    }
}


RenderTheme* theme()
{
    static RenderThemeBal balTheme;
    return &balTheme;
}


RenderThemeBal::RenderThemeBal()
    : m_balWindow(0)
    , m_balContainer(0)
    , m_balEntry(0)
    , m_balTreeView(0)
{
}


bool RenderThemeBal::supportsFocusRing(const RenderStyle* style) const
{
    return false;
}


bool RenderThemeBal::controlSupportsTints(const RenderObject* o) const
{
    return isEnabled(o);
}


int RenderThemeBal::baselinePosition(const RenderObject* o) const
{
    const WebCore::EAppearance a = o->style()->appearance();

    if ((a == CheckboxAppearance) || (a == RadioAppearance))
        return o->marginTop() + o->height() - 2;

    return RenderTheme::baselinePosition(o);
}


void RenderThemeBal::setCheckboxSize(RenderStyle* style) const
{
    // If the width and height are both specified, then we have nothing to do.
    if (!style->width().isIntrinsicOrAuto() && !style->height().isAuto())
        return;

    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();

    if (style->width().isIntrinsicOrAuto())
        style->setWidth(Length((int)param.mCheckboxSize, Fixed));
    if (style->height().isAuto())
        style->setHeight(Length((int)param.mCheckboxSize, Fixed));
}


bool RenderThemeBal::paintCheckbox(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
    const bool isChecked = o->element()->isChecked();
    const bool isActive  = o->element()->active();	
	const bool isDisabled = !o->element()->isEnabled();

    i.context->save();

    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();
    EA::WebKit::ButtonDrawInfo bdi;

    EA::Raster::ISurface* const pSurface = i.context->platformContext();;
    EA::WebKit::View* pView = NULL;
    if(pSurface)
        pView  = static_cast<EA::WebKit::View*>(pSurface->GetUserData());  // Need to verify if this cast is safe...
  
	// 8/17/2010 Gautam Narain : We need to offset the rectangle by origin width and height. I am not sure if the mDirtyRect was incorrect due to this issue.
	// Hence I have commented that code instead of deleting it completely        
    bdi.mpView			= pView;
    bdi.mpSurface		= pSurface;
	//bdi.mDirtyRect		= r;			// This is probably not correct
    bdi.mDirtyRect		= EA::Raster::Rect(r.x() + i.context->origin().width(), r.y() + i.context->origin().height(), r.width(), r.height());            // This probably isn't correct.
    bdi.mButtonRect		= EA::Raster::Rect(r.x() + i.context->origin().width(), r.y() + i.context->origin().height(), r.width(), r.height());
    bdi.mButtonType		= EA::WebKit::ButtonDrawInfo::Checkbox;
    bdi.mIsHovered		= false;        // How do we tell this?
    bdi.mIsChecked		= isChecked;
    bdi.mIsFocused		= isActive;	
	bdi.mIsDisabled		= isDisabled;

    if(!pVN || !pVN->DrawButton(bdi))
    {
        i.context->setStrokeStyle(SolidStroke);
        i.context->setStrokeColor(Color::black);
        RefPtr<RenderStyle> style = RenderStyle::create();
        addIntrinsicMargins(style.get());

        EBorderStyle v = INSET;
        style->setBorderTopStyle(v);
        style->setBorderLeftStyle(v);
        style->setBorderBottomStyle(v);
        style->setBorderRightStyle(v);
        int borderWidth = 1;
        style->setBorderTopWidth(borderWidth);
        style->setBorderLeftWidth(borderWidth);
        style->setBorderBottomWidth(borderWidth);
        style->setBorderRightWidth(borderWidth);

        // 2/12/09 CSidhall - Forced background of a checkbox to be white because the font is forced to black so need it white (or light)to see the "X"
        o->paintFillLayerExtended(i,
            Color::white, o->style()->backgroundLayers(),
            r.y(), o->height(), r.x(), r.y(), o->width(), o->height());
        o->paintBorder(i.context,
            r.x(), r.y(), r.width(), r.height(),
            style.get(), true, true);

        if( isActive ) {
            // CS - This just flickers the check box when clicked...
            IntRect r2(r);
            r2.inflate(-2);
            i.context->setFillColor(WebCore::Color(0xc0,0xc0,0xc0));
            i.context->drawRect(r2);
            v = GROOVE;
            style->setBorderTopStyle(v);
            style->setBorderLeftStyle(v);
            style->setBorderBottomStyle(v);
            style->setBorderRightStyle(v);
            int borderWidth = 1;
            style->setBorderTopWidth(borderWidth);
            style->setBorderLeftWidth(borderWidth);
            style->setBorderBottomWidth(borderWidth);
            style->setBorderRightWidth(borderWidth);
            o->paintFillLayerExtended(i,
            o->style()->backgroundColor(), o->style()->backgroundLayers(),
            r.y(), o->height(), r.x(), r.y(), o->width(), o->height());
            o->paintBorder(i.context,
                r.x(), r.y(), r.width(), r.height(),
                style.get(), true, true);
        }

        if(isChecked) {
            i.context->setStrokeColor(Color::black);
            i.context->setStrokeStyle(SolidStroke);
            IntRect r2(r);
            r2.inflate(-borderWidth-2);
            i.context->drawLine(IntPoint(r2.x(),r2.y()), IntPoint(r2.x()+r2.width(),r2.y()+r2.height()));
            i.context->drawLine(IntPoint(r2.x()+r2.width(),r2.y()), IntPoint(r2.x(),r2.y()+r2.height()));
        }
    }

    i.context->restore();

    return false; // WebKit appears to expect us to return false.
}


void RenderThemeBal::setRadioSize(RenderStyle* style) const
{
    // This is the same as checkboxes.
    setCheckboxSize(style);
}


bool RenderThemeBal::paintRadio(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
    i.context->save();

    const bool isChecked = o->element()->isChecked();
    const bool isActive  = o->element()->active();
	
    // 11/18/09 CS - Disabled or unfocus check 	
	const bool isDisabled = !o->element()->isEnabled();

    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();
    EA::WebKit::ButtonDrawInfo bdi;

    EA::Raster::ISurface* const pSurface = i.context->platformContext();;
    EA::WebKit::View* pView = NULL;
    if(pSurface)
        pView  = static_cast<EA::WebKit::View*>(pSurface->GetUserData());

	// 8/17/2010 Gautam Narain : We need to offset the rectangle by origin width and height. I am not sure if the mDirtyRect was incorrect due to this issue.
	// Hence I have commented that code instead of deleting it completely
    bdi.mpView      = pView;
    bdi.mpSurface   = pSurface;
	//bdi.mDirtyRect  = r;		// This probably isn't correct.
	bdi.mDirtyRect  = EA::Raster::Rect(r.x() + i.context->origin().width(), r.y() + i.context->origin().height(), r.width(), r.height());            
    bdi.mButtonRect = EA::Raster::Rect(r.x() + i.context->origin().width(), r.y() + i.context->origin().height(), r.width(), r.height());
    bdi.mButtonType = EA::WebKit::ButtonDrawInfo::Radio;
    bdi.mIsHovered  = false;        // How do we tell this?
    bdi.mIsChecked  = isChecked;
    bdi.mIsFocused  = isActive;
	bdi.mIsDisabled = isDisabled;

    // CSidhall - TODO: It should be noted that we don't currently pass down the user if the radio is disabled

    if(!pVN || !pVN->DrawButton(bdi))
    {
        const int borderWidth = 1;
        //border
        int halfWidth = r.width() >> 1;
        int halfHeight = r.height() >> 1;
        EA::Raster::Point center(r.x() + halfWidth, r.y() + halfHeight);

        // 2/12/09 CSidahll - Added to fix scroll problem with radio butter which did not move          
        center.x +=i.context->origin().width();
        center.y +=i.context->origin().height();
        const EA::Raster::Color circleColor(60,60,60);
        EA::Raster::IEARaster* pRaster =EA::WebKit::GetEARasterInstance();     
        if(isChecked) {
            // Selected
            pRaster->FilledEllipseColor(bdi.mpSurface, center.x, center.y, (halfWidth - borderWidth), (halfHeight - borderWidth), EA::Raster::Color::WHITE);
            pRaster->EllipseColor(bdi.mpSurface, center.x, center.y, halfWidth, halfHeight, circleColor);

            halfWidth -= borderWidth + 2;
            halfHeight -= borderWidth + 2;
            pRaster->FilledEllipseColor(bdi.mpSurface, center.x, center.y, halfWidth, halfHeight, EA::Raster::Color::BLACK);
        }
        else if(isDisabled) {
           
            // 11/18/09 CS - For a disabled radio
            // We don't want a too dark grey color or it looks like it is selected.
            const EA::Raster::Color fillColorUnfocused(210,210,210);                 
            const EA::Raster::Color circleColorUnfocused(140,140,140);    
            pRaster->FilledEllipseColor(bdi.mpSurface, center.x, center.y, (halfWidth - borderWidth), (halfHeight - borderWidth), fillColorUnfocused);
            pRaster->EllipseColor(bdi.mpSurface, center.x, center.y, halfWidth, halfHeight, circleColorUnfocused);
        }
        else {
            // Not selected
            pRaster->FilledEllipseColor(bdi.mpSurface, center.x, center.y, (halfWidth - borderWidth), (halfHeight - borderWidth), EA::Raster::Color::WHITE);
            pRaster->EllipseColor(bdi.mpSurface, center.x, center.y, halfWidth, halfHeight, circleColor);
        }
    }
    
    i.context->restore();   // 2/8/09 CSidhall - Added to fix context leak

    return false; // WebKit appears to expect us to return false.
}

void RenderThemeBal::adjustButtonStyle(CSSStyleSelector* selector, RenderStyle* style, WebCore::Element* e) const
{
    addIntrinsicMargins(style);
}

bool RenderThemeBal::paintButton(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
    const bool isActive = o->element()->active();
	const bool isDisabled = !o->element()->isEnabled();

    i.context->save();	

    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();
    EA::WebKit::ButtonDrawInfo bdi;
    EA::Raster::ISurface* const pSurface = i.context->platformContext();;
    EA::WebKit::View* pView = NULL;
    if(pSurface)
        pView  = static_cast<EA::WebKit::View*>(pSurface->GetUserData());

	// 8/17/2010 Gautam Narain : We need to offset the rectangle by origin width and height. I am not sure if the mDirtyRect was incorrect due to this issue.
	// Hence I have commented that code instead of deleting it completely    
    bdi.mpView      = pView;
    bdi.mpSurface   = pSurface;
	//bdi.mDirtyRect	= r;			// This probably isn't correct.
	bdi.mDirtyRect  = EA::Raster::Rect(r.x() + i.context->origin().width(), r.y() + i.context->origin().height(), r.width(), r.height());        
    bdi.mButtonRect = EA::Raster::Rect(r.x() + i.context->origin().width(), r.y() + i.context->origin().height(), r.width(), r.height());
    bdi.mButtonType = EA::WebKit::ButtonDrawInfo::Button;
    bdi.mIsHovered  = false;    // How do we tell this?
    bdi.mIsChecked  = false;
    bdi.mIsFocused  = isActive;
	bdi.mIsDisabled = isDisabled;

	if(!pVN || !pVN->DrawButton(bdi))
    {
        o->paintFillLayerExtended(i, o->style()->backgroundColor(), o->style()->backgroundLayers(), r.y(), o->height(), r.x(), r.y(), o->width(), o->height());
        o->paintBorder(i.context, r.x(), r.y(), r.width(), r.height(), o->style(), true, true);
    }

    i.context->restore();

    return false; // WebKit appears to expect us to return false.
}


void RenderThemeBal::adjustMenuListStyle(CSSStyleSelector* selector, RenderStyle* style, WebCore::Element* e) const
{
    style->resetBorder();
    style->resetPadding();
    style->setHeight(Length(Auto));
    style->setWhiteSpace(PRE);

    style->setPaddingLeft(Length(kPopupMenuBorderPadding, Fixed));
    style->setPaddingRight(Length(kPopupMenuArrowSize + kPopupMenuBorderPadding, Fixed));  // Add space for the dropdown arrow graphic.

    // We don't currently have any padding here.
    // style->setPaddingTop(Length(___, Fixed));
    // style->setPaddingBottom(Length(___, Fixed));
}


bool RenderThemeBal::paintMenuList(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
    const bool isActive = o->element()->active();	
	const bool isDisabled = !o->element()->isEnabled();

	i.context->save();

    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();
    EA::WebKit::PopupMenuDrawInfo pdi;

    // This function is called to draw the 'inline' portion of a popup (a.k.a. dropdown) menu.
    // This menu consists of a static (uneditable) text box with a drop-down arrow on the right
    // side (for left-to-right locales) which indicates this is clickable item. When this is 
    // clicked then 
    
    EA::Raster::ISurface* const pSurface = i.context->platformContext();;
    EA::WebKit::View* pView = NULL;
    if(pSurface)
        pView  = static_cast<EA::WebKit::View*>(pSurface->GetUserData());
   
    
    pdi.mpView      = pView;
    pdi.mpSurface   = pSurface;
    pdi.mDirtyRect   = EA::Raster::Rect(r.x() + i.context->origin().width(), r.y() + i.context->origin().height(), r.width(), r.height());        // This probably isn't correct.
    pdi.mMenuRect    = EA::Raster::Rect(r.x() + i.context->origin().width(), r.y() + i.context->origin().height(), r.width(), r.height());
    pdi.mIsHovered   = false;    // How do we tell this?
    pdi.mIsFocused   = isActive;
    pdi.mIsActivated = false;
	pdi.mIsDisabled	 = isDisabled;

    if(!pVN || !pVN->DrawPopupMenu(pdi))
    {
        //WebCore::RenderStyle  style;
        RefPtr<RenderStyle> style = RenderStyle::create();


        WebCore::EBorderStyle borderStyle = WebCore::INSET;
        WebCore::Length       length(10, Fixed);
        WebCore::RenderStyle* pRenderStyle = o->style();

        style->setMarginLeft(length);
        style->setMarginTop(length);
        style->setBorderTopStyle(borderStyle);
        style->setBorderLeftStyle(borderStyle);
        style->setBorderBottomStyle(borderStyle);
        style->setBorderRightStyle(borderStyle);
        style->setBorderTopWidth(1);
        style->setBorderLeftWidth(1);
        style->setBorderBottomWidth(1);
        style->setBorderRightWidth(1);

        const int rx = r.x();
        const int ry = r.y();
        const int rw = r.width();
        const int rh = r.height();
        const int ow = o->width();
        const int oh = o->height();

        const WebCore::Color&     backColor  = pRenderStyle->backgroundColor();
        const WebCore::FillLayer* pFillLayer = pRenderStyle->backgroundLayers();

        // Fill the background.
        o->paintFillLayerExtended(i, backColor, pFillLayer, ry, oh, rx, ry, ow, oh);

        // Draw the outline.
        o->paintBorder(i.context, rx, ry, rw, rh, style.get(), true, true);

        // Paint the down arrow on the right side of the box.
        const IntSize& xyOrigin = i.context->origin();  // Need to take into account view scrolling.
        const int      arrowX   = xyOrigin.width()  + (rx + rw) - kPopupMenuBorderPadding - (kPopupMenuArrowSize / 2);
        const int      arrowY   = xyOrigin.height() + (ry + (rh / 2) + 2);

        EA::WebKit::GetEARasterInstance()->SimpleTriangle(pdi.mpSurface, arrowX, arrowY, 4, EA::Raster::kODown, Color(0xff000000));  // To consider: Make this color configurable or make it it match the text color or something.
    }

    i.context->restore();

    return false; // WebKit appears to expect us to return false.    
}


void RenderThemeBal::adjustTextFieldStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    style->resetBorder();
    style->resetPadding();
    style->setHeight(Length(Auto));
    style->setWhiteSpace(PRE);
}


bool RenderThemeBal::paintTextField(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
    i.context->save();

    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();
    EA::WebKit::TextFieldDrawInfo tdi;
    EA::WebKit::View* pView = NULL;
    EA::Raster::ISurface* const pSurface = i.context->platformContext();;
    if(pSurface)
        pView  = static_cast<EA::WebKit::View*>(pSurface->GetUserData());
    
    tdi.mpView      = pView;
    tdi.mpSurface   = pSurface;
    tdi.mDirtyRect  = r;        // This probably isn't correct.
    tdi.mTextRect   = r;

    if(!pVN || !pVN->DrawTextArea(tdi))
    {
        i.context->setStrokeStyle(SolidStroke);
        i.context->setStrokeColor(Color::black);
        RefPtr<RenderStyle> style = RenderStyle::create();
        addIntrinsicMargins(style.get());

        EBorderStyle v = INSET;
        style->setBorderTopStyle(v);
        style->setBorderLeftStyle(v);
        style->setBorderBottomStyle(v);
        style->setBorderRightStyle(v);
        int borderWidth = 1;
        style->setBorderTopWidth(borderWidth);
        style->setBorderLeftWidth(borderWidth);
        style->setBorderBottomWidth(borderWidth);
        style->setBorderRightWidth(borderWidth);
        o->paintFillLayerExtended(i,
            o->style()->backgroundColor(), o->style()->backgroundLayers(),
            r.y(), o->height(), r.x(), r.y(), o->width(), o->height());
        o->paintBorder(i.context,
            r.x(), r.y(), r.width(), r.height(),
            style.get(), true, true);
    }

    i.context->restore();

    return false; // WebKit appears to expect us to return false.
}


void RenderThemeBal::adjustTextAreaStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    adjustTextFieldStyle(selector, style, e);
}


bool RenderThemeBal::paintTextArea(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r)
{
    return paintTextField(o, i, r);
}


void RenderThemeBal::adjustSearchFieldResultsButtonStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    adjustSearchFieldCancelButtonStyle(selector, style, e);
}


bool RenderThemeBal::paintSearchFieldResultsButton(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& rect)
{
    return true;
}


void RenderThemeBal::adjustSearchFieldResultsDecorationStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    style->resetBorder();
    style->resetPadding();

    IntSize size = IntSize(14, 14);
    style->setWidth(Length(size.width(), Fixed));
    style->setHeight(Length(size.height(), Fixed));
}


bool RenderThemeBal::paintSearchFieldResultsDecoration(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& rect)
{
    return true;
}


void RenderThemeBal::adjustSearchFieldCancelButtonStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    style->resetBorder();
    style->resetPadding();

    IntSize size = IntSize(14, 14);
    style->setWidth(Length(size.width(), Fixed));
    style->setHeight(Length(size.height(), Fixed));
}

bool RenderThemeBal::paintSearchFieldCancelButton(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& rect)
{
    return true;
}

void RenderThemeBal::adjustSearchFieldStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    adjustTextFieldStyle(selector, style, e);
}

bool RenderThemeBal::paintSearchField(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& rect)
{
    return paintTextField(o, i, rect);
}


Color RenderThemeBal::platformActiveSelectionBackgroundColor() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mColors[EA::WebKit::kColorActiveSelectionBack].rgb();
}

Color RenderThemeBal::platformInactiveSelectionBackgroundColor() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mColors[EA::WebKit::kColorInactiveSelectionBack].rgb();
}

Color RenderThemeBal::platformActiveSelectionForegroundColor() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mColors[EA::WebKit::kColorActiveSelectionFore].rgb();
}

Color RenderThemeBal::platformInactiveSelectionForegroundColor() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mColors[EA::WebKit::kColorInactiveSelectionFore].rgb();
}

Color RenderThemeBal::activeListBoxSelectionBackgroundColor() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mColors[EA::WebKit::kColorActiveListBoxSelectionBack].rgb();
}

Color RenderThemeBal::inactiveListBoxSelectionBackgroundColor() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mColors[EA::WebKit::kColorInactiveListBoxSelectionBack].rgb();
}

Color RenderThemeBal::activeListBoxSelectionForegroundColor() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mColors[EA::WebKit::kColorActiveListBoxSelectionFore].rgb();
}

Color RenderThemeBal::inactiveListBoxSelectionForegroundColor() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mColors[EA::WebKit::kColorInactiveListBoxSelectionFore].rgb();
}


double RenderThemeBal::caretBlinkFrequency() const
{
    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();
    return param.mCaretBlinkSeconds;
}


void RenderThemeBal::systemFont(int /*propId*/, WebCore::FontDescription& fd) const
{
    // This function is called to retrieve system font information. System fonts
    // refer to "operating system" fonts used in operating system GUI elements such
    // as menus, text inputs, buttons. For web browsers like Safari these system
    // fonts are not treated the same as HTML fonts.

    // WebKit calls this function with fd already set to some default value. 
    // If we leave it alone then WebKit will use what is there.

    const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();

    // Note that Safari has code like this: 
    // switch (propId)
    // {
    //     case CSSValueSmallCaption:       Use a small font
    //     case CSSValueMenu:               Use a menu font
    //     case CSSValueStatusBar:          etc.
    //     case CSSValueWebkitMiniControl:
    //     case CSSValueWebkitSmallControl:
    //     case CSSValueWebkitControl:
    //     default:
    // }

    if(param.mSystemFontDescription.mFamilies[0])
    {
        fd.firstFamily().setFamily(param.mSystemFontDescription.mFamilies); // Actually, mFamilies is supposed to support a semicolon-delimited list.
        fd.setComputedSize((float)param.mSystemFontDescription.mSize);
        fd.setSpecifiedSize((float)param.mSystemFontDescription.mSize);
        fd.setIsAbsoluteSize(true);
        fd.setSmallCaps(false);

        if(param.mSystemFontDescription.mBold)
            fd.setWeight(WebCore::FontWeightBold);
        else
            fd.setWeight(WebCore::FontWeightNormal);

        fd.setItalic(param.mSystemFontDescription.mItalic);
        fd.setGenericFamily(FontDescription::NoFamily); // enum GenericFamilyType { NoFamily, StandardFamily, SerifFamily, SansSerifFamily, MonospaceFamily, CursiveFamily, FantasyFamily };
    }
}

BalContainer* RenderThemeBal::balContainer() const
{
    if (m_balContainer)
        return m_balContainer;

    return m_balContainer;
}

BalWidget* RenderThemeBal::balEntry() const
{
    if (m_balEntry)
        return m_balEntry;

    return m_balEntry;
}

BalWidget* RenderThemeBal::balTreeView() const
{
    if (m_balTreeView)
        return m_balTreeView;

    return m_balTreeView;
}

}
