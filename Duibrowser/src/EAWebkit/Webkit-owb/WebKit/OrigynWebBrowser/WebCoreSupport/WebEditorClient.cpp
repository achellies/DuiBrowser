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
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"
#include "WebEditorClient.h"

#include "WebView.h"

#include <PlatformString.h>
#include <Document.h>
#include <EditCommand.h>
#include <HTMLElement.h>
#include <HTMLInputElement.h>
#include <HTMLNames.h>
#include <KeyboardEvent.h>
#include <PlatformKeyboardEvent.h>
#include <NotImplemented.h>
#include <Range.h>
#include <EAWebKit/EAWebKitView.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;
using namespace HTMLNames;

#include <wtf/FastAllocBase.h>

struct KeyBoardClassMap
{
	EA::WebKit::KeyboardType mKeyboardType;
	const char*	mKeyboardClass;			
};
static KeyBoardClassMap sKeyBoardClassMap[ ] =
{
	{EA::WebKit::kDefaultKeyBoard,					"eaw-kb-default"},
	{EA::WebKit::kNumericKeyBoard,					"eaw-kb-numeric"},
	{EA::WebKit::kJapaneseFullKeyBoard,				"eaw-kb-japanese-full"},
	{EA::WebKit::kJapaneseHiraganaKeyBoard,			"eaw-kb-japanese-hiragana"},
	{EA::WebKit::kJapaneseKatakanaKeyBoard,			"eaw-kb-japanese-katakana"},
	{EA::WebKit::kRussianFullKeyBoard,				"eaw-kb-russian"},
	{EA::WebKit::kKoreanFullKeyBoard,				"eaw-kb-korean"},
	{EA::WebKit::kTraditionalChineseFullKeyBoard,	"eaw-kb-traditional-chinese"},
	{EA::WebKit::kSimplifiedChineseKeyBoard,		"eaw-kb-simplified-chinese"},
	{EA::WebKit::kLatinFullKeyBoard,				"eaw-kb-latin-full"},
	{EA::WebKit::kURLKeyBoard,						"eaw-kb-url"},
	{EA::WebKit::kPasswordKeyBoard,					"eaw-kb-password"},
	{EA::WebKit::kEmailKeyBoard,					"eaw-kb-email"},
	{EA::WebKit::kGamerTagKeyBoard,					"eaw-kb-gamertag"},
	{EA::WebKit::kPhoneKeyBoard,					"eaw-kb-phone"},
	{EA::WebKit::kIPAddressKeyBoard,				"eaw-kb-ipaddress"},
	
	
	{EA::WebKit::kCountKeyBoardTypes,				0}
};

class WebUndoCommand/*: public WTF::FastAllocBase*/ {
public:
    WebUndoCommand() {DS_CONSTRUCT();}
    virtual ~WebUndoCommand() {DS_DESTRUCT();}
    virtual void execute() = 0;
};

class WebEditorUndoTarget/*: public WTF::FastAllocBase*/
{
public:
    WebEditorUndoTarget();
    virtual ~WebEditorUndoTarget() {DS_DESTRUCT();}
    virtual void invoke(String actionName, WebUndoCommand *obj);

};

WebEditorUndoTarget::WebEditorUndoTarget()
{
    DS_CONSTRUCT();
}


void WebEditorUndoTarget::invoke(String actionName, WebUndoCommand *undoCommand)
{
    if(undoCommand) {
        undoCommand->execute();
    }
}

WebEditorClient::WebEditorClient(WebView* webView)
    : m_webView(webView)
    , m_undoTarget(0)
{
    m_undoTarget = new WebEditorUndoTarget();
    DS_CONSTRUCT();
}

WebEditorClient::~WebEditorClient()
{
    delete m_undoTarget;
    DS_DESTRUCT();
}

void WebEditorClient::pageDestroyed()
{
    delete this;
}

