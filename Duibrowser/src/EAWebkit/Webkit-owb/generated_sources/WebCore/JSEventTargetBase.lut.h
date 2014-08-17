// Automatically generated from D:/Development/Internet/OWB/doduo/WebCore/bindings/js/JSEventTargetBase.cpp using D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/create_hash_table. DO NOT EDIT!

#include "lookup.h"

namespace KJS {

static const struct HashTableValue JSEventTargetPropertiesTableValues[41] = {
   { "onabort", (intptr_t)WebCore::JSEventTargetProperties::OnAbort, DontDelete, 0 },
   { "onblur", (intptr_t)WebCore::JSEventTargetProperties::OnBlur, DontDelete, 0 },
   { "onchange", (intptr_t)WebCore::JSEventTargetProperties::OnChange, DontDelete, 0 },
   { "onclick", (intptr_t)WebCore::JSEventTargetProperties::OnClick, DontDelete, 0 },
   { "oncontextmenu", (intptr_t)WebCore::JSEventTargetProperties::OnContextMenu, DontDelete, 0 },
   { "ondblclick", (intptr_t)WebCore::JSEventTargetProperties::OnDblClick, DontDelete, 0 },
   { "onbeforecut", (intptr_t)WebCore::JSEventTargetProperties::OnBeforeCut, DontDelete, 0 },
   { "oncut", (intptr_t)WebCore::JSEventTargetProperties::OnCut, DontDelete, 0 },
   { "onbeforecopy", (intptr_t)WebCore::JSEventTargetProperties::OnBeforeCopy, DontDelete, 0 },
   { "oncopy", (intptr_t)WebCore::JSEventTargetProperties::OnCopy, DontDelete, 0 },
   { "onbeforepaste", (intptr_t)WebCore::JSEventTargetProperties::OnBeforePaste, DontDelete, 0 },
   { "onpaste", (intptr_t)WebCore::JSEventTargetProperties::OnPaste, DontDelete, 0 },
   { "ondrag", (intptr_t)WebCore::JSEventTargetProperties::OnDrag, DontDelete, 0 },
   { "ondragend", (intptr_t)WebCore::JSEventTargetProperties::OnDragEnd, DontDelete, 0 },
   { "ondragenter", (intptr_t)WebCore::JSEventTargetProperties::OnDragEnter, DontDelete, 0 },
   { "ondragleave", (intptr_t)WebCore::JSEventTargetProperties::OnDragLeave, DontDelete, 0 },
   { "ondragover", (intptr_t)WebCore::JSEventTargetProperties::OnDragOver, DontDelete, 0 },
   { "ondragstart", (intptr_t)WebCore::JSEventTargetProperties::OnDragStart, DontDelete, 0 },
   { "ondrop", (intptr_t)WebCore::JSEventTargetProperties::OnDrop, DontDelete, 0 },
   { "onerror", (intptr_t)WebCore::JSEventTargetProperties::OnError, DontDelete, 0 },
   { "onfocus", (intptr_t)WebCore::JSEventTargetProperties::OnFocus, DontDelete, 0 },
   { "oninput", (intptr_t)WebCore::JSEventTargetProperties::OnInput, DontDelete, 0 },
   { "onkeydown", (intptr_t)WebCore::JSEventTargetProperties::OnKeyDown, DontDelete, 0 },
   { "onkeypress", (intptr_t)WebCore::JSEventTargetProperties::OnKeyPress, DontDelete, 0 },
   { "onkeyup", (intptr_t)WebCore::JSEventTargetProperties::OnKeyUp, DontDelete, 0 },
   { "onload", (intptr_t)WebCore::JSEventTargetProperties::OnLoad, DontDelete, 0 },
   { "onmousedown", (intptr_t)WebCore::JSEventTargetProperties::OnMouseDown, DontDelete, 0 },
   { "onmousemove", (intptr_t)WebCore::JSEventTargetProperties::OnMouseMove, DontDelete, 0 },
   { "onmouseout", (intptr_t)WebCore::JSEventTargetProperties::OnMouseOut, DontDelete, 0 },
   { "onmouseover", (intptr_t)WebCore::JSEventTargetProperties::OnMouseOver, DontDelete, 0 },
   { "onmouseup", (intptr_t)WebCore::JSEventTargetProperties::OnMouseUp, DontDelete, 0 },
   { "onmousewheel", (intptr_t)WebCore::JSEventTargetProperties::OnMouseWheel, DontDelete, 0 },
   { "onreset", (intptr_t)WebCore::JSEventTargetProperties::OnReset, DontDelete, 0 },
   { "onresize", (intptr_t)WebCore::JSEventTargetProperties::OnResize, DontDelete, 0 },
   { "onscroll", (intptr_t)WebCore::JSEventTargetProperties::OnScroll, DontDelete, 0 },
   { "onsearch", (intptr_t)WebCore::JSEventTargetProperties::OnSearch, DontDelete, 0 },
   { "onselect", (intptr_t)WebCore::JSEventTargetProperties::OnSelect, DontDelete, 0 },
   { "onselectstart", (intptr_t)WebCore::JSEventTargetProperties::OnSelectStart, DontDelete, 0 },
   { "onsubmit", (intptr_t)WebCore::JSEventTargetProperties::OnSubmit, DontDelete, 0 },
   { "onunload", (intptr_t)WebCore::JSEventTargetProperties::OnUnload, DontDelete, 0 },
   { 0, 0, 0, 0 }
};

extern const struct HashTable JSEventTargetPropertiesTable = { 2047, JSEventTargetPropertiesTableValues, 0 };

} // namespace

#include "lookup.h"

namespace KJS {

static const struct HashTableValue JSEventTargetPrototypeTableValues[4] = {
   { "addEventListener", (intptr_t)WebCore::jsEventTargetAddEventListener, DontDelete|Function, 3 },
   { "removeEventListener", (intptr_t)WebCore::jsEventTargetRemoveEventListener, DontDelete|Function, 3 },
   { "dispatchEvent", (intptr_t)WebCore::jsEventTargetDispatchEvent, DontDelete|Function, 1 },
   { 0, 0, 0, 0 }
};

extern const struct HashTable JSEventTargetPrototypeTable = { 3, JSEventTargetPrototypeTableValues, 0 };

} // namespace
