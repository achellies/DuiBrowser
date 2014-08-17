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
// BCFileSystemEA.cpp
// Created by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "FileSystem.h"
#include "CString.h"
#include "PlatformString.h"
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitString.h>   

#if EAWEBKIT_EAIO_ENABLED  // This is disabled by default. May be deleted altogether.
    #include <EAIO/EAFileUtil.h>
    #include <EAIO/PathString.h>
#endif


namespace {

    #if defined(_MSC_VER)
        const char16_t kDirectorySeparator     = '\\';
        const char16_t kFilePathDriveSeparator = ':';
    #else
        const char16_t kDirectorySeparator     = '/';
        const char16_t kFilePathDriveSeparator = 0;
    #endif


    inline bool IsDirectorySeparator(char16_t c)
    {
        #if defined(_MSC_VER)
            return (c == '/') || (c == '\\');
        #else
            return (c == '/');
        #endif
    }


    inline bool HasUNCPrefix(const char16_t* pBegin, const char16_t* pEnd)
    {
        // UNC paths specifically use '\' and not '/'.
        return (((pBegin + 2) <= pEnd) && (pBegin[0] == '\\') && (pBegin[1] == '\\'));
    }


    const char16_t* GetFileName(const char16_t* pBegin, const char16_t* pEnd = NULL)
    {
        if(pEnd == NULL) 
            pEnd = pBegin + EA::Internal::Strlen(pBegin);

        // If it's a trailing separator, then there's no file name
        if((pBegin < pEnd) && IsDirectorySeparator(pEnd[-1])) 
            return pEnd;

        const char16_t* pName = pEnd;

        // Skip over any non-file-path separators
        while((pName > pBegin)
            && !IsDirectorySeparator(pName[-1])
            && (pName[-1] != kFilePathDriveSeparator))
                --pName;

        // A UNC machine name is not, I am afraid, a file name
        if((pName == (pBegin + 2)) && HasUNCPrefix(pBegin, pEnd))
            return pEnd;

        return pName;
    }

}  // namespace