bool WebEditorClient::isContinuousSpellCheckingEnabled()
{
    bool enabled = m_webView->isContinuousSpellCheckingEnabled();
    return !!enabled;
}

void WebEditorClient::toggleContinuousSpellChecking()
{
    //m_webView->toggleContinuousSpellChecking(0);
}

bool WebEditorClient::isGrammarCheckingEnabled()
{
    bool enabled = m_webView->isGrammarCheckingEnabled();
    return !!enabled;
}

void WebEditorClient::toggleGrammarChecking()
{
    //m_webView->toggleGrammarChecking(0);
}

/*static void initViewSpecificSpelling(WebView* view)
{
    // we just use this as a flag to indicate that we've spell checked the document
    // and need to close the spell checker out when the view closes.
    int tag = view->spellCheckerDocumentTag();
}*/

int WebEditorClient::spellCheckerDocumentTag()
{
    // we don't use the concept of spelling tags
    notImplemented();
    return 0;
}

bool WebEditorClient::shouldBeginEditing(Range*)
{
    notImplemented();
    return true;
}

bool WebEditorClient::shouldEndEditing(Range*)
{
    notImplemented();
    return true;
}

void WebEditorClient::didBeginEditing()
{
    notImplemented();
}

void WebEditorClient::respondToChangedContents()
{
    notImplemented();
}

void WebEditorClient::respondToChangedSelection()
{
    m_webView->selectionChanged();
}

void WebEditorClient::didEndEditing()
{
    notImplemented();
}

void WebEditorClient::didWriteSelectionToPasteboard()
{
    notImplemented();
}

void WebEditorClient::didSetSelectionTypesForPasteboard()
{
    notImplemented();
}

bool WebEditorClient::shouldDeleteRange(Range* /*range*/)
{
    notImplemented(); 
    return true; 

    // FIXME: calling m_webView->editingDelegate() will cause an assertion failure so we don't want to enable this code until that's implemented. 
    //BOOL result = false;
    //IWebViewEditingDelegate* editingDelegate;
    //// FIXME: DOMRange needs to be implemented before anything meaningful can be done here
    //IDOMRange* domRange(0);
    //if (SUCCEEDED(m_webView->editingDelegate(&editingDelegate))) {
    //    editingDelegate->shouldDeleteDOMRange(m_webView, domRange, &result);
    //    editingDelegate->Release();
    //}
    //return !!result;
}

bool WebEditorClient::shouldInsertNode(Node* /*node*/, Range* /*replacingRange*/, EditorInsertAction /*givenAction*/)
{ 
    notImplemented(); 
    return true; 
}

bool WebEditorClient::shouldInsertText(const String& /*str*/, Range* /* replacingRange */, EditorInsertAction /*givenAction*/)
{     
    notImplemented(); 
    return true; 

    // FIXME: calling m_webView->editingDelegate() will cause an assertion failure so we don't want to enable this code until that's implemented. 
    //BOOL result = false;
    //IWebViewEditingDelegate* editingDelegate;
    //// FIXME: DOMRange needs to be implemented before anything meaningful can be done here
    //IDOMRange* domRange(0); // make a DOMRange from replacingRange
    //BString text(str);
    //if (SUCCEEDED(m_webView->editingDelegate(&editingDelegate))) {
    //    editingDelegate->shouldInsertText(m_webView, text, domRange, (WebViewInsertAction) givenAction, &result);
    //    editingDelegate->Release();
    //}
    //return !!result;
}

//bool WebEditorClient::shouldChangeSelectedRange(Range *currentRange, Range *toProposedRange, SelectionAffinity selectionAffinity, bool stillSelecting)
//{ notImplemented(); return false; }

bool WebEditorClient::shouldApplyStyle(CSSStyleDeclaration* /*style*/, Range* /*toElementsInDOMRange*/)
{ notImplemented(); return true; }

bool WebEditorClient::shouldMoveRangeAfterDelete(Range* /*range*/, Range* /*rangeToBeReplaced*/)
{ notImplemented(); return true; }

