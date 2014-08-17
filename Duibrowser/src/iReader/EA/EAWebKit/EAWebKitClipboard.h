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

/////////////////////////////////////////////////////////////////////////////
// EAClipboard.h
// Created by Paul Pedriana - 2003
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// This class implements the clipboard pattern plus an implementation of 
// a OS-savvy clipboard for desktop platforms (e.g. Windows clipboard).
//
// The clipboard paradigm is also known as the bulletin board pattern and
// refers to the model of a persistent messaging system of arbitrary 
// messages. Somebody posts something to the clipboard and it stays there
// indefinitely.
/////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EACLIPBOARD_H
#define EAWEBKIT_EACLIPBOARD_H


#include <EABase/eabase.h>
#include <EAWebKit/EAWebKitConfig.h>
#ifdef  USE_EATHREAD_LIBRARY
    #include <eathread/eathread_mutex.h>
#endif

namespace EA
{
    namespace WebKit
    {
        // Constants
        const uint32_t kDataTypeNone        = 0x00000000;    ///
        const uint32_t kDataTypeAll         = 0xffffffff;    ///
        const uint32_t kDataTypeRaw         = 0x00000001;    /// Simply a raw block of data.
        const uint32_t kDataTypeText8       = 0x00000002;    /// UTF8 text. The data is a pointer to 0-terminated char8_t (and not to a string class).
        const uint32_t kDataTypeText16      = 0x00000003;    /// UTF16 text. The data is a pointer to 0-terminated char16_t (and not to a string class).

        const uint32_t kClipboardIdNone     = 0x00000000;
        const uint32_t kClipboardIdAll      = 0xffffffff;
        const uint32_t kClipboardIdPrimary  = 0x00000001;

        /// A global ID for clipboards, used with OSGlobal to declare a global clipboard
        const uint32_t kGlobalID_Clipboard  = 0xcffba579;


        /// IData
        ///
        /// Manages data for a single clipboard.
        /// You would normally make a subclass of this class and provide implementations of 
        /// the member functions. The functions are virtual so that there can be a standard 
        /// interface for working with different types of data.
        /// 
        /// Thread safety:
        /// IData is an interface class and thread safety needs to be implemented by its
        /// concrete subclasses. By default, items posted to the clipboard are read-only
        /// items and users need not make their IData subclasses thread-safe. The Clipboard
        /// class takes care of making sure the systems' view of memory is consistent after
        /// data is put on the clipboard.
        ///
        /// MemoryAllocation:
        /// IData is an interface class and memory allocation behviour is entirely dependent
        /// on the implementation of the concrete IData subclasses. The reference counting
        /// mechanism allows for the control of the memory allocation and lifetime.
        ///
        class IData
        {
        public:
            /// ~IData
            /// Virtual destructor.
            virtual ~IData() { }

            /// AddRef
            /// Increments the reference count and returns the new reference count.
            virtual uint32_t AddRef() = 0;

            /// Release
            /// Decrements the reference count and returns the new reference count.
            /// A return value of zero means the object has been destroyed.
            virtual uint32_t Release() = 0;

            /// GetType
            /// Returns the data type for this clipboard data (e.g. kDataTypeText16).
            virtual uint32_t GetType() const = 0;

            /// GetData
            /// Returns the data itself. This function is not a const function, as it 
            /// is possible the implementation may provide delayed data rendering.
            virtual const void* GetData() = 0;

            /// GetSize
            /// Returns the size of the data. This function is not a const function, as it 
            /// is possible the implementation may provide delayed data rendering. For text
            /// types such as kDataTypeText16, the size includes the terminating zero.
            virtual size_t GetSize() = 0;
        };



        /// Clipboard
        ///
        /// Implements a single clipboard. A clipboard may have multiple Data attached to it.
        /// As of this implementation, there isn't a way to register for callback notification
        /// upon clipboard change. A previous incarnation of this class had such functionality
        /// but it was never used.
        /// 
        /// Thread safety:
        /// The Clipboard class is thread-safe internally. You can use any of its APIs from 
        /// multiple threads concurrently. Additionally, the Clipboard class implements a 
        /// read/write memory barrier whenever data is posted to the clipboard in order that
        /// the image of data is correctly seen other threads.
        ///
        /// Memory allocation:
        /// The clipboard class allocates no memory except for the Win32 platform when it 
        /// interacts with the OS clipboard, in which case it necessarily allocates some
        /// temporary memory due to the design of the Windows clipboard system.
        ///
        class Clipboard
        {
        public:
            enum Option
            {
                kOptionNone,
                kOptionMaxTypes,    /// Defaults to 1000.     Sets the max number of concurrent data objects on the clipboard.
                kOptionOSSync,      /// Defaults to 1 (true). If true, then the clipboard is synchronized with the OS clipboard (if there is an OS clipboard).
            };

            /// Clipboard
            Clipboard();
            Clipboard(const Clipboard&);

            /// ~Clipboard
            ~Clipboard();

            /// operator=
            Clipboard& operator=(const Clipboard&);

            /// Init
            bool Init();

            /// Shutdown
            bool Shutdown();

