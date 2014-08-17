/*
Copyright (C) 2008 Electronic Arts, Inc.  All rights reserved.

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
// EAWebKitFileSystem.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include <EAWebKit/EAWebKitFileSystem.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <string.h>

#include "PlatformString.h"
using namespace OWBAL;

#if EAWEBKIT_DEFAULT_FILE_SYSTEM_ENABLED
    #include <stdio.h>

    #if defined(EA_PLATFORM_WINDOWS)
        #pragma warning(push, 1)
        #include <windows.h>
        #include <direct.h>
        #include <sys/stat.h>
        #pragma warning(pop)

    #elif defined(EA_PLATFORM_WINCE)
        #pragma warning(push, 1)
        #include <windows.h>
        //#include <direct.h>
        //#include <sys/stat.h>
		#include "ce_time.h"
		#include <EAIO/FnEncode.h>
		#include <EAIO/PathString.h>
        #pragma warning(pop)

    #elif defined(EA_PLATFORM_XENON)
        #pragma warning(push, 1)
        #include <comdecl.h>
        #include <direct.h>
        #include <sys/stat.h>
        #include <sys/utime.h>
        #pragma warning(pop)
 
    #elif defined(EA_PLATFORM_UNIX) || defined(EA_PLATFORM_PS3)
		#include <stdio.h>
		#include <errno.h>
		#include <fcntl.h>
		#include <unistd.h>
		#include <sys/stat.h>
		#include <sys/types.h>
		#include <utime.h>		// Some versions may require <sys/utime.h>
		#ifndef S_ISREG
			#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
		#endif

		#ifndef S_ISDIR
			#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
		#endif
    #else
		#error "The support for this platform's file system is missing."
	#endif

#include <EAIO/EAFileBase.h>
#endif


namespace EA
{

namespace WebKit
{


#if EAWEBKIT_DEFAULT_FILE_SYSTEM_ENABLED
    FileSystemDefault gFileSystemDefault;
    FileSystem*       gpFileSystem = &gFileSystemDefault;
#else
    FileSystem*       gpFileSystem = NULL;
#endif



EAWEBKIT_API void SetFileSystem(FileSystem* pFileSystem)
{
    if(pFileSystem) //If 0 is passed, use the default file system.
		gpFileSystem = pFileSystem;
}


EAWEBKIT_API FileSystem* GetFileSystem()
{
    return gpFileSystem;
}



///////////////////////////////////////////////////////////////////////////////
// FileSystemDefault
///////////////////////////////////////////////////////////////////////////////

#if EAWEBKIT_DEFAULT_FILE_SYSTEM_ENABLED


struct FileInfo
{
    FILE* mpFile;
    bool  mbOpen;

    FileInfo() : mpFile(NULL), mbOpen(false) { }
};


FileSystem::FileObject FileSystemDefault::CreateFileObject()
{
    FileInfo* pFileInfo = new FileInfo;

    return (uintptr_t)pFileInfo;
}


void FileSystemDefault::DestroyFileObject(FileObject fileObject)
{
    FileInfo* pFileInfo = reinterpret_cast<FileInfo*>(fileObject);

    delete pFileInfo; 
}


bool FileSystemDefault::OpenFile(FileObject fileObject, const char* path, int openFlags)
{
    FileInfo* pFileInfo = reinterpret_cast<FileInfo*>(fileObject);

    EAW_ASSERT(!pFileInfo->mbOpen);
	//Note by Arpit Baldeva - 
	//Paul Pedriana's real fix for the "\n" in the EAWebKit cookies file not working as intended.
	//Add binary flag 
	//http://msdn.microsoft.com/en-us/library/yeby3zcb%28VS.80%29.aspx
	//b Open in binary (untranslated) mode; translations involving carriage-return and linefeed characters are suppressed. 

#ifdef _MSC_VER
	String fileName = String::fromUTF8(path);

	ULONG cchWideChar = static_cast<ULONG>(fileName.length());
	ULONG cbMultiByte = WideCharToMultiByte(CP_ACP, 0, fileName.charactersWithNullTermination(), cchWideChar, NULL, 0, NULL, NULL);

	char* ansiFileName = new char[cbMultiByte+1];
	EAW_ASSERT(!ansiFileName);
	
	ULONG cchSize = WideCharToMultiByte(CP_ACP, 0, fileName.charactersWithNullTermination(), cchWideChar, ansiFileName, cbMultiByte, NULL, NULL);
	ansiFileName[cbMultiByte] = '\0';

	pFileInfo->mpFile = fopen(ansiFileName, openFlags & kWrite ? "wb" : "rb");

	delete[] ansiFileName;
	ansiFileName = NULL;
#else
	pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "w" : "r");
#endif

    if(pFileInfo->mpFile)
        pFileInfo->mbOpen = true;

    return pFileInfo->mbOpen;
}


/*
EAIO_API bool MakeTempPathName(char8_t* pPath, const char8_t* pDirectory, const char8_t* pFileName, const char8_t* pExtension, uint32_t nDestPathLength)
{
    // User must allocate space for the resulting temp path.
    if(pPath)
    {
        static const char8_t pFileNameDefault[]  = { 't', 'e', 'm', 'p', 0 };
        static const char8_t pExtensionDefault[] = { '.', 't', 'm', 'p', 0 };

        time_t nTime = time(NULL);

        char8_t tempPath[kMaxPathLength];

        if(!pFileName)
            pFileName = pFileNameDefault;

        if(!pExtension)
            pExtension = pExtensionDefault;

        if(!pDirectory)
        {
            if(!GetTempDirectory(tempPath))
                return false;
            pDirectory = tempPath;
        }

        for(size_t i = 0; i < 5000; i++, nTime--)
        {
            char8_t buffer[16];

            Path::PathString8 tempFilePath(pDirectory);
            Path::Append(tempFilePath, pFileName);

            tempFilePath.operator+=(EAIOItoa8((uint32_t)nTime, buffer));
            tempFilePath.operator+=(pExtension);

            uint32_t nSrcPathLength = (uint32_t)tempFilePath.length();
            if (nSrcPathLength > nDestPathLength)
                break;

            EAIOStrlcpy8(pPath, tempFilePath.c_str(), nDestPathLength);

            FileStream fileStream(pPath);
            if(fileStream.Open(kAccessFlagReadWrite, kCDCreateNew))
            {
                fileStream.Close();
                return true;
            }
        }
    }

    return false;
}
*/