bool WebEditorClient::shouldChangeTypingStyle(CSSStyleDeclaration* /*currentStyle*/, CSSStyleDeclaration* /*toProposedStyle*/)
{ notImplemented(); return false; }

void WebEditorClient::webViewDidChangeTypingStyle(WebNotification* /*notification*/)
{  notImplemented(); }

void WebEditorClient::webViewDidChangeSelection(WebNotification* /*notification*/)
{  notImplemented(); }

bool WebEditorClient::shouldShowDeleteInterface(HTMLElement* /*element*/)
{ notImplemented(); return false; }

bool WebEditorClient::smartInsertDeleteEnabled(void)
{ 
    return m_webView->smartInsertDeleteEnabled();
}

bool WebEditorClient::shouldChangeSelectedRange(WebCore::Range*, WebCore::Range*, WebCore::EAffinity, bool)
{ notImplemented(); return true; }

void WebEditorClient::textFieldDidBeginEditing(Element* e)
{
    /*IWebFormDelegate* formDelegate;
    if (SUCCEEDED(m_webView->formDelegate(&formDelegate)) && formDelegate) {
        IDOMElement* domElement = DOMElement::createInstance(e);
        if (domElement) {
            IDOMHTMLInputElement* domInputElement;
            if (SUCCEEDED(domElement->QueryInterface(IID_IDOMHTMLInputElement, (void**)&domInputElement))) {
                formDelegate->textFieldDidBeginEditing(domInputElement, kit(e->document()->frame()));
                domInputElement->Release();
            }
            domElement->Release();
        }
        formDelegate->Release();
    }*/
}

void WebEditorClient::textFieldDidEndEditing(Element* e)
{
    /*IWebFormDelegate* formDelegate;
    if (SUCCEEDED(m_webView->formDelegate(&formDelegate)) && formDelegate) {
        IDOMElement* domElement = DOMElement::createInstance(e);
        if (domElement) {
            IDOMHTMLInputElement* domInputElement;
            if (SUCCEEDED(domElement->QueryInterface(IID_IDOMHTMLInputElement, (void**)&domInputElement))) {
                formDelegate->textFieldDidEndEditing(domInputElement, kit(e->document()->frame()));
                domInputElement->Release();
            }
            domElement->Release();
        }
        formDelegate->Release();
    }*/
}

void WebEditorClient::textDidChangeInTextField(Element* e)
{
    /*IWebFormDelegate* formDelegate;
    if (SUCCEEDED(m_webView->formDelegate(&formDelegate)) && formDelegate) {
        IDOMElement* domElement = DOMElement::createInstance(e);
        if (domElement) {
            IDOMHTMLInputElement* domInputElement;
            if (SUCCEEDED(domElement->QueryInterface(IID_IDOMHTMLInputElement, (void**)&domInputElement))) {
                formDelegate->textDidChangeInTextField(domInputElement, kit(e->document()->frame()));
                domInputElement->Release();
            }
            domElement->Release();
        }
        formDelegate->Release();
    }*/
}

bool WebEditorClient::doTextFieldCommandFromEvent(Element* e, KeyboardEvent* ke)
{
    /*BOOL result = FALSE;
    IWebFormDelegate* formDelegate;
    if (SUCCEEDED(m_webView->formDelegate(&formDelegate)) && formDelegate) {
        IDOMElement* domElement = DOMElement::createInstance(e);
        if (domElement) {
            IDOMHTMLInputElement* domInputElement;
            if (SUCCEEDED(domElement->QueryInterface(IID_IDOMHTMLInputElement, (void**)&domInputElement))) {
                String command = m_webView->interpretKeyEvent(ke);
                // We allow empty commands here because the app code actually depends on this being called for all key presses.
                // We may want to revisit this later because it doesn't really make sense to send an empty command.
                formDelegate->doPlatformCommand(domInputElement, BString(command), kit(e->document()->frame()), &result);
                domInputElement->Release();
            }
            domElement->Release();
        }
        formDelegate->Release();
    }
    return !!result;*/
    return false;
}

