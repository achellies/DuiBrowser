/*
 *  Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"
#include "lookup.h"
#include "lookup.h"


namespace KJS {


//////////////////////////////////////////////////////////
// Added by Paul Pedriana, 2/2009 to handle memory leaks.
//
// There is a HashTable system which creates HashTable
// objects out of static memory as the application runs
// However, the HashTable memory was never freed during
// application runtime. What we do here is keep track of
// all the tables and provide a single shutdown function
// which frees all the tables.

//        T*    fastNew<T>();
//        T*    fastNew<T>(arg);
//        T*    fastNew<T>(arg, arg);
//        T*    fastNewArray<T>(count);
//        void  fastDelete(T* p);
//        void  fastDeleteArray(T* p);
//        void  fastNonNullDelete(T* p);
//        void  fastNonNullDeleteArray(T* p);


HashTable** allHashTables         = NULL;
size_t      allHashTablesCount    = 0;
size_t      allHashTablesCapacity = 0;

void AddHashTable(const HashTable* pHashTable)
{
    if(allHashTablesCount == allHashTablesCapacity) // If out of capacity... realloc the table array.
    {
        if(allHashTablesCapacity == 0)
            allHashTablesCapacity = 64;
        else
            allHashTablesCapacity += 32;

        HashTable** pNew = EAWEBKIT_NEW("AddHashTable") HashTable*[allHashTablesCapacity];//WTF::fastNewArray<HashTable*>(allHashTablesCapacity);
        memcpy(pNew, allHashTables, allHashTablesCount * sizeof(HashTable*));
        EAWEBKIT_DELETE[] allHashTables;//WTF::fastDeleteArray(allHashTables);
        allHashTables = pNew;
    }

    allHashTables[allHashTablesCount++] = const_cast<HashTable*>(pHashTable);
}

void FreeHashTables()
{
    for(size_t i = 0; i < allHashTablesCount; i++)
    {
        HashTable* pHashTable = allHashTables[i];

        if(pHashTable)
        {
            for(int j = 0; j <= pHashTable->hashSizeMask; ++j)      // 4/2/09 CSidhall - Leak fix: added <= instead of just <
            {
                HashEntry* pHashEntry = const_cast<HashEntry*>(&pHashTable->table[j]);

                if(pHashEntry && pHashEntry->key)
                    pHashEntry->key->deref();     // Takes care of freeing the memory lost in the createTable function when it uses releaseRef.
            }

            // Disabled for now because it seems that in some cases this is already deleted by runtime code. And sometimes not. Ugh.
            // 2/24/09 CSidhall - Activated delete for only found 1 case in runtime (lexer.cpp destructor in which I commented out the delete).
            // Tested on a number of sites and found no problems so far. 
            delete[] pHashTable->table;  // This will use HashTable member operator delete. Matches the new HashEntry code in the createTable function.
            pHashTable->table = NULL;
        }

        allHashTables[i] = NULL;      // We don't really need to do this, but it makes it easier for debugging.
    }

    // 4/01/09 CSidhall - Added delete for leak fix
    if(allHashTables) {
        EAWEBKIT_DELETE[] allHashTables;//WTF::fastDeleteArray(allHashTables);
        allHashTablesCapacity = 0;
		allHashTables = NULL;
    }

    allHashTablesCount = 0;
}
//////////////////////////////////////////////////////////



void HashTable::createTable(JSGlobalData* globalData) const
{
    ASSERT(!table);
    HashEntry* entries = new HashEntry[hashSizeMask + 1];
    for (int i = 0; i <= hashSizeMask; ++i)
        entries[i].key = 0;
    for (int i = 0; values[i].key; ++i) {
        UString::Rep* identifier = Identifier::add(globalData, values[i].key).releaseRef(); // Note by Paul Pedriana, 2/2009: This releaseRef appears to be an intentional memory leak. We attempt to counteract it in our FreeHashTables function.
        int hashIndex = identifier->computedHash() & hashSizeMask;
        ASSERT(!entries[hashIndex].key);
        entries[hashIndex].key = identifier;
        entries[hashIndex].integerValue = values[i].value;
        entries[hashIndex].attributes = values[i].attributes;
        entries[hashIndex].length = values[i].length;
    }
    table = entries;

    AddHashTable(this);      // Added by Paul Pedriana, 2/2009 to handle memory leaks.
}

JSValue* staticFunctionGetter(ExecState* exec, const Identifier& propertyName, const PropertySlot& slot)
{
    // Look for cached value in dynamic map of properties (in JSObject)
    ASSERT(slot.slotBase()->isObject());
    JSObject* thisObj = static_cast<JSObject*>(slot.slotBase());
    JSValue* cachedVal = thisObj->getDirect(propertyName);
    if (cachedVal)
        return cachedVal;

    const HashEntry* entry = slot.staticEntry();
    JSValue* val = new (exec) PrototypeFunction(exec, entry->length, propertyName, entry->functionValue);
    thisObj->putDirect(propertyName, val, entry->attributes);
    return val;
}

void setUpStaticFunctionSlot(ExecState* exec, const HashEntry* entry, JSObject* thisObj, const Identifier& propertyName, PropertySlot& slot)
{
    ASSERT(entry->attributes & Function);
    PrototypeFunction* function = new (exec) PrototypeFunction(exec, entry->length, propertyName, entry->functionValue);
    thisObj->putDirect(propertyName, function, entry->attributes);
    slot.setValueSlot(thisObj->getDirectLocation(propertyName));
}

}
