/*
Copyright (C) 2004,2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EATextLayout.cpp
//
// By Paul Pedriana - 2004
//
///////////////////////////////////////////////////////////////////////////////



#include <EAText/EATextLayout.h>
#include <EAText/EATextFont.h>
#include <EAText/EATextBreak.h>
#include <EAText/EATextUnicode.h>
#include EA_ASSERT_HEADER
#if EATEXT_THREAD_SAFETY_ENABLED
    #include <eathread/eathread_futex.h>
#endif
#include <math.h>


#ifdef _MSC_VER // Enable highest warning for this file alone under MSVC.
    #pragma warning(push, 4)
    #pragma warning(disable: 4100) // unreferenced formal parameter. To do: Remove this asap.
    #pragma warning(disable: 4127) // conditional expression is constant.
#endif




namespace EA
{

namespace Text
{

///////////////////////////////////////////////////////////////////////////////
// Layout
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Layout
//
Layout::Layout()
  : mLayoutSettings()
{
    // Empty
}


///////////////////////////////////////////////////////////////////////////////
// ~Layout
//
Layout::~Layout()
{
    // Empty
}

} // namespace Text

} // namespace EA















