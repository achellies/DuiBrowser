/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// EAWebKitString.h
// (5/7/09 CSidhall - This was extracted from: EAText StdC.h By Paul Pedriana)
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKITSTRING_H
#define EAWEBKITSTRING_H

#include <string.h>
#include <EABase/eabase.h>
#include <stdlib.h>
#include <wchar.h>
#include <EAWebKit/EAWebKitConfig.h>
#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif


namespace EA
{
    namespace Internal
    {
        int Strlcpy(char8_t*  pDest, const char16_t* pSource, size_t nDestCapacity, size_t nSourceLength);
        int Strlcpy(char16_t* pDest, const char8_t*  pSource, size_t nDestCapacity, size_t nSourceLength);
        int Strlcpy(char16_t* pDestination, const char16_t* pSource, size_t nDestCapacity);    
        int Stricmp(const char8_t* pString1, const char8_t* pString2);       
        int Stricmp(const char16_t* pString1, const char16_t* pString2);
        char8_t* Stristr(const char8_t* s1, const char8_t* s2);
		char16_t* Stristr(const char16_t* pString, const char16_t* pSubString);

        // Inlines           
        inline size_t Strlen(const char8_t* pString)
        {
            return strlen(pString);
        }

        inline size_t Strlen(const char16_t* pString)
        {
            return wcslen(pString);
        }
    

        inline int Strcmp(const char8_t*  pString1, const char8_t*  pString2)
        {
            return strcmp(pString1, pString2);
        }

        inline int Strcmp(const char16_t* pString1, const char16_t* pString2)
        {
            return wcscmp(pString1, pString2);
        }
      
       inline char16_t* Strcpy(char16_t* pDestination, const char16_t* pSource)
        {
            return wcscpy(pDestination, pSource);
        }
    
        #if !defined(__CYGWIN__)
        inline uint32_t AtoU32(const char16_t* pString)
        {
            return (uint32_t)wcstoul(pString, NULL, 10);
        }
        #endif
    
        inline char16_t* Strcat(char16_t* pDestination, const char16_t* pSource)
        {
            return wcscat(pDestination, pSource);
        }

        inline char16_t* Strncpy(char16_t* pDestination, const char16_t* pSource, size_t n)
        {
            return wcsncpy(pDestination, pSource, n);
        }
    }
}
#endif // Header include guard

