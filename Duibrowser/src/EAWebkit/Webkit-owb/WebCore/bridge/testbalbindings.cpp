// -*- c-basic-offset: 2 -*-
/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 *
 */
#include "config.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "JSValue.h"
#include "JSObject.h"
//#include "types.h"
#include "interpreter.h"
#include "kjs/JSGlobalObject.h"
#include "CString.h"

#include "bal_object.h"
#include "bal_runtime.h"
#include "bal_class.h"
#include "bal_instance.h"

using namespace KJS;
using namespace KJS::Bindings;


class MyObject2 : public BalObject
{
public:
    MyObject2()
    {
        printf("myobject2\n");
        addMethod("log");
    }

    ~MyObject2()
    {
    }

    BalValue *invoke( const char *name, Vector<BalValue *> args)
    {
        printf("invoke objet 2\n");
        if (!strcmp(name, "log") ) {
            BalType type = args.at(0)->type();
            if (args.size() == 1)
            {
                logMessage (args.at(0)->toString().utf8().data());
            }
        }
        BalValue *val = new BalValue();
        val->balUndefined();
        return val;
    }

    BalValue *getProperty(const char *name)
    {
        printf("get property objet 2\n");
        BalValue *val = new BalValue();
        val->balUndefined();
        return val;
    };
    void setProperty( const char *name, const BalValue *value) 
    {
        printf("set property objet 2\n");
    };
    void logMessage (const char *message)
    {
        printf ("%s\n", message);
    }
};



class MyObject : public BalObject
{
public:
    MyObject():doubleValue(5.5), intValue(454), stringValue( "toto" ), booleanValue( true ), myObject2(0)
    {
        printf("myobject\n");
        addMethod("logMessage");
        addMethod("setIntValue");
        addMethod("getIntValue");
        addMethod("getStringValue");

        addProperty("doubleValue");
        addProperty("intValue");
        addProperty("stringValue");
        addProperty("booleanValue");
        addProperty("nullValue");
        addProperty("undefinedValue");
        addProperty("myTest");
    }

    ~MyObject()
    {
        if( myObject2 != NULL )
            delete myObject2;
    }

    BalValue *invoke( const char *name, Vector<BalValue*> args)
    { 
        BalValue *val = new BalValue();
        if (!strcmp(name, "logMessage") ) {
            BalType type = args.at(0)->type();
            if (args.size() == 1)
            {
                logMessage (args.at(0)->toString().utf8().data());
            }
        }
        else if (!strcmp(name, "setIntValue") ) {
            BalType type = args.at(0)->type();
            intValue = (int)args.at(0)->toNumber();
            val->balNumber(intValue);
            return val;
        }
        else if (!strcmp(name, "getIntValue") ) {
            val->balNumber(intValue);
            return val;
        }
        else if (!strcmp(name, "getStringValue") ) {
            val->balString(stringValue);
            return val;
        }
        val->balUndefined();
        return val;
    }

    BalValue *getProperty(const char * name)
    {
        BalValue *val = new BalValue();
        if( !strcmp( name, "doubleValue" ) )
        {
            val->balNumber(doubleValue);
            return val;
        }
        else if( !strcmp( name, "intValue" ) )
        {
            val->balNumber(intValue);
            return val;
        }
        else if( !strcmp( name, "stringValue" ) )
        {
            val->balString(stringValue);
            return val;
        }
        else if( !strcmp( name, "booleanValue" ) )
        {
            val->balBoolean(booleanValue);
            return val;
        }
        else if( !strcmp( name, "nullValue" ) )
        {
            val->balNull();
            return val;
        }
        else if( !strcmp( name, "undefinedValue" ) )
        {
            val->balUndefined();
            return val;
        }
// FIXME
        else if( !strcmp( name, "myTest" ) )
        {
            if( myObject2 == NULL )
            {
                myObject2 = new MyObject2();
            }
            val->balObject(myObject2);
            return val;
        }
        val->balUndefined();
        return val;
    };
    void setProperty( const char *name, const BalValue *value) 
    {
        if( !strcmp( name, "intValue" ) )
        {
            intValue = (int)value->toNumber();
        }
    };
    void logMessage (const char *message)
    {
        printf ("%s\n", message);
    }

private:
    double doubleValue;
    int intValue;
    char *stringValue;
    bool booleanValue;
    MyObject2 *myObject2;
};

// --------------------------------------------------------

class Global : public JSGlobalObject {
public:
  virtual UString className() const { return "global"; }
};

#define BufferSize 200000
static char code[BufferSize];

const char *readJavaScriptFromFile (const char *file)
{
    FILE *f = fopen(file, "r");
    if (!f) {
        fprintf(stderr, "Error opening %s.\n", file);
        return 0;
    }

    int num = fread(code, 1, BufferSize, f);
    code[num] = '\0';
    if(num >= BufferSize)
        fprintf(stderr, "Warning: File may have been too long.\n");

    fclose(f);

    return code;
}

int main(int argc, char **argv)
{
    // expecting a filename
    if (argc < 2) {
        fprintf(stderr, "You have to specify at least one filename\n");
        return -1;
    }

    bool ret = true;
    {
        JSLock lock;

        // create interpreter w/ global object
        //Global* global = new Global();
        Interpreter *interp = new Interpreter();
        //ExecState *exec = interp->globalExec();
	JSGlobalObject *obj = new JSGlobalObject();
	ExecState *exec = obj->globalExec();
		

        MyObject *myObject = new MyObject();
        RefPtr<RootObject> root = RootObject::create(0, obj);
	KJS::Bindings::Instance::createRuntimeObject(exec, KJS::Bindings::BalInstance::create(static_cast<BalObject*>(myObject), root));

        for (int i = 1; i < argc; i++) {
            const char *code = readJavaScriptFromFile(argv[i]);

            if (code) {
                // fixme run
                //Completion comp(interp->evaluate(exec,"", 0, code, 0));
		;
            }
        }

        delete myObject;

    } // end block, so that Interpreter and global get deleted

    return ret ? 0 : 3;
}
