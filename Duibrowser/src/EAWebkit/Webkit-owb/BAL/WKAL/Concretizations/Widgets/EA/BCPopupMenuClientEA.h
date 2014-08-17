/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

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

///////////////////////////////////////////////////////////////////////////////
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef PopupMenuClient_h
#define PopupMenuClient_h

#include "BALBase.h"

namespace WebCore {

class Color;
class Document;
class FontSelector;
class String;
class RenderStyle;
class RenderBlock;

class PopupMenuClient/* : public WKALBase */{
public:
    virtual ~PopupMenuClient() {}
    virtual void valueChanged(unsigned listIndex, bool fireEvents = true) = 0;

    virtual String itemText(unsigned listIndex) const = 0;
    virtual bool itemIsEnabled(unsigned listIndex) const = 0;
    virtual Color itemBackgroundColor(unsigned listIndex) const = 0;
    virtual RenderStyle* itemStyle(unsigned listIndex) const = 0;
    virtual RenderStyle* clientStyle() const = 0;
    virtual Document* clientDocument() const = 0;
    virtual RenderBlock* clientRenderBlock() = 0;
    virtual int clientInsetLeft() const = 0;
    virtual int clientInsetRight() const = 0;
    virtual int clientPaddingLeft() const = 0;
    virtual int clientPaddingRight() const = 0;
    virtual int listSize() const = 0;
    virtual int selectedIndex() const = 0;
    virtual void hidePopup() = 0;
    virtual bool itemIsSeparator(unsigned listIndex) const = 0;
    virtual bool itemIsLabel(unsigned listIndex) const = 0;
    virtual bool itemIsSelected(unsigned listIndex) const = 0;
    virtual bool shouldPopOver() const = 0;
    virtual bool valueShouldChangeOnHotTrack() const = 0;
    virtual void setTextFromItem(unsigned listIndex) = 0;
    virtual FontSelector* fontSelector() const = 0;
};

}

#endif
