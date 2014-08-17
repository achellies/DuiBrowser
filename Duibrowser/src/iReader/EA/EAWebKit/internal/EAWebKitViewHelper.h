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
// EAWebKitViewhelper.h
// By Chris Sidhall - 2010
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_EAWEBKITVIEWHELPER_H
#define EAWEBKIT_EAWEBKITVIEWHELPER_H

#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif

#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitConfig.h>
#include <EASTL/fixed_vector.h>
#include <EAWebKit/internal/EAWebKitAssert.h>

namespace EA
{
    namespace WebKit
    {

        // Auto active view class.
        // This helps setting an active view for notifications that don't have a view context.
        // This won't however work if our views are multithreaded.
        // Also used for example with fire timer callbacks where a callback is possibly for a
        // different view than the current view tick for example.
        class AutoSetActiveView
        {
            public:
	            AutoSetActiveView(View *pView)
                    :mpSavedView(spActiveView)
                {
                    spActiveView = pView;        
                }
            
	            ~AutoSetActiveView()
                {
                    spActiveView = mpSavedView;
                }    

                static View* GetActiveView(){ return spActiveView; }

            private:
                View*   mpSavedView;
				static View* spActiveView;
        };


    #define SET_AUTO_ACTIVE_VIEW(pView) EA::WebKit::AutoSetActiveView  autoSetActiveView(pView)    




    // View Array pointer singleton container class
    typedef eastl::fixed_vector<View*, 8> ViewPtrArray;

    class ViewArray
    {
    public:

        static ViewPtrArray& GetArray()
        {
            if(!mpViewArrayInstance)        
            {
                mpViewArrayInstance = new ViewArray;    
                EAW_ASSERT(mpViewArrayInstance);
            }
            return mpViewArrayInstance->mpViewPtrArray; 
        }

        static void staticFinalize()
        {
            if(mpViewArrayInstance)
            {
                delete mpViewArrayInstance; 
                mpViewArrayInstance = NULL;
            }
        }

    private:
        ViewArray(){}
        ~ViewArray() 
        {
            mpViewPtrArray.clear();    
        }
        
        ViewPtrArray mpViewPtrArray;
        static ViewArray* mpViewArrayInstance;
    };


    } // namespace 
} // namespace 

#endif // Header include guard
