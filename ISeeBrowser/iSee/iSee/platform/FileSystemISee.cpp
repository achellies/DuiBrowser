/*
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Collabora, Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "FileSystem.h"

#include "CString.h"
#include "NotImplemented.h"
#include "PlatformString.h"

#include <windows.h>
#include <sys/stat.h>
#include <shlwapi.h>

extern LPCWSTR GetAppPath();

namespace WebCore {

static bool statFile(String path, struct _stat& st)
{
    ASSERT_ARG(path, !path.isNull());
    return !_wstat(path.charactersWithNullTermination(), &st) && (st.st_mode & _S_IFMT) == _S_IFREG;
}

bool getFileSize(const String& path, long long& result)
{
    struct _stat sb;
    if (!statFile(path, sb))
        return false;
    result = sb.st_size;
    return true;
}

bool getFileModificationTime(const String& path, time_t& result)
{
    struct _stat st;
    if (!statFile(path, st))
        return false;
    result = st.st_mtime;
    return true;
}

bool fileExists(const String& path) 
{
    struct _stat st;
    return statFile(path, st);
}

bool deleteFile(const String& path)
{
    String filename = path;
    return !!DeleteFileW(filename.charactersWithNullTermination());
}

bool deleteEmptyDirectory(const String& path)
{
    String filename = path;
    return !!RemoveDirectoryW(filename.charactersWithNullTermination());
}

String pathByAppendingComponent(const String& path, const String& component)
{
#ifdef _WIN32_WCE
	String res = path;
	res.replace('/', '\\');
	if (res.endsWith("\\"))
		return (res + component);
	else
		return (res + "\\" + component);
#else
	Vector<UChar> buffer(MAX_PATH);

	if (path.length() + 1 > buffer.size())
		return String();

	memcpy(buffer.data(), path.characters(), path.length() * sizeof(UChar));
	buffer[path.length()] = '\0';

	String componentCopy = component;
	if (!PathAppendW(buffer.data(), componentCopy.charactersWithNullTermination()))
		return String();

	buffer.resize(wcslen(buffer.data()));

	return String::adopt(buffer);
#endif
}

CString fileSystemRepresentation(const String&)
{
	return "";
}

bool makeAllDirectories(const String& path)
{
	String fullPath = path;
	if (!CreateDirectoryW(fullPath.charactersWithNullTermination(), 0)) {
		DWORD error = GetLastError();
		if (error != ERROR_FILE_EXISTS && error != ERROR_ALREADY_EXISTS) {
			LOG_ERROR("Failed to create path %s", path.ascii().data());
			return false;
		}
	}
	return true;
}

String homeDirectoryPath()
{
	notImplemented();
	return "";
}

String pathGetFileName(const String& path)
{
	String res = path;
	res.replace('/', '\\');
	return res.substring(res.reverseFind('\\') + 1);
}

String directoryName(const String& path)
{
    notImplemented();
    return String();
}

CString openTemporaryFile(const char* prefix, PlatformFileHandle& handle)
{
	char appPath[MAX_PATH];
	if (WideCharToMultiByte(CP_ACP,
							0,
							GetAppPath(),
							wcslen(GetAppPath()),
							appPath,
							MAX_PATH - 1,
							NULL,
							NULL
							) > 0)
	{
		static int tempFileId = 0;
		char tempFile[MAX_PATH];
		int fileNameLen = sprintf(tempFile, "%s\\%s%04d.tmp", appPath, prefix, tempFileId++);
		if (tempFileId >= 9999)
			tempFileId = 0;
		
		if (fileNameLen > 0)
		{
			HANDLE tempHandle = ::CreateFileA(tempFile, GENERIC_READ | GENERIC_WRITE, 0, 0, 
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (isHandleValid(tempHandle)) {
				handle = tempHandle;
				return tempFile;
			}
		}
	}

    return 0;
}

void closeFile(PlatformFileHandle& handle)
{
    if (isHandleValid(handle)) {
        ::CloseHandle(handle);
        handle = invalidPlatformFileHandle;
    }
}

int writeToFile(PlatformFileHandle handle, const char* data, int length)
{
    if (!isHandleValid(handle))
        return -1;

    DWORD bytesWritten;
    bool success = WriteFile(handle, data, length, &bytesWritten, 0);

    if (!success)
        return -1;
    return static_cast<int>(bytesWritten);
}

bool unloadModule(PlatformModule module)
{
    return ::FreeLibrary(module);
}

Vector<String> listDirectory(const String& path, const String& filter)
{
    Vector<String> entries;
    notImplemented();
    return entries;
}

} // namespace WebCore
