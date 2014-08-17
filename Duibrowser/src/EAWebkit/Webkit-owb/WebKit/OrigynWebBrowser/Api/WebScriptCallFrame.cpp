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
#include "WebScriptCallFrame.h"

#include <interpreter.h>
#include <JSGlobalObject.h>
#include <JSValueRef.h>
#include <PropertyNameArray.h>

#include <PlatformString.h>
#include "CString.h"
#include <wtf/Assertions.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace KJS;
using namespace WebCore;

UString WebScriptCallFrame::jsValueToString(KJS::ExecState* state, JSValue* jsvalue)
{
    if (!jsvalue)
        return "undefined";

    switch (jsvalue->type()) {
        case NullType:
        case UndefinedType:
        case UnspecifiedType:
        case GetterSetterType:
            break;
        case StringType:
            return jsvalue->getString();
            break;
        case NumberType:
            return UString::from(jsvalue->getNumber());
            break;
        case BooleanType:
            return jsvalue->getBoolean() ? "True" : "False";
            break;
        case ObjectType:
            jsvalue = jsvalue->getObject()->defaultValue(state, StringType);
            return jsvalue->getString();
            break;
    }

    return "undefined";
}

static ExecState* callingFunctionOrGlobalExecState(ExecState* exec)
{
    /*for (ExecState* current = exec; current; current = current->callingExecState())
        if (current->codeType() == FunctionCode || current->codeType() == GlobalCode)
            return current;*/
    return 0;
}

WebScriptCallFrame::WebScriptCallFrame(ExecState* state)
    : m_state(callingFunctionOrGlobalExecState(state))
{
    ASSERT_ARG(state, state);
    ASSERT(m_state);
    DS_CONSTRUCT();
}

WebScriptCallFrame::~WebScriptCallFrame()
{
    m_state = 0;
    DS_DESTRUCT();
}

WebScriptCallFrame* WebScriptCallFrame::createInstance(ExecState* state)
{
    WebScriptCallFrame* instance = new WebScriptCallFrame(state);
    return instance;
}


WebScriptCallFrame* WebScriptCallFrame::caller()
{
    //return m_state->callingExecState() ? WebScriptCallFrame::createInstance(m_state->callingExecState()) : 0;
    return 0;
}

String WebScriptCallFrame::functionName()
{
    /*if (!m_state->scopeNode())
        return String();

    FunctionImp* func = m_state->function();
    if (!func)
        return String();

    const Identifier& funcIdent = func->functionName();
    if (!funcIdent.isEmpty())
        return funcIdent.ascii();*/

    return String();
}

String WebScriptCallFrame::stringByEvaluatingJavaScriptFromString(String script)
{
    JSLock lock;

    JSValue* scriptExecutionResult = valueByEvaluatingJavaScriptFromString(script);
    return jsValueToString(m_state, scriptExecutionResult);
}

PropertyNameArray WebScriptCallFrame::variableNames()
{
    PropertyNameArray propertyNames(m_state);

    //m_state->scopeChain().top()->getPropertyNames(m_state, propertyNames);

    return propertyNames;
}

String WebScriptCallFrame::valueForVariable(String key)
{
    Identifier identKey(m_state, key.utf8().data());

    JSValue* jsvalue = 0;
    /*ScopeChain scopeChain = m_state->scopeChain();
    for (ScopeChainIterator it = scopeChain.begin(); it != scopeChain.end() && !jsvalue; ++it)
        jsvalue = (*it)->get(m_state, identKey);*/

    return jsValueToString(m_state, jsvalue);
}

JSValue* WebScriptCallFrame::valueByEvaluatingJavaScriptFromString(String script)
{
#if 0
    ExecState* state = m_state;
    JSGlobalObject* globObj = state->dynamicGlobalObject();

    // find "eval"
    JSObject* eval = 0;
    if (state->scopeNode()) {  // "eval" won't work without context (i.e. at global scope)
        JSValue* v = globObj->get(state, "eval");
        if (v->isObject() && static_cast<JSObject*>(v)->implementsCall())
            eval = static_cast<JSObject*>(v);
        else
            // no "eval" - fallback operates on global exec state
            state = globObj->globalExec();
    }

    JSValue* savedException = state->exception();
    state->clearException();

    UString code(script.utf8().data());

    // evaluate
    JSValue* scriptExecutionResult;
    if (eval) {
        List args;
        args.append(jsString(code));
        scriptExecutionResult = eval->call(state, 0, args);
    } else
        // no "eval", or no context (i.e. global scope) - use global fallback
        scriptExecutionResult = Interpreter::evaluate(state, UString(), 0, code.data(), code.size(), globObj).value();

    if (state->hadException())
        scriptExecutionResult = state->exception();    // (may be redundant depending on which eval path was used)
    state->setException(savedException);

    return scriptExecutionResult;
#else
    return jsNull();
#endif
}
