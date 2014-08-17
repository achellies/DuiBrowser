/*
Copyright (C) 2003,2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EAWebKitClipboard.cpp
//
// Written by Paul Pedriana, 4/2001; updated in 9/2004, 1/2009.
//
// Implements a generic clipboard system, which may be used as a traditional 
// GUI environment copy/paste clipboard but may also be used as a dynamic
// data sharing mechanism that is much more flexible than environment variables.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This is a sample optional clipboard implemetation for EAWebKit users.
///////////////////////////////////////////////////////////////////////////////


#include <EAWebKit/EAWebKitClipboard.h>
#include <EASTL/string.h>
#include <string.h>
#include <new>
#include <assert.h>

#ifdef EA_PLATFORM_WINDOWS
    #include <windows.h>
    #include <winuser.h>
#endif


#include <wtf/FastAllocBase.h>
#ifndef EACLIPBOARD_NEW
    #define EACLIPBOARD_NEW    new
    #define EACLIPBOARD_DELETE delete
#endif


namespace EA
{

namespace WebKit
{


static size_t Strlen16(const char16_t* pString)
{
    size_t nLength = (size_t)-1;
    
    do{
        ++nLength;
    }while(*pString++);
        
    return nLength;
}


size_t GetClipboardText(Clipboard* pClipboard, char16_t* pText16, size_t nMaxCount)
{
    IData* pData;
    size_t nReturnValue = 0;

    if(pClipboard->GetClipboardData(kDataTypeText16, &pData)) // AddRefs pData.
    {
        size_t nDataCount = (pData->GetSize() / sizeof(char16_t)); // nDataCount should be equal to strlen(pData->GetData()) + 1.

        if(nDataCount > nMaxCount)
            nDataCount = nMaxCount;

        if(pText16)
        {
            memcpy(pText16, pData->GetData(), nDataCount * sizeof(char16_t));
            pText16[nDataCount - 1] = 0; // Make sure the return value is null-terminated.
        }

        nReturnValue = nDataCount - 1;
        pData->Release(); // Matches AddRef from GetClipboardData
    }

    return nReturnValue;
}


size_t GetClipboardText(Clipboard* pClipboard, char8_t* pText8, size_t nMaxCount)
{
    IData* pData;
    size_t nReturnValue = 0;

    if(pClipboard->GetClipboardData(kDataTypeText8, &pData)) // AddRefs pData.
    {
        size_t nDataCount = (pData->GetSize() / sizeof(char8_t)); // nDataCount should be equal to strlen(pData->GetData()) + 1.

        if(nDataCount > nMaxCount)
            nDataCount = nMaxCount;

        if(pText8)
        {
            memcpy(pText8, pData->GetData(), nDataCount * sizeof(char8_t));
            pText8[nDataCount - 1] = 0; // Make sure the return value is null-terminated.
        }

        nReturnValue = nDataCount - 1;
        pData->Release(); // Matches AddRef from GetClipboardData
    }

    return nReturnValue;
}


bool SetClipboardText(Clipboard* pClipboard, const char16_t* pText16, size_t nStrlen, bool /*bClearExistingData*/)
{
    bool bReturnValue = false;
    DataText16* const pDataText16 = DataText16::CreateDataText16(pText16, nStrlen);

    if(pDataText16)
    {
        bReturnValue = pClipboard->SetClipboardData(pDataText16, true);
        pDataText16->Release();
    }

    return bReturnValue;
}


bool SetClipboardText(Clipboard* pClipboard, const char8_t* pText8, size_t nStrlen, bool /*bClearExistingData*/)
{
    bool bReturnValue = false;
    DataText8* const pDataText8 = DataText8::CreateDataText8(pText8, nStrlen);

    if(pDataText8)
    {
        bReturnValue = pClipboard->SetClipboardData(pDataText8, true);
        pDataText8->Release();
    }

    return bReturnValue;
}


///////////////////////////////////////////////////////////////////////////////
// DataBasic
///////////////////////////////////////////////////////////////////////////////


DataBasic::DataBasic(uint32_t nType)
  : mnType(nType), mnRefCount(0) {}


