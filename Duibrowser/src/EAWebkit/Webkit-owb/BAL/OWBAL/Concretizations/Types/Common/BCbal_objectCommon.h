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

#ifndef BINDINGS_BAL_OBJECT_H_
#define BINDINGS_BAL_OBJECT_H_

#include <wtf/FastAllocBase.h>
#include "Vector.h"
#include "PlatformString.h"

class BalObject;
class BalValuePrivate;


// 1/26/10 CSidhall - Swapped UndefinedType with NumberType to stay in sync with JSType.h JSType enum. 
enum BalType {
    UnspecifiedType   = 0,
    UndefinedType     = 1,
    BooleanType       = 2,
    NumberType        = 3,
    NullType          = 4,
    StringType        = 5,
    ObjectType        = 6,
    GetterSetterType  = 7
};

class BalValue/*: public WTF::FastAllocBase*/ {
    public:
        BalValue();
        BalValue(BalValuePrivate *priv);
        ~BalValue();

        BalType type() const;
        bool isNumber() const;
        bool isString() const;
        bool isObject() const;

        bool toBoolean() const;
        double toNumber() const;
        WebCore::String toString() const;
        BalObject *toObject() const;
        ///return value converted from string color (named or valued) to binary color. returns 0x0 if problem when decoding color
        unsigned int toRGBA32() const;

        void balUndefined();
        void balNull();
        void balNaN();
        void balBoolean(bool b);
        void balNumber(double d);
        void balString(WebCore::String s);
        void balObject(BalObject *obj);

        BalValuePrivate *d;
        BalObject *m_obj;
};

class BalClass;
class BalMethod;

class BalObject/*: public WTF::FastAllocBase*/
{
public:
    BalObject();
    virtual ~BalObject();
    void invalidate();
    virtual bool hasMethod(const char *name);
    virtual WebCore::String getName() { return ""; };
    virtual BalValue *invoke(const char *name, Vector<BalValue *> args, KJS::ExecState* exec=0); // 3/25/10 CSidhall - Added passing exec. Fixes a binding return string alloc problem.
    bool hasProperty(const char *name);
    virtual BalValue *getProperty(const char *name, KJS::ExecState* exec=0); // 3/25/10 CSidhall - Added passing exec. Fixes a binding string alloc problem.
    virtual void setProperty( const char *name, BalValue *value);
    void addMethod(const char *);
    void removeMethod(const char *);
    void addProperty(const char *);
    void removeProperty(const char *);
private:
    Vector<const char *> m_balMethodList;
    Vector<const char *> m_balPropertyList;
};

#endif