void WebEditorClient::textWillBeDeletedInTextField(Element* e)
{
    // We're using the deleteBackward command for all deletion operations since the autofill code treats all deletions the same way.
    /*IWebFormDelegate* formDelegate;
    if (SUCCEEDED(m_webView->formDelegate(&formDelegate)) && formDelegate) {
        IDOMElement* domElement = DOMElement::createInstance(e);
        if (domElement) {
            IDOMHTMLInputElement* domInputElement;
            if (SUCCEEDED(domElement->QueryInterface(IID_IDOMHTMLInputElement, (void**)&domInputElement))) {
                BOOL result;
                formDelegate->doPlatformCommand(domInputElement, BString(L"DeleteBackward"), kit(e->document()->frame()), &result);
                domInputElement->Release();
            }
            domElement->Release();
        }
        formDelegate->Release();
    }*/
}

void WebEditorClient::textDidChangeInTextArea(Element* e)
{
    /*IWebFormDelegate* formDelegate;
    if (SUCCEEDED(m_webView->formDelegate(&formDelegate)) && formDelegate) {
        IDOMElement* domElement = DOMElement::createInstance(e);
        if (domElement) {
            IDOMHTMLTextAreaElement* domTextAreaElement;
            if (SUCCEEDED(domElement->QueryInterface(IID_IDOMHTMLTextAreaElement, (void**)&domTextAreaElement))) {
                formDelegate->textDidChangeInTextArea(domTextAreaElement, kit(e->document()->frame()));
                domTextAreaElement->Release();
            }
            domElement->Release();
        }
        formDelegate->Release();
    }*/
}

class WebEditorUndoCommand : public WebUndoCommand
{
public:
    WebEditorUndoCommand(PassRefPtr<EditCommand> editCommand, bool isUndo);
    void execute();

private:
    RefPtr<EditCommand> m_editCommand;
    bool m_isUndo;
};

WebEditorUndoCommand::WebEditorUndoCommand(PassRefPtr<EditCommand> editCommand, bool isUndo)
    : m_editCommand(editCommand)
    , m_isUndo(isUndo) 
{ 
}

void WebEditorUndoCommand::execute()
{
    if (m_isUndo)
        m_editCommand->unapply();
    else
        m_editCommand->reapply();
}


/*static String undoNameForEditAction(EditAction editAction)
{
    switch (editAction) {
        case EditActionUnspecified: return "";
        case EditActionSetColor: return "Set Color";
        case EditActionSetBackgroundColor: return "Set Background Color";
        case EditActionTurnOffKerning: return "Turn Off Kerning";
        case EditActionTightenKerning: return "Tighten Kerning";
        case EditActionLoosenKerning: return "Loosen Kerning";
        case EditActionUseStandardKerning: return "Use Standard Kerning";
        case EditActionTurnOffLigatures: return "Turn Off Ligatures";
        case EditActionUseStandardLigatures: return "Use Standard Ligatures";
        case EditActionUseAllLigatures: return "Use All Ligatures";
        case EditActionRaiseBaseline: return "Raise Baseline";
        case EditActionLowerBaseline: return "Lower Baseline";
        case EditActionSetTraditionalCharacterShape: return "Set Traditional Character Shape";
        case EditActionSetFont: return "Set Font";
        case EditActionChangeAttributes: return "Change Attributes";
        case EditActionAlignLeft: return "Align Left";
        case EditActionAlignRight: return "Align Right";
        case EditActionCenter: return "Center";
        case EditActionJustify: return "Justify";
        case EditActionSetWritingDirection: return "Set Writing Direction";
        case EditActionSubscript: return "Subscript";
        case EditActionSuperscript: return "Superscript";
        case EditActionUnderline: return "Underline";
        case EditActionOutline: return "Outline";
        case EditActionUnscript: return "Unscript";
        case EditActionDrag: return "Drag";
        case EditActionCut: return "Cut";
        case EditActionPaste: return "Paste";
        case EditActionPasteFont: return "Paste Font";
        case EditActionPasteRuler: return "Paste Ruler";
        case EditActionTyping: return "Typing";
        case EditActionCreateLink: return "Create Link";
        case EditActionUnlink: return "Unlink";
        case EditActionInsertList: return "Insert List";
        case EditActionFormatBlock: return "Formatting";
        case EditActionIndent: return "Indent";
        case EditActionOutdent: return "Outdent";
    }
    return String();
}*/