uint32_t DataBasic::AddRef()
{
    return ++mnRefCount;
}


uint32_t DataBasic::Release()
{
    if(mnRefCount > 1)
        return --mnRefCount;
    EAWEBKIT_DELETE this;//WTF::fastDeleteArray<uint8_t> (reinterpret_cast<uint8_t*>(this)); // Todo: switch to the standard memory free function.
    return 0;
}

uint32_t DataBasic::GetType() const
{
    return mnType;
}


///////////////////////////////////////////////////////////////////////////////
// DataRaw
///////////////////////////////////////////////////////////////////////////////


DataRaw::DataRaw(const void* pData, size_t nDataSize)
  : DataBasic(kDataTypeRaw), mnDataSize(nDataSize)
{
    if(pData)
        memcpy(mData, pData, nDataSize);
}

const void* DataRaw::GetData()
{
    return mData;
}

size_t DataRaw::GetSize()
{
    return mnDataSize;
}



///////////////////////////////////////////////////////////////////////////////
// DataText16
///////////////////////////////////////////////////////////////////////////////


DataText16::DataText16(const char16_t* pText16, size_t nStrlen)
  : DataRaw(NULL, 0)
{
    mnType = kDataTypeText16;

    if(pText16)
    {
        // Note that this code assumes the user has allocated an 
        // appropriately sized amount of space for this class.
        if(nStrlen == (size_t)-1)
            nStrlen = Strlen16(pText16);
        memcpy(mData, pText16, nStrlen * sizeof(char16_t));
    }
    else
        nStrlen = 0;

    ((char16_t*)mData)[nStrlen] = 0;
    mnDataSize = ((nStrlen + 1) * sizeof(char16_t));
}


DataText16* DataText16::CreateDataText16(const char16_t* pText16, size_t nStrlen)
{
    // To consider: Find a way to have the malloc below use some pool instead of 
    //              using the global heap.
    const size_t      nDataSize   = sizeof(DataText16) + ((nStrlen + 1) * sizeof(char16_t)) + sizeof(int); // Add sizeof int in order to account for unusual alignment issues.
    void*       const pMemory     = EAWEBKIT_NEW("datatext16") uint8_t[nDataSize];//WTF::fastNewArray<uint8_t> (nDataSize);
    DataText16* const pDataText16 = new(pMemory) DataText16(pText16, nStrlen);

    if(pDataText16)
        pDataText16->AddRef();
    return pDataText16;
}


DataText8::DataText8(const char8_t* pText8, size_t nStrlen)
  : DataRaw(NULL, 0)
{
    mnType = kDataTypeText8;

    if(pText8)
    {
        // Note that this code assumes the user has allocated an 
        // appropriately sized amount of space for this class.
        if(nStrlen == (size_t)-1)
            nStrlen = strlen(pText8);
        memcpy(mData, pText8, nStrlen * sizeof(char8_t));
    }
    else
        nStrlen = 0;

    mData[nStrlen] = 0;
    mnDataSize = ((nStrlen + 1) * sizeof(char8_t));
}


DataText8* DataText8::CreateDataText8(const char8_t* pText8, size_t nStrlen)
{
    // To consider: Find a way to have the malloc below use some pool instead of 
    //              using the global heap.
    const size_t     nDataSize  = sizeof(DataText8) + ((nStrlen + 1) * sizeof(char8_t)) + sizeof(int); // Add sizeof int in order to account for unusual alignment issues.
    void*      const pMemory    = EAWEBKIT_NEW("datatext8") uint8_t[nDataSize];//WTF::fastNewArray<uint8_t> (nDataSize);
    DataText8* const pDataText8 = new(pMemory) DataText8(pText8, nStrlen);

    if(pDataText8)
        pDataText8->AddRef();
    return pDataText8;
}




