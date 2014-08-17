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

#include "config.h"
#include "bal_instance.h"

#include "bal_class.h"
#include "bal_runtime.h"
#include "list.h"

#include "bal_object.h"
#include "balValuePrivate.h"
#include "wtf/HashMap.h"
#include "ObjectPrototype.h"

#include <stdio.h>

namespace KJS {
namespace Bindings {

// Derived RuntimeObject
class BalRuntimeObjectImp : public RuntimeObjectImp {
    public:
        BalRuntimeObjectImp(PassRefPtr<Instance>);
        ~BalRuntimeObjectImp();
        virtual void invalidate();

        // Additions
        //virtual bool implementsConstruct() const {return implementsCall();}
        virtual JSObject* construct(ExecState* exec, const ArgList& args);
    protected:
        void removeFromCache();
};

BalRuntimeObjectImp::BalRuntimeObjectImp(PassRefPtr<Instance> instance)
    : RuntimeObjectImp(instance)
{
}

BalRuntimeObjectImp::~BalRuntimeObjectImp()
{
    removeFromCache();
}

void BalRuntimeObjectImp::invalidate()
{
    removeFromCache();
    RuntimeObjectImp::invalidate();
}

void BalRuntimeObjectImp::removeFromCache()
{
    /*JSLock lock;
    BalInstance* key = cachedObjects.get(this);
    if (key)
        cachedObjects.remove(key);*/
}

JSObject* BalRuntimeObjectImp::construct(ExecState* exec, const ArgList& args)
{
    // ECMA 15.2.2.1 (?)
    CallData callData;
    CallType callType = getCallData(callData);
    JSValue* val = call(exec, this, callType, callData, this, args);

    if (!val || val->type() == NullType || val->type() == UndefinedType)
        return new (exec) JSObject(exec->lexicalGlobalObject()->objectPrototype()->prototype());
    else
        return val->toObject(exec);
}



BalInstance::BalInstance(BalObject* o, PassRefPtr<RootObject> rootObject)
    : Instance(rootObject),
      m_class(0),
      m_object(o)
{
}

BalInstance::~BalInstance() 
{
    m_object = 0;
    m_class = 0;
    //cachedObjects.remove(this);
}

/*BalInstance::BalInstance(const BalInstance& other)
    : Instance(other.rootObject()), m_class(0), m_object(other.m_object)
{
}*/

BalInstance& BalInstance::operator=(const BalInstance& other)
{
    if (this == &other)
        return *this;

    m_object = other.m_object;
    m_class = 0;

    return *this;
}

Class* BalInstance::getClass() const
{
    if (!m_class)
        m_class = BalClass::classForObject(m_object);
    return m_class;
}

void BalInstance::mark()
{
}

void BalInstance::begin()
{
    // Do nothing.
}

void BalInstance::end()
{
    // Do nothing.
}

JSValue* BalInstance::invokeMethod(ExecState* exec, const MethodList& methodList, const ArgList& args)
{
    ASSERT(methodList.size() == 1);

    BalMethod* method = static_cast<BalMethod*>(methodList[0]);

    const char *ident = method->name();
    if (!m_object->hasMethod( ident ))
        return jsUndefined();

    unsigned count = args.size();
    Vector<BalValue *, 128> cArgs(count);

    unsigned i;
    for (i = 0; i < count; i++) {
        BalValuePrivate *priv = new BalValuePrivate(exec, args.at(i));
        cArgs[i] = new BalValue(priv);
    }

    BalValue *val = m_object->invoke(ident, cArgs, exec); // // 3/25/10 CSidhall -Added passing exec

	for (i = 0; i < count; i++) {
		delete cArgs[i];
		cArgs[i] = 0;
	}


    cArgs.clear();

    // 3/25/10 CSidhall - Safety.  For the default invoke returns NULL.
    if(val)
        return val->d->getValue();
    else
        return 0;
}


JSValue* BalInstance::invokeDefaultMethod(ExecState* exec, const ArgList& args)
{
    return jsUndefined();
}

JSValue* BalInstance::defaultValue(ExecState* exec, JSType hint) const
{
    if (hint == (JSType)StringType)
        return stringValue(exec);
    if (hint == (JSType)NumberType)
        return numberValue(exec);
    if (hint == (JSType)BooleanType)
        return booleanValue();
    return valueOf(exec);
}

JSValue* BalInstance::stringValue(ExecState* exec) const
{
    #if defined(_MSC_VER)
        #define snprintf_imp _snprintf
    #else
        #define snprintf_imp snprintf
    #endif

    char buf[1024];
    snprintf_imp(buf, sizeof(buf), "BalObject %p (%s)", m_object, m_class->name());
    return jsString(exec, buf);
}

JSValue* BalInstance::numberValue(ExecState* exec) const
{
    return jsNumber(exec, 0);
}

JSValue* BalInstance::booleanValue() const
{
    // FIXME: Implement something sensible.
    return jsBoolean(false);
}

JSValue* BalInstance::valueOf(ExecState* exec) const 
{
    return stringValue(exec);
}

RuntimeObjectImp* BalInstance::getRuntimeObject(ExecState* exec, PassRefPtr<BalInstance> instance)
{
    /*JSLock lock;
    JSObject* ret = cachedObjects.value(instance.get());
    if (!ret) {
        ret = new (exec) BalRuntimeObjectImp(instance);
        cachedObjects.insert(instance.get(), ret);
    }
    return ret;*/
    JSLock lock;
    RuntimeObjectImp* ret = new (exec) BalRuntimeObjectImp(instance);
    return ret;
}

void BalInstance::getPropertyNames(ExecState*, PropertyNameArray& nameArray) 
{
    /*if (!NP_CLASS_STRUCT_VERSION_HAS_ENUM(_object->_class) ||
        !_object->_class->enumerate)
        return;

    unsigned count;
    NPIdentifier* identifiers;
    
    {
        JSLock::DropAllLocks dropAllLocks;
        if (!_object->_class->enumerate(_object, &identifiers, &count))
            return;
    }
    
    for (unsigned i = 0; i < count; i++) {
        PrivateIdentifier* identifier = static_cast<PrivateIdentifier*>(identifiers[i]);
        
        if (identifier->isString)
            nameArray.add(identifierFromNPIdentifier(identifier->value.string));
        else
            nameArray.add(Identifier::from(identifier->value.number));
    }
         
    // FIXME: This should really call NPN_MemFree but that's in WebKit
    WTF::fastFree(identifiers);*/
}

CallType BalInstance::getCallData(CallData&)
{
    // See if we have qscript_call
    /*if (m_defaultMethodIndex == -2) {
        if (m_object) {
            const QMetaObject* meta = m_object->metaObject();
            int count = meta->methodCount();
            const QByteArray defsig("qscript_call");
            for (int index = count - 1; index >= 0; --index) {
                const QMetaMethod m = meta->method(index);

                QByteArray signature = m.signature();
                signature.truncate(signature.indexOf('('));

                if (defsig == signature) {
                    m_defaultMethodIndex = index;
                    break;
                }
            }
        }

        if (m_defaultMethodIndex == -2) // Not checked
            m_defaultMethodIndex = -1; // No qscript_call
    }

    // typeof object that implements call == function
    return (m_defaultMethodIndex >= 0 ? CallTypeNative : CallTypeNone);*/
    return CallTypeNone;
}


}
}

