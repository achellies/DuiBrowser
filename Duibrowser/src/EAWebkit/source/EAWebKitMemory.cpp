/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

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
// EAWebKitMemory.cpp
// By Paul Pedriana
//
// This file implements the redirection of all memory allocation of WebKit and
// related packages to our EA::WebKit::WebKitAlloc functions. Thus you need 
// only setup this one set of callbacks and all WebKit allocations will be 
// directed to it.
///////////////////////////////////////////////////////////////////////////////


#include "config.h"                 // WebKit config.h file.
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitConfig.h>
#include <stdlib.h>
#include <stdio.h>


 



// Curl package ///////////////////////////////////////////////////////////////

#if USE(CURL)
    // Curl lets you override its memory allocation by simply calling 
    // curl_global_init_mem with function pointers to your own malloc functions.
    // Curl has a function called curl_global_init_mem which allows you to set
    // the malloc functions, but it is mis-designed. It unilaterally also tries
    // to init curl and if curl is already initialized by somebody else then 
    // curl_global_init_mem ignores your malloc functions. 
    #include <curl/curl.h>

    extern "C" {

        void* curl_malloc_custom(size_t n)
        {
            return EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/Curl");
        }

        void curl_free_custom(void* p)
        {
            return EA::WebKit::GetAllocator()->Free(p, 0);
        }

        void* curl_realloc_custom(void* p, size_t n)
        {
            return EA::WebKit::GetAllocator()->Realloc(p, n, 0);
        }

        char* curl_strdup_custom(const char* p)
        {
            const size_t n = strlen(p) + 1;

            char* const pNew = (char*)EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/Curl");

            if(pNew)
                strcpy(pNew, p);

            return pNew;
        }

        void* curl_calloc_custom(size_t count, size_t n)
        {
            void* p = EA::WebKit::GetAllocator()->Malloc(count * n, 0, "EAWebKit/Curl");

            if(p)
                memset(p, 0, count * n);

            return p;
        }

        // Curl has a function called curl_global_init_mem which allows you to set
        // the malloc functions, but it is mis-designed. It unilaterally also tries
        // to init curl and if curl is already initialized by somebody else then 
        // curl_global_init_mem ignores your malloc functions. So we make our own
        // version of how curl_global_init_mem should be. 

        extern curl_malloc_callback  Curl_cmalloc;
        extern curl_free_callback    Curl_cfree;
        extern curl_realloc_callback Curl_crealloc;
        extern curl_strdup_callback  Curl_cstrdup;
        extern curl_calloc_callback  Curl_ccalloc;

        void curl_init_mem(curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c)
        {
            Curl_cmalloc   = m;
            Curl_cfree     = f;
            Curl_crealloc  = r;
            Curl_cstrdup   = s;
            Curl_ccalloc   = c;
        }

    } // extern "C"
#endif



// libjpeg package  ///////////////////////////////////////////////////////////

// libjpeg expects the user to implement memory overrides by writing a 
// custom version of jmemansi.c. Note that libjpeg already has some 
// custom versions of thus, such as jmemdos.c, jmemmac.c etc.
// What we do is simply don't include any of those jmem*.c files in
// the libjpeg build and instead provide the implementation here.

