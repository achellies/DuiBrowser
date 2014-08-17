/*
Copyright (C) 2004,2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// LocalizedStrings.cpp
// By Paul Pedriana
//
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "LocalizedStrings.h"
#include "PlatformString.h"
#include "IntSize.h"
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitViewHelper.h> // For multiview support

namespace OWBAL {


String getLocalizedString(LocalizedStringType type)
{
    bool bHandled = false;
    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();

    if(pVN)
    {
        EA::WebKit::View* pView = EA::WebKit::AutoSetActiveView::GetActiveView();
        EA::WebKit::StringInfo si = { pView, type }; si.mString16[0] = 0; si.mString8[0] = 0;
        bHandled = pVN->GetString(si);

        if(bHandled)
        {
            if(si.mString16[0])
                return String(si.mString16);
            else
                return String::fromUTF8(si.mString8);
        }
    }

    if(!bHandled)
    {
        #if EAWEBKIT_DEFAULT_STRING_HANDLING_ENABLED && defined(EA_PLATFORM_WINDOWS)
            const char  empty = 0;
            const char* p;

            switch (type)
            {
                case kLSsearchableIndexIntroduction:                    p = "This is a searchable index. Enter search keywords: "; break;
                case kLSsubmitButtonDefaultLabel:                       p = "Submit"; break;
                case kLSinputElementAltText:                            p = "Submit"; break;
                case kLSresetButtonDefaultLabel:                        p = "Reset"; break;
                case kLSfileButtonChooseFileLabel:                      p = "Choose File"; break;
                case kLSfileButtonNoFileSelectedLabel:                  p = "no file selected"; break;
                case kLScontextMenuItemTagOpenLinkInNewWindow:          p = "Open Link in New Window"; break;
                case kLScontextMenuItemTagDownloadLinkToDisk:           p = "Download Linked File"; break;
                case kLScontextMenuItemTagCopyLinkToClipboard:          p = "Copy Link"; break;
                case kLScontextMenuItemTagOpenImageInNewWindow:         p = "Open Image in New Window"; break;
                case kLScontextMenuItemTagDownloadImageToDisk:          p = "Download Image"; break;
                case kLScontextMenuItemTagCopyImageToClipboard:         p = "Copy Image"; break;
                case kLScontextMenuItemTagOpenFrameInNewWindow:         p = "Open Frame in New Window"; break;
                case kLScontextMenuItemTagCopy:                         p = "Copy"; break;
                case kLScontextMenuItemTagGoBack:                       p = "Back"; break;
                case kLScontextMenuItemTagGoForward:                    p = "Forward"; break;
                case kLScontextMenuItemTagStop:                         p = "Stop"; break;
                case kLScontextMenuItemTagReload:                       p = "Reload"; break;
                case kLScontextMenuItemTagCut:                          p = "Cut"; break;
                case kLScontextMenuItemTagPaste:                        p = "Paste"; break;
                case kLScontextMenuItemTagNoGuessesFound:               p = "No Guesses Found"; break;
                case kLScontextMenuItemTagIgnoreSpelling:               p = "Ignore Spelling"; break;
                case kLScontextMenuItemTagLearnSpelling:                p = "Learn Spelling"; break;
                case kLScontextMenuItemTagSearchWeb:                    p = "Search with Google"; break;
                case kLScontextMenuItemTagLookUpInDictionary:           p = "Look Up in Dictionary"; break;
                case kLScontextMenuItemTagOpenLink:                     p = "Open Link"; break;
                case kLScontextMenuItemTagIgnoreGrammar:                p = "Ignore Grammar"; break;
                case kLScontextMenuItemTagSpellingMenu:                 p = "Spelling and Grammar"; break;
                case kLScontextMenuItemTagCheckSpelling:                p = "Check Document Now"; break;
                case kLScontextMenuItemTagCheckSpellingWhileTyping:     p = "Check Spelling While Typing"; break;
                case kLScontextMenuItemTagCheckGrammarWithSpelling:     p = "Check Grammar With Spelling"; break;
                case kLScontextMenuItemTagFontMenu:                     p = "Font"; break;
                case kLScontextMenuItemTagBold:                         p = "Bold"; break;
                case kLScontextMenuItemTagItalic:                       p = "Italic"; break;
                case kLScontextMenuItemTagUnderline:                    p = "Underline"; break;
                case kLScontextMenuItemTagOutline:                      p = "Outline"; break;
                case kLScontextMenuItemTagWritingDirectionMenu:         p = "Writing Direction"; break;
                case kLScontextMenuItemTagDefaultDirection:             p = "Default"; break;
                case kLScontextMenuItemTagLeftToRight:                  p = "Left to Right"; break;
                case kLScontextMenuItemTagRightToLeft:                  p = "Right to Left"; break;
                case kLScontextMenuItemTagInspectElement:               p = "Inspect Element"; break;
                case kLSsearchMenuNoRecentSearchesText:                 p = "No recent searches"; break;
                case kLSsearchMenuRecentSearchesText:                   p = "Recent Searches"; break;
                case kLSsearchMenuClearRecentSearchesText:              p = "Clear Recent Searches"; break;
                case kLSAXWebAreaText:                                  p = "web area"; break;
                case kLSAXLinkText:                                     p = "link"; break;
                case kLSAXListMarkerText:                               p = "list marker"; break;
                case kLSAXImageMapText:                                 p = "image map"; break;
                case kLSAXHeadingText:                                  p = "heading"; break;
                case kLSAXButtonActionVerb:                             p = "press"; break;
                case kLSAXRadioButtonActionVerb:                        p = "select"; break;
                case kLSAXTextFieldActionVerb:                          p = "activate"; break;
                case kLSAXCheckedCheckBoxActionVerb:                    p = "uncheck"; break;
                case kLSAXUncheckedCheckBoxActionVerb:                  p = "check"; break;
                case kLSAXLinkActionVerb:                               p = "jump"; break;
                case kLSunknownFileSizeText:                            p = "Unknown"; break;
                case kLSuploadFileText:                                 p = "Upload file"; break;
                case kLSallFilesText:                                   p = "All Files"; break;
                default:                                                p = &empty; break;
            }

            return String::fromUTF8(p);
        #endif
    }

    return String(); // We don't want to be executing this.
}


String contextMenuItemTagShowSpellingPanel(bool show)
{
    return String(show ? L"_Show Spelling and Grammar" : L"_Hide Spelling and Grammar");
}


String imageTitle(const String& filename, const IntSize& size)
{
    String result;
    result.format("%ls (%d×%d)", filename.characters(), size.width(), size.height());
    return result;
}



} // namespace