FileSystem::FileObject FileSystemDefault::OpenTempFile(const char* prefix, char* path)
{
    // To do: Make generic version of this (EAIO code above):
    //
    // char path[EA::IO::kMaxPathLength];
    // 
    // if(EA::IO::MakeTempPathName(path, NULL, "EATemp", ".tmp", EA::IO::kMaxPathLength))
    // {
    //     EA::IO::FileStream* pFileStream = new EA::IO::FileStream(path);
    // 
    //     if(pFileStream->Open(EA::IO::kAccessFlagReadWrite, EA::IO::kCDCreateAlways)) // Could also use kCDOpenExisting
    //         return (uintptr_t)pFileStream;
    //     else
    //         delete pFileStream;
    // }

    EAW_FAIL_MSG("FileSystemDefault::OpenTempFile: not yet completed.");

    return kFileObjectInvalid;
}


void FileSystemDefault::CloseFile(FileObject fileObject)
{
    FileInfo* pFileInfo = reinterpret_cast<FileInfo*>(fileObject);

    EAW_ASSERT(pFileInfo->mbOpen);
    fclose(pFileInfo->mpFile);
}


int64_t FileSystemDefault::ReadFile(FileObject fileObject, void* buffer, int64_t size)
{
    FileInfo* pFileInfo = reinterpret_cast<FileInfo*>(fileObject);

    EAW_ASSERT(pFileInfo->mbOpen);
    int64_t result = (int64_t)fread(buffer, 1, size, pFileInfo->mpFile);

    if((result != size) && !feof(pFileInfo->mpFile))
        result = -1;

    return result;  // result might be different from size simply if the end of file was reached.
}


bool FileSystemDefault::WriteFile(FileObject fileObject, const void* buffer, int64_t size)
{
    FileInfo* pFileInfo = reinterpret_cast<FileInfo*>(fileObject);

    const int64_t result = (int64_t)fwrite(buffer, 1, size, pFileInfo->mpFile);

    return (result == size);
}


