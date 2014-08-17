/*
 *  Copyright (C) 2007 Alp Toker <alp@atoker.com>
 *  Copyright (C) 2008 Nuanti Ltd.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "EditorClientISee.h"

#include "EditCommand.h"
#include "Editor.h"
#include "FocusController.h"
#include "Frame.h"
#include "Document.h"
#include "KeyboardEvent.h"
#include "NotImplemented.h"
#include "Page.h"
#include "PlatformKeyboardEvent.h"

#include "WebView.h"

#include <Windows.h>
#include <stdio.h>

using namespace WebCore;

namespace WebKit {

EditorClient::EditorClient(WebView *webView)
: m_webView(webView)
{
}

void EditorClient::setInputMethodState(bool enabled)
{
	m_webView->setInputMethodState(enabled);
}


bool EditorClient::shouldDeleteRange(Range*)
{
    notImplemented();
    return true;
}

bool EditorClient::shouldShowDeleteInterface(HTMLElement*)
{
    return false;
}

bool EditorClient::isContinuousSpellCheckingEnabled()
{
    notImplemented();
    return false;
}

bool EditorClient::isGrammarCheckingEnabled()
{
    notImplemented();
    return false;
}

int EditorClient::spellCheckerDocumentTag()
{
    notImplemented();
    return 0;
}

bool EditorClient::shouldBeginEditing(WebCore::Range*)
{
	notImplemented();
	return true;
}

bool EditorClient::shouldEndEditing(WebCore::Range*)
{
	notImplemented();
	return true;
}

bool EditorClient::shouldInsertText(const String&, Range*, EditorInsertAction)
{
    notImplemented();
    return true;
}

bool EditorClient::shouldChangeSelectedRange(Range*, Range*, EAffinity, bool)
{
    notImplemented();
    return true;
}

bool EditorClient::shouldApplyStyle(WebCore::CSSStyleDeclaration*, WebCore::Range*)
{
	notImplemented();
	return true;
}

bool EditorClient::shouldMoveRangeAfterDelete(Range*, Range*)
{
	notImplemented();
	return true;
}

void EditorClient::didBeginEditing()
{
    notImplemented();
}

void EditorClient::respondToChangedContents()
{
    notImplemented();
}

void EditorClient::respondToChangedSelection()
{
    notImplemented();
}

void EditorClient::didEndEditing()
{
    notImplemented();
}

void EditorClient::didWriteSelectionToPasteboard()
{
    notImplemented();
}

void EditorClient::didSetSelectionTypesForPasteboard()
{
    notImplemented();
}

bool EditorClient::isEditable()
{
    notImplemented();
    return false;
}

void EditorClient::registerCommandForUndo(WTF::PassRefPtr<WebCore::EditCommand>)
{
    notImplemented();
}

void EditorClient::registerCommandForRedo(WTF::PassRefPtr<WebCore::EditCommand>)
{
    notImplemented();
}

void EditorClient::clearUndoRedoOperations()
{
    notImplemented();
}

bool EditorClient::canUndo() const
{
    notImplemented();
    return false;
}

bool EditorClient::canRedo() const
{
    notImplemented();
    return false;
}

void EditorClient::undo()
{
    notImplemented();
}

void EditorClient::redo()
{
    notImplemented();
}

bool EditorClient::shouldInsertNode(Node*, Range*, EditorInsertAction)
{
	notImplemented(); 
	return true; 
}

void EditorClient::pageDestroyed()
{
    delete this;
}

bool EditorClient::smartInsertDeleteEnabled()
{
    notImplemented();
    return false;
}

void EditorClient::toggleContinuousSpellChecking()
{
    notImplemented();
}

void EditorClient::toggleGrammarChecking()
{
	notImplemented();
}

void EditorClient::handleKeyboardEvent(KeyboardEvent* evt)
{
	if (m_webView->handleEditingKeyboardEvent(evt))
		evt->setDefaultHandled();
}

void EditorClient::handleInputMethodKeydown(KeyboardEvent*)
{
    notImplemented();
}

void EditorClient::textFieldDidBeginEditing(Element*)
{
    notImplemented();
}

void EditorClient::textFieldDidEndEditing(Element*)
{
    notImplemented();
}

void EditorClient::textDidChangeInTextField(Element*)
{
    notImplemented();
}

bool EditorClient::doTextFieldCommandFromEvent(Element*, KeyboardEvent*)
{
    notImplemented();
    return false;
}

void EditorClient::textWillBeDeletedInTextField(Element*)
{
    notImplemented();
}

void EditorClient::textDidChangeInTextArea(Element*)
{
    notImplemented();
}

void EditorClient::ignoreWordInSpellDocument(const String&)
{
    notImplemented();
}

void EditorClient::learnWord(const String&)
{
    notImplemented();
}

void EditorClient::checkSpellingOfString(const UChar*, int, int*, int*)
{
    notImplemented();
}

void EditorClient::checkGrammarOfString(const UChar*, int, Vector<GrammarDetail>&, int*, int*)
{
    notImplemented();
}

void EditorClient::updateSpellingUIWithGrammarString(const String&, const GrammarDetail&)
{
    notImplemented();
}

void EditorClient::updateSpellingUIWithMisspelledWord(const String&)
{
    notImplemented();
}

void EditorClient::showSpellingUI(bool)
{
    notImplemented();
}

bool EditorClient::spellingUIIsShowing()
{
    notImplemented();
    return false;
}

void EditorClient::getGuessesForWord(const String&, Vector<String>&)
{
    notImplemented();
}

} // namespace WebKit
