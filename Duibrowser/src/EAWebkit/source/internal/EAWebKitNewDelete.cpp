/*
Copyright (C) 2010 Electronic Arts, Inc.  All rights reserved.

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
// EAWebKitNewDelete.cpp
// By Arpit Baldeva 
///////////////////////////////////////////////////////////////////////////////

//#include "config.h"                 // WebKit config.h file.
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitNewDelete.h>
#include <EAWebKit/internal/EAWebKitAssert.h>

#if EAWEBKIT_MEMORY_DEBUG
#include <stdio.h>
#endif

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

//TODO:Get rid of the (void) cast in following.
void* operator new(size_t size)
{
	//EAW_ASSERT_MSG( false, "Intercepted EAWebKit Anonymous new\n");
	
	return EA::WebKit::GetAllocator()->Malloc(size, 0, "EAWebKit Anonymous new");
	
}

void* operator new[](size_t size)
{
	//EAW_ASSERT_MSG( false, "Intercepted EAWebKit Anonymous new[]\n");
	
	return EA::WebKit::GetAllocator()->Malloc(size, 0, "EAWebKit Anonymous new[]");
	
}

void operator delete(void *p)
{
	//EAW_ASSERT_MSG( false, "Intercepted EAWebKit Anonymous delete\n");

	EA::WebKit::GetAllocator()->Free(p,0);
}

void operator delete[](void *p)
{
	//EAW_ASSERT_MSG( false, "Intercepted EAWebKit Anonymous delete[]\n");

	EA::WebKit::GetAllocator()->Free(p,0);
}

void* operator new(size_t size, const char* name, int flags, 
				   unsigned debugFlags, const char* file, int line)
{
	(void)name; (void)flags; (void)debugFlags; (void)file; (void)line;

#if EAWEBKIT_MEMORY_DEBUG
	char allocTag[256];
	snprintf(allocTag,sizeof(allocTag),"tag:%s - File:%s - Line:%d",name,file,line);
	return EA::WebKit::GetAllocator()->Malloc(size, flags, allocTag);
#else
	return EA::WebKit::GetAllocator()->Malloc(size, flags, name);
#endif
}

void* operator new(size_t size, size_t alignment, size_t alignmentOffset, const char* name, 
				   int flags, unsigned debugFlags, const char* file, int line)
{
	(void)alignmentOffset; (void)name; (void)flags; (void)debugFlags; (void)file; (void)line;

#if EAWEBKIT_MEMORY_DEBUG
	char allocTag[256];
	snprintf(allocTag,sizeof(allocTag),"tag:%s - File:%s - Line:%d",name,file,line);
	return EA::WebKit::GetAllocator()->MallocAligned(size, alignment, alignmentOffset, 0, allocTag);
#else
	return EA::WebKit::GetAllocator()->MallocAligned(size, alignment, alignmentOffset, 0, name);
#endif
}

/*
void operator delete(void *p, int flags)
{
	(void)flags;
	EA::WebKit::GetAllocator()->Free(p,0);
}
*/

void* operator new[](size_t size, const char* name, int flags, 
					 unsigned debugFlags, const char* file, int line)
{
	(void)name; (void)flags; (void)debugFlags; (void)file; (void)line;
	
#if EAWEBKIT_MEMORY_DEBUG
	char allocTag[256];
	snprintf(allocTag,sizeof(allocTag),"tag:%s - File:%s - Line:%d",name,file,line);
	return EA::WebKit::GetAllocator()->Malloc(size, flags, allocTag);
#else
	return EA::WebKit::GetAllocator()->Malloc(size, flags, name);
#endif

}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* name, 
					 int flags, unsigned debugFlags, const char* file, int line)
{
	(void)alignmentOffset;(void)name; (void)flags; (void)debugFlags; (void)file; (void)line;

#if EAWEBKIT_MEMORY_DEBUG
	char allocTag[256];
	snprintf(allocTag,sizeof(allocTag),"tag:%s - File:%s - Line:%d",name,file,line);
	return EA::WebKit::GetAllocator()->MallocAligned(size, alignment, alignmentOffset, 0, allocTag);
#else
	return EA::WebKit::GetAllocator()->MallocAligned(size, alignment, alignmentOffset, 0, name);
#endif

}
/*
//Placement new
void* operator new(size_t, void* p) 
{ 
	return p; 
}
void* operator new[](size_t, void* p) 
{ 
	return p; 
}
*/
#if 0 
void operator delete[](void *p, const char* /*name*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
	EA::WebKit::GetAllocator()->Free(p,0);
}

void operator delete[](void *p, size_t /*alignment*/, size_t /*alignmentOffset*/, const char* /*name*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
	EA::WebKit::GetAllocator()->Free(p,0);
}
#endif