extern "C" {
    #include <jpeglib.h> // libjpeg requires you to #include its headers from C++ via extern "C".
    #include <jmemsys.h>

    void* jpeg_get_small(j_common_ptr /*cinfo*/, size_t n)
    {
        return EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/LibJpeg");
    }

    void jpeg_free_small(j_common_ptr /*cinfo*/, void* p, size_t /*n*/)
    {
        return EA::WebKit::GetAllocator()->Free(p, 0);
    }

    void* jpeg_get_large(j_common_ptr /*cinfo*/, size_t n)
    {
        return EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/LibJpeg");
    }

    void jpeg_free_large(j_common_ptr /*cinfo*/, void* p, size_t /*n*/)
    {
        return EA::WebKit::GetAllocator()->Free(p, 0);
    }

    long jpeg_mem_available(j_common_ptr /*cinfo*/, long /*min_bytes_needed*/, long max_bytes_needed, long /*already_allocated*/)
    {
        return max_bytes_needed;
    }

    static void read_backing_store(j_common_ptr cinfo, backing_store_ptr info, void* buffer_address, long file_offset, long byte_count)
    {
        #if defined(EA_PLATFORM_WINDOWS) ||defined(EA_PLATFORM_UNIX) // Add other platforms as necessary.
            if(fseek(info->temp_file, file_offset, SEEK_SET) == 0)
            {
                if(fread(buffer_address, 1, byte_count, info->temp_file) == (size_t)byte_count)
                    return; // success
            }
		#elif defined(EA_PLATFORM_WINCE)
            if(fseek(info->temp_file, file_offset, SEEK_SET) == 0)
            {
                if(fread(buffer_address, 1, byte_count, info->temp_file) == (size_t)byte_count)
                    return; // success
            }
        #endif

        (void)cinfo; (void)info; (void)file_offset;
        memset(buffer_address, 0, byte_count);
    }

    static void write_backing_store(j_common_ptr cinfo, backing_store_ptr info, void* buffer_address, long file_offset, long byte_count)
    {
        #if defined(EA_PLATFORM_WINDOWS) ||defined(EA_PLATFORM_UNIX) // Add other platforms as necessary.
            if(fseek(info->temp_file, file_offset, SEEK_SET) == 0)
            {
                if(fwrite(buffer_address, 1, byte_count, info->temp_file) == (size_t)byte_count)
                    return; // success
            }
		#elif defined(EA_PLATFORM_WINCE)
            if(fseek(info->temp_file, file_offset, SEEK_SET) == 0)
            {
                if(fwrite(buffer_address, 1, byte_count, info->temp_file) == (size_t)byte_count)
                    return; // success
            }
        #endif

        (void)cinfo; (void)info; (void)buffer_address; (void)file_offset; (void)byte_count;
    }

    static void close_backing_store(j_common_ptr cinfo, backing_store_ptr info)
    {
        #if defined(EA_PLATFORM_WINDOWS) ||defined(EA_PLATFORM_UNIX) // Add other platforms as necessary.
            fclose(info->temp_file);
		#elif defined(EA_PLATFORM_WINCE)
			fclose(info->temp_file);
        #else
            // Do nothing.
        #endif
    }

    void jpeg_open_backing_store(j_common_ptr /*cinfo*/, backing_store_ptr info, long /*total_bytes_needed*/)
    {
        info->read_backing_store  = read_backing_store;
        info->write_backing_store = write_backing_store;
        info->close_backing_store = close_backing_store;
    }

    long jpeg_mem_init(j_common_ptr /*cinfo*/)
    {
        return 0;
    }

    void jpeg_mem_term(j_common_ptr /*cinfo*/)
    {
        // Nothing to do.
    }

} // extern "C"


// libpng package  ///////////////////////////////////////////////////////////

// The original libpng package doesn't allow you to specify a global memory 
// allocation default, but instead requires that the user pass a malloc/free
// function for each use of the library. This is fine but the lack of a global
// memory allocation default creates a big problem: we can't control third 
// party code that fails to pass a malloc/free function and thus third party
// code will use malloc/free and there isn't anything we can do about it.
// Well there is something we can do about it and that is to modify libpng
// to support the concept of a user-settable global memory allocator instead
// of the currently hard-coded call to malloc. So I did that and implemented
// the png_set_default_mem_fn function.

#include <png.h>

//extern "C" {

    void* png_malloc(size_t n)
    {
        return EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/LibPng");
    }

    void png_free(void* p)
    {
        return EA::WebKit::GetAllocator()->Free(p, 0);
    }

// } // extern "C"



// libxml2 /////////////////////////////////////////////////////////////////

#if USE(LIBXML2)
    // libxml has been set up to require you to provide the functions below.
    #include <libxml/xmlmemory.h>

    extern "C" {

        void* xmlMallocCustom(size_t n)
        {
            return EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/LibXml");
        }

        void xmlFreeCustom(void* p)
        {
            return EA::WebKit::GetAllocator()->Free(p, 0);
        }

        void* xmlReallocCustom(void* p, size_t n)
        {
            return EA::WebKit::GetAllocator()->Realloc(p, n, 0);
        }

        char* xmlStrdupCustom(const char* p)
        {
            const size_t n = strlen(p) + 1;
            
            char* const pNew = (char*)EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/LibXml");
            
            if(pNew)
                strcpy(pNew, p);
            
            return pNew;
        }

    } // extern "C"
#endif



// SDL ////////////////////////////////////////////////////////////////////////

// #if USE(SDL)  // We don't have simple means to enable this in our build file at this time. So we just enable it and let the linker ignore these functions.

    // I edited SDL's SDL_stdinc.h to add a new config define called SDL_CUSTOM_MALLOC_ENABLED
    // which allows us to unilaterally force the use of SDL_malloc, etc. The original
    // SDL code assumed that if the standard library had a malloc then you were going 
    // to use it.

    extern "C" {

        void* SDL_malloc(size_t n)
        {
            return EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/SDL");
        }

        void SDL_free(void* p)
        {
            return EA::WebKit::GetAllocator()->Free(p, 0);
        }

        void* SDL_realloc(void* p, size_t n)
        {
            return EA::WebKit::GetAllocator()->Realloc(p, n, 0);
        }

        void* SDL_calloc(size_t count, size_t n)
        {
            void* p = EA::WebKit::GetAllocator()->Malloc(count * n, 0, "EAWebKit/SDL");

            if(p)
                memset(p, 0, count * n);

            return p;
        }

    } // extern "C"

