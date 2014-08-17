/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
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

#ifndef DATE_OBJECT_H
#define DATE_OBJECT_H

#include "JSWrapperObject.h"

namespace KJS {

    struct GregorianDateTime;
    class FunctionPrototype;
    class ObjectPrototype;

    class DateInstance : public JSWrapperObject {
    public:
        DateInstance(JSObject* prototype);
        virtual ~DateInstance();

        double internalNumber() const { return internalValue()->uncheckedGetNumber(); }

        bool getTime(GregorianDateTime&, int& offset) const;
        bool getUTCTime(GregorianDateTime&) const;
        bool getTime(double& milliseconds, int& offset) const;
        bool getUTCTime(double& milliseconds) const;
        
        static const ClassInfo info;

        void msToGregorianDateTime(double, bool outputIsUTC, GregorianDateTime&) const;

    private:
        virtual const ClassInfo* classInfo() const { return &info; }

        using JSWrapperObject::internalValue;

        struct Cache;
        mutable Cache* m_cache;
    };

} // namespace KJS

#endif