///////////////////////////////////////////////////////////////////////////////
// Win32Clipboard
//
// We implement a Win32 clipboard change detector so that the Win32 clipboard
// and our clipboard can be kept synchronized.
//
#ifdef EA_PLATFORM_WINDOWS
    struct Win32Clipboard
    {
        Win32Clipboard(Clipboard* pPrimaryClipboard);
       ~Win32Clipboard();

        bool ReadText(eastl::string16& string16);
        bool WriteText(const char16_t* pText16, size_t nStrlen);
        static LRESULT CALLBACK Win32Clipboard::WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        bool           mbEnabled;           // True if enabled.
        int            mnGettingText;       // Used as a message to ourselves to currently ignore clipboard requests to update the Win32 clipboard.
        int            mnSettingText;       // Used as a message to ourselves to currently ignore Win32 clipboard change messages.
        Clipboard*     mpPrimaryClipboard;  // When the Win32 clipboard changes, we modify the primary clipboard to reflect the change.
        HWND           mHWND;               // Our HWND.
        HWND           mNextHWND;           // Who we pass on HWND messages to. 
        DWORD          mnLocale;            // Win32 want's a CF_LOCALE item on the clipboard to accompany CF_TEXT data.
    };
#endif




///////////////////////////////////////////////////////////////////////////////
// Clipboard
//
Clipboard::Clipboard() :
    mbInitialized(false),
    mnId(kClipboardIdNone),
    mpOSData(NULL),
#ifdef  USE_EATHREAD_LIBRARY
    mMutex(),
#endif
    mnMaxTypes(1000),
    mbOSSync(true)
{
    memset(mDataInfoArray, 0, sizeof(mDataInfoArray));
    mDataInfoArray[kDataInfoArrayCount].mnType = kDataTypeNone;
    mDataInfoArray[kDataInfoArrayCount].mpData = NULL;
}



///////////////////////////////////////////////////////////////////////////////
// ~Clipboard
//
Clipboard::~Clipboard()
{
   Shutdown();
}



