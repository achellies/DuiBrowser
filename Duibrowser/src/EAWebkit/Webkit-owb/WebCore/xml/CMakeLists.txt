LIST (APPEND XML_SRC
    xml/AccessControlList.cpp
    xml/AccessItem.cpp
    xml/AccessItemRule.cpp
    xml/DOMParser.cpp
    xml/XMLSerializer.cpp
    xml/NativeXPathNSResolver.cpp
    xml/XPathEvaluator.cpp
    xml/XPathExpression.cpp
    xml/XPathExpressionNode.cpp
    xml/XPathFunctions.cpp
    xml/XPathNamespace.cpp
    xml/XPathNodeSet.cpp
    xml/XPathNSResolver.cpp
    xml/XPathParser.cpp
    xml/XPathPath.cpp
    xml/XPathPredicate.cpp
    xml/XPathResult.cpp
    xml/XPathStep.cpp
    xml/XPathUtil.cpp
    xml/XPathValue.cpp
    xml/XPathVariableReference.cpp
)

# IF (WEBKIT_USE_XSLT_SUPPORT)
    LIST (APPEND XML_SRC
        xml/XSLImportRule.cpp
        xml/XSLStyleSheet.cpp
        xml/XSLTExtensions.cpp
        xml/XSLTProcessor.cpp
        xml/XSLTUnicodeSort.cpp
    )
# ENDIF (WEBKIT_USE_XSLT_SUPPORT)

LIST (APPEND XML_SRC
    xml/XMLHttpRequest.cpp
)
