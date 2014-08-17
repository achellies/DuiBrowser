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


/**
 * @file  bal_instance.h
 *
 * Header file for bal_instance.
 *
 * Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */

#ifndef BINDINGS_BAL_INSTANCE_H_
#define BINDINGS_BAL_INSTANCE_H_

#include "runtime.h"
#include "runtime_root.h"

class BalObject;

namespace KJS {

namespace Bindings {

class BalClass;

class BalInstance : public Instance
{
public:
    static PassRefPtr<BalInstance> create(BalObject* object, PassRefPtr<RootObject> rootObject)
    {
        return adoptRef(new BalInstance(object, rootObject));
    }

    ~BalInstance ();
    
    virtual Class* getClass() const;
    
    //BalInstance (const BalInstance &other);

    BalInstance &operator=(const BalInstance &other);
    
    virtual void begin();
    virtual void end();
    
    virtual JSValue* valueOf(ExecState* exec) const;
    virtual JSValue* defaultValue (ExecState* exec, JSType hint) const;	

    virtual CallType getCallData(CallData&);

    virtual void mark(); // This isn't inherited
    
    virtual JSValue* invokeMethod (ExecState *exec, const MethodList &method, const ArgList &args);
    virtual JSValue* invokeDefaultMethod (ExecState *exec, const ArgList &args);
    virtual void getPropertyNames(ExecState*, PropertyNameArray&);

    JSValue* stringValue(ExecState* exec) const;
    JSValue* numberValue(ExecState* exec) const;
    JSValue* booleanValue() const;
    
    BalObject* getObject() const { return m_object; }

    virtual BindingLanguage getBindingLanguage() const { return BalLanguage; }
    static RuntimeObjectImp* getRuntimeObject(ExecState* exec, PassRefPtr<BalInstance>);
private:
    BalInstance(BalObject*, PassRefPtr<RootObject>);

    mutable BalClass* m_class;
    BalObject *m_object;
};


} // namespace Bindings

} // namespace KJS

#endif