void WebEditorClient::registerCommandForUndo(PassRefPtr<EditCommand> command)
{
    /*IWebUIDelegate* uiDelegate = 0;
    if (SUCCEEDED(m_webView->uiDelegate(&uiDelegate))) {
        LPCTSTR actionName = undoNameForEditAction(command->editingAction());
        WebEditorUndoCommand* undoCommand = new WebEditorUndoCommand(command, true);
        if (!undoCommand)
            return;
        uiDelegate->registerUndoWithTarget(m_undoTarget, 0, undoCommand);
        undoCommand->Release(); // the undo manager owns the reference
        BSTR actionNameBSTR = SysAllocString(actionName);
        if (actionNameBSTR) {
            uiDelegate->setActionTitle(actionNameBSTR);
            SysFreeString(actionNameBSTR);
        }
        uiDelegate->Release();
    }*/
}

void WebEditorClient::registerCommandForRedo(PassRefPtr<EditCommand> command)
{
    /*IWebUIDelegate* uiDelegate = 0;
    if (SUCCEEDED(m_webView->uiDelegate(&uiDelegate))) {
        WebEditorUndoCommand* undoCommand = new WebEditorUndoCommand(command, false);
        if (!undoCommand)
            return;
        uiDelegate->registerUndoWithTarget(m_undoTarget, 0, undoCommand);
        undoCommand->Release(); // the undo manager owns the reference
        uiDelegate->Release();
    }*/
}

void WebEditorClient::clearUndoRedoOperations()
{
    /*IWebUIDelegate* uiDelegate = 0;
    if (SUCCEEDED(m_webView->uiDelegate(&uiDelegate))) {
        uiDelegate->removeAllActionsWithTarget(m_undoTarget);
        uiDelegate->Release();
    }*/
}

bool WebEditorClient::canUndo() const
{
    /*BOOL result = FALSE;
    IWebUIDelegate* uiDelegate = 0;
    if (SUCCEEDED(m_webView->uiDelegate(&uiDelegate))) {
        uiDelegate->canUndo(&result);
        uiDelegate->Release();
    }
    return !!result;*/
    return false;
}

bool WebEditorClient::canRedo() const
{
    /*BOOL result = FALSE;
    IWebUIDelegate* uiDelegate = 0;
    if (SUCCEEDED(m_webView->uiDelegate(&uiDelegate))) {
        uiDelegate->canRedo(&result);
        uiDelegate->Release();
    }
    return !!result;*/
    return false;
}

void WebEditorClient::undo()
{
    /*IWebUIDelegate* uiDelegate = 0;
    if (SUCCEEDED(m_webView->uiDelegate(&uiDelegate))) {
        uiDelegate->undo();
        uiDelegate->Release();
    }*/
}

void WebEditorClient::redo()
{
    /*IWebUIDelegate* uiDelegate = 0;
    if (SUCCEEDED(m_webView->uiDelegate(&uiDelegate))) {
        uiDelegate->redo();
        uiDelegate->Release();
    }*/
}

void WebEditorClient::handleKeyboardEvent(KeyboardEvent* evt)
{
    if (m_webView->handleEditingKeyboardEvent(evt))
        evt->setDefaultHandled();
}

void WebEditorClient::handleInputMethodKeydown(KeyboardEvent* )
{
}

