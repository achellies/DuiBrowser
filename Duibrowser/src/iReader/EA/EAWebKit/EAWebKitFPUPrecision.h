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
// EAWebKitFPUPrecision.h
// By Arpit Baldeva - 2010
// Part of the implementation/comments are borrowed from Paul Pedriana/other EA packages.
///////////////////////////////////////////////////////////////////////////////

/*
EAWebKit needs to operate in at least double precision mode. Some applications(like games/DirectX) reduce the FPU precision for efficiency 
reasons. For this reason, we wrap the EAWebKit user API with extended precision using this helper class. This class follows RAII pattern.
*/

#include <EABase/eabase.h>

namespace EA
{
	namespace WebKit
	{
		/// FPUPrecision
		///
		/// Defines floating point precision types. 
		/// Some floating point processors (e.g. x87) execute in either single 
		/// precision or double precision and the user specifies which one is 
		/// used at runtime. Other processors (e.g. PowerPC) have two sets of
		/// FPU instructions, one for single precision and one for double 
		/// precision and the user specifies at compile-time which to use.
		///
		enum FPUPrecision
		{
			kFPUPrecisionSingle,    /// Generally 32 bit. This can often make divisions in particular go faster (~2x faster or better).
			kFPUPrecisionDouble,    /// Generally 53 or 64 bits, depending on the FPU type.
			kFPUPrecisionExtended   /// Generally 64 or 80 bits, depending on the FPU type.
		};
		class AutoFPUPrecision
		{
		public:
			AutoFPUPrecision(FPUPrecision precisionDesired);
			~AutoFPUPrecision();
		private:
			unsigned int mSavedControlWord;
		};
	}
}

//We use SET_AUTOFPUPRECISION macro so that we don't pay for object creation and destruction on PS3 and Xbox 360.
#if defined(EA_PLATFORM_WINDOWS)
#define SET_AUTOFPUPRECISION(fpuPrecision) EA::WebKit::AutoFPUPrecision autoFpuPrecision(fpuPrecision)
#else
#define SET_AUTOFPUPRECISION(fpuPrecision) 
#endif