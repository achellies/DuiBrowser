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
// LocalizedStrings.h
// By Paul Pedriana
//
///////////////////////////////////////////////////////////////////////////////


#ifndef LocalizedStrings_h
#define LocalizedStrings_h

#include <EAWebKit/EAWebKitLocalizedStringsEnum.h>

namespace WebCore {

    class String;
    class IntSize;

   String getLocalizedString(LocalizedStringType type);






    // To do: Propose to the WebKit people that we do away with all the standalone functions.
    // If that doesn't succeed then convert these to macros and cross fingers that they don't
    // collide with something else.

    /*
    inline String inputElementAltText()                            { return getLocalizedString(OWBAL::kLSinputElementAltText); }
    inline String resetButtonDefaultLabel()                        { return getLocalizedString(OWBAL::kLSresetButtonDefaultLabel); }
    inline String searchableIndexIntroduction()                    { return getLocalizedString(OWBAL::kLSsearchableIndexIntroduction); }
    inline String submitButtonDefaultLabel()                       { return getLocalizedString(OWBAL::kLSsubmitButtonDefaultLabel); }
    inline String fileButtonChooseFileLabel()                      { return getLocalizedString(OWBAL::kLSfileButtonChooseFileLabel); }
    inline String fileButtonNoFileSelectedLabel()                  { return getLocalizedString(OWBAL::kLSfileButtonNoFileSelectedLabel); }
    inline String copyImageUnknownFileLabel()                      { return getLocalizedString(OWBAL::kLScopyImageUnknownFileLabel); }
    inline String contextMenuItemTagOpenLinkInNewWindow()          { return getLocalizedString(OWBAL::kLScontextMenuItemTagOpenLinkInNewWindow); }
    inline String contextMenuItemTagDownloadLinkToDisk()           { return getLocalizedString(OWBAL::kLScontextMenuItemTagDownloadLinkToDisk); }
    inline String contextMenuItemTagCopyLinkToClipboard()          { return getLocalizedString(OWBAL::kLScontextMenuItemTagCopyLinkToClipboard); }
    inline String contextMenuItemTagOpenImageInNewWindow()         { return getLocalizedString(OWBAL::kLScontextMenuItemTagOpenImageInNewWindow); }
    inline String contextMenuItemTagDownloadImageToDisk()          { return getLocalizedString(OWBAL::kLScontextMenuItemTagDownloadImageToDisk); }
    inline String contextMenuItemTagCopyImageToClipboard()         { return getLocalizedString(OWBAL::kLScontextMenuItemTagCopyImageToClipboard); }
    inline String contextMenuItemTagOpenFrameInNewWindow()         { return getLocalizedString(OWBAL::kLScontextMenuItemTagOpenFrameInNewWindow); }
    inline String contextMenuItemTagCopy()                         { return getLocalizedString(OWBAL::kLScontextMenuItemTagCopy); }
    inline String contextMenuItemTagGoBack()                       { return getLocalizedString(OWBAL::kLScontextMenuItemTagGoBack); }
    inline String contextMenuItemTagGoForward()                    { return getLocalizedString(OWBAL::kLScontextMenuItemTagGoForward); }
    inline String contextMenuItemTagStop()                         { return getLocalizedString(OWBAL::kLScontextMenuItemTagStop); }
    inline String contextMenuItemTagReload()                       { return getLocalizedString(OWBAL::kLScontextMenuItemTagReload); }
    inline String contextMenuItemTagCut()                          { return getLocalizedString(OWBAL::kLScontextMenuItemTagCut); }
    inline String contextMenuItemTagPaste()                        { return getLocalizedString(OWBAL::kLScontextMenuItemTagPaste); }
    inline String contextMenuItemTagDelete()                       { return getLocalizedString(OWBAL::kLScontextMenuItemTagDelete); }
    inline String contextMenuItemTagSelectAll()                    { return getLocalizedString(OWBAL::kLScontextMenuItemTagSelectAll); }
    inline String contextMenuItemTagInputMethods()                 { return getLocalizedString(OWBAL::kLScontextMenuItemTagInputMethods); }
    inline String contextMenuItemTagUnicode()                      { return getLocalizedString(OWBAL::kLScontextMenuItemTagUnicode); }
    inline String contextMenuItemTagNoGuessesFound()               { return getLocalizedString(OWBAL::kLScontextMenuItemTagNoGuessesFound); }
    inline String contextMenuItemTagIgnoreSpelling()               { return getLocalizedString(OWBAL::kLScontextMenuItemTagIgnoreSpelling); }
    inline String contextMenuItemTagLearnSpelling()                { return getLocalizedString(OWBAL::kLScontextMenuItemTagLearnSpelling); }
    inline String contextMenuItemTagSearchWeb()                    { return getLocalizedString(OWBAL::kLScontextMenuItemTagSearchWeb); }
    inline String contextMenuItemTagLookUpInDictionary()           { return getLocalizedString(OWBAL::kLScontextMenuItemTagLookUpInDictionary); }
    inline String contextMenuItemTagOpenLink()                     { return getLocalizedString(OWBAL::kLScontextMenuItemTagOpenLink); }
    inline String contextMenuItemTagIgnoreGrammar()                { return getLocalizedString(OWBAL::kLScontextMenuItemTagIgnoreGrammar); }
    inline String contextMenuItemTagSpellingMenu()                 { return getLocalizedString(OWBAL::kLScontextMenuItemTagSpellingMenu); }
    inline String contextMenuItemTagShowSpellingPanel(bool show);  // Implemented in the .cpp file.
    inline String contextMenuItemTagCheckSpelling()                { return getLocalizedString(OWBAL::kLScontextMenuItemTagCheckSpelling); }
    inline String contextMenuItemTagCheckSpellingWhileTyping()     { return getLocalizedString(OWBAL::kLScontextMenuItemTagCheckSpellingWhileTyping); }
    inline String contextMenuItemTagCheckGrammarWithSpelling()     { return getLocalizedString(OWBAL::kLScontextMenuItemTagCheckGrammarWithSpelling); }
    inline String contextMenuItemTagFontMenu()                     { return getLocalizedString(OWBAL::kLScontextMenuItemTagFontMenu); }
    inline String contextMenuItemTagBold()                         { return getLocalizedString(OWBAL::kLScontextMenuItemTagBold); }
    inline String contextMenuItemTagItalic()                       { return getLocalizedString(OWBAL::kLScontextMenuItemTagItalic); }
    inline String contextMenuItemTagUnderline()                    { return getLocalizedString(OWBAL::kLScontextMenuItemTagUnderline); }
    inline String contextMenuItemTagOutline()                      { return getLocalizedString(OWBAL::kLScontextMenuItemTagOutline); }
    inline String contextMenuItemTagWritingDirectionMenu()         { return getLocalizedString(OWBAL::kLScontextMenuItemTagWritingDirectionMenu); }
    inline String contextMenuItemTagDefaultDirection()             { return getLocalizedString(OWBAL::kLScontextMenuItemTagDefaultDirection); }
    inline String contextMenuItemTagLeftToRight()                  { return getLocalizedString(OWBAL::kLScontextMenuItemTagLeftToRight); }
    inline String contextMenuItemTagRightToLeft()                  { return getLocalizedString(OWBAL::kLScontextMenuItemTagRightToLeft); }

    inline String contextMenuItemTagInspectElement()               { return getLocalizedString(OWBAL::kLScontextMenuItemTagInspectElement); }

    inline String searchMenuNoRecentSearchesText()                 { return getLocalizedString(OWBAL::kLSsearchMenuNoRecentSearchesText); }
    inline String searchMenuRecentSearchesText()                   { return getLocalizedString(OWBAL::kLSsearchMenuRecentSearchesText); }
    inline String searchMenuClearRecentSearchesText()              { return getLocalizedString(OWBAL::kLSsearchMenuClearRecentSearchesText); }

    inline String AXWebAreaText()                                  { return getLocalizedString(OWBAL::kLSAXWebAreaText); }
    inline String AXLinkText()                                     { return getLocalizedString(OWBAL::kLSAXLinkText); }
    inline String AXListMarkerText()                               { return getLocalizedString(OWBAL::kLSAXListMarkerText); }
    inline String AXImageMapText()                                 { return getLocalizedString(OWBAL::kLSAXImageMapText); }
    inline String AXHeadingText()                                  { return getLocalizedString(OWBAL::kLSAXHeadingText); }

    inline String AXButtonActionVerb()                             { return getLocalizedString(OWBAL::kLSAXButtonActionVerb); }
    inline String AXRadioButtonActionVerb()                        { return getLocalizedString(OWBAL::kLSAXRadioButtonActionVerb); }
    inline String AXTextFieldActionVerb()                          { return getLocalizedString(OWBAL::kLSAXTextFieldActionVerb); }
    inline String AXCheckedCheckBoxActionVerb()                    { return getLocalizedString(OWBAL::kLSAXCheckedCheckBoxActionVerb); }
    inline String AXUncheckedCheckBoxActionVerb()                  { return getLocalizedString(OWBAL::kLSAXUncheckedCheckBoxActionVerb); }
    inline String AXLinkActionVerb()                               { return getLocalizedString(OWBAL::kLSAXLinkActionVerb); }

    inline String unknownFileSizeText()                            { return getLocalizedString(OWBAL::kLSunknownFileSizeText); }
    inline String uploadFileText()                                 { return getLocalizedString(OWBAL::kLSuploadFileText); }
    inline String allFilesText()                                   { return getLocalizedString(OWBAL::kLSallFilesText); }

    String imageTitle(const String& filename, const IntSize& size); // Implemented in the .cpp file.
    */