bool WebEditorClient::isEditable()
{
    return false;
}

void WebEditorClient::ignoreWordInSpellDocument(const String& word)
{
    /*COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return;

    initViewSpecificSpelling(m_webView);
    ed->ignoreWordInSpellDocument(m_webView, BString(word));*/
}

void WebEditorClient::learnWord(const String& word)
{
    /*COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return;

    ed->learnWord(BString(word));*/
}

void WebEditorClient::checkSpellingOfString(const UChar* text, int length, int* misspellingLocation, int* misspellingLength)
{
/*    *misspellingLocation = -1;
    *misspellingLength = 0;

    COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return;

    initViewSpecificSpelling(m_webView);
    ed->checkSpellingOfString(m_webView, text, length, misspellingLocation, misspellingLength);*/
}

void WebEditorClient::checkGrammarOfString(const UChar* text, int length, Vector<GrammarDetail>& details, int* badGrammarLocation, int* badGrammarLength)
{
    /*details.clear();
    *badGrammarLocation = -1;
    *badGrammarLength = 0;

    COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return;

    initViewSpecificSpelling(m_webView);
    COMPtr<IEnumWebGrammarDetails> enumDetailsObj;
    if (FAILED(ed->checkGrammarOfString(m_webView, text, length, &enumDetailsObj, badGrammarLocation, badGrammarLength)))
        return;

    while (true) {
        ULONG fetched;
        COMPtr<IWebGrammarDetail> detailObj;
        if (enumDetailsObj->Next(1, &detailObj, &fetched) != S_OK)
            break;

        GrammarDetail detail;
        if (FAILED(detailObj->length(&detail.length)))
            continue;
        if (FAILED(detailObj->location(&detail.location)))
            continue;
        BSTR userDesc;
        if (FAILED(detailObj->userDescription(&userDesc)))
            continue;
        detail.userDescription = String(userDesc, SysStringLen(userDesc));
        SysFreeString(userDesc);

        COMPtr<IEnumSpellingGuesses> enumGuessesObj;
        if (FAILED(detailObj->guesses(&enumGuessesObj)))
            continue;
        while (true) {
            BSTR guess;
            if (enumGuessesObj->Next(1, &guess, &fetched) != S_OK)
                break;
            detail.guesses.append(String(guess, SysStringLen(guess)));
            SysFreeString(guess);
        }

        details.append(detail);
    }*/
}

void WebEditorClient::updateSpellingUIWithGrammarString(const String& string, const WebCore::GrammarDetail& detail)
{
    /*COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return;

    Vector<BSTR> guessesBSTRs;
    for (unsigned i = 0; i < detail.guesses.size(); i++) {
        BString guess(detail.guesses[i]);
        guessesBSTRs.append(guess.release());
    }
    BString userDescriptionBSTR(detail.userDescription);
    ed->updateSpellingUIWithGrammarString(BString(string), detail.location, detail.length, userDescriptionBSTR, guessesBSTRs.data(), (int)guessesBSTRs.size());
    for (unsigned i = 0; i < guessesBSTRs.size(); i++)
        SysFreeString(guessesBSTRs[i]);*/
}

void WebEditorClient::updateSpellingUIWithMisspelledWord(const String& word)
{
    /*COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return;

    ed->updateSpellingUIWithMisspelledWord(BString(word));*/
}

void WebEditorClient::showSpellingUI(bool show)
{
    /*COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return;
    
    ed->showSpellingUI(show);*/
}

bool WebEditorClient::spellingUIIsShowing()
{
    /*COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return false;

    BOOL showing;
    if (FAILED(ed->spellingUIIsShowing(&showing)))
        return false;

    return !!showing;*/
    return false;
}

