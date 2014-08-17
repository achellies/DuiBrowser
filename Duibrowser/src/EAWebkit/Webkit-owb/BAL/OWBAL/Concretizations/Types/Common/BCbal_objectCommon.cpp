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

#include "bal_object.h"
#include "balValuePrivate.h"
#include "JSValue.h"

#include "Color.h"

#include <stdio.h>


BalValue::BalValue()
    : d(new BalValuePrivate())
    , m_obj(0)
{
}

BalValue::BalValue(BalValuePrivate *priv)
    : m_obj(0)
{
    d = priv;
}

BalValue::~BalValue()
{
    if(d)
        delete d;
    d = 0;
}

BalType BalValue::type() const
{
    return d->type();
}

bool BalValue::isNumber() const
{
    return d->isNumber();
}

bool BalValue::isString() const
{
    return d->isString();
}

bool BalValue::isObject() const
{
    return d->isObject();
}

unsigned int BalValue::toRGBA32() const
{
    //FIXME: remove this namespace from here! :)
    WebCore::Color   color(d->toString());
    return color.rgb();
}

bool BalValue::toBoolean() const
{
    return d->toBoolean();
}

double BalValue::toNumber() const
{
    return d->toNumber();
}

WebCore::String BalValue::toString() const
{
    return d->toString();
}

BalObject *BalValue::toObject() const
{
    return d->toObject();
}


void BalValue::balUndefined()
{
    d->balUndefined();
}

void BalValue::balNull()
{
    d->balNull();
}

void BalValue::balNaN()
{
    d->balNaN();
}

void BalValue::balBoolean(bool b)
{
    d->balBoolean(b);
}

void BalValue::balNumber(double dd)
{
    d->balNumber(dd);
}

void BalValue::balString(WebCore::String s)
{
    d->balString(s);
}

void BalValue::balObject(BalObject *obj)
{
     m_obj = obj;
}

BalObject::BalObject()
{
}

BalObject::~BalObject()
{
    m_balPropertyList.clear();
    m_balMethodList.clear();
}

void BalObject::invalidate()
{
}

bool BalObject::hasMethod(const char *name)
{
    for (unsigned i=0; i<m_balMethodList.size(); i++)
    {
        if (!strcmp(name, m_balMethodList[i]))
        {
            return true;
            break;
        }
    }
    return false;
}

BalValue *BalObject::invoke(const char *name, Vector<BalValue *> args, KJS::ExecState* exec)     // 3/25/10 CSidhall - Added exec param for string alloc. 
{
    return NULL;
}

bool BalObject::hasProperty( const char *name)
{
    for (unsigned i = 0; i < m_balPropertyList.size(); i++)
    {
        if (!strcmp(name, m_balPropertyList[i]))
        {
            return true;
            break;
        }
    }
    return false;
}

BalValue *BalObject::getProperty(const char *name, KJS::ExecState* exec)    // 3/25/10 CSidhall - Added exec param for string alloc.
{
    return NULL;
}

void BalObject::setProperty(const char *name, BalValue *value)
{
	//OWB_PRINTF("set property\n");
}


void BalObject::addMethod(const char *aMethod)
{
    m_balMethodList.append(aMethod);
}

void BalObject::removeMethod(const char *aMethod)
{
    for (unsigned i=0; i<m_balMethodList.size(); i++)
    {
        if (!strcmp(aMethod, m_balMethodList[i]))
        {
            m_balMethodList.remove(i);
            break;
        }
    }
}

void BalObject::addProperty(const char *aProperty)
{
    m_balPropertyList.append(aProperty);
}

void BalObject::removeProperty(const char *aProperty)
{
    for (unsigned i = 0; i<m_balPropertyList.size(); i++)
    {
        if (!strcmp(aProperty, m_balPropertyList[i]))
        {
            m_balPropertyList.remove(i);
            break;
        }
    }
}

