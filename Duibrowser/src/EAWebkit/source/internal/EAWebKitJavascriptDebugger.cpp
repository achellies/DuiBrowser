/*
Copyright (C) 2010 Electronic Arts, Inc.  All rights reserved.

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
// EAWebKitJavascriptDebugListener.cpp
// By David Siems
///////////////////////////////////////////////////////////////////////////////
#include <EAWebKit/EAWebKitView.h>

#include <EAWebKit/internal/EAWebKitJavascriptDebugger.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <EAWebKit/internal/EAWebKitJavascriptBinding.h>

namespace EA { namespace WebKit {

EAWebKitJavascriptDebugger::EAWebKitJavascriptDebugger(void)
: mListener(NULL) {
    // Do nothing.
}

void EAWebKitJavascriptDebugger::SetListener(EAWebKitJavascriptDebugListener *listener) {
    mListener = listener;
}

void EAWebKitJavascriptDebugger::sourceParsed(KJS::ExecState* state, int sourceId, const KJS::UString& sourceURL, const KJS::SourceProvider& source, int startingLineNumber, int errorLine, const KJS::UString& errorMsg) {
    EAW_ASSERT(mListener);

    KJS::CStringBuffer cString;
    sourceURL.getCString(cString);

    if (errorLine == -1) {
        // Successful parse.
        SourceEntry newEntry = {sourceId, startingLineNumber};
        mFileToId[cString.data()].push_back(newEntry);
        mIdToFile[sourceId] = cString.data();
    }
    else {
        // Error.
        KJS::CStringBuffer message;
        errorMsg.getCString(message);
        mListener->OnParseFailure(cString.data(), errorLine, message.data());
    }
}

void EAWebKitJavascriptDebugger::exception(const KJS::DebuggerCallFrame &debuggerCallFrame, int sourceId, int line) {
    if (!UpdateCallFrame(debuggerCallFrame, sourceId, line)) {
        return;
    }

    EAW_ASSERT(mListener);
    mListener->OnException(sourceId, line);
}

void EAWebKitJavascriptDebugger::atStatement(const KJS::DebuggerCallFrame &debuggerCallFrame, int sourceId, int line) {    
    if (!UpdateCallFrame(debuggerCallFrame, sourceId, line)) {
        return;
    }

    EAW_ASSERT(mListener);
    mListener->OnStatement(sourceId, line);
}

void EAWebKitJavascriptDebugger::callEvent(const KJS::DebuggerCallFrame &debuggerCallFrame, int sourceId, int line) {
    mCurrentCallFrame = WebCore::JavaScriptCallFrame::create(debuggerCallFrame, mCurrentCallFrame, sourceId, line);

    EAW_ASSERT(mListener);
    mListener->OnFunctionCall(sourceId, line);
}

void EAWebKitJavascriptDebugger::returnEvent(const KJS::DebuggerCallFrame &debuggerCallFrame, int sourceId, int line) {
    if (!UpdateCallFrame(debuggerCallFrame, sourceId, line)) {
        return;
    }

    mCurrentCallFrame = mCurrentCallFrame->caller();

    EAW_ASSERT(mListener);
    mListener->OnFunctionReturn(sourceId, line);
}

void EAWebKitJavascriptDebugger::willExecuteProgram(const KJS::DebuggerCallFrame &debuggerCallFrame, int sourceId, int line) {
    mCurrentCallFrame = WebCore::JavaScriptCallFrame::create(debuggerCallFrame, mCurrentCallFrame, sourceId, line);
}

void EAWebKitJavascriptDebugger::didExecuteProgram(const KJS::DebuggerCallFrame &debuggerCallFrame, int sourceId, int line) {
    if (!UpdateCallFrame(debuggerCallFrame, sourceId, line)) {
        return;
    }

    mCurrentCallFrame = mCurrentCallFrame->caller();
}

void EAWebKitJavascriptDebugger::didReachBreakpoint(const KJS::DebuggerCallFrame &debuggerCallFrame, int sourceId, int line) {
    if (!UpdateCallFrame(debuggerCallFrame, sourceId, line)) {
        return;
    }

    EAW_ASSERT(mListener);
    mListener->OnJavascriptBreakpoint(sourceId, line);
}

// Translates a sourceId to a filename.
const char *EAWebKitJavascriptDebugger::IdToFile(int sourceId) {
    SourceIdMap::iterator iId = mIdToFile.find(sourceId);
    if (iId == mIdToFile.end()) {
        return NULL;
    }

    return iId->second.c_str();
}

// Translates a filename and line to a sourceId.
int EAWebKitJavascriptDebugger::FileToId(const char *filename, unsigned line) {
    SourceFileMap::iterator iFile = mFileToId.find(filename);
    if (iFile == mFileToId.end()) {
        return -1;
    }

    int sourceId = -1;
    int bestDistance = INT_MAX;
    for (unsigned i = 0; i < iFile->second.size(); ++i) {
        const SourceEntry &entry = iFile->second[i];
        int distance = static_cast<int>(line) - entry.mStartingLine;
        if (distance >= 0 && distance < bestDistance) {
            sourceId = entry.mSourceId;
            bestDistance = distance;
        }
    }

    return sourceId;
}

namespace {
    // Translates from an internal javascript value to one clients can use.
    void TranslateJSValue(KJS::JSValue *value, JavascriptValue *resultOut, KJS::ExecState *exec) {   
        switch (value->type()) {
        case KJS::ObjectType:
            {
                KJS::JSObject* pJSObject = value->getObject();
                if (pJSObject->isObject(&KJS::JSArray::info)) {
                    resultOut->SetArrayType();

                    // Special treatment for arrays. We return an actual array instead of hash map
                    KJS::JSArray* pJSArray = static_cast<KJS::JSArray*>(pJSObject);

                    unsigned arrayLength = pJSArray->getLength();
                    VectorJavaScriptValue* pJavaScriptValues = GetVectorJavaScriptValue(resultOut->GetArrayValue());
                    pJavaScriptValues->resize(arrayLength);

                    for (unsigned i = 0; i < arrayLength; ++i) {
                        KJS::JSValue* subValue = pJSArray->get(exec, i);
                        TranslateJSValue(subValue, &((*pJavaScriptValues)[i]), exec);
                    }
                }
                else {
                    resultOut->SetObjectType();

                    KJS::PropertyNameArray propertyNames(exec);
                    pJSObject->getPropertyNames(exec, propertyNames);

                    HashMapJavaScriptValue* pJavascriptValues = GetHashMap(resultOut->GetHashMapValue());

                    KJS::PropertyNameArray::const_iterator start = propertyNames.begin();
                    KJS::PropertyNameArray::const_iterator end = propertyNames.end();
                    for (KJS::PropertyNameArray::const_iterator iProp = start; iProp != end; ++iProp) {
                        KJS::JSValue *subValue = pJSObject->get(exec, *iProp);
                        const char16_t *name = iProp->ustring().data();

                        TranslateJSValue(subValue, &((*pJavascriptValues)[name]), exec);
                    }
                }
            }
            break;
        case KJS::NumberType:
            resultOut->SetNumberValue(value->getNumber());
            break;
        case KJS::BooleanType:
            resultOut->SetBooleanValue(value->getBoolean());
            break;
        case KJS::StringType:
            {
                GetFixedString(resultOut->GetStringValue())->assign(value->getString().data());
                resultOut->SetStringType();
            }
            break;
        case KJS::UndefinedType:
            resultOut->SetUndefined();
            break;
        default:
            resultOut->SetUndefined();
            break;
        }
    }
}

bool EAWebKitJavascriptDebugger::EvaluateExpression(const char *expression, JavascriptValue *resultOut) {
    if (!mCurrentCallFrame || !resultOut) {
        return false;
    }

    KJS::JSValue *exception = NULL;
    KJS::JSValue *result = mCurrentCallFrame->evaluate(expression, exception);
    TranslateJSValue(result, resultOut, mCurrentCallFrame->scopeChain()->globalObject()->globalExec());
    
    return true;
}

int EAWebKitJavascriptDebugger::GetCurrentLine(void) {
    if (!mCurrentCallFrame) {
        return -1;
    }

    return mCurrentCallFrame->line();
}

const char *EAWebKitJavascriptDebugger::GetCurrentFile(void) {
    if (!mCurrentCallFrame) {
        return NULL;
    }

    return IdToFile(mCurrentCallFrame->sourceIdentifier());
}

bool EAWebKitJavascriptDebugger::UpdateCallFrame(const KJS::DebuggerCallFrame &debugFrame, int sourceId, int line) {
    ASSERT(mCurrentCallFrame);
    if (!mCurrentCallFrame) {
        return false;
    }

    mCurrentCallFrame->update(debugFrame, sourceId, line);
    return true;
}

}}