void WebEditorClient::getGuessesForWord(const String& word, Vector<String>& guesses)
{
    /*guesses.clear();

    COMPtr<IWebEditingDelegate> ed;
    if (FAILED(m_webView->editingDelegate(&ed)) || !ed.get())
        return;

    COMPtr<IEnumSpellingGuesses> enumGuessesObj;
    if (FAILED(ed->guessesForWord(BString(word), &enumGuessesObj)))
        return;

    while (true) {
        ULONG fetched;
        BSTR guess;
        if (enumGuessesObj->Next(1, &guess, &fetched) != S_OK)
            break;
        guesses.append(String(guess, SysStringLen(guess)));
        SysFreeString(guess);
    }*/
}

void WebEditorClient::setInputMethodState(bool enabled)
{
    // (old code that was commented out...)
    //m_webView->setInputMethodState(enabled);

    // CSidhall 1/22/09 Addded notify user app of text input state for possible virtual keyboard...
    // We can't fully trust the enabled flag because the input field might be a password in which case we still
    // want to activate the keyboard input. So we do our own checking and also get extra info...   
    
	//Note by Arpit Baldeva: We are interested in the <input> and <textarea> elements. The problem is that we can't rely on the shouldUseInputMethod() of node to reliably detect an
	//editable node since it does not include password(as noted above). Webkit trunk has some cryptic comment explaining why that is the right thing to do. So we do as follows. 
	// We could add a new method to the class hierarchy to achieve following but want to minimize the changes inside core layer.

	bool inputActive	= enabled;
    bool searchActive	= false;
    bool passwordActive = false;
	EA::WebKit::KeyboardType kbType = EA::WebKit::kDefaultKeyBoard;

    WebFrame* pFocusedFrame = m_webView->focusedFrame();
    if(pFocusedFrame) 
	{    
        WebCore::Document* pDOM = pFocusedFrame->DOMDocument();
        if(pDOM)
		{
            WebCore::Node* pNode = pDOM->focusedNode();
            if( pNode && pNode->isHTMLElement())
			{
				if(pNode->hasTagName(WebCore::HTMLNames::inputTag) ) 
				{	
					HTMLInputElement* pInputElement = static_cast<HTMLInputElement*> (pNode);
                	// Get the flags
					inputActive		= pInputElement->isTextField();
					searchActive	= pInputElement->isSearchField();
					passwordActive	= pInputElement->isPasswordField();
				}
/*				//Note by Arpit Baldeva: This will always come back as true but provided the commented out code here for the sack of clarity
				else if(pNode->hasTagName(WebCore::HTMLNames::textareaTag))
				{
					inputActive = enabled;
				}
*/

				HTMLElement* pElement = static_cast<HTMLElement*> (pNode);
				if(pElement->hasClass())
				{
					const WebCore::ClassNames& classNames = pElement->classNames();
					for(int i = 0; i < EA::WebKit::kCountKeyBoardTypes; ++i)
					{
						if(classNames.contains(sKeyBoardClassMap[i].mKeyboardClass))
						{
							kbType = sKeyBoardClassMap[i].mKeyboardType;
							break;
						}
					}
				}
				// Update - 12/20/2010. Since we have the password information available from the HTML, we use it
				// if the keyboard is not overridden.
				if(passwordActive && (kbType == EA::WebKit::kDefaultKeyBoard))
				{
					kbType = EA::WebKit::kPasswordKeyBoard;
				}
            }        
        }
    }

    EA::WebKit::View* pView= EA::WebKit::GetView(m_webView);
    ASSERT(pView);
    if(pView) 
	{
        // Store the current settings
        EA::WebKit::TextInputStateInfo& textInfo = pView->GetTextInputStateInfo();
        textInfo.mpView				= pView;
        textInfo.mIsActivated		= inputActive;
        textInfo.mIsPasswordField	= passwordActive;
        textInfo.mIsSearchField		= searchActive;
		textInfo.mKeyboardType		= kbType;

        // Notify the local app
        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
 
        if(pViewNotification)
            pViewNotification->TextInputState( textInfo );
    }
}