///////////////////////////////////////////////////////////////////////////////
// Init
//
bool Clipboard::Init()
{
#ifdef  USE_EATHREAD_LIBRARY
    Thread::AutoMutex autoMutex(mMutex);
#endif
    if(!mbInitialized)
    {
        mbInitialized = true;
        InitOSData();
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Shutdown
//
bool Clipboard::Shutdown()
{
#ifdef  USE_EATHREAD_LIBRARY
    Thread::AutoMutex autoMutex(mMutex);
#endif
    if(mbInitialized)
    {
      Clear(kDataTypeAll);
      ShutdownOSData();
      mbInitialized = false;
   }

   return true;
}



///////////////////////////////////////////////////////////////////////////////
// InitOSData
//
bool Clipboard::InitOSData()
{
#ifdef  USE_EATHREAD_LIBRARY
    // No mutex lock because this is an internal function and the lock should be done internally as needed before getting here.
    assert(mMutex.GetLockCount() > 0);
#endif

    #ifdef EA_PLATFORM_WINDOWS
        if(mbOSSync)
        {
            if(!mpOSData && (mnId == kClipboardIdPrimary)) // If we are the primary clipboard and if we haven't initialized os data yet...
            {
                Win32Clipboard* const pWin32Clipboard = EACLIPBOARD_NEW Win32Clipboard(this);
                mpOSData = pWin32Clipboard;
            }
        }
    #endif

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// ShutdownOSData
//
bool Clipboard::ShutdownOSData()
{
#ifdef  USE_EATHREAD_LIBRARY
    // No mutex lock because this is an internal function and the lock should be done internally as needed before getting here.
    assert(mMutex.GetLockCount() > 0);
#endif

    #ifdef EA_PLATFORM_WINDOWS
        if(mpOSData)
        {
            // We want to make sure our critical section is unlocked when calling out to this code.
            Win32Clipboard* pWin32Clipboard = reinterpret_cast<Win32Clipboard*>(mpOSData);
            EACLIPBOARD_DELETE pWin32Clipboard; // Todo: Change this to use tracked memory.
            mpOSData = NULL;
        }
    #endif

    return true;
}


///////////////////////////////////////////////////////////////////////////////
// GetID
//
uint32_t Clipboard::GetId() const
{
   return mnId;
}


///////////////////////////////////////////////////////////////////////////////
// SetId
//
void Clipboard::SetId(uint32_t nId)
{
#ifdef  USE_EATHREAD_LIBRARY    
    Thread::AutoMutex autoMutex(mMutex);
#endif
    assert((nId != (uint32_t)kClipboardIdNone) && (nId != (uint32_t)kClipboardIdAll));
    mnId = nId;

    if(mbInitialized)
    {
        if(nId == kClipboardIdPrimary)
            InitOSData();
        else
            ShutdownOSData(); //We call this even though it is perhaps already non-existant or shut down.
    }
}


///////////////////////////////////////////////////////////////////////////////
// SetOption
//
void Clipboard::SetOption(Option option, int value)
{
#ifdef  USE_EATHREAD_LIBRARY    
    Thread::AutoMutex autoMutex(mMutex);
#endif
    switch(option)
    {
        case kOptionMaxTypes:
            mnMaxTypes = value; // We allow values of <= 0.
            break;

        case kOptionOSSync:
            mbOSSync = (value != 0);
            if(mbOSSync)
                InitOSData();
            else
                ShutdownOSData();
            break;
            
        case kOptionNone:
        default:
            break;
    }
}


///////////////////////////////////////////////////////////////////////////////
// GetDataTypes
//
uint32_t Clipboard::GetDataTypes(uint32_t pDataTypeArray[], size_t nCount) const
{
#ifdef  USE_EATHREAD_LIBRARY
    Thread::AutoMutex autoMutex(mMutex);
#endif
    uint32_t i;

    // Recall that we have a permanent null entry at the end of mDataInfoArray, so we can't walk off it.
    for(i = 0; (i < nCount) && (mDataInfoArray[i].mnType != kDataTypeNone); ++i)
    {
        if(pDataTypeArray)
            pDataTypeArray[i] = mDataInfoArray[i].mnType;
    }

    return i;
}


///////////////////////////////////////////////////////////////////////////////
// GetClipboardData
//
bool Clipboard::GetClipboardData(uint32_t nDataType, IData** ppData)
{
#ifdef  USE_EATHREAD_LIBRARY    
    Thread::AutoMutex autoMutex(mMutex);
#endif
    // Recall that we have a permanent null entry at the end of mDataInfoArray, so we can't walk off it.
    for(size_t i = 0; mDataInfoArray[i].mnType != kDataTypeNone; ++i)
    {
        if(mDataInfoArray[i].mnType == nDataType)
        {
            if(ppData)
            {
                *ppData = mDataInfoArray[i].mpData;
                (*ppData)->AddRef();
            }
            return true;
        }
    }

    if(ppData)
        *ppData = NULL;
    return false;
}



///////////////////////////////////////////////////////////////////////////////
// GetClipboardDataData
//
const void* Clipboard::GetClipboardDataData(uint32_t nDataType, size_t* pSize)
{
#ifdef  USE_EATHREAD_LIBRARY    
    Thread::AutoMutex autoMutex(mMutex);
#endif
    // Recall that we have a permanent null entry at the end of mDataInfoArray, so we can't walk off it.
    for(size_t i = 0; mDataInfoArray[i].mnType != kDataTypeNone; ++i)
    {
        if(mDataInfoArray[i].mnType == nDataType)
        {
            if(pSize)
                *pSize = mDataInfoArray[i].mpData->GetSize();
            return mDataInfoArray[i].mpData->GetData();
        }
    }

    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
// SetClipboardData
//
bool Clipboard::SetClipboardData(IData* pData, bool bClearExistingData)
{
#ifdef  USE_EATHREAD_LIBRARY    
    mMutex.Lock();
#endif
    if(bClearExistingData)
        Clear(kDataTypeAll);

    if(pData)
    {
        pData->AddRef(); // AddRef it for our container.

        const uint32_t nDataType = pData->GetType();

        Clear(nDataType);
        
        // Recall that we have a permanent null entry at the end of mDataInfoArray, so we can't walk off it.
        size_t i;
        for(i = 0; i < kDataInfoArrayCount; ++i)
        {
            if(mDataInfoArray[i].mnType == kDataTypeNone) // If this is an empty slot...
            {
                mDataInfoArray[i].mnType = nDataType;
                mDataInfoArray[i].mpData = pData;
                break;
            }
        }

        if(i == kDataInfoArrayCount) // If there was no more room in the clipboard...
        {
            // We clear the first (oldest) entry and write this as the last entry.
            Clear(mDataInfoArray[0].mnType);
            mDataInfoArray[i - 1].mnType = nDataType;
            mDataInfoArray[i - 1].mpData = pData;
        }

        #ifdef EA_PLATFORM_WINDOWS
            if(mnId == kClipboardIdPrimary)
            {
                // As of now, the only type of data we share with the Win32 clipboard is text.
                // If we are pasting text, we also paste it into the Win32 clipboard. If we aren't
                // pasting text, we leave the Win32 clipboard alone. Perhaps we should be consistent
                // and clear the Win32 clipboard if we aren't pasting text.
                if(nDataType == kDataTypeText16)
                {
                    if(mpOSData)
                    {
#ifdef  USE_EATHREAD_LIBRARY
                        mMutex.Unlock(); // We don't want our critical section locked when calling out to external code.
#endif
                        Win32Clipboard* const pWin32Clipboard = reinterpret_cast<Win32Clipboard*>(mpOSData);
                        pWin32Clipboard->WriteText((char16_t*)pData->GetData(), (pData->GetSize() / sizeof(char16_t)) - 1); // '-1'  because the WriteText function expects the second argument to be the strlen of the string and thus not include the terminating null.
#ifdef  USE_EATHREAD_LIBRARY
                        mMutex.Lock();
#endif                    
                    }
                }
            }
        #endif
    }
#ifdef  USE_EATHREAD_LIBRARY
    mMutex.Unlock(); // On all platforms that we are aware of, this Unlock implements a memory barrier that guarantees other threads correctly see our memory and the memory of the clipboard data.
#endif
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Clear
//
size_t Clipboard::Clear(uint32_t nDataType)
{
#ifdef  USE_EATHREAD_LIBRARY    
    Thread::AutoMutex autoMutex(mMutex);
#endif

    size_t nCount;

    if(nDataType == kDataTypeAll)
    {
        // Recall that we have a permanent null entry at the end of mDataInfoArray, so we can't walk off it.
        for(nCount = 0; mDataInfoArray[nCount].mnType != kDataTypeNone; ++nCount)
        {
            mDataInfoArray[nCount].mnType = kDataTypeNone;
            mDataInfoArray[nCount].mpData->Release();
            mDataInfoArray[nCount].mpData = NULL;
        }
    }
    else
    {
        nCount = 0;

        // Recall that we have a permanent null entry at the end of mDataInfoArray, so we can't walk off it.
        for(size_t i = 0; mDataInfoArray[i].mnType != kDataTypeNone; ++i)
        {
            if(mDataInfoArray[i].mnType == nDataType)
            {
                // Release this entry
                mDataInfoArray[i].mpData->Release();
                nCount++;

                // Now move down all successive entries.
                for(++i; mDataInfoArray[i - 1].mnType != kDataTypeNone; ++i)
                {
                    mDataInfoArray[i - 1].mnType = mDataInfoArray[i].mnType;
                    mDataInfoArray[i - 1].mpData = mDataInfoArray[i].mpData;
                }

                break;
            }
        }
    }

    return nCount;
}


namespace
{
    //////////////////////////////////////////////////////////////////////////
    /// Global pointer to clipboard singleton.
    /// This used to be an OSGlobal, but we are 
    Clipboard* gpClipboard = NULL;
}

// Get the global clipboard singleton.
// This function uses global operator new, so you shouldn't allow it to auto-create
// if you want to avoid global operator new. You can always manually set the clipboard.
Clipboard* GetClipboard(bool bAutoCreate)
{
    if(!gpClipboard && bAutoCreate)
        SetClipboard(new Clipboard);

    return gpClipboard;
}

// Set the global clipboard singleton.
Clipboard* SetClipboard(Clipboard* pClipboard)
{
    Clipboard* const pOldClipboard = gpClipboard;
    gpClipboard = pClipboard;

    return pOldClipboard;
}



#ifdef EA_PLATFORM_WINDOWS


///////////////////////////////////////////////////////////////////////////////
// Win32Clipboard
//
Win32Clipboard::Win32Clipboard(Clipboard* pPrimaryClipboard)
  : mbEnabled(true),
    mnGettingText(0),
    mnSettingText(0),
    mpPrimaryClipboard(pPrimaryClipboard),
    mHWND(NULL),
    mNextHWND(NULL),
    mnLocale(0x0409) // This is the value for US English. We'll need to deal with this in a better way for non-English.
{
    const DWORD dwVersion(::GetVersion());

    if(dwVersion >= 0x80000000) // If running under the Win95/Win98/WinME family...
    {
        // Unfortunately, Win95 usually crashes if something is placed on the
        // clipboard while a clipboard-using app is paused on a breakpoint
        // within a debugger. This is perhaps due to the 'Win16 mutex'.
        // So we disable the clipboard in debug builds when running under the Win95 family.
        #ifdef EA_DEBUG
            mbEnabled = false;
        #endif
    }

    if(mbEnabled)
    {
        // We create an HWND and listen for WM_DRAWCLIPBOARD.
        WNDCLASS    wndClass;
        const TCHAR* pClassName  = TEXT("EAClipboardClass");
        const TCHAR* pWindowName = TEXT("EAClipboard");

        memset(&wndClass, 0, sizeof(wndClass));
        wndClass.style          = 0;
        wndClass.lpfnWndProc    = WindowProcedure;
        wndClass.cbClsExtra     = 0;
        wndClass.cbWndExtra     = sizeof(Win32Clipboard*);
        wndClass.hInstance      = ::GetModuleHandle(NULL);
        wndClass.hIcon          = NULL;
        wndClass.hCursor        = NULL;
        wndClass.hbrBackground  = NULL;
        wndClass.lpszMenuName   = NULL;
        wndClass.lpszClassName  = pClassName;
        ::RegisterClass(&wndClass);

        mHWND = ::CreateWindow(pClassName, pWindowName, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, ::GetModuleHandle(NULL), NULL);
        if(mHWND)
        {
            #ifdef _WIN64
                ::SetWindowLongPtr(mHWND, GWLP_USERDATA, (LONG_PTR)(uintptr_t)static_cast<Win32Clipboard*>(this));
            #else
                ::SetWindowLongPtr(mHWND, GWLP_USERDATA, (LONG)    (uintptr_t)static_cast<Win32Clipboard*>(this));
            #endif
            mNextHWND = ::SetClipboardViewer(mHWND); //Make it so that we get clipboard change notifications.
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// ~Win32Clipboard
//
Win32Clipboard::~Win32Clipboard()
{
    if(mHWND)
    {
        ::ChangeClipboardChain(mHWND, mNextHWND);   // Remove ourselves from change notification list.
        ::DestroyWindow(mHWND);                     // Destroy our HWND.
    }
}


///////////////////////////////////////////////////////////////////////////////
// ReadText
//
bool Win32Clipboard::ReadText(eastl::string16& string16)
{
    // Question: Do we need to lock the Clipboard mutex here?

    bool bReturnValue(false);

    string16.clear();

    if(mbEnabled)
    {
        if(!mnSettingText) // Prevents re-entry of various types.
        {
            mnGettingText++;

            if(::OpenClipboard((HWND)NULL))
            {
                // As of 2004, we support Unicode text only. If the clipboard doesn't have Unicode
                // text, we don't read it. It turns out that WinNT+ converts non-Unicode to Unicode
                // whenever it is placed on the clipboard auotmatically.
                if(::IsClipboardFormatAvailable(CF_UNICODETEXT))
                {
                    // From the Win32 SDK documentation:
                    // The clipboard controls the handle that the GetClipboardData function returns,
                    // not the application. The application should copy the data immediately.
                    // The application cannot rely on being able to make long-term use of the handle.
                    // The application must not free the handle nor leave it locked.
                    HANDLE hGlobal = ::GetClipboardData(CF_UNICODETEXT);

                    if(hGlobal)
                    {
                        const char16_t* pText16 = (char16_t*)::GlobalLock(hGlobal);

                        if(pText16)
                        {
                            const size_t nStringLength = (size_t)(GlobalSize(hGlobal) / sizeof(char16_t));
                            string16.reserve((eastl_size_t)nStringLength);

                            for(const char16_t* const pTextEnd = pText16 + nStringLength; (pText16 < pTextEnd) && *pText16; ++pText16)
                                string16 += *pText16;

                            ::GlobalUnlock(hGlobal);

                            bReturnValue = true;
                        }
                    }
                }

                ::CloseClipboard();
            }
            mnGettingText--;
        }
    }

    return bReturnValue;
}




///////////////////////////////////////////////////////////////////////////////
// WriteText
//
bool Win32Clipboard::WriteText(const char16_t* pText16, size_t nStrlen)
{
    // Question: Do we need to lock the Clipboard mutex here?

    bool bReturnValue(false);

    if(mbEnabled)
    {
        if(!mnGettingText)
        {
            mnSettingText++;

            if(::OpenClipboard((HWND)NULL))
            {
                ::EmptyClipboard(); // Clear what is currently in the clipboard.

                // As of 2004, we write only Unicode text to the clipboard.
                const size_t nAllocationSize = (nStrlen + 1) * sizeof(char16_t); // '+ 1' to include the terminating null.
                HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, nAllocationSize);
                if(hGlobal)
                {
                    char16_t* const pText16Win32 = (char16_t*)::GlobalLock(hGlobal);

                    if(pText16Win32)
                    {
                        ::memcpy(pText16Win32, pText16, nStrlen * sizeof(char16_t));
                        pText16Win32[nStrlen] = 0;

                        ::GlobalUnlock(hGlobal);

                        HANDLE const hResult = ::SetClipboardData(CF_UNICODETEXT, hGlobal);
                        if(hResult)
                            bReturnValue = true;
                        else
                            ::GlobalFree(hGlobal); // I cannot tell from the Win32 API documentation how the ownership of the handle is managed. The implication is that if the SetClipboardData function succeeds, then the OS permanently owns the handle.
                    }
                }

                ::CloseClipboard();
            }
            mnSettingText--;
        }
    }
    return bReturnValue;
}



///////////////////////////////////////////////////////////////////////////////
// WindowProcedure
//
// Let me (Paul Pedriana) just say here that the way Win32 clipboard
// notifications work is one of the worst implemented, scariest designs
// I have had the misfortune of working with for quite some time.
// The system is a house of cards that can be blown over way too easily.
//
LRESULT CALLBACK Win32Clipboard::WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG_PTR        const pThisLong = ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    Win32Clipboard* const pThis     = reinterpret_cast<Win32Clipboard*>(pThisLong);

    if(pThis && pThis->mbEnabled)
    {
        if(uMsg == WM_CHANGECBCHAIN) // For this message, wParam is the handle to window to be removed and lParam is the handle to the window after the one being removed.
        {
            if((HWND)(uintptr_t)wParam == (HWND)(uintptr_t)pThis->mNextHWND)
                pThis->mNextHWND = (HWND)(intptr_t)lParam;
            else if(pThis->mNextHWND)
                ::SendMessageA(pThis->mNextHWND, uMsg, wParam, lParam);
            return 0;
        }
        else if(uMsg == WM_DRAWCLIPBOARD)  // For this message, we simply need to pass it on to the next window in the list.
        {
            eastl::string16 string16;

            if(pThis->mpPrimaryClipboard && pThis->ReadText(string16))
            {
                pThis->mnGettingText++; // We set this flag so that we don't get recursion problems.
                SetClipboardText(pThis->mpPrimaryClipboard, string16.data(), string16.length(), true);
                pThis->mnGettingText--;
            }
            if(pThis->mNextHWND)
                ::SendMessageA(pThis->mNextHWND, uMsg, wParam, lParam);
            return 0;
        }
    }

    return (long)::DefWindowProc(hWnd, uMsg, wParam, lParam);
}



#endif // EA_PLATFORM_WINDOWS



} // namespace Clipboard

} // namespace EA