#if !EAWEBKIT_EAIO_ENABLED 

    namespace OWBAL {

        bool fileExists(const String& path)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                char path8[256]; // Note that path is natively 16 bit. We don't call path.utf8() because that triggers heap activity.
                EA::Internal::Strlcpy(path8, path.characters(), 256, path.length());

                return pFS->FileExists(path8);
            }

            return false;
        }


        bool deleteFile(const String& path)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                char path8[256];
                EA::Internal::Strlcpy(path8, path.characters(), 256, path.length());

                return pFS->RemoveFile(path8);
            }

            return false;
        }


        bool deleteEmptyDirectory(const String& path)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                char path8[256]; // Note that path is natively 16 bit. We don't call path.utf8() because that triggers heap activity.
                EA::Internal::Strlcpy(path8, path.characters(), 256, path.length());

                return pFS->DeleteDirectory(path8);
            }

            return false;
        }


        bool getFileSize(const String& path, long long& result)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                char path8[256];
                EA::Internal::Strlcpy(path8, path.characters(), 256, path.length());

                return pFS->GetFileSize(path8, result);
            }

            return false;
        }


        bool getFileModificationTime(const String& path, time_t& result)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                char path8[256];
                EA::Internal::Strlcpy(path8, path.characters(), 256, path.length());

                return pFS->GetFileModificationTime(path8, result);
            }

            return false;
        }


        String pathByAppendingComponent(const String& path, const String& component)
        {
            const unsigned len = path.length();

            if(len && IsDirectorySeparator(path[len - 1]))
                return path + component;

            String pathResult(path);
            pathResult.append(kDirectorySeparator);
            pathResult.append(component);

            return pathResult;
        }


        /*
            bool makeAllDirectories(const String& path)
            {
                CString fullPath = fileSystemRepresentation(path);
                if (!access(fullPath.data(), F_OK))
                    return true;

                char* p = fullPath.mutableData() + 1;
                int length = fullPath.length();

                if(p[length - 1] == '/')
                    p[length - 1] = '\0';
                for (; *p; ++p)
                    if (*p == '/') {
                        *p = '\0';
                        if (access(fullPath.data(), F_OK))
                            if (mkdir(fullPath.data(), S_IRWXU))
                                return false;
                        *p = '/';
                    }
                if (access(fullPath.data(), F_OK))
                    if (mkdir(fullPath.data(), S_IRWXU))
                        return false;

                return true;
            }
 
        */


        bool makeAllDirectories(const String& path)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS && !path.isEmpty()) // Logic below depends on there being at least one char.
            {
                char path8[256];
                int  length = EA::Internal::Strlcpy(path8, path.characters(), 256, path.length());

                if(!pFS->DirectoryExists(path8))
                {
                    char8_t* p    = path8;
                    char8_t* pEnd = path8 + length; 

                    #if defined(EA_PLATFORM_WINDOWS) // Windows has the concept of UNC paths which begin with two back slashes \\server\dir\dir
                        if(IsDirectorySeparator(*p))
                        {
                            if(IsDirectorySeparator(*++p)) // Move past an initial path separator.
                                ++p;
                        }
					#elif defined(EA_PLATFORM_WINCE)
                        if(IsDirectorySeparator(*p))
                        {
                            if(IsDirectorySeparator(*++p)) // Move past an initial path separator.
                                ++p;
                        }
                    #endif

                    #if defined(_MSC_VER)
                        if(p[0] && (p[1] == ':') && IsDirectorySeparator(p[2])) // Move past an initial C:/
                            p += 3;
                    #else
                        if(IsDirectorySeparator(*p)) // Move past an initial path separator.
                            ++p;
                    #endif

                    if(IsDirectorySeparator(pEnd[-1])) // Remove a trailing path separator if present.
                       pEnd[-1] = 0;

                    for(; *p; ++p) // Walk through the path, creating each component of it if necessary.
                    {
                        if(IsDirectorySeparator(*p))
                        {
                            *p = 0;

                            if(!pFS->DirectoryExists(path8))
                            {
                                if(pFS->MakeDirectory(path8))
                                    return false;
                            }

                            *p = kDirectorySeparator;
                        }
                    }

                    if(!pFS->DirectoryExists(path8))
                    {
                        if(pFS->MakeDirectory(path8))
                            return false;
                    }
                }

                return true;
            }

            return false;
        }


        String pathGetFileName(const String& path)
        {
            const char16_t* pFileName = GetFileName(path.characters());  // GetFileName is a local function.
            return String(pFileName);
        }


        String directoryName(const String& path)
        {
            // EA::IO::Path doesn't have a function to get the end of a directory, just a function to get start of file name.
            const UChar* pPath     = path.characters();
            const UChar* pFileName = GetFileName(pPath);  // Note that (PathString::iterator == UChar* == char16_t*)

            if(pFileName > pPath)
                --pFileName;

            if(IsDirectorySeparator(*pFileName))
                --pFileName;

            return String(pPath, (unsigned)((pFileName + 1) - pPath));
        }


        CString fileSystemRepresentation(const String& file)
        {
            // We are assuming UTF8, and that's what we currently use in our EAWebKit interface.
            // But that may change to UCS2 in the future.
            return file.utf8();
        }


        bool unloadModule(PlatformModule)
        {
            return false;
        }


        String homeDirectoryPath()
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                char path8[256];

                if(pFS->GetDataDirectory(path8))
                    return String(path8);
            }

            return String();
        }


        CString openTemporaryFile(const char* prefix, PlatformFileHandle& fileHandle)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                char path8[256];
                EA::WebKit::FileSystem::FileObject fileObject = pFS->OpenTempFile(prefix, path8);

                // Currently we rely that EA::WebKit::FileObject == PlatformFileHandle, but we need to make this guaranteed.
                if(fileObject != EA::WebKit::FileSystem::kFileObjectInvalid)
                {
                    // EA_COMPILETIME_ASSERT(sizeof fileHandle == sizeof fileObject);
                    fileHandle = (WebCore::PlatformFileHandle)fileObject;
                    return CString(path8);
                }
            }

            fileHandle = invalidPlatformFileHandle;
            return CString();
        }


        void closeFile(PlatformFileHandle& fileHandle)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                // Currently we rely that EA::WebKit::FileObject == PlatformFileHandle, but we need to make this guaranteed.
                EA::WebKit::FileSystem::FileObject fileObject = (EA::WebKit::FileSystem::FileObject)fileHandle;
                pFS->CloseFile(fileObject);
                pFS->DestroyFileObject(fileObject);
            }
        }


        int writeToFile(const PlatformFileHandle& fileHandle, const char* data, int length)
        {
            EA::WebKit::FileSystem* pFS = EA::WebKit::GetFileSystem();

            if(pFS)
            {
                // Currently we rely that EA::WebKit::FileObject == PlatformFileHandle, but we need to make this guaranteed.
                EA::WebKit::FileSystem::FileObject fileObject = (EA::WebKit::FileSystem::FileObject)fileHandle;

                if(pFS->WriteFile(fileObject, data, length))
                    return length;
            }

            return -1;
        }

    } // namespace OWBAL

