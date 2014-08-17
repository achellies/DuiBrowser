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
// Modified by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef ContextMenuItem_h
#define ContextMenuItem_h

#include <wtf/FastAllocBase.h>
#include "PlatformMenuDescription.h"
#include "PlatformString.h"
#include <wtf/OwnPtr.h>
#include "BALBase.h"

namespace WKAL {

    class ContextMenu;

    // This enum needs to be in sync with the WebMenuItemTag enum in WebUIDelegate.h and the
    // extra values in WebUIDelegatePrivate.h
    enum ContextMenuAction {
        ContextMenuItemTagNoAction=0, // This item is not actually in WebUIDelegate.h
        ContextMenuItemTagOpenLinkInNewWindow=1,
        ContextMenuItemTagDownloadLinkToDisk,
        ContextMenuItemTagCopyLinkToClipboard,
        ContextMenuItemTagOpenImageInNewWindow,
        ContextMenuItemTagDownloadImageToDisk,
        ContextMenuItemTagCopyImageToClipboard,
        ContextMenuItemTagOpenFrameInNewWindow,
        ContextMenuItemTagCopy,
        ContextMenuItemTagGoBack,
        ContextMenuItemTagGoForward,
        ContextMenuItemTagStop,
        ContextMenuItemTagReload,
        ContextMenuItemTagCut,
        ContextMenuItemTagPaste,
        ContextMenuItemTagDelete,
        ContextMenuItemTagSelectAll,
        ContextMenuItemTagInputMethods,
        ContextMenuItemTagUnicode,
        ContextMenuItemTagSpellingGuess,
        ContextMenuItemTagNoGuessesFound,
        ContextMenuItemTagIgnoreSpelling,
        ContextMenuItemTagLearnSpelling,
        ContextMenuItemTagOther,
        ContextMenuItemTagSearchInSpotlight,
        ContextMenuItemTagSearchWeb,
        ContextMenuItemTagLookUpInDictionary,
        ContextMenuItemTagOpenWithDefaultApplication,
        ContextMenuItemPDFActualSize,
        ContextMenuItemPDFZoomIn,
        ContextMenuItemPDFZoomOut,
        ContextMenuItemPDFAutoSize,
        ContextMenuItemPDFSinglePage,
        ContextMenuItemPDFFacingPages,
        ContextMenuItemPDFContinuous,
        ContextMenuItemPDFNextPage,
        ContextMenuItemPDFPreviousPage,
        // These are new tags! Not a part of API!!!!
        ContextMenuItemTagOpenLink = 2000,
        ContextMenuItemTagIgnoreGrammar,
        ContextMenuItemTagSpellingMenu, // Spelling or Spelling/Grammar sub-menu
        ContextMenuItemTagShowSpellingPanel,
        ContextMenuItemTagCheckSpelling,
        ContextMenuItemTagCheckSpellingWhileTyping,
        ContextMenuItemTagCheckGrammarWithSpelling,
        ContextMenuItemTagFontMenu, // Font sub-menu
        ContextMenuItemTagShowFonts,
        ContextMenuItemTagBold,
        ContextMenuItemTagItalic,
        ContextMenuItemTagUnderline,
        ContextMenuItemTagOutline,
        ContextMenuItemTagStyles,
        ContextMenuItemTagShowColors,
        ContextMenuItemTagSpeechMenu, // Speech sub-menu
        ContextMenuItemTagStartSpeaking,
        ContextMenuItemTagStopSpeaking,
        ContextMenuItemTagWritingDirectionMenu, // Writing Direction sub-menu
        ContextMenuItemTagDefaultDirection,
        ContextMenuItemTagLeftToRight,
        ContextMenuItemTagRightToLeft,
        ContextMenuItemTagPDFSinglePageScrolling,
        ContextMenuItemTagPDFFacingPagesScrolling,
        ContextMenuItemTagInspectElement,
        ContextMenuItemBaseApplicationTag = 10000
    };

    enum ContextMenuItemType {
        ActionType,
        CheckableActionType,
        SeparatorType,
        SubmenuType
    };

    struct PlatformMenuItemDescription/*: public WTF::FastAllocBase*/ {
        PlatformMenuItemDescription()
            : type(ActionType)
            , action(ContextMenuItemTagNoAction)
            , subMenu(0)
            , checked(false)
            , enabled(true)
        {}

        ContextMenuItemType type;
        ContextMenuAction action;
        String title;
        BalMenu* subMenu;
        bool checked;
        bool enabled;
    };


    class ContextMenuItem/*: public WTF::FastAllocBase*/ {
    public:
        ContextMenuItem(PlatformMenuItemDescription);
        ContextMenuItem(ContextMenu* subMenu = 0);
        ContextMenuItem(ContextMenuItemType type, ContextMenuAction action, const String& title, ContextMenu* subMenu = 0);
        ContextMenuItem(BalMenuItem*);
        ~ContextMenuItem();

        PlatformMenuItemDescription releasePlatformDescription();

        ContextMenuItemType type() const;
        void setType(ContextMenuItemType);

        ContextMenuAction action() const;
        void setAction(ContextMenuAction);

        String title() const;
        void setTitle(const String&);

        PlatformMenuDescription platformSubMenu() const;
        void setSubMenu(ContextMenu*);

        void setChecked(bool = true);
        
        void setEnabled(bool = true);
        bool enabled() const;

        // FIXME: Do we need a keyboard accelerator here?
        static BalMenuItem* createNativeMenuItem(const PlatformMenuItemDescription&);

    private:
        PlatformMenuItemDescription m_platformDescription;
    };

}

#endif // ContextMenuItem_h
