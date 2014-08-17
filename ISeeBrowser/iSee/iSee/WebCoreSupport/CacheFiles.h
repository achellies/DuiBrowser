#ifndef CAHCE_FILES_H
#define CAHCE_FILES_H

#include "PlatformString.h"
#include "HTTPHeaderMap.h"

#define DIRECT_GET_FILE	0

namespace WebCore {

class CacheFiles
{
public:
	static void startCaches(const String &dir, long maxSize = 0);
	static void endCaches();
	static void writeCaches();
	static void updateUrl(const String &url, const HTTPHeaderMap& headers);
	static String getCacheUrl(const String &url, String &mimeType, bool haveExpire = false);
	static String getIfModifiedSince(const String &url);

	static void *openFile(const String &url, const HTTPHeaderMap& headers);
	static void closeFile(const String &url, void *fileHandle);
	static int writeFile(void *fileHandle, const char *data, int length);
};

} // namespace WebCore

#endif // CAHCE_FILES_H