            /// GetId
            /// Returns the clipboard Id, not to be confused with Data type id.
            uint32_t GetId() const;

            /// SetId
            /// Sets the clipboard Id to the given value. The value needs to be unique.
            void SetId(uint32_t nId);

            /// SetOption
            /// Sets options for the Clipboard.
            void SetOption(Option option, int value);

            /// GetDataTypes
            /// Gives the user the available data types. Passing in NULL for the 
            /// array causes function to return the available count. The input
            /// nCount is the number of entries the data type array can hold.
            /// If there are more data types available than the array can hold,
            /// some items will be left out of the returned array.
            uint32_t GetDataTypes(uint32_t pDataTypeArray[], size_t nCount) const;
 
            /// GetClipboardData
            /// Gives the user an AddRef'd Data pointer. AddRef is necessary because
            /// otherwise in a multithreaded environment the data could be destroyed
            /// before the user is done working with it. You must call Release on the
            /// returned pointer or else a memory leak will occur. The return value of
            /// the function is true if the data type existed and *ppData was set.
            /// Otherwise the return value is false and *ppData is set to NULL.
            /// If the ppData parameter is NULL, this function simply indicates if the 
            /// given data is present.
            bool GetClipboardData(uint32_t nDataType, IData** ppData = NULL);

            /// GetClipboardDataData
            /// Returns the low level raw data of the clipboard, as opposed to the 
            /// GetClipboardData function, which returns the object which holds the 
            /// the raw low level data. For example, with kDataTypeText16 the return
            /// value of this function is the null-terminated char16_t string.
            /// If the output paramater pSize is non-NULL, then upon return it will
            /// hold the size of the raw data. The returned pointer doesn't point to 
            /// AddRef'd data, so this function is in that sense not thread-safe.
            const void* GetClipboardDataData(uint32_t nDataType, size_t* pSize = NULL);

            /// SetClipboardData
            /// Sets (adds) the given clipboard data. If data of the given type is 
            /// already present, this function releases the old instance and adds 
            /// this instance in its place. If there is a limit to the number of 
            /// data types that can be attached to a clipboard, this function 
            /// causes the removal of old data types. The input pData will be 
            /// AddRef'd for internal usage and it will be Released when the 
            /// pData is later removed from the clipboard. The return value is 
            /// true if pData could be added to the clipboard; upon which  
            /// pData will be AddRef'd.
            bool SetClipboardData(IData* pData, bool bClearExistingData = true);

            /// Clear
            /// Removes just the given data type or can clear all present data types.
            /// The removal does a Release on the Data. Returns the number of 
            /// clipboard Data that was removed.
            size_t Clear(uint32_t nDataType = kDataTypeAll);

        protected:
            bool InitOSData();
            bool ShutdownOSData();

            struct DataInfo
            {
                uint32_t mnType;
                IData*   mpData;
            };

            enum { kDataInfoArrayCount = 32 };

            bool                  mbInitialized;                           /// True if we have been initialized.
            uint32_t              mnId;                                    /// Our unique ID. May be kPrimaryClipboard.
            void*                 mpOSData;                                /// Pointer to any OS-specific clipboard info we might want to associate with.
            DataInfo              mDataInfoArray[kDataInfoArrayCount + 1]; /// Array of clipboard data. There is a limit to the number of concurrent data types present. '+1' because we place a permanent null entry at the end.
#ifdef USE_EATHREAD_LIBRARY            
            mutable Thread::Mutex mMutex;                                  /// Allows thread safety.
#endif            
            int                   mnMaxTypes;                              /// 
            bool                  mbOSSync;                                /// 
        };

        ///////////////////////////////////////////////////////////////////////
        // Global clipboard singleton
        ///////////////////////////////////////////////////////////////////////

        /// Get the global clipboard singleton
        /// This function uses global operator new, so you shouldn't allow it to auto-create
        /// if you want to avoid global operator new. You can always manually set the clipboard.
        Clipboard* GetClipboard(bool bAutoCreate = false);

        /// Set the global clipboard singleton
        Clipboard* SetClipboard(Clipboard* pClipboard);


        ///////////////////////////////////////////////////////////////////////
        // Helper functionality
        ///////////////////////////////////////////////////////////////////////


        /// GetClipboardText
        ///
        /// Copies clipboard text to the user-supplied data pointer. The copied text 
        /// is null-terminated. The input nMaxCount is the entry count of the input pText16.
        /// The return value is the strlen of the returned text. Note that this convention
        /// of using strlen is different from the raw data functions of the IData and 
        /// Clipboard interfaces, which return the true full data length which includes
        /// the terminating null character.
        /// If you want the clipboard text in a format other than UTF16, you'll have
        /// to do conversions yourself or go through the Clipboard interface directly.
        /// The input pClipboard must be non-NULL.
        ///
        size_t GetClipboardText(Clipboard* pClipboard, char16_t* pText16, size_t nMaxCount);
        size_t GetClipboardText(Clipboard* pClipboard, char8_t*  pText8,  size_t nMaxCount);