    // This may be problematic due to collisions, though the long length of the macro names may help avoid this.
    #define inputElementAltText()                            getLocalizedString(OWBAL::kLSinputElementAltText)
    #define resetButtonDefaultLabel()                        getLocalizedString(OWBAL::kLSresetButtonDefaultLabel)
    #define searchableIndexIntroduction()                    getLocalizedString(OWBAL::kLSsearchableIndexIntroduction)
    #define submitButtonDefaultLabel()                       getLocalizedString(OWBAL::kLSsubmitButtonDefaultLabel)
    #define fileButtonChooseFileLabel()                      getLocalizedString(OWBAL::kLSfileButtonChooseFileLabel)
    #define fileButtonNoFileSelectedLabel()                  getLocalizedString(OWBAL::kLSfileButtonNoFileSelectedLabel)
    #define copyImageUnknownFileLabel()                      getLocalizedString(OWBAL::kLScopyImageUnknownFileLabel)
    #define contextMenuItemTagOpenLinkInNewWindow()          getLocalizedString(OWBAL::kLScontextMenuItemTagOpenLinkInNewWindow)
    #define contextMenuItemTagDownloadLinkToDisk()           getLocalizedString(OWBAL::kLScontextMenuItemTagDownloadLinkToDisk)
    #define contextMenuItemTagCopyLinkToClipboard()          getLocalizedString(OWBAL::kLScontextMenuItemTagCopyLinkToClipboard)
    #define contextMenuItemTagOpenImageInNewWindow()         getLocalizedString(OWBAL::kLScontextMenuItemTagOpenImageInNewWindow)
    #define contextMenuItemTagDownloadImageToDisk()          getLocalizedString(OWBAL::kLScontextMenuItemTagDownloadImageToDisk)
    #define contextMenuItemTagCopyImageToClipboard()         getLocalizedString(OWBAL::kLScontextMenuItemTagCopyImageToClipboard)
    #define contextMenuItemTagOpenFrameInNewWindow()         getLocalizedString(OWBAL::kLScontextMenuItemTagOpenFrameInNewWindow)
    #define contextMenuItemTagCopy()                         getLocalizedString(OWBAL::kLScontextMenuItemTagCopy)
    #define contextMenuItemTagGoBack()                       getLocalizedString(OWBAL::kLScontextMenuItemTagGoBack)
    #define contextMenuItemTagGoForward()                    getLocalizedString(OWBAL::kLScontextMenuItemTagGoForward)
    #define contextMenuItemTagStop()                         getLocalizedString(OWBAL::kLScontextMenuItemTagStop)
    #define contextMenuItemTagReload()                       getLocalizedString(OWBAL::kLScontextMenuItemTagReload)
    #define contextMenuItemTagCut()                          getLocalizedString(OWBAL::kLScontextMenuItemTagCut)
    #define contextMenuItemTagPaste()                        getLocalizedString(OWBAL::kLScontextMenuItemTagPaste)
    #define contextMenuItemTagDelete()                       getLocalizedString(OWBAL::kLScontextMenuItemTagDelete)
    #define contextMenuItemTagSelectAll()                    getLocalizedString(OWBAL::kLScontextMenuItemTagSelectAll)
    #define contextMenuItemTagInputMethods()                 getLocalizedString(OWBAL::kLScontextMenuItemTagInputMethods)
    #define contextMenuItemTagUnicode()                      getLocalizedString(OWBAL::kLScontextMenuItemTagUnicode)
    #define contextMenuItemTagNoGuessesFound()               getLocalizedString(OWBAL::kLScontextMenuItemTagNoGuessesFound)
    #define contextMenuItemTagIgnoreSpelling()               getLocalizedString(OWBAL::kLScontextMenuItemTagIgnoreSpelling)
    #define contextMenuItemTagLearnSpelling()                getLocalizedString(OWBAL::kLScontextMenuItemTagLearnSpelling)
    #define contextMenuItemTagSearchWeb()                    getLocalizedString(OWBAL::kLScontextMenuItemTagSearchWeb)
    #define contextMenuItemTagLookUpInDictionary()           getLocalizedString(OWBAL::kLScontextMenuItemTagLookUpInDictionary)
    #define contextMenuItemTagOpenLink()                     getLocalizedString(OWBAL::kLScontextMenuItemTagOpenLink)
    #define contextMenuItemTagIgnoreGrammar()                getLocalizedString(OWBAL::kLScontextMenuItemTagIgnoreGrammar)
    #define contextMenuItemTagSpellingMenu()                 getLocalizedString(OWBAL::kLScontextMenuItemTagSpellingMenu)

