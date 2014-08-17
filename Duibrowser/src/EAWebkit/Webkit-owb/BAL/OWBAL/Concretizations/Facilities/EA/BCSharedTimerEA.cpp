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
// BCSharedTimerEA.cpp
// Created by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "SharedTimer.h"
#include "SystemTime.h"
#include <EAWebKit/internal/EAWebKitAssert.h>  
#include <stdio.h>
#include <string.h>
#include "Platform.h"


namespace OWBAL {


void (*gSharedTimerFiredFunction)() = NULL;

// We need a timer list here. 
size_t       gFireTimeCount = 0;        // Number of valid elements in gFireTimeArray.
const size_t kFireTimeCapacity = 64;    // To do: Make this a dynamic struct (e.g. Vector) instead of fixed-size.


double       gFireTimeArray[kFireTimeCapacity];
double       gLastCheckTime = 0;
double       gLastFireTime = 0;

static double sFireTimerRate = 1.0/60.0;   // 60Hz as default       


void setSharedTimerFiredFunction(void (*f)()) 
{
    gSharedTimerFiredFunction = f;
}


void setFireTimerRate(EA::WebKit::FireTimerRate rate)
{
    double adjustedRate;

    switch (rate)
    {
        case EA::WebKit::kFireTimerRate120Hz:
            adjustedRate = 1.0/120.0;     
            break;
        
        case EA::WebKit::kFireTimerRate60Hz:
            adjustedRate = 1.0/60.0; 
            break;
        
        case EA::WebKit::kFireTimerRate30Hz:
            adjustedRate = 1.0/30.0;         
            break;

        case EA::WebKit::kFireTimerRate20Hz:
            adjustedRate = 1.0/20.0; 
            break;

        case EA::WebKit::kFireTimerRate15Hz:
            adjustedRate = 1.0/15.0; 
            break;

        default:        
            EAW_ASSERT_MSG(false, "Invalid rate. Using default 60 Hz");  // Invalid rate
            adjustedRate = 1.0/60.0;         
            break;
    }

    sFireTimerRate = adjustedRate;
}

double GetFireTimerRate()
{
    return sFireTimerRate;
}




// The fire time is relative to the classic POSIX epoch of January 1, 1970,
// as the result of currentTime() is.
void setSharedTimerFireTime(double fireTime)
{
     size_t fireCount = gFireTimeCount;
    
    //+ 1/20/10 CSidhall - This +3 seems mostly a safety to prevent the timer list from overflow.
    // A bad javascript might fill the timer array with too many far distant timers.
    // It is however somewhat pointless and wrong to set a distant trigger to +3 seconds if the real trigger is at +900
    // seconds and won't actually fire even if called at +3 seconds.
    // But this might be ok for now to keep as safety for eventually a future timer will trigger at some point
    // the original timer request. So will keep it for now for only if our array is starting to fill up.
    // Old code:    
    //      if(fireTime > (currentTime + 3))  // Note by Paul Pedriana: I don't know if we really want to do this.
    //        fireTime = (currentTime + 3);  // We need to find out more about how timers internally expect to work.
    // New code: 
    static const size_t kFireTimeOverflow = kFireTimeCapacity/4;    // Any small size will do really. We just want to keep some empty slots around
    if(fireCount > kFireTimeOverflow)
    {
        const double currentTime = OWBAL::currentTime();
        if(fireTime > (currentTime + 3))  // Note by Paul Pedriana: I don't know if we really want to do this.
            fireTime = (currentTime + 3);  // We need to find out more about how timers internally expect to work.
    }

    // Added removal of duplicate time triggers as not having it can overflow the array.
     for(size_t i = 0; i < fireCount; ++i)
     {
            double time = gFireTimeArray[i];
            // Ignore if we have one already 
            if(time == fireTime)
                return;
     }
    //-CS

    EAW_ASSERT_MSG(gFireTimeCount < kFireTimeCapacity, "Fire timers exceed the capacity");
    if(gFireTimeCount < kFireTimeCapacity)
    {
        // To consider: Maintain this list in sorted order. We aren't dependent on it being so, but it might allow us to be a little more efficient.
        gFireTimeArray[gFireTimeCount++] = fireTime;
    }
}


void stopSharedTimer()
{
    gFireTimeCount = 0;
}


void fireTimerIfNeeded()
{
    const static double kFireTimerSafetyDelay = 15;  // in seconds. Safety to fire timer anyway just in case... 
    const double currentTime = OWBAL::currentTime();

    //Note by Arpit Baldeva - 0.033333f means 30 frames per second. Originally it was 0.10.
	if(currentTime >= (gLastCheckTime + sFireTimerRate)) // We don't want to have the possibility of firing very soon, as some JavaScript appears to be dependent on a minimum time between ticks.
    {
        gLastCheckTime = currentTime;

        //+ 1/20/10 CSidhall - Added collection of all expired timers and shrinking of the fire timer array.
        // The old code exited after the first timer was found and removed but left possible expired timers.
        // This could cause empty fire triggers and also risk to overflow of the timer array.
        int foundCount =0;
        size_t fireCount = gFireTimeCount;    
        for(size_t i = 0; i < fireCount; ++i)
        {
            double fireTime = gFireTimeArray[i];
            if(currentTime >= fireTime)
                foundCount++;        
            else if(foundCount)
                gFireTimeArray[i-foundCount] = fireTime;  
        }
        
        // Trigger the timer function if we found a trigger
        if(foundCount)
        {
            gFireTimeCount -=foundCount;
            gLastFireTime = currentTime;
            gSharedTimerFiredFunction();
        }

    }
    else if( gLastFireTime && (currentTime > (gLastFireTime + kFireTimerSafetyDelay)) )
    {
        // Safety to trigger the fire timer anyway if we have no recent triggers 
        // In theory, should not be needed and no timer triggers should be found but does not really hurt much to have.
        gLastFireTime = currentTime;
        gSharedTimerFiredFunction();
    }

        // Old code for ref:
/*
        for(size_t i = 0; i < gFireTimeCount; ++i)
        {
            double fireTime = gFireTimeArray[i];

            if(currentTime >= fireTime)
            {
                //Note by Arpit Baldeva: Replace memcpy with memmove as the source and the destination memory area overlap. I have seen memcpy crash
				//on consoles in such cases on some previous projects I have worked on.
				//http://www.opengroup.org/onlinepubs/000095399/functions/memmove.html
				memmove(gFireTimeArray + i, gFireTimeArray + i + 1, (gFireTimeCount - (i + 1)) * sizeof(double));
                gFireTimeCount--;
                gLastFireTime = currentTime;
                gSharedTimerFiredFunction();
                return;
            }
        }

        // It's not clear if we need this fallback code or not. This was added because there seemed 
        // to be a case whereby the code expected to be fired but the next timer time was far in the future. 
        if(gFireTimeCount && (currentTime > (gLastFireTime + 5))) // If we haven't fired recently, do so anyway.
        {
			//Note by Arpit Baldeva: Replace memcpy with memmove as the source and the destination memory area overlap. I have seen memcpy crash
			//on consoles in such cases on some previous projects I have worked on.
			//http://www.opengroup.org/onlinepubs/000095399/functions/memmove.html
			memmove(gFireTimeArray, gFireTimeArray + 1, (gFireTimeCount - 1) * sizeof(double)); // Remove first item. Ideally we'd remove the soonest item.
            gFireTimeCount--;
            gLastFireTime = currentTime;
            gSharedTimerFiredFunction();
        }
    }
*/
}


} // namespace OWBAL