#else

    namespace OWBAL {

        bool fileExists(const String& path)
        {
            return EA::IO::File::Exists(path.characters());
        }

        bool deleteFile(const String& path)
        {
            return EA::IO::File::Remove(path.characters());
        }

        bool deleteEmptyDirectory(const String& path)
        {
            return EA::IO::Directory::Remove(path.characters());
        }

        bool getFileSize(const String& path, long long& result)
        {
            const EA::IO::size_type size = EA::IO::File::GetSize(path.characters());

            if(size != EA::IO::kSizeTypeError)
            {
                result = (long long)(unsigned long long)size;
                return true;
            }

            result = 0;
            return false;
        }

        bool getFileModificationTime(const String& path, time_t& result)
        {
            result = EA::IO::File::GetTime(path.characters(), EA::IO::kFileTimeTypeLastModification);

            return (result != 0);
        }

        String pathByAppendingComponent(const String& path, const String& component)
        {
            #if defined(_MSC_VER)
                if (path.endsWith("/") || path.endsWith("\\"))
                    return path + component;
            #else
                if (path.endsWith("/"))
                    return path + component;
            #endif

            return path + "\\" + component;
        }


        bool makeAllDirectories(const String& path)
        {
            return EA::IO::Directory::Create(path.characters()); // EAIO directory creation is already 'smart'.
        }


        String pathGetFileName(const String& path)
        {
            return String(EA::IO::Path::GetFileName(path.characters()));
        }


        String directoryName(const String& path)
        {
            // EA::IO::Path doesn't have a function to get the end of a directory, just a function to get start of file name.
            const UChar* pPath     = path.characters();
            const UChar* pFileName = EA::IO::Path::GetFileName(pPath);  // Note that (PathString::iterator == UChar* == char16_t*)

            if(pFileName > pPath)
                --pFileName;

            #if defined(_MSC_VER)
            if(*pFileName == '\\' || *pFileName == '/')
            #else
            if(*pFileName == '/')
            #endif
                --pFileName;

            return String(pPath, (unsigned)(pFileName + 1) - pPath);
        }

        CString fileSystemRepresentation(const String& file)
        {
            return file.utf8();
        }

        bool unloadModule(PlatformModule)
        {
            return false;
        }

        String homeDirectoryPath()
        {
            char16_t path[EA::IO::kMaxPathLength];

            /* const int strLength = */ 
            EA::IO::Directory::GetCurrentWorkingDirectory(path, EA::IO::kMaxPathLength);

            return String(path);
        }

        CString openTemporaryFile(const char* prefix, PlatformFileHandle& fileHandle)
        {
            char16_t path[EA::IO::kMaxPathLength];
            char16_t prefix16[32];

            for(int i = 0; prefix && *prefix; ++prefix)
                prefix16[i] = *(unsigned char*)prefix;

            if(EA::IO::MakeTempPathName(path, NULL, prefix16))
            {
                EA::IO::FileStream* const pFileStream = new EA::IO::FileStream; // To do: Find a way to avoid the heap for creation/destruction of FileStream objects.
                (fileHandle = pFileStream;                                      // Also, convert this 'new' usage to using our user-defined heap.
                return CString(path);
            }

            ASSERT(false);
            return CString();
        }

        void closeFile(PlatformFileHandle& fileHandle)
        {
            if (fileHandle)
            {
                EA::IO::FileStream* const pFileStream = (EA::IO::FileStream*)fileHandle;
                pFileStream->Close();
                delete pFileStream;     // To do: Find a way to avoid the heap for creation/destruction of FileStream objects.
            }
        }

        int writeToFile(const PlatformFileHandle& fileHandle, const char* data, int length)
        {
            EA::IO::FileStream* const pFileStream = (EA::IO::FileStream*)fileHandle;

            if(pFileStream->Write(data, (EA::IO::size_type)(unsigned)length))
                return length;

            return -1;
        }

    } // namespace OWBAL

#endif



/* Windows version

namespace OWBAL {

static String cachedStorageDirectory(DWORD pathIdentifier)
{
    static HashMap<DWORD, String> directories;

    HashMap<DWORD, String>::iterator it = directories.find(pathIdentifier);
    if (it != directories.end())
        return it->second;

    String directory = storageDirectory(pathIdentifier);
    directories.add(pathIdentifier, directory);

    return directory;
}

String localUserSpecificStorageDirectory()
{
    return cachedStorageDirectory(CSIDL_LOCAL_APPDATA);
}

String roamingUserSpecificStorageDirectory()
{
    return cachedStorageDirectory(CSIDL_APPDATA);
}

bool safeCreateFile(const String& path, CFDataRef data)
{
    // Create a temporary file.
    WCHAR tempDirPath[MAX_PATH];
    if (!GetTempPathW(ARRAYSIZE(tempDirPath), tempDirPath))
        return false;

    WCHAR tempPath[MAX_PATH];
    if (!GetTempFileNameW(tempDirPath, L"WEBKIT", 0, tempPath))
        return false;

    HANDLE tempFileHandle = CreateFileW(tempPath, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (tempFileHandle == INVALID_HANDLE_VALUE)
        return false;

    // Write the data to this temp file.
    DWORD written;
    if (!WriteFile(tempFileHandle, CFDataGetBytePtr(data), static_cast<DWORD>(CFDataGetLength(data)), &written, 0))
        return false;

    CloseHandle(tempFileHandle);

    // Copy the temp file to the destination file.
    String destination = path;
    if (!MoveFileExW(tempPath, destination.charactersWithNullTermination(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
        return false;

    return true;
}

} // namespace WebCore

*/

