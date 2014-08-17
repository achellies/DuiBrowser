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
* This file was modified by Electronic Arts Inc Copyright © 2008-2010
*/

///////////////////////////////////////////////////////////////////////////////
// BCPasteboardEA.cpp
// Modified by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "Pasteboard.h"

#include "CString.h"
#include "DocumentFragment.h"
#include "Frame.h"
#include "PlatformString.h"
#include "Image.h"
#include "RenderImage.h"
#include "KURL.h"
#include "markup.h"
#include <WebCore/dom/Range.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitView.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>
#include <stdio.h>
#include <wtf/FastAllocBase.h>


namespace WKAL {

/* FIXME: we must get rid of this and use the enum in webkitwebview.h someway */
enum WebKitWebViewTargetInfo
{
    WEBKIT_WEB_VIEW_TARGET_INFO_HTML = -1,
    WEBKIT_WEB_VIEW_TARGET_INFO_TEXT = -2
};


Pasteboard* Pasteboard::m_StaticPasteboard = NULL;

Pasteboard* Pasteboard::generalPasteboard()
{
    // OWB_PRINTF("Pasteboard::generalPasteboard\n");
    if(m_StaticPasteboard == NULL)
        m_StaticPasteboard = new Pasteboard();
    return m_StaticPasteboard;
}

void Pasteboard::staticFinalize()
{
    if(m_StaticPasteboard)
    {
        delete m_StaticPasteboard;
        m_StaticPasteboard = NULL;
    }
}

// Read from the clipboard
String Pasteboard::plainText(Frame* pFrame)
{
    WebCore::String retVal;

    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();

    if(pVN) // This should always be true.
    {
        EA::WebKit::ClipboardEventInfo cei;

        cei.mpView = EA::WebKit::GetView(pFrame);
        cei.mReadFromClipboard = true;

        pVN->ClipboardEvent(cei); // We can ignore the return value and just look at cei.mText.

        if(!GetFixedString(cei.mText)->empty())
            retVal = GetFixedString(cei.mText)->c_str();
    }

    return retVal;
}

// Write to the clipboard
void Pasteboard::writeSelection(Range* /*selectedRange*/, bool /*canSmartCopyOrDelete*/, Frame* frame)
{
    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();

    if(pVN) // This should always be true.
    {
        EA::WebKit::ClipboardEventInfo cei;
        EA::WebKit::View*   pView= NULL;
        if(frame)
            pView = EA::WebKit::GetView(frame);
        
        cei.mpView = pView;
        cei.mReadFromClipboard = false;

        const String str = frame->selectedText();
        GetFixedString(cei.mText)->assign(str.characters(), str.length());

        pVN->ClipboardEvent(cei);
    }
}

void Pasteboard::writeURL(const KURL& /*url*/, const String&, Frame* /*frame*/)
{
    // OWB_PRINTF("Pasteboard::writeURL\n");
}

void Pasteboard::writeImage(Node* /*node*/, const KURL&, const String&)
{
    // OWB_PRINTF("Pasteboard::writeImage\n");
    // NotImplemented();
}

void Pasteboard::clear()
{
    // We don't currently have a means to clear the clipboard, 
    // other than setting it to an empty string.
}

bool Pasteboard::canSmartReplace()
{
    // OWB_PRINTF("Pasteboard::canSmartReplace\n");
    // NotImplemented();
    return false;
}

PassRefPtr<DocumentFragment> Pasteboard::documentFragment(Frame* /*frame*/, PassRefPtr<Range>, bool /*allowPlainText*/, bool& /*chosePlainText*/)
{
    // OWB_PRINTF("Pasteboard::documentFragment\n");
    return 0; 
}

//void Pasteboard::setHelper(PasteboardHelper* helper)
//{
//    m_helper = helper;
//}


}   //namespace WKAL 