        /// SetClipboardText
        ///
        /// Sets the clipboard text to the input text. The text is copied from the 
        /// input pointer, so the input pointer need not remain valid in the future.
        /// If the input nStrlen is -1, then the strlen is determined within the function.
        /// If the input nStrlen is less than the actual strlen of the string, the 
        /// input nStrlen is obeyed.
        /// The input pClipboard must be non-NULL.
        ///
        bool SetClipboardText(Clipboard* pClipboard, const char16_t* pText16, size_t nStrlen = (size_t)-1, bool bClearExistingData = true);
        bool SetClipboardText(Clipboard* pClipboard, const char8_t*  pText8,  size_t nStrlen = (size_t)-1, bool bClearExistingData = true);


        /// DataBasic
        ///
        /// A default implementation of a basic Data class. This implementation provides
        /// a standard non-thread-safe implementation of reference counting. Additionally,
        /// this version's Release function uses a standard memory free function, whereas
        /// you might instead want to use a special allocator for Data.
        /// Users of this class need to subclass it and provide the GetData and GetDataSize functions.
        ///
        class DataBasic : public IData
        {
        public:
            DataBasic(uint32_t nType);
            virtual uint32_t AddRef();
            virtual uint32_t Release();
            virtual uint32_t GetType() const;

        protected:
            uint32_t mnType;
            uint32_t mnRefCount;
        };


        /// DataRaw
        ///
        /// This class is a variable length class with a specific purpose: to hold a block of data.
        /// This class copies the data passed into the ctor; it does not take over ownership of it.
        /// You need to allocate this class as such via placement new:
        ///     void*    pMemory  = malloc(sizeof(DataRaw) + nDataSize);
        ///     DataRaw* pDataRaw = new DataRaw(pMemory, pData, nDataSize);
        ///
        class DataRaw : public DataBasic 
        {
        public:
            DataRaw(const void* pData = NULL, size_t nDataSize = 0);
            virtual const void* GetData();
            virtual size_t GetSize();

        protected:
            size_t  mnDataSize; // The actual size of the data in mData.
            uint8_t mData[1];   // A subclass of DataRaw may choose to use this as a place to store data.
        };


        /// DataText16
        ///
        /// Holds a 0-terminated string of 16 bit Unicode characters.
        /// The GetSize function returns the byte size of the string, including the 
        /// terminating 0. Thus for a (16 bit) string with a given strlen, the GetSize
        /// function returns:
        ///    ((strlen + 1) * sizeof(uint16_t))
        /// Similarly, if the GetSize function returns the value n, the corresponding 
        /// strlen is:
        ///    ((n / sizeof(uint16_t)) - 1)
        ///
        class DataText16 : public DataRaw
        {
        public:
            /// DataText16
            /// If the input nStrlen is (size_t)-1, then the size should be calculated 
            /// by the basis of the strlen function. If the input nStrlen is less than 
            /// the actual strlen of the string, the input nStrlen is obeyed.
            /// It is assumed that the DataText16 structure is large enough to hold all 
            /// text plus the terminating zero. 
            DataText16(const char16_t* pText16, size_t nStrlen);

            /// CreateDataText16
            /// This is a factory helper function that creates an instance of an 
            /// appropriately sized DataText16 instance. Recall that DataText16 inherits
            /// from DataRaw and that the size of the object varies depending on the 
            /// required size of the stored data. The returned pointer is AddRef'd for 
            /// the caller. If the input pText16 is non-NULL, then it is copied into 
            /// the returned instance.
            static DataText16* CreateDataText16(const char16_t* pText16, size_t nStrlen);
        };


        /// DataText8
        ///
        /// Holds a 0-terminated string of 8 bit UTF8-encoded Unicode characters.
        /// The GetSize function returns the byte size of the string, including the 
        /// terminating 0. Thus for a (8 bit) string with a given strlen, the GetSize
        /// function returns:
        ///    ((strlen + 1) * sizeof(uint8_t))
        /// Similarly, if the GetSize function returns the value n, the corresponding 
        /// strlen is:
        ///    ((n / sizeof(uint8_t)) - 1)
        ///
        class DataText8 : public DataRaw
        {
        public:
            /// DataText8
            /// If the input nStrlen is (size_t)-1, then the size should be calculated 
            /// by the basis of the strlen function. If the input nStrlen is less than 
            /// the actual strlen of the string, the input nStrlen is obeyed.
            /// It is assumed that the DataText8 structure is large enough to hold all 
            /// text plus the terminating zero. 
            DataText8(const char8_t* pText8, size_t nStrlen);

            /// CreateDataText8
            /// This is a factory helper function that creates an instance of an 
            /// appropriately sized DataText8 instance. Recall that DataText8 inherits
            /// from DataRaw and that the size of the object varies depending on the 
            /// required size of the stored data. The returned pointer is AddRef'd for 
            /// the caller. If the input pText8 is non-NULL, then it is copied into 
            /// the returned instance.
            static DataText8* CreateDataText8(const char8_t* pText8, size_t nStrlen);
        };

    } // namespace Clipboard

} // namespace EA


#endif //File sentry

