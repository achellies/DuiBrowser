/*
 * Copyright (C) 2007 Pleyo.  All rights reserved.
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

#ifndef BINDINGS_BALVALUEPRIVATE_H_
#define BINDINGS_BALVALUEPRIVATE_H_

#include <wtf/FastAllocBase.h>
#include "JSValue.h"
#include "ExecState.h"
#include "bal_object.h"
#include "PlatformString.h"
#include "JSGlobalObject.h"
#include "runtime_object.h"
#include "bal_instance.h"

using namespace KJS;
class BalValuePrivate/*: public WTF::FastAllocBase*/ {
    public:
        BalValuePrivate()
        {
            m_val = 0;
            m_exec = 0;
        }
        BalValuePrivate(ExecState* exec, JSValue *value)
        {
            m_val = value;
            m_exec = exec;
        }

        ~BalValuePrivate()
        {
            m_val = 0;
            m_exec = 0;
        }

        BalType type() const;
        bool isNumber() const;
        bool isString() const;
        bool isObject() const;

        bool toBoolean() const;
        double toNumber() const;
        WebCore::String toString() const;
        BalObject *toObject() const;

        void balUndefined();
        void balNull();
        void balNaN();
        void balBoolean(bool b);
        void balNumber(double d);
        void balString(WebCore::String s);
        JSValue *balObject(BalObject *obj, ExecState *exec);

        JSValue *getValue() { return m_val; }

        // CSidhall 3/25/10 - Added for binding to be able to set the exec so that a js string can allocate.
        void setExec(ExecState *exec) { m_exec = exec; }

    private:
        JSValue *m_val;
        ExecState *m_exec;
};

class Global : public JSGlobalObject {
public:
  virtual UString className() const { return "global"; }
};

inline BalType BalValuePrivate::type() const
{
    return (BalType)m_val->type();
}

inline bool BalValuePrivate::isNumber() const
{
    return m_val->isNumber();
}

inline bool BalValuePrivate::isString() const
{
    return m_val->isString();
}

inline bool BalValuePrivate::isObject() const
{
    return m_val->isObject();
}

inline bool BalValuePrivate::toBoolean() const
{
    return m_val->toBoolean(m_exec);
}

inline double BalValuePrivate::toNumber() const
{
    return m_val->toNumber(m_exec);
}

inline WebCore::String BalValuePrivate::toString() const
{
    return m_val->toString(m_exec);
}

inline BalObject *BalValuePrivate::toObject() const
{
    JSObject* object = m_val->toObject(m_exec);
    if (object->classInfo() == &RuntimeObjectImp::s_info) {
        RuntimeObjectImp* imp = static_cast<RuntimeObjectImp *>(m_val);
        Bindings::BalInstance* instance = static_cast<Bindings::BalInstance*>(imp->getInternalInstance());
        if (instance) {
            BalObject* obj = instance->getObject();
            return obj;
        }
    }
    return NULL;
}

inline void BalValuePrivate::balUndefined()
{
    m_val = jsUndefined();
}

inline void BalValuePrivate::balNull()
{
    m_val = jsNull();
}

inline void BalValuePrivate::balNaN()
{
    m_val = jsNaN(m_exec);
}

inline void BalValuePrivate::balBoolean(bool b)
{
    m_val = jsBoolean(b);
}

inline void BalValuePrivate::balNumber(double d)
{
    m_val = jsNumber(m_exec, d);
}

inline void BalValuePrivate::balString(WebCore::String s)
{
    m_val = jsString(m_exec, s);
}

inline JSValue *BalValuePrivate::balObject(BalObject *obj, ExecState *exec)
{
    return Bindings::Instance::createRuntimeObject(exec, Bindings::BalInstance::create(obj, Bindings::findRootObject(exec->dynamicGlobalObject())));
    //return Bindings::Instance::createRuntimeObject(Bindings::Instance::BalLanguage, obj, Bindings::findRootObject(exec->dynamicGlobalObject()));
}

#endif
