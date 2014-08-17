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

#include "config.h"
#include "JSObject.h"
#include "runtime_object.h"
#include "bal_runtime.h"
#include "bal_instance.h"
#include "balValuePrivate.h"

/*
* This file was modified by Electronic Arts Inc Copyright © 2010
*/

namespace KJS {
namespace Bindings {

const char* BalField::name() const
{
    return "";
}

JSValue* BalField::valueFromInstance(ExecState* exec, const Instance* inst) const
{
    const BalInstance* instance = static_cast<const BalInstance*>(inst);
    BalObject* obj = instance->getObject();
    BalValue* val = obj->getProperty(m_ident, exec);    // 3/25/10 CSidhall - Added exec so that a bal string can be set in JS.
    JSValue *v;
    if(val->m_obj != NULL)
        v = val->d->balObject(val->m_obj, exec);
    else
        v = val->d->getValue();
    delete val;
    return v;
}

void BalField::setValueToInstance(ExecState* exec, const Instance* inst, JSValue* aValue) const
{
    const BalInstance* instance = static_cast<const BalInstance*>(inst);
    BalObject* obj = instance->getObject();
    BalValuePrivate *priv = new BalValuePrivate(exec, aValue);
    BalValue *val = new BalValue(priv);
    obj->setProperty(m_ident, val);
    delete val;
}

} }