int64_t FileSystemDefault::GetFileSize(FileObject fileObject)
{
    FileInfo* pFileInfo = reinterpret_cast<FileInfo*>(fileObject); 

    // Save the current offset. 
    const int savedPos = ftell(pFileInfo->mpFile); 

    // Read the size. 
    fseek(pFileInfo->mpFile, 0, SEEK_END); 
    const int endPos = ftell(pFileInfo->mpFile); 

    // Restore original offset. 
    fseek(pFileInfo->mpFile, savedPos, SEEK_SET); 

    return endPos; 
}


int64_t FileSystemDefault::GetFilePosition(FileObject fileObject)
{
    FileInfo* pFileInfo = reinterpret_cast<FileInfo*>(fileObject);
    return ftell(pFileInfo->mpFile);
}


bool FileSystemDefault::FileExists(const char* path)
{
    // The following is copied from the EAIO package.
	if(path && *path)
	{
#if defined(EA_PLATFORM_WINDOWS)

		const DWORD dwAttributes = ::GetFileAttributesA(path);
		return ((dwAttributes != INVALID_FILE_ATTRIBUTES) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0));

#elif defined(EA_PLATFORM_WINCE)

		using namespace EA::IO::Path;
		PathString16 path16;
		EA::IO::ConvertPathUTF8ToUTF16(path16, path);

		const DWORD dwAttributes = ::GetFileAttributes(path16.c_str());
		return ((dwAttributes != INVALID_FILE_ATTRIBUTES) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0));

#elif defined(EA_PLATFORM_XENON)

		const DWORD dwAttributes = ::GetFileAttributesA(path);
		return ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0));

#elif defined(EA_PLATFORM_UNIX) || defined(EA_PLATFORM_PS3)

		struct stat tempStat;
		const int result = stat(path, &tempStat);

		if(result == 0)
			return S_ISREG(tempStat.st_mode) == 0;
#endif
	}

	return false;
}


bool FileSystemDefault::DirectoryExists(const char* path)
{
    // The following is copied from the EAIO package.
	
	if(path && *path)
	{
		#if defined(EA_PLATFORM_WINDOWS)
	        
			const DWORD dwAttributes = ::GetFileAttributesA(path);
		    return ((dwAttributes != INVALID_FILE_ATTRIBUTES) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0));

		#elif defined(EA_PLATFORM_WINCE)
			using namespace EA::IO::Path;
            PathString16 path16;
			EA::IO::ConvertPathUTF8ToUTF16(path16, path);

			const DWORD dwAttributes = ::GetFileAttributes(path16.c_str());
		    return ((dwAttributes != INVALID_FILE_ATTRIBUTES) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0));
	
		#elif defined(EA_PLATFORM_XENON)
	        
			const DWORD dwAttributes = ::GetFileAttributesA(path); // GetFileAttributesA accepts a directory with a trailing path separator.
		    return ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0));

		#elif defined(EA_PLATFORM_UNIX) || defined(EA_PLATFORM_PS3)
			
			struct stat tempStat;
			const int result = stat(path, &tempStat);
	
			if(result == 0)
				return S_ISDIR(tempStat.st_mode) != 0;
		#endif
	}
    return false;
}


bool FileSystemDefault::RemoveFile(const char* path)
{
    // The following is copied from the EAIO package.
	if(path && *path)
	{
    #if defined(EA_PLATFORM_WINDOWS) || defined(EA_PLATFORM_XENON)

        const BOOL bResult = ::DeleteFileA(path);
        return (bResult != 0);

	#elif defined(EA_PLATFORM_WINCE)
		using namespace EA::IO::Path;
		PathString16 path16;
		EA::IO::ConvertPathUTF8ToUTF16(path16, path);

		const BOOL bResult = ::DeleteFile(path16.c_str());
		return (bResult != 0);

    #elif defined(EA_PLATFORM_UNIX) || defined(EA_PLATFORM_PS3)

		const int result = unlink(path);
		return (result == 0);
    #endif
	}

	return false;
}