    #define contextMenuItemTagCheckSpelling()                getLocalizedString(OWBAL::kLScontextMenuItemTagCheckSpelling)
    #define contextMenuItemTagCheckSpellingWhileTyping()     getLocalizedString(OWBAL::kLScontextMenuItemTagCheckSpellingWhileTyping)
    #define contextMenuItemTagCheckGrammarWithSpelling()     getLocalizedString(OWBAL::kLScontextMenuItemTagCheckGrammarWithSpelling)
    #define contextMenuItemTagFontMenu()                     getLocalizedString(OWBAL::kLScontextMenuItemTagFontMenu)
    #define contextMenuItemTagBold()                         getLocalizedString(OWBAL::kLScontextMenuItemTagBold)
    #define contextMenuItemTagItalic()                       getLocalizedString(OWBAL::kLScontextMenuItemTagItalic)
    #define contextMenuItemTagUnderline()                    getLocalizedString(OWBAL::kLScontextMenuItemTagUnderline)
    #define contextMenuItemTagOutline()                      getLocalizedString(OWBAL::kLScontextMenuItemTagOutline)
    #define contextMenuItemTagWritingDirectionMenu()         getLocalizedString(OWBAL::kLScontextMenuItemTagWritingDirectionMenu)
    #define contextMenuItemTagDefaultDirection()             getLocalizedString(OWBAL::kLScontextMenuItemTagDefaultDirection)
    #define contextMenuItemTagLeftToRight()                  getLocalizedString(OWBAL::kLScontextMenuItemTagLeftToRight)
    #define contextMenuItemTagRightToLeft()                  getLocalizedString(OWBAL::kLScontextMenuItemTagRightToLeft)

