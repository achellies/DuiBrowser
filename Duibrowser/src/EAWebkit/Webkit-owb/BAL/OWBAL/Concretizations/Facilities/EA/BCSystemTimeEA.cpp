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


/////////////////////////////////////////////////////////////////////////////
// BCSystemTimeEA.cpp
// Created by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "SystemTime.h"
#include <wtf/Platform.h>
#include <time.h>
#include <EABase/eabase.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitFPUPrecision.h>

#if defined(EA_PLATFORM_WINDOWS)
    #include <windows.h>
	#include <float.h>
#elif defined(EA_PLATFORM_WINCE)
    #include <windows.h>
	#include <float.h>
#endif

namespace EA
{
	namespace WebKit
	{
		extern EAWebKitTimerCallback gTimerCallback;
	}
}
namespace OWBAL {

    #if defined(_MSC_VER)
        // The gettimeofday function returns the current date and time in the struct timeval 
        // structure indicated by tp. Information about the time zone is returned in the 
        // structure pointed at tzp. If the tzp argument is a null pointer, time zone 
        // information is ignored.
        // The return value is 0 on success and -1 on failure. 

        struct timezone
        {
            int tz_minuteswest;     // This is the number of minutes west of GMT.
            int tz_dsttime;         // If nonzero, daylight savings time applies during some part of the year. 
        };

    #endif


    #if defined(EA_PLATFORM_XENON) || defined(EA_PLATFORM_PS3)
        // Return the current system time in seconds, using the classic POSIX epoch of January 1, 1970.
        double currentTime()
        {
			//If the application has installed a timer, use it. Otherwise, use the standard C time function.
			if(EA::WebKit::gTimerCallback)
				return EA::WebKit::gTimerCallback();

			EAW_ASSERT_MSG(false, "The current time resolution is in seconds. This means that some JavaScript that may require better resolution (like for animation effects) would not work correctly. An application may provide a higher resolution timer using the EAWebKit API.");

			return time(0);
        }

    #elif defined(EA_PLATFORM_UNIX)

        // Return the current system time in seconds, using the classic POSIX epoch of January 1, 1970.
        // Like time(0) from <time.h>, except with a wider range of values and higher precision.

        double currentTime()
        {
			//If the application has installed a timer, use it. Otherwise, use the standard C time function.
			if(EA::WebKit::gTimerCallback)
				return EA::WebKit::gTimerCallback();
			
			EAW_ASSERT_MSG(false, "The current time resolution is in seconds. This means that some JavaScript that may require better resolution (like for animation effects) would not work correctly. An application may provide a higher resolution timer using the EAWebKit API.");
			
			timeval  aTimeval;
            timezone aTimezone;
            double   dTime;

            gettimeofday(&aTimeval, &aTimezone);
            dTime = (double)aTimeval.tv_sec + (double)(aTimeval.tv_usec / 1000000.0);

            return dTime;
        }

    #elif defined(EA_PLATFORM_WINDOWS)
		double currentTime()
        {
			//If the application has installed a timer, use it. 
			if(EA::WebKit::gTimerCallback)
				return EA::WebKit::gTimerCallback();

			FILETIME      ft;
            LARGE_INTEGER li;
            double        dTime;

            GetSystemTimeAsFileTime(&ft);

            li.LowPart  = ft.dwLowDateTime;
            li.HighPart = ft.dwHighDateTime;

            // This function requires full double precision FPU math. Some applications reduce 
            // FPU precision in order to run faster. A prime example is DirectX applications. 
			SET_AUTOFPUPRECISION(EA::WebKit::kFPUPrecisionExtended);   

            dTime = (double)li.QuadPart;    // In 100-nanosecond intervals
            dTime /= 10000000.0;            // Convert to Seconds
            dTime -= 11644473600.0;         // Convert from January 1, 1601 to January 1, 1970

            // Make sure we don't have a time measurement bug.
            // If this assertion fails then most likely the 
            #ifdef EA_DEBUG 
                static double dLastTime = 0;
                EAW_ASSERT(dTime >= dLastTime);
                dLastTime = dTime;
            #endif

            return dTime;
        }
	#elif defined(EA_PLATFORM_WINCE)

		double currentTime()
        {
			//If the application has installed a timer, use it. 
			if(EA::WebKit::gTimerCallback)
				return EA::WebKit::gTimerCallback();

			FILETIME      ft;
            LARGE_INTEGER li;
            double        dTime;

			SYSTEMTIME systime = {0};
			GetSystemTime(&systime);
			SystemTimeToFileTime(&systime, &ft);

            li.LowPart  = ft.dwLowDateTime;
            li.HighPart = ft.dwHighDateTime;

            // This function requires full double precision FPU math. Some applications reduce 
            // FPU precision in order to run faster. A prime example is DirectX applications. 
			SET_AUTOFPUPRECISION(EA::WebKit::kFPUPrecisionExtended);   

            dTime = (double)li.QuadPart;    // In 100-nanosecond intervals
            dTime /= 10000000.0;            // Convert to Seconds
            dTime -= 11644473600.0;         // Convert from January 1, 1601 to January 1, 1970

            // Make sure we don't have a time measurement bug.
            // If this assertion fails then most likely the 
            #ifdef EA_DEBUG 
                static double dLastTime = 0;
                EAW_ASSERT(dTime >= dLastTime);
                dLastTime = dTime;
            #endif

            return dTime;
        }

    #endif // EA_PLATFORM_XXX
}
