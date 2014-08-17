LIST (APPEND BINDINGS_JS_SRC
    bindings/js/GCController.cpp
    bindings/js/JSAttrCustom.cpp
    bindings/js/JSAudioConstructor.cpp
    bindings/js/JSCSSRuleCustom.cpp
    bindings/js/JSCSSStyleDeclarationCustom.cpp
    bindings/js/JSCSSValueCustom.cpp
    bindings/js/JSCanvasPixelArrayCustom.cpp
    bindings/js/JSCanvasRenderingContext2DCustom.cpp
    bindings/js/JSClipboardCustom.cpp
    bindings/js/JSConsoleCustom.cpp
    bindings/js/JSCustomVoidCallback.cpp
    bindings/js/JSCustomXPathNSResolver.cpp
    bindings/js/JSDocumentCustom.cpp
#    bindings/js/JSDOMApplicationCacheCustom.cpp
    bindings/js/JSDOMBinding.cpp
    bindings/js/JSDOMWindowBase.cpp
    bindings/js/JSDOMWindowCustom.cpp
    bindings/js/JSDOMWindowShell.cpp
    bindings/js/JSElementCustom.cpp
    bindings/js/JSEventCustom.cpp
    bindings/js/JSEventListener.cpp
    bindings/js/JSEventTargetBase.cpp
    bindings/js/JSEventTargetNode.cpp
    bindings/js/JSHTMLAppletElementCustom.cpp
    bindings/js/JSHTMLCollectionCustom.cpp
    bindings/js/JSHTMLDocumentCustom.cpp
    bindings/js/JSHTMLElementCustom.cpp
    bindings/js/JSHTMLEmbedElementCustom.cpp
    bindings/js/JSHTMLFormElementCustom.cpp
    bindings/js/JSHTMLFrameElementCustom.cpp
    bindings/js/JSHTMLFrameSetElementCustom.cpp
    bindings/js/JSHTMLIFrameElementCustom.cpp
    bindings/js/JSHTMLInputElementBase.cpp
    bindings/js/JSHTMLObjectElementCustom.cpp
    bindings/js/JSHTMLOptionElementConstructor.cpp
    bindings/js/JSHTMLOptionsCollectionCustom.cpp
    bindings/js/JSHTMLSelectElementCustom.cpp
    bindings/js/JSHistoryCustom.cpp
    bindings/js/JSImageConstructor.cpp
    bindings/js/JSInspectorCallbackWrapper.cpp
    bindings/js/JSInspectedObjectWrapper.cpp
    bindings/js/JSJavaScriptCallFrameCustom.cpp
    bindings/js/JSLocationCustom.cpp
    bindings/js/JSMimeTypeArrayCustom.cpp
    bindings/js/JSNamedNodeMapCustom.cpp
    bindings/js/JSNamedNodesCollection.cpp
    bindings/js/JSNavigatorCustom.cpp
    bindings/js/JSNodeCustom.cpp
    bindings/js/JSNodeFilterCondition.cpp
    bindings/js/JSNodeFilterCustom.cpp
    bindings/js/JSNodeIteratorCustom.cpp
    bindings/js/JSNodeListCustom.cpp
    bindings/js/JSQuarantinedObjectWrapper.cpp
    bindings/js/JSPluginCustom.cpp
    bindings/js/JSPluginArrayCustom.cpp
    bindings/js/JSPluginElementFunctions.cpp
    bindings/js/JSStyleSheetCustom.cpp
    bindings/js/JSStyleSheetListCustom.cpp
    bindings/js/JSRGBColor.cpp
    bindings/js/JSTextCustom.cpp
    bindings/js/JSTreeWalkerCustom.cpp
    bindings/js/JSXMLHttpRequestConstructor.cpp
    bindings/js/JSXMLHttpRequestCustom.cpp
    bindings/js/JSXSLTProcessorConstructor.cpp
    bindings/js/JSXSLTProcessorCustom.cpp
    bindings/js/PausedTimeouts.cpp
    bindings/js/ScheduledAction.cpp
    bindings/js/ScriptController.cpp
)
IF (WEBKIT_USE_DATABASE)
    LIST (APPEND BINDINGS_JS_SRC
        bindings/js/JSCustomSQLStatementCallback.cpp
        bindings/js/JSCustomSQLStatementErrorCallback.cpp
        bindings/js/JSCustomSQLTransactionCallback.cpp
        bindings/js/JSCustomSQLTransactionErrorCallback.cpp
        bindings/js/JSDatabaseCustom.cpp
        bindings/js/JSSQLResultSetRowListCustom.cpp
        bindings/js/JSSQLTransactionCustom.cpp
    )
ENDIF (WEBKIT_USE_DATABASE)

IF (WEBKIT_USE_DOM_STORAGE)
    LIST (APPEND BINDINGS_JS_SRC
        bindings/js/JSStorageCustom.cpp
    )
ENDIF (WEBKIT_USE_DOM_STORAGE)

IF (WEBKIT_USE_SVG)
    LIST (APPEND BINDINGS_JS_SRC
        bindings/js/JSSVGLazyEventListener.cpp
        bindings/js/JSSVGMatrixCustom.cpp
        bindings/js/JSSVGPathSegCustom.cpp
        bindings/js/JSSVGPathSegListCustom.cpp
        bindings/js/JSSVGPointListCustom.cpp
        bindings/js/JSSVGTransformListCustom.cpp
    )
ENDIF (WEBKIT_USE_SVG)

IF (WEBKIT_OFFLINE_WEB_APPLICATIONS)
    LIST (APPEND BINDINGS_JS_SRC
        bindings/js/JSDOMApplicationCacheCustom.cpp
    )
ENDIF (WEBKIT_OFFLINE_WEB_APPLICATIONS)
