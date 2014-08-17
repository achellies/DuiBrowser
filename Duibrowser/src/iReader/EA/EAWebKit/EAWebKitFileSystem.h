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
// EAWebKitFileSystem.h
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITFILESYSTEM_H
#define EAWEBKIT_EAWEBKITFILESYSTEM_H


#include <EAWebKit/EAWebKitConfig.h>
#include <EABase/eabase.h>

namespace EA
{
    namespace WebKit
    {
        class FileSystem;


        EAWEBKIT_API void        SetFileSystem(FileSystem* pFileSystem);
        EAWEBKIT_API FileSystem* GetFileSystem();



        // File system paths are UTF8-encoded.
        // File system paths are expected to hold at least 260 chars.
        // Files open for writing are expected to be created if needed, truncated, and positioned at zero.
        //
        // Example usage:
        //     FileSystem::FileObject file = pFileSystem->CreateFileObject();
        //
        //     if(file != FileSystem::kFileObjectInvalid){
        //         if(pFileSystem->OpenFile(file, "/dir/file.txt", FileSystem::kRead)){
        //             char buffer[20];
        //             
        //             int64_t result = pFileSystem->ReadFile(file, buffer, 20);
        //             // A result >= 0 means OK.
        //
        //             pFileSystem->CloseFile(file);
        //         }
        //         
        //         pFileSystem->DestroyFileObject(file);
        //     }
        //
        class EAWEBKIT_API FileSystem
        {
        public:
            enum OpenFlags
            {
                kRead  = 0x01,
                kWrite = 0x02
            };

            // A FileObject can hold a pointer or an integer.
            typedef uintptr_t FileObject;
            static const uintptr_t kFileObjectInvalid = 0;

            virtual ~FileSystem(){ }

            // Synchronous file IO
            virtual FileObject	CreateFileObject() = 0;
            virtual void		DestroyFileObject(FileObject fileObject) = 0;
            virtual bool		OpenFile(FileObject fileObject, const char* path, int openFlags) = 0;  // To consider: Add creation disposition parameter.
            virtual FileObject	OpenTempFile(const char* prefix, char* path) = 0;
            virtual void		CloseFile(FileObject fileObject) = 0;
            virtual int64_t		ReadFile(FileObject fileObject, void* buffer, int64_t size) = 0;
            virtual bool		WriteFile(FileObject fileObject, const void* buffer, int64_t size) = 0;
            virtual int64_t		GetFileSize(FileObject fileObject) = 0;
            virtual int64_t		GetFilePosition(FileObject fileObject) = 0;

            // File system functionality
            virtual bool		FileExists(const char* path) = 0;
            virtual bool		DirectoryExists(const char* path) = 0;
            virtual bool		RemoveFile(const char* path) = 0;
            virtual bool		DeleteDirectory(const char* path) = 0;
            virtual bool		GetFileSize(const char* path, int64_t& size) = 0;
            virtual bool		GetFileModificationTime(const char* path, time_t& result) = 0;
            virtual bool		MakeDirectory(const char* path) = 0;
            virtual bool		GetDataDirectory(char* path) = 0;

			// Update 12/18/2010 : Added a GetBaseDirectory. This is slightly similar to GetDataDirectory() function just above it but differs
			// in 2 significant ways.
			// 1. If the user returns a valid path, it is used as the base path for all other file paths like cookies/cache and those paths are
			// assumed to be relative paths. 
			// 2. It is a new function in order to be backward compatible. If the user does not want the current behavior to change, i.e., 
			// cookies/caches etc. paths specified as full paths, simply not implement this function. Default implementation is provided below.
			virtual bool		GetBaseDirectory(char8_t* path, size_t pathBufferCapacity) 
			{ 
				path[0] = 0; 
				return false;
			}

        };


        // We provide an optional default implementation of FileSystem.
        // A given application may well want to ignore this and provide its own.
        #if EAWEBKIT_DEFAULT_FILE_SYSTEM_ENABLED

            class EAWEBKIT_API FileSystemDefault : public FileSystem
            {
            public:
                FileObject CreateFileObject();
                void       DestroyFileObject(FileObject);
                bool       OpenFile(FileObject, const char* path, int openFlags);
                FileObject OpenTempFile(const char* prefix, char* path);
                void       CloseFile(FileObject);
                int64_t    ReadFile(FileObject, void* buffer, int64_t size);
                bool       WriteFile(FileObject, const void* buffer, int64_t size);
                int64_t    GetFileSize(FileObject fileObject);
                int64_t    GetFilePosition(FileObject fileObject);

                // File system functionality
                bool       FileExists(const char* path);
                bool       DirectoryExists(const char* path);
                bool       RemoveFile(const char* path);
                bool       DeleteDirectory(const char* path);
                bool       GetFileSize(const char* path, int64_t& size);
                bool       GetFileModificationTime(const char* path, time_t& result);
                bool       MakeDirectory(const char* path);
                bool       GetDataDirectory(char* path);
				bool	   GetBaseDirectory(char8_t* path, size_t pathBufferCapacity); 
            };
        #endif

    } // namespace WebKit

} // namespace EA


#endif // Header include guard