// #endif


// zlib ///////////////////////////////////////////////////////////////////////

// zlib uses to allocation functions calloc and free. If you #define MY_ZCALLOC
// then zlib presumes that you will provide the implementation of these functions.

extern "C" {

    void* zcalloc(void* /*pContext*/, unsigned int count, unsigned int n)
    {
        void* p = EA::WebKit::GetAllocator()->Malloc(count * n, 0, "EAWebKit/ZLib");

        if(p)
            memset(p, 0, count * n);

        return p;
    }

    void  zcfree(void* /*pContext*/, void* p)
    {
        return EA::WebKit::GetAllocator()->Free(p, 0);
    }

} // extern "C"




// fastMalloc /////////////////////////////////////////////////////////////////

// fastMalloc is the malloc functionality that WebKit uses, aside from its 
// unfortunate usage of global operator new. WTF is the namespace used by 
// the JavaScriptCore within WebKit. If you are getting link errors saying
// that the functions below are a duplicate of the same thing in FastMalloc.cpp
// then you need to edit FastMalloc.cpp to disable that version.
//
namespace WTF {

    void* fastMalloc(size_t n, int flags, const char* pName) 
    {
        void* p = EA::WebKit::GetAllocator()->Malloc(n, flags, pName);
#ifdef MALLOC_PURGE
        if((!p) && (n))
        {
            // 8/26/09 CSidhall - Added size check as some allocators might return NULL for a size 0.       
            // 7/9/09 CSidhall - We might be crashing here so do an emergency purge to try to free some mem 
            EA::WebKit::PurgeCache(true, true, true);
            p = EA::WebKit::GetAllocator()->Malloc(n, flags, pName);
        }
#endif
        return p;
    }

    void* fastMallocAligned(size_t n, size_t alignment, size_t offset, int flags, const char* pName) 
    {
        void* p= EA::WebKit::GetAllocator()->MallocAligned(n, alignment, offset, flags, pName);
#ifdef MALLOC_PURGE
        if((!p) && (n))
        {
            // 7/9/09 CSidhall - We might be crashing here so do an emergency purge to try to free some mem 
            EA::WebKit::PurgeCache(true, true, true);
            p= EA::WebKit::GetAllocator()->MallocAligned(n, alignment, offset, flags, pName);
        }
#endif
        return p;
    
    }

    // Currently FastMalloc.cpp/BCFastMalloc.cpp implements this for us via fastMalloc.
    // void* fastZeroedMalloc(size_t n)
    // {
    //     void* p = EA::WebKit::GetAllocator()->Malloc(n, 0, "EAWebKit/WTF");
    // 
    //     if(p)
    //         memset(p, 0, n);
    // 
    //     return p;
    // }

    void* fastCalloc(size_t count, size_t n, int flags, const char* pName)
    {
        void* p = EA::WebKit::GetAllocator()->Malloc(count * n, flags, pName);
#ifdef MALLOC_PURGE      
        if((!p) && (n*count))
        {
            // 7/9/09 CSidhall - We might be crashing here so do an emergency purge to try to free some mem 
            EA::WebKit::PurgeCache(true, true, true);
            p = EA::WebKit::GetAllocator()->Malloc(count * n, flags, pName);
        }
#endif
        if(p)
            memset(p, 0, count * n);

        return p;
    }

    void fastFree(void* p)
    {
        return EA::WebKit::GetAllocator()->Free(p, 0);
    }

    void* fastRealloc(void* p, size_t n, int flags)
    {
        return EA::WebKit::GetAllocator()->Realloc(p, n, flags);
    }

} // namespace WTF



// EAWebKit user-specified allocation ///////////////////////////////////////////////////////////////

namespace EA
{
namespace WebKit
{

void SetPackageAllocators()
{
    /////////////////////////////////////////////////////////////////////////
    // Setup open source package memory allocation.
    // These callback functions in turn will call the user-specified allocator.
    #if USE(CURL)
        curl_init_mem(curl_malloc_custom, curl_free_custom, curl_realloc_custom, curl_strdup_custom, curl_calloc_custom);
    #endif

    // Nothing to do for libjpeg (jpeg_get_small, etc.)
    png_set_default_mem_fn(png_malloc, png_free);

    #if USE(LIBXML2)
        xmlMemSetup(xmlFreeCustom, xmlMallocCustom, xmlReallocCustom, xmlStrdupCustom);
    #endif

    // Nothing to do for sdl (SDL_malloc, etc.).
    // Nothing to do for zlib (zcalloc, etc.).
    // Nothing to do for WTF/WebKit (fastMalloc, etc.)
    /////////////////////////////////////////////////////////////////////////
}

} // namespace WebKit
} // namespace EA





