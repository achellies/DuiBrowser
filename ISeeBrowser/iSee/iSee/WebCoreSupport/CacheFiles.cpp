#include "config.h"
#include "CacheFiles.h"

#include "CString.h"
#include "StringHash.h"
#include "FileSystem.h"
#include "KURL.h"
#include "HTTPParsers.h"
#include "Threading.h"

#include <Windows.h>
#include <WinSock2.h>
#include <curl/curl.h>

#include <stdio.h>

#define MIN_CACHE_SIZE	5
#define MAX_CACHE_SIZE	200
#define MEGA			1024 * 1024

namespace WebCore {

struct CacheFile
{
	String lastModified;
	String age;
	String cacheName;
	String mimeType;
	FILE *fpCache;

	CacheFile() : fpCache(NULL) { }
};

static HashMap<String, CacheFile*, CaseFoldingHash> cacheFiles;
static String cacheDirectory;
static long maxCacheSize = MIN_CACHE_SIZE * MEGA;
const String CACHES_INDEX = String("caches.idx"); // 文本文件

static Mutex cachesMutex;

static void deleteCacheFiles()
{
	WIN32_FIND_DATA wfd;
	String path = cacheDirectory + "\\*";
	HANDLE h = FindFirstFile(path.charactersWithNullTermination(), &wfd);
	if (h)
	{
		while (FindNextFile(h, &wfd))
		{
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				deleteFile(cacheDirectory + "\\" + String(wfd.cFileName));
			}
		}
		FindClose(h);
	}
}

void CacheFiles::startCaches(const String &dir, long maxSize)
{
	MutexLocker locker(cachesMutex);
	cacheDirectory = dir;
	makeAllDirectories(dir);
	if (maxSize >= MIN_CACHE_SIZE && maxSize <= MAX_CACHE_SIZE)
		maxCacheSize = maxSize * MEGA;

	// 读取缓冲下来的文件列表
	// 目前暂时删除缓冲
	deleteCacheFiles();
}

void CacheFiles::endCaches()
{
	MutexLocker locker(cachesMutex);
	writeCaches();
	// 清理内存
	cacheFiles.clear();
	// 目前暂时删除缓冲
	deleteCacheFiles();
}

void CacheFiles::writeCaches()
{
	// 写入磁盘
}

void CacheFiles::updateUrl(const String &url, const HTTPHeaderMap& headers)
{
	MutexLocker locker(cachesMutex);
	CacheFile *me = cacheFiles.get(url);
	if (me)
	{
		String lastModified = headers.get("last-modified");
		if (!lastModified.isEmpty())
			me->lastModified = lastModified;
		String age = headers.get("age");
		if (!age.isEmpty())
			me->age = age;
		cacheFiles.set(url, me);
	}
}

String CacheFiles::getCacheUrl(const String &url, String &mimeType, bool haveExpire)
{
	MutexLocker locker(cachesMutex);
	CacheFile *me = cacheFiles.get(url);
	if (me)
	{
		if (haveExpire)
		{
			time_t expire = curl_getdate(me->lastModified.latin1().data(), NULL) + me->age.toUInt();
			if (time(NULL) < expire)
			{
				mimeType = me->mimeType;
#ifdef _WIN32_WCE
				return "file://" + me->cacheName;
#else
				return "file:///" + me->cacheName;
#endif
			}
			else
				deleteFile(me->cacheName);
		}
		else
		{
			mimeType = me->mimeType;
#ifdef _WIN32_WCE
				return "file://" + me->cacheName;
#else
				return "file:///" + me->cacheName;
#endif
		}
	}
	return String();
}

String CacheFiles::getIfModifiedSince(const String &url)
{
	MutexLocker locker(cachesMutex);
	CacheFile *me = cacheFiles.get(url);
	if (me && !me->lastModified.isEmpty())
		return "If-Modified-Since: " + me->lastModified + (me->age.isEmpty() ? "" : ("; length=" + me->age));
	return String();
}

static String getCacheFile(const String &url)
{
	static unsigned cacheFileId = 0;
	String ext;
	String path = KURL(url).path();
	int pos = path.reverseFind('.');
	if (pos > 0)
		ext = path.substring(pos);
	else
		ext = ".htm";
	String cacheName = String::format("%s/temp%04d%s", cacheDirectory.latin1().data(), cacheFileId++, ext.latin1().data());
	if (cacheFileId >= 100000)
		cacheFileId = 0;
	return cacheName;
}

void *CacheFiles::openFile(const String &url, const HTTPHeaderMap& headers)
{
	MutexLocker locker(cachesMutex);
	CacheFile *me = cacheFiles.get(url);
	if (NULL == me)
	{
		me = new CacheFile();
		if (NULL == me)
			return NULL;
		me->cacheName = getCacheFile(url);
	}
	
	String lastModified = headers.get("last-modified");
	if (!lastModified.isEmpty())
		me->lastModified = lastModified;
	
	String age = headers.get("age");
	if (!age.isEmpty())
		me->age = age;
	
	String mimeType = extractMIMETypeFromMediaType(headers.get("Content-Type"));
	if (!mimeType.isEmpty())
		me->mimeType = mimeType;

	me->fpCache = fopen(me->cacheName.latin1().data(), "wb");
	cacheFiles.set(url, me);
	return me->fpCache;
}

void CacheFiles::closeFile(const String &url, void *fileHandle)
{
	MutexLocker locker(cachesMutex);
	CacheFile *me = cacheFiles.get(url);
	if (fileHandle && me->fpCache == fileHandle)
	{
		fclose(me->fpCache);
		me->fpCache = NULL;
		cacheFiles.set(url, me);
	}
}

int CacheFiles::writeFile(void *fileHandle, const char *data, int length)
{
	FILE *fp = reinterpret_cast<FILE *>(fileHandle);
	if (fp)
		if (fwrite(data, length, 1, fp) > 0)
			return length;
	return 0;
}

} // namespace WebCore