bool FileSystemDefault::DeleteDirectory(const char* path)
{
    // The following is copied from the EAIO package.
    // This code is not smart enough to do a recursive delete, but the one in EAIO is.

    // Windows doesn't like it when the directory path ends with a 
    // separator (e.g. '\') character, so we correct for this if needed.
    if(path && *path)
	{

		const size_t nStrlen = strlen(path);

		if((path[nStrlen - 1] != '/') && (path[nStrlen - 1] != '\\'))
		{
			#if defined(EA_PLATFORM_WINDOWS) || defined(EA_PLATFORM_XENON)
				return (RemoveDirectoryA(path) != 0);
			#elif defined(EA_PLATFORM_WINCE)
				using namespace EA::IO::Path;
				PathString16 path16;
				EA::IO::ConvertPathUTF8ToUTF16(path16, path);
				return (RemoveDirectory(path16.c_str()) != 0);

			#elif defined(EA_PLATFORM_UNIX) || defined(EA_PLATFORM_PS3)
				return (rmdir(path) == 0);
			#endif
		}

		// Else we need to remove the separator.
		char pathMod[256];
		memcpy(pathMod, path, nStrlen - 1);   // Force 0 terminator in place of directory separator
		pathMod[nStrlen - 1] = 0;

		return DeleteDirectory(pathMod);  // Call ourselves recursively.
	}
	
	return false;
}


bool FileSystemDefault::GetFileSize(const char* path, int64_t& size)
{
    // The following is copied from the EAIO package.
	if(path && *path)
	{
		#if defined(EA_PLATFORM_WINDOWS) || defined(EA_PLATFORM_XENON)

			WIN32_FIND_DATAA win32FindDataA;
			HANDLE hFindFile = FindFirstFileA(path, &win32FindDataA);

			if(hFindFile != INVALID_HANDLE_VALUE)
			{
				size = win32FindDataA.nFileSizeLow | ((int64_t)win32FindDataA.nFileSizeHigh << (int64_t)32);
				FindClose(hFindFile);
				return true;
			}

			return false;

		#elif defined(EA_PLATFORM_WINCE)

			using namespace EA::IO::Path;
			PathString16 path16;
			EA::IO::ConvertPathUTF8ToUTF16(path16, path);

			WIN32_FIND_DATA win32FindData;
			HANDLE hFindFile = FindFirstFile(path16.c_str(), &win32FindData);

			if(hFindFile != INVALID_HANDLE_VALUE)
			{
				size = win32FindData.nFileSizeLow | ((int64_t)win32FindData.nFileSizeHigh << (int64_t)32);
				FindClose(hFindFile);
				return true;
			}

			return false;

		#elif defined(EA_PLATFORM_UNIX) || defined(EA_PLATFORM_PS3)

			struct stat tempStat;
			const int result = stat(path, &tempStat);

			if(result == 0)
			{
				size = tempStat.st_size;
				return true;
			}

			return false;

			#endif
	}

	return false;
}


