/*
 * Copyright (C) 2008 Pleyo.  All rights reserved.
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
* This file was modified by Electronic Arts Inc Copyright © 2008-2010
*/

///////////////////////////////////////////////////////////////////////////////
// BCNVK_MIMETypeRegistry.cpp
// Modified by Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "MIMETypeRegistry.h"
#include <ctype.h>


namespace OWBAL {


struct ExtensionMap
{
    const char* extension;
    const char* mimeType;
};


static const ExtensionMap extensionMap [] =
{
    { "bmp", "image/bmp" },
    { "css", "text/css" },
    { "gif", "image/gif" },
    { "html", "text/html" },
    { "ico", "image/x-icon" },
    { "jpeg", "image/jpeg" },
    { "jpg", "image/jpeg" },
    { "js", "application/x-javascript" },
    { "pdf", "application/pdf" },
    { "png", "image/png" },
    { "rss", "application/rss+xml" },
    { "svg", "image/svg+xml" },
    { "text", "text/plain" },
    { "txt", "text/plain" },
    { "xbm", "image/x-xbitmap" },
    { "xml", "text/xml" },
    { "xsl", "text/xsl" },
    { "xhtml", "application/xhtml+xml" },
    { "avi", "movie/avi" },
    { "flv", "movie/flv" },
    { 0, 0 }
};


String MIMETypeRegistry::getMIMETypeForExtension(const String& ext)
{
    for(const ExtensionMap* e = extensionMap; e->extension; ++e)
    {
        const UChar* const p = ext.characters();
        unsigned i, iEnd;

        for(i = 0, iEnd = ext.length(); (i < iEnd) && e->extension[i]; ++i)
        {
            if(tolower((unsigned char)p[i]) != e->extension[i])
                break;
        }

        if(i == iEnd)
            return e->mimeType;
    }

    return String();  // This should be an uncommon pathway.
}
//NVK:  Oddly enough, it seems that this class has a 'split' implementation with BCMimeTypeRegistryWK.cpp.
//I am not sure what to make of it.
}
