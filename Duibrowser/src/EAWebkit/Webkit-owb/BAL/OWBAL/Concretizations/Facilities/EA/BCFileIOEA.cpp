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
// BCFileIOEA.cpp
// Created by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "BCFileIOEA.h"
#include "CString.h"
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitString.h>   // For Strlcpy, which does safe UTF8/UCS2 conversions.

#if EAWEBKIT_EAIO_ENABLED  // This is disabled by default. May be deleted altogether.
    #include <EAIO/EAFileStream.h>
#endif


#if !EAWEBKIT_EAIO_ENABLED 

    namespace OWBAL {

        OwbFile::OwbFile(const String& path)
          : mFileObject(EA::WebKit::FileSystem::kFileObjectInvalid),
            mPath(path)
        {
        }


        int OwbFile::open(char openType)
        {
            int accessFlags = (openType == 'r') ? EA::WebKit::FileSystem::kRead : EA::WebKit::FileSystem::kWrite;
          //int creationDisposition;

            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                mFileObject = pFS->CreateFileObject();

                if(mFileObject != EA::WebKit::FileSystem::kFileObjectInvalid)
                {
                    char path8[256]; // Note that path is natively 16 bit. We don't call path.utf() because that triggers heap activity.
                    EA::Internal::Strlcpy(path8, mPath.characters(), 256, mPath.length());
                    
                    if(pFS->OpenFile(mFileObject, path8, accessFlags))
                        return 1; // We don't have a portable means to convert the file handle to an int. Hopefully the user doesn't rely on the return value being a file handle.

                    pFS->DestroyFileObject(mFileObject);
                    mFileObject = EA::WebKit::FileSystem::kFileObjectInvalid;
                }
            }

            return -1; // Currently user code expects the return value to be -1 (failure) or anything else for success.
        }


        void OwbFile::close()
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                pFS->CloseFile(mFileObject);
                pFS->DestroyFileObject(mFileObject);
            }
        }


        char* OwbFile::read(size_t size)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
				char* const pData = EAWEBKIT_NEW("fileread") char[size + 1]; //WTF::fastNewArray<char>(size + 1);

                // We have a problem here: what if the user passes in a huge size but 
                // the file is small. That would seem to waste a lot of space.
                const int64_t count = pFS->ReadFile(mFileObject, pData, (int64_t)size);

                if(count > 0)
                {
                    pData[count] = '\0';
                    return pData;
                }

                EAWEBKIT_DELETE[] pData;//WTF::fastDeleteArray<char> (pData);
            }

            return NULL;
        }


        void OwbFile::write(const String& dataToWrite)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                // We avoid a possible heap allocation by copying to a temporary local buffer.
                if(dataToWrite.length() < 200)  // We assume a max 1:3 ratio between UCS2 and UTF8. That's not theoretically the limit.
                {
                    // We attempt to write the string via a local UTF8 buffer first.
                    char path8[600]; // Note that dataToWrite is natively 16 bit.
                    const int len = EA::Internal::Strlcpy(path8, dataToWrite.characters(), 600, dataToWrite.length());
                    pFS->WriteFile(mFileObject, path8, len);
                }
                else
                    pFS->WriteFile(mFileObject, dataToWrite.utf8().data(), (int64_t)dataToWrite.length());
            }
        }


        int OwbFile::getSize()
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
                return (int)pFS->GetFileSize(mFileObject);

            return 0; // This should never exucute because this function should only be called if a file was successfully opened.
        }

    } // namespace OWBAL

#else 

    namespace OWBAL {

        OwbFile::OwbFile(const String& path)
          : FileStream(path.characters())   // Problem: String allows implementations of it to have non-null-terminated arrays. 
        {                                   // Does this happen in practice? If so then we should implement a different mechanism, 
        }                                   // though it would be best to avoid charactersWithNullTermination(), as it unilaterally mallocs.


        int OwbFile::open(char openType)
        {
            int accessFlags;
            int creationDisposition;

            if (openType == 'r')
            {
                accessFlags         = EA::IO::kAccessFlagRead;
                creationDisposition = EA::IO::kCDOpenExisting;
            }
            else
            {
                accessFlags         = EA::IO::kAccessFlagWrite;
                creationDisposition = EA::IO::kCDCreateAlways;
            }

            if(Open(accessFlags, creationDisposition))
                return 1; // We don't have a portable means to convert the file handle to an int. Hopefully the user doesn't rely on the return value being a file handle.

            return -1; // Currently user code expects the return value to be -1 (failure) or anything else for success.
        }


        void OwbFile::close()
        {
            FileStream::Close();
        }


        char* OwbFile::read(size_t size)
        {
            char* const readData = EAWEBKIT_NEW("fileread") char[size + 1];//WTF::fastNewArray<char>(size + 1);

            Read(readData, (EA::IO::size_type)size);
            readData[size] = '\0';

            return readData;
        }


        void OwbFile::write(const String& dataToWrite)
        {
            Write(dataToWrite.utf8().data(), (EA::IO::size_type)dataToWrite.length());
        }


        int OwbFile::getSize()
        {
            return (int)(unsigned)FileStream::GetSize();
        }

    } // namespace OWBAL

#endif