bool FileSystemDefault::GetFileModificationTime(const char* path, time_t& result)
{
    // The following is copied from the EAIO package.
	if(path && *path) 
	{
		#if defined(EA_PLATFORM_WINDOWS) || defined(EA_PLATFORM_XENON)

			#if defined(EA_PLATFORM_WINDOWS)
				struct _stat tempStat;
				const int r = _stat(path, &tempStat);
			#else
				struct stat tempStat;
				const int r = stat(path, &tempStat);
			#endif
	    
			if(r == 0)
				return tempStat.st_mtime;

			return 0;

		#elif defined(EA_PLATFORM_WINCE)

			using namespace EA::IO::Path;
			PathString16 path16;
			EA::IO::ConvertPathUTF8ToUTF16(path16, path);

			HANDLE hFile = CreateFile(path16.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if(hFile == INVALID_HANDLE_VALUE)
			{
				FILETIME ftCreationTime = {0};
				FILETIME ftLastAccessTime = {0};
				FILETIME ftLastWriteTime = {0};

				GetFileTime(hFile, &ftCreationTime, &ftLastAccessTime, &ftLastWriteTime);
				CloseHandle(hFile);

				SYSTEMTIME systime;
				struct tm tmtime;		

				FileTimeToSystemTime(&ftLastWriteTime, &systime);

				tmtime.tm_year = systime.wYear-1900;
				tmtime.tm_mon = systime.wMonth-1;
				tmtime.tm_mday = systime.wDay;
				tmtime.tm_wday = systime.wDayOfWeek;
				tmtime.tm_hour = systime.wHour;
				tmtime.tm_min = systime.wMinute;
				tmtime.tm_sec = systime.wSecond;

				result = mktime(&tmtime);
				return true;
			}
			return 0;

		#elif defined(EA_PLATFORM_UNIX) || defined(EA_PLATFORM_PS3)

			struct stat tempStat;
			const int r = stat(path, &tempStat);

			if(r == 0)
				return tempStat.st_mtime;

			return 0;

		#endif
	}

	return false;
}


bool FileSystemDefault::MakeDirectory(const char* path)
{
    // The following is copied from the EAIO package.
    // The version here isn't recusive; the one in EAIO is.

	if(path && *path)
	{
		const size_t nStrlen = strlen(path);

		if((path[nStrlen - 1] != '/') && (path[nStrlen - 1] != '\\'))
		{
			#if defined(EA_PLATFORM_WINDOWS) || defined(EA_PLATFORM_XENON)

				const BOOL bResult = CreateDirectoryA(path, NULL);
				return bResult || (GetLastError() == ERROR_ALREADY_EXISTS);

			#elif defined(EA_PLATFORM_WINCE)
				using namespace EA::IO::Path;
				PathString16 path16;
				EA::IO::ConvertPathUTF8ToUTF16(path16, path);
				const BOOL bResult = CreateDirectory(path16.c_str(), NULL);
				return bResult || (GetLastError() == ERROR_ALREADY_EXISTS);

			#elif defined(EA_PLATFORM_UNIX)|| defined(EA_PLATFORM_PS3)

				const int result = mkdir(path, 0777);
				return ((result == 0) || (errno == EEXIST));
			#endif
		}

		// Else we need to remove the separator.
		char pathMod[256];
		memcpy(pathMod, path, nStrlen - 1);   // Force 0 terminator in place of directory separator
		pathMod[nStrlen - 1] = 0;

		return MakeDirectory(pathMod);  // Call ourselves recursively.
	}

	return false;
}


bool FileSystemDefault::GetDataDirectory(char* path)
{
	if(path)    
	{

		#if defined(EA_PLATFORM_WINDOWS)
			// See EAIO::GetSpecialDirectory for a correct way to get the user data directory.
			// And we probably need to have an EAWebKit-size SetDataDirectory function.
			strcpy(path, ".\\");
			return true;

		#elif defined(EA_PLATFORM_WINCE)

			strcpy(path, ".\\");
			return true;

		#elif defined(EA_PLATFORM_XENON)
			strcpy(path, "D:\\");
			return true;

		#elif defined(EA_PLATFORM_PS3)

			// See the EAPlatform package GameDataPS3.h/cpp. The following is not correct,
			// And we probably need to solve it by having an EAWebKit-size SetDataDirectory function.
			strcpy(path, "./");
			return true;

		#elif defined(EA_PLATFORM_UNIX)

			// Need a better implementation than this.
			strcpy(path, "./");
			return true;

		#endif
	}
	return false;
}



bool FileSystemDefault::GetBaseDirectory(char8_t* path, size_t pathBufferCapacity)
{
	if(path)
	{
		char8_t baseDir[EA::IO::kMaxDirectoryLength];
		memset(baseDir, 0, EA::IO::kMaxDirectoryLength);

#if defined(EA_PLATFORM_WINDOWS) 
		strcpy(baseDir,".\\temp\\pc\\");
#elif defined(EA_PLATFORM_WINCE)
		strcpy(baseDir,".\\temp\\pc\\");
#elif defined(EA_PLATFORM_XENON)
		strcpy(baseDir,".\\temp\\xenon\\");
#elif defined (EA_PLATFORM_PS3)
		strcpy(baseDir,"./temp/ps3/");
#endif
		if(pathBufferCapacity > strlen(baseDir))
		{
			strcpy(path, baseDir);
			return true;
		}
	}

	return false;

}
#endif // EAWEBKIT_DEFAULT_FILE_SYSTEM_ENABLED


} // namespace WebKit

} // namespace EA




