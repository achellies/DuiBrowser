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


/**
 * @file  bal_runtime.h
 *
 * Header file for bal_runtime.
 *
 * Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */

#ifndef BINDINGS_BAL_RUNTIME_H_
#define BINDINGS_BAL_RUNTIME_H_

#include "runtime.h"
#include "bal_object.h"
#include <wtf/FastAllocBase.h>

namespace KJS {
namespace Bindings {

class BalInstance;

class BalField : public Field {
public:
    BalField(const char *p)
    {
        m_ident = (char *) WTF::fastMalloc (strlen(p) +1);
        strcpy( m_ident, p );
    }

    ~BalField()
    {
        if( m_ident != NULL )
            WTF::fastFree( m_ident );
    }
    virtual JSValue* valueFromInstance(ExecState*, const Instance*) const;
    virtual void setValueToInstance(ExecState*, const Instance*, JSValue*) const;
    virtual const char* name() const;

private:
    char *m_ident;
};


class BalMethod : public Method
{
public:
    BalMethod(BalObject *mo, int i, const char *ident, int numParameters)
        : m_balObject(mo),
          m_index(i),
          m_nParams(numParameters)
    {
        m_identifier = (char *) WTF::fastMalloc (strlen(ident) + 1);
        strcpy( m_identifier, ident );
    }
    ~BalMethod()
    {
    	m_balObject = 0;
        if( m_identifier != NULL )
            WTF::fastFree( m_identifier );
    }
    virtual const char* name() const { return m_identifier; }
    virtual int numParameters() const { return m_nParams; }
    BalObject *getObject() { return m_balObject ; }

private:
    friend class BalInstance;
    BalObject *m_balObject;
    int m_index;
    char *m_identifier;
    int m_nParams;
};

} // namespace Bindings
} // namespace KJS

#endif