    #define contextMenuItemTagInspectElement()               getLocalizedString(OWBAL::kLScontextMenuItemTagInspectElement)

    #define searchMenuNoRecentSearchesText()                 getLocalizedString(OWBAL::kLSsearchMenuNoRecentSearchesText)
    #define searchMenuRecentSearchesText()                   getLocalizedString(OWBAL::kLSsearchMenuRecentSearchesText)
    #define searchMenuClearRecentSearchesText()              getLocalizedString(OWBAL::kLSsearchMenuClearRecentSearchesText)

    #define AXWebAreaText()                                  getLocalizedString(OWBAL::kLSAXWebAreaText)
    #define AXLinkText()                                     getLocalizedString(OWBAL::kLSAXLinkText)
    #define AXListMarkerText()                               getLocalizedString(OWBAL::kLSAXListMarkerText)
    #define AXImageMapText()                                 getLocalizedString(OWBAL::kLSAXImageMapText)
    #define AXHeadingText()                                  getLocalizedString(OWBAL::kLSAXHeadingText)

    #define AXButtonActionVerb()                             getLocalizedString(OWBAL::kLSAXButtonActionVerb)
    #define AXRadioButtonActionVerb()                        getLocalizedString(OWBAL::kLSAXRadioButtonActionVerb)
    #define AXTextFieldActionVerb()                          getLocalizedString(OWBAL::kLSAXTextFieldActionVerb)
    #define AXCheckedCheckBoxActionVerb()                    getLocalizedString(OWBAL::kLSAXCheckedCheckBoxActionVerb)
    #define AXUncheckedCheckBoxActionVerb()                  getLocalizedString(OWBAL::kLSAXUncheckedCheckBoxActionVerb)
    #define AXLinkActionVerb()                               getLocalizedString(OWBAL::kLSAXLinkActionVerb)

    #define unknownFileSizeText()                            getLocalizedString(OWBAL::kLSunknownFileSizeText)
    #define uploadFileText()                                 getLocalizedString(OWBAL::kLSuploadFileText)
    #define allFilesText()                                   getLocalizedString(OWBAL::kLSallFilesText)

    // Implemented in the .cpp file.
    String contextMenuItemTagShowSpellingPanel(bool show);  
    String imageTitle(const String& filename, const IntSize& size);

} // namespace 

#endif
