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
#include "identifier.h"

#include "bal_class.h"
#include "bal_instance.h"
#include "bal_runtime.h"

#include <bal_object.h>
#include <stdio.h>

namespace KJS {
namespace Bindings {


BalClass::~BalClass()
{
    deleteAllValues(m_methods);
    deleteAllValues(m_fields);
}
    
typedef HashMap<const BalObject*, BalClass*> ClassesByBalObject;
static ClassesByBalObject* classesByBalObject = 0;

BalClass* BalClass::classForObject(BalObject* o)
{
    if (!classesByBalObject)
        classesByBalObject = new ClassesByBalObject;

    const BalObject* mo = o;
    BalClass* aClass = classesByBalObject->get(mo);
    if (!aClass) {
        aClass = new BalClass();
        classesByBalObject->set(mo, aClass);
    }

    return aClass;
}

void BalClass::cleanup()
{
	if (classesByBalObject)
	{
		for (ClassesByBalObject::iterator i = classesByBalObject->begin(); i != classesByBalObject->end(); ++i)
		{
			delete i->second;
		}

		delete classesByBalObject;
		classesByBalObject = 0;
	}
}

const char* BalClass::name() const
{
    return "";
}

MethodList BalClass::methodsNamed(const Identifier& identifier, Instance* instance) const
{
    MethodList methodList;

    Method* method = m_methods.get(identifier.ustring().rep());
    if (method) {
        methodList.append(method);
        return methodList;
    }

	const UChar* ident16 = identifier.ustring().data();
	char ident[256];
	sprintf(ident,"%S",ident16);
	ident[identifier.ustring().size()] = '\0';

    const BalInstance* inst = static_cast<const BalInstance*>(instance);
    BalObject* obj = inst->getObject();
    if( obj->hasMethod( ident ) )
    {
        BalMethod *aMethod= new BalMethod(obj, 0, ident, 0);
        m_methods.set(identifier.ustring().rep(), aMethod);
        methodList.append(aMethod);
    }

    return methodList;
}


Field* BalClass::fieldNamed(const Identifier& identifier, Instance *instance) const
{
    Field* aField = m_fields.get(identifier.ustring().rep());
    if (aField)
        return aField;
	const UChar* ident16 = identifier.ustring().data();
	char ident[256];
	sprintf(ident,"%S",ident16);
	ident[identifier.ustring().size()] = '\0';

    const BalInstance* inst = static_cast<const BalInstance*>(instance);
    
    BalObject* obj = inst->getObject();
    if( obj->hasProperty( ident ) )
    {
        aField = new BalField(ident);
        m_fields.set(identifier.ustring().rep(), aField);
    }
    return aField;
}

}
}

